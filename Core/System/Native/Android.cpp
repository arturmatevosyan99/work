#include "Android.hpp"

#include <QGuiApplication>
#include <QInputMethod>
#include <QScreen>
#include <QDebug>
#include <QtGlobal>
#include <QFontDatabase>
#include <QAndroidJniObject>
#include <android/log.h>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	const Native::FactoryMethod Native::Factory = &(Native::Instance<Android>);

	struct NativeBack
	{
		static void MessageLog(QtMsgType type, const QMessageLogContext& context, const QString& message)
		{
			Queen::Body::SystemMessageHandler(type, context, message);
			const char* file(context.file ? context.file : "");
			const char* function(context.function ? context.function : "");
			switch (type)
			{
				case QtDebugMsg:
//					__android_log_print(ANDROID_LOG_DEBUG, "QueenLog", "%s> %s (%s:%u)\n", function, message.toLocal8Bit().constData(), file, context.line);
					QMetaObject::invokeMethod(Skeleton::ActiveInstance(), "debugMessaged", Qt::DirectConnection, Q_ARG(QString, context.file), Q_ARG(QString, context.function), Q_ARG(int, context.line), Q_ARG(QString, message));
					break;

				case QtInfoMsg:
//					__android_log_print(ANDROID_LOG_INFO, "QueenLog", "%s> %s (%s:%u)\n", function, message.toLocal8Bit().constData(), file, context.line);
					QMetaObject::invokeMethod(Skeleton::ActiveInstance(), "infoMessaged", Qt::DirectConnection, Q_ARG(QString, context.file), Q_ARG(QString, context.function), Q_ARG(int, context.line), Q_ARG(QString, message));
					break;

				case QtWarningMsg:
//					__android_log_print(ANDROID_LOG_WARN, "QueenLog", "%s> %s (%s:%u)\n", function, message.toLocal8Bit().constData(), file, context.line);
					QMetaObject::invokeMethod(Skeleton::ActiveInstance(), "warningMessaged", Qt::DirectConnection, Q_ARG(QString, context.file), Q_ARG(QString, context.function), Q_ARG(int, context.line), Q_ARG(QString, message));
					break;

				case QtCriticalMsg:
//					__android_log_print(ANDROID_LOG_ERROR, "QueenLog", "%s> %s (%s:%u)\n", function, message.toLocal8Bit().constData(), file, context.line);
					QMetaObject::invokeMethod(Skeleton::ActiveInstance(), "criticalMessaged", Qt::DirectConnection, Q_ARG(QString, context.file), Q_ARG(QString, context.function), Q_ARG(int, context.line), Q_ARG(QString, message));
					break;

				case QtFatalMsg:
//					__android_log_print(ANDROID_LOG_FATAL, "QueenLog", "%s> %s (%s:%u)\n", function, message.toLocal8Bit().constData(), file, context.line);
					QMetaObject::invokeMethod(Skeleton::ActiveInstance(), "fatalMessaged", Qt::DirectConnection, Q_ARG(QString, context.file), Q_ARG(QString, context.function), Q_ARG(int, context.line), Q_ARG(QString, message));
					break;
			}
		}

		static void ThrowException(JNIEnv* environment, jobject self, jstring message)
		{
			QMetaObject::invokeMethod
			(
				Native::Instance(), "exceptionThrown",
				Qt::DirectConnection,
				Q_ARG(QString, QString(environment->GetStringUTFChars(message, nullptr)))
			);
		}

		static void UpdateApplication(JNIEnv* environment, jobject self, bool ready)
		{
			QMetaObject::invokeMethod
			(
				Native::Instance(), "applicationUpdated",
				Qt::DirectConnection,
				Q_ARG(bool, ready)
			);
		}

		static void PickAlbumItem(JNIEnv* environment, jobject self, jstring filename)
		{
			QMetaObject::invokeMethod
			(
				Native::Instance(), "albumItemPicked",
				Qt::DirectConnection,
				Q_ARG(QUrl, QUrl::fromLocalFile(environment->GetStringUTFChars(filename, nullptr)))
			);
		}

		static void PickAlbumDescriptor(JNIEnv* environment, jobject self, int handle)
		{
			QMetaObject::invokeMethod
			(
				Native::Instance(), "albumItemPicked",
				Qt::DirectConnection,
				Q_ARG(QUrl, QUrl(QString("fd:%1").arg(handle)))
			);
		}

		static void AlertInput(JNIEnv* environment, jobject self, jstring text, bool success, jstring ident)
		{
			QMetaObject::invokeMethod
			(
				Native::Instance(), "inputAlertDone",
				Qt::DirectConnection,
				Q_ARG(QString, environment->GetStringUTFChars(text, nullptr)),
				Q_ARG(bool, success),
				Q_ARG(QString, environment->GetStringUTFChars(ident, nullptr))
			);
		}

		static void AlertAction(JNIEnv* environment, jobject self, int index, jstring ident)
		{
			QMetaObject::invokeMethod
			(
				Native::Instance(), "actionAlertDone",
				Qt::DirectConnection,
				Q_ARG(int, index), Q_ARG(QString, environment->GetStringUTFChars(ident, nullptr))
			);
		}

		NativeBack()
		{
//			qInstallMessageHandler(&NativeBack::MessageLog);
			QtAndroid::runOnAndroidThreadSync
			(
				[]
				()
				{
					JNINativeMethod methods[]
					{
						{"ThrowException", "(Ljava/lang/String;)V", reinterpret_cast<void*>(NativeBack::ThrowException)},
						{"UpdateApplication", "(Z)V", reinterpret_cast<void*>(NativeBack::UpdateApplication)},
						{"PickAlbumItem", "(Ljava/lang/String;)V", reinterpret_cast<void*>(NativeBack::PickAlbumItem)},
						{"PickAlbumDescriptor", "(I)V", reinterpret_cast<void*>(NativeBack::PickAlbumDescriptor)},
						{"AlertInput", "(Ljava/lang/String;ZLjava/lang/String;)V", reinterpret_cast<void*>(NativeBack::AlertInput)},
						{"AlertAction", "(ILjava/lang/String;)V", reinterpret_cast<void*>(NativeBack::AlertAction)},
					};

					QAndroidJniObject java_class("systems.cadabra.queen.QueenActivity");
					QAndroidJniEnvironment jni_environment;
					jclass object_class = jni_environment->GetObjectClass(java_class.object<jobject>());
					jni_environment->RegisterNatives(object_class, methods, sizeof(methods) / sizeof(methods[0]));
					jni_environment->DeleteLocalRef(object_class);
				}
			);
		}

		~NativeBack()
		{
//			qInstallMessageHandler(Queen::Body::SystemMessageHandler);
		}
	};

	QString Android::BundleIdentifier()
	{
		QString retval;
		QtAndroid::runOnAndroidThreadSync
		(
			[&retval]
			{
				retval = QtAndroid::androidActivity().callObjectMethod("getPackageName", "()Ljava/lang/String;").toString();
			}
		);
		return retval;
	}

	QStringList Android::CommandLine()
	{
		QString command_line;
		QtAndroid::runOnAndroidThreadSync
		(
			[&command_line]
			()
			{
				command_line.clear();
				QAndroidJniObject j_intent(QtAndroid::androidActivity().callObjectMethod("getIntent", "()Landroid/content/Intent;"));
				if (!j_intent.isValid()) {
					return ;
				}
				QAndroidJniObject j_bundle(j_intent.callObjectMethod("getExtras", "()Landroid/os/Bundle;"));
				if (!j_bundle.isValid() || j_bundle == nullptr) {
					return ;
				}
				command_line = j_bundle.callObjectMethod("getString", "(Ljava/lang/String;)Ljava/lang/String;", QAndroidJniObject::fromString("queen").object<jstring>()).toString();
			}
		);
		return command_line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
	}

	int Android::DotsPerInch()
	{
		int retval(0);
		QtAndroid::runOnAndroidThreadSync
		(
			[&retval]
			()
			{
				QAndroidJniObject system_resources
				(
					QAndroidJniObject::callStaticObjectMethod("android/content/res/Resources", "getSystem", "()Landroid/content/res/Resources;")
				);
				retval = system_resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;").getField<int>("densityDpi");
			}
		);
		return retval > 0 ? qRound(retval / 160.0) : 0;
	}

	qreal Android::DefaultFontScale()
	{
		qreal retval(1);
		QtAndroid::runOnAndroidThreadSync
		(
			[&retval]
			()
			{
				QAndroidJniObject resource_set(QtAndroid::androidContext().callObjectMethod("getResources", "()Landroid/content/res/Resources;"));
				if (!resource_set.isValid() || nullptr == resource_set) {
					return ;
				}
				QAndroidJniObject configuration(resource_set.callObjectMethod("getConfiguration", "()Landroid/content/res/Configuration;"));
				if (!configuration.isValid() || nullptr == configuration) {
					return ;
				}
				retval = configuration.getField<jfloat>("fontScale");
			}
		);
		return retval;
	}

	QString Android::NativeId()
	{
		QString retval;
		QtAndroid::runOnAndroidThreadSync
		(
			[&retval]
			()
			{
				retval = QtAndroid::androidActivity().callObjectMethod("generateId", "()Ljava/lang/String;").toString();
			}
		);
		return retval;
	}

	Android::Android()
	:
		Native(
				Platform::Android,
				Android::BundleIdentifier(),
				Android::NativeId()
		)
	{
		static const NativeBack Instance;
	}

	Android::~Android()
	{
	}

	bool Android::filterEvent(QObject* receiver, QEvent* event)
	{
		if (event->type() == QEvent::Close) {
			emit backRequested();
			return true;
		}
		return false;
	}

	bool Android::openFile(QUrl url)
	{
		bool retval(false);
		QtAndroid::runOnAndroidThreadSync
		(
			[&retval, &url]
			()
			{
				QAndroidJniObject j_url = QAndroidJniObject::fromString(url.toLocalFile());
				retval = QtAndroid::androidActivity().callMethod<jboolean>("openFile", "(Ljava/lang/String;)Z", j_url.object<jstring>()) == JNI_TRUE;
			}
		);
		return retval;
	}

	bool Android::shareFile(QUrl url)
	{
		bool retval(false);
		QtAndroid::runOnAndroidThreadSync
		(
			[&retval, &url]
			()
			{
				QAndroidJniObject j_url = QAndroidJniObject::fromString(url.toLocalFile());
				retval = QtAndroid::androidActivity().callMethod<jboolean>("shareFile", "(Ljava/lang/String;)Z", j_url.object<jstring>()) == JNI_TRUE;
			}
		);
		return retval;
	}

	void Android::openAlbum()
	{
		QtAndroid::runOnAndroidThreadSync
		(
			[]
			()
			{
				QtAndroid::androidActivity().callMethod<void>("openAlbum", "()V");
			}
		);
	}

	void Android::openCamera()
	{
		QtAndroid::runOnAndroidThreadSync
		(
			[]
			()
			{
				QtAndroid::androidActivity().callMethod<void>("openCamera", "()V");
			}
		);
	}

	void Android::requestApplicationRate()
	{
		return ;
	}

	void Android::checkApplicationUpdate()
	{
		QtAndroid::runOnAndroidThread
		(
			[]
			()
			{
				QtAndroid::androidActivity().callMethod<void>("checkApplicationUpdate", "()V");
			}
		);
	}

	void Android::runApplicationUpdate()
	{
		QtAndroid::runOnAndroidThread
		(
			[]
			()
			{
				QtAndroid::androidActivity().callMethod<void>("runApplicationUpdate", "()V");
			}
		);
	}

	void Android::vibrate()
	{
		QtAndroid::runOnAndroidThreadSync
		(
			[]
			()
			{
				QtAndroid::androidActivity().callMethod<void>("vibrate", "()V");
			}
		);
	}

	void Android::alertInput(const QString& title, const QString& placeholder, AlertFlags flag_set, const QString& ident)
	{
		QAndroidJniObject j_title = QAndroidJniObject::fromString(title);
		QAndroidJniObject j_placeholder = QAndroidJniObject::fromString(placeholder);
		QAndroidJniObject j_ident = QAndroidJniObject::fromString(ident);

		QtAndroid::runOnAndroidThreadSync
		(
			[&]
			()
			{
				QtAndroid::androidActivity().callMethod<void>
				(
					"alertInput",
					"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
					j_title.object<jstring>(),
					j_placeholder.object<jstring>(),
					j_ident.object<jstring>()
				);
			}
		);
		return ;
	}

	void Android::alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set, const QString& ident)
	{
		QAndroidJniEnvironment j_environment;
		jobjectArray action_array = j_environment->NewObjectArray(action_list.count(), j_environment->FindClass("java/lang/String"), nullptr);
		for (int a = 0; a < action_list.count(); ++a) {
			/// @todo implement in java
			char* action(action_list.at(a).toLocal8Bit().data());
			j_environment->SetObjectArrayElement(action_array, a, j_environment->NewStringUTF((action[0] == '!' || action[0] == '^') ? ++action : action));
		}

		QAndroidJniObject j_title = QAndroidJniObject::fromString(title);
		QAndroidJniObject j_message = QAndroidJniObject::fromString(message);
		QAndroidJniObject j_action_array = QAndroidJniObject::fromLocalRef(action_array);
		QAndroidJniObject j_ident = QAndroidJniObject::fromString(ident);

		QtAndroid::runOnAndroidThreadSync
		(
			[&]
			()
			{
				QtAndroid::androidActivity().callMethod<void>
				(
					"alertAction",
					"(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;ZLjava/lang/String;)V",
					j_title.object<jstring>(),
					j_message.object<jstring>(),
					j_action_array.object<jobjectArray>(),
					flag_set.testFlag(AlertProperty::ActionCancel),
					j_ident.object<jstring>()
				);
			}
		);
		return ;
	}

	QStringList Android::getRunCommandLine() const
	{
		QStringList retval(Android::CommandLine());
		retval.prepend(getApplicationName());
		return retval;
	}

	qreal Android::getScreenKeyboardScreenHeight() const
	{
		return
				QGuiApplication::inputMethod()->keyboardRectangle().height()
				/
				QGuiApplication::primaryScreen()->devicePixelRatio()
		;
	}

	int Android::getScreenDensity() const
	{
		static const int retval(Android::DotsPerInch());
		return retval;
	}

	qreal Android::getDefaultFontScale() const
	{
		static const qreal retval(Android::DefaultFontScale());
		return retval;
	}

	AndroidListener::AndroidListener(QObject* parent)
	:
		QObject(parent)
	{
	}

	AndroidListener::operator jobject()
	{
		return _jni_object.object<jobject>();
	}

	void DialogClickListener::Back(JNIEnv* environment, jobject self, jlong native, jint index)
	{
		QScopedPointer<DialogClickListener, QScopedPointerDeleteLater> listener(reinterpret_cast<DialogClickListener*>(native));
		emit listener->done(index);
	}

	DialogClickListener::DialogClickListener(QObject* parent)
	:
		AndroidListener(parent)
	{
		QAndroidJniObject& jni_object(_jni_object = QAndroidJniObject("systems/cadabra/queen/DialogClickListener", "(J)V", static_cast<jlong>(reinterpret_cast<quintptr>(this))));
		QtAndroid::runOnAndroidThreadSync
		(
			[&jni_object]()
			{
				QAndroidJniEnvironment jni_environment;
				jclass j_class = jni_environment->GetObjectClass(jni_object.object<jobject>());
				if (j_class != nullptr) {
					JNINativeMethod NativeMethod{"Back", "(JI)V", reinterpret_cast<void*>(DialogClickListener::Back)};
					jni_environment->RegisterNatives(j_class, &NativeMethod, 1);
					jni_environment->DeleteLocalRef(j_class);
				}
			}
		);
	}

	void DialogCancelListener::Back(JNIEnv* environment, jobject self, jlong native)
	{
		QScopedPointer<DialogCancelListener, QScopedPointerDeleteLater> listener(reinterpret_cast<DialogCancelListener*>(native));
		emit listener->done();
	}

	DialogCancelListener::DialogCancelListener(QObject* parent)
	:
		AndroidListener(parent)
	{
		QAndroidJniObject& jni_object(_jni_object = QAndroidJniObject("systems/cadabra/queen/DialogCancelListener", "(J)V", static_cast<jlong>(reinterpret_cast<quintptr>(this))));
		QtAndroid::runOnAndroidThreadSync
		(
			[&jni_object]()
			{
				QAndroidJniEnvironment jni_environment;
				jclass j_class = jni_environment->GetObjectClass(jni_object.object<jobject>());
				if (j_class != nullptr) {
					JNINativeMethod NativeMethod{"Back", "(J)V", reinterpret_cast<void*>(DialogCancelListener::Back)};
					jni_environment->RegisterNatives(j_class, &NativeMethod, 1);
					jni_environment->DeleteLocalRef(j_class);
				}
			}
		);
	}
} } } }
