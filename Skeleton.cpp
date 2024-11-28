#include "Skeleton.hpp"

#include "Body.hpp"
#include "Bone.hpp"
#include "Environment.hpp"
#include "Core/View/Theme.hpp"
#include "Core/System/Native.hpp"
#include "Core/System/Alert.hpp"
#include "Core/System/Alert/StateAlert.hpp"

#include "Qaos/Qaos.hpp"

#include <QtGlobal>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QtConcurrent>
#include <QtWebView>
#include <QLocale>
#include <QtGlobal>
#include <QSysInfo>
#include <QByteArray>
#include <QTextStream>
#include <QMap>
#include <QPair>
#include <QVariant>

#if defined (Q_OS_ANDROID)
/// @todo Move to WindowManager
#include <QtAndroid>
#endif

namespace Cadabra { namespace Queen {
#ifdef QT_DEBUG
	const bool Skeleton::DebugMode = true;
#else
	const bool Skeleton::DebugMode = false;
#endif

	quint32 FrameworkSerial(const QString& application_version)
	{
		quint32 serial(0);
		const QStringList versions_string_list(application_version.split("."));
		for (int i = 0; i < versions_string_list.size(); i++) {
			bool conversion_result;
			const int integer_version(versions_string_list.at(i).toInt(&conversion_result));
			if (!conversion_result) {
				qWarning() << "The framework version could not be converted correctly, because one of its parts is not an integer value";
				return 0;
			} else if (integer_version > std::numeric_limits<quint8>::max()) {
				qWarning() << "The framework version could not be converted correctly, because one of its parts does not fit into quint8";
				return 0;
			}
			serial += (static_cast<quint8>(integer_version) << ((3 - i) * 8));
		}
		return serial;
	}

	static Skeleton* ActiveInstance = nullptr;
	Skeleton* Skeleton::ActiveInstance()
	{
		return Cadabra::Queen::ActiveInstance;
	}

	QThread* Skeleton::ThreadInstance(const QString& key)
	{
		Skeleton* queen(ActiveInstance());
		if (!queen) {
			return QCoreApplication::instance()->thread();
		}

		QPointer<QThread>& retval(queen->_thread_hashmap[key]);
		if (retval.isNull()) {
			retval = new QThread(queen);
			retval->start();
		}
		return retval.data();
	}

	Skeleton::Skeleton(const QString& name, int& argc, char** argv, int flags)
	:
		::QGuiApplication(argc, argv, flags),

		_application_name("queen." + name),
		_target_name(name),
		_platform_name(QGuiApplication::platformName()),
		_framework_version(QCoreApplication::applicationVersion()),
		_framework_serial(FrameworkSerial(_framework_version)),

		_root_object(nullptr),

		_log_level_option("log-level", "Logging <level>", "level", ""),
		_boot_mode_option("boot-mode", "Custom boot <mode>", "mode", ""),
		_string_envar_option("string-envar", "String environment <variable>", "variable", ""),
		_integer_envar_option("integer-envar", "Integer environment <variable>", "variable", ""),
		_double_envar_option("double-envar", "Double environment <variable>", "variable", "")
	{
		QCoreApplication::setApplicationName(_application_name);

		QtWebView::initialize();
		Qaos::Instance();
		QObject::connect(Core::System::Native::Factory(), &Core::System::Native::exitRequested, this, &Skeleton::requestClose);

		const QDir& base_directory(Core::System::Native::Instance()->getDataDirectory());
		if (!base_directory.mkpath(".")) {
			qCritical() << "Could not create data directory at" << base_directory.absolutePath();
		} else {
			_log_file.setFileName(base_directory.filePath("Queen.log"));
			if (_log_file.open(QFile::WriteOnly | QFile::Append) && _log_file.isWritable()) {
				QObject::connect(this, &Skeleton::warningMessaged, this, &Skeleton::fileLog, Qt::AutoConnection);
				QObject::connect(this, &Skeleton::criticalMessaged, this, &Skeleton::fileLog, Qt::AutoConnection);
				QObject::connect(this, &Skeleton::fatalMessaged, this, &Skeleton::fileLog, Qt::AutoConnection);
			} else {
				qCritical() << "Queen is not able to log to" << _log_file.fileName();
			}
		}

		qmlRegisterAnonymousType<Skeleton>("", 0);
		qmlRegisterAnonymousType<Bone>("", 0);

		_cli_parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
		_cli_parser.addVersionOption();
		_translator.setObjectName("en");
	}

	Skeleton::~Skeleton()
	{
		if (_root_object) {
			Skeleton& self(*this);
			std::for_each
			(
				_bone_list.begin(), _bone_list.end(),
				[&self]
				(Bone* bone)
				{
					bone->onDetach(self);
				}
			);
		}

		for (QPointer<QThread>& thread : _thread_hashmap) {
			if (!thread.isNull() && thread->isRunning()) {
				thread->quit();
				thread->wait();
				thread->deleteLater();
				thread.clear();
			}
		}
	}

	bool Skeleton::notify(QObject* receiver, QEvent* event)
	{
		return
				(receiver != _root_object && receiver != this)
				||
				!Skeleton::ActiveInstance()
				||
				!Core::System::Native::Instance()->filterEvent(receiver, event)
				? QGuiApplication::notify(receiver, event)
				: false
		;
	}

	bool Skeleton::config()
	{
		QCommandLineParser& cli_parser(_cli_parser);
		_cli_parser.addOption(_log_level_option);
		_cli_parser.addOption(_boot_mode_option);
		_cli_parser.addOption(_string_envar_option);
		_cli_parser.addOption(_integer_envar_option);
		_cli_parser.addOption(_double_envar_option);

		if (
			!std::all_of
			(
				_bone_list.begin(), _bone_list.end(),
				[&cli_parser]
				(Bone* bone) -> bool
				{
					for (Bone::OptionMap::mapped_type& option : bone->getCommandLineOptionMap()) {
						if (!cli_parser.addOption(option.first)) {
							return false;
						}
					}
					return true;
				}
			)
		) {
			qCritical() << "Could not add option to the command line parser";
			return false;
		} else if (!_cli_parser.parse(Core::System::Native::Instance()->getRunCommandLine())) {
			qCritical() << "Unexpected CLI parser error";
			return false;
		} else if (
			!std::all_of
			(
				_bone_list.begin(), _bone_list.end(),
				[&cli_parser]
				(Bone* bone) -> bool
				{
					for (
						Bone::OptionMap::Iterator o = bone->getCommandLineOptionMap().begin();
						o != bone->getCommandLineOptionMap().end();
						++o
					) {
						if (cli_parser.isSet(o.value().first)) {
							const QMetaType::Type type(static_cast<QMetaType::Type>(o.value().second.type()));
							switch (type) {
								case QMetaType::QUrl:
									 o.value().second = QUrl(cli_parser.value(o.value().first));
									break;

								default:
									o.value().second = cli_parser.value(o.value().first);
							}
							if (!ActiveInstance()->setEnvironmentVariable(o.key(), o.value().second)) {
								return false;
							}
						}
					}
					return true;
				}
			)
		) {
			qCritical() << "Could not add evaluate or save command line option";
			return false;
		}

		const QString log_level(_cli_parser.value(_log_level_option));
		if (!log_level.isEmpty()) {
			if (log_level.compare("debug") == 0) {
				QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtDebugMsg, true);
				QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtInfoMsg, true);
				QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtWarningMsg, true);
				QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtCriticalMsg, true);
				QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtFatalMsg, true);
			} else if (log_level.compare("info") == 0) {
				QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtDebugMsg, false);
				QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtInfoMsg, true);
				QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtWarningMsg, true);
				QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtCriticalMsg, true);
				QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtFatalMsg, true);
			}
		}

		for (
			QPair<QVariant::Type, const QCommandLineOption&> environment_list
			:
			std::initializer_list<QPair<QVariant::Type, const QCommandLineOption&>>
			{
				{QVariant::Type::String, qAsConst(_string_envar_option)},
				{QVariant::Type::Int, qAsConst(_integer_envar_option)},
				{QVariant::Type::Double, qAsConst(_double_envar_option)}
			}
		) {
			if (_cli_parser.isSet(environment_list.second)) {
				const QStringList environment_variable_list(_cli_parser.values(environment_list.second));
				for (const QString& value : environment_variable_list) {
					const int colon_offset(value.indexOf(':'));
					if (colon_offset > 0) {
						if (environment_list.first == QVariant::Type::String) {
							setEnvironmentVariable(value.left(colon_offset), value.mid(colon_offset + 1));
						} else if (environment_list.first == QVariant::Type::Int) {
							setEnvironmentVariable(value.left(colon_offset), value.mid(colon_offset + 1).toInt());
						} else if (environment_list.first == QVariant::Type::Double) {
							setEnvironmentVariable(value.left(colon_offset), value.mid(colon_offset + 1).toDouble());
						}
					}
				}
			}
		}

		return true;
	}

	bool Skeleton::service()
	{
		return true;
	}

	bool Skeleton::tune()
	{
		Core::View::Theme::Instance();
		Core::View::StatusBar::Instance();

#if defined (Q_OS_ANDROID)
		/// @todo Move to AndroidWindowManager
		QtAndroid::hideSplashScreen();

		Core::View::Theme::Font(QStringLiteral(":/Core/Asset/Font/Inter.Variable.ttf"));
		Core::View::Theme::Font(QStringLiteral(":/Core/Asset/Font/AppleColor.Emoji.ttf"));
//		Core::View::Theme::Font(QStringLiteral(":/Core/Asset/Font/NotoColor.Emoji.ttf"));
#endif
		return true;
	}

	bool Skeleton::extend()
	{
		return false;
	}

	bool Skeleton::extend(Bone* bone)
	{
		if (!bone) {
			return false;
		} else if (_root_object) {
			bone->deleteLater();
			return false;
		}

		_bone_list.push_back(bone);
		_bone_list.back()->setParent(this);
		return true;
	}

	bool Skeleton::translate(const QString& language)
	{
		QString ui_language, language_filename;
		QLocale system_locale(QLocale::system());
		switch (system_locale.language()) {
			case QLocale::Language::Russian:
				ui_language = "ru";
				language_filename = "Russian";
				break;
			default:
				return true;
//				ui_language = "en";
//				language_filename = "English";
		}

		if (!_translator.load(QStringLiteral(":/%1.qm").arg(language_filename))) {
			return false;
		} else {
			if (_translator.objectName() != "en") {
				removeTranslator(&_translator);
				_translator.setObjectName("en");
			}
			if (!installTranslator(&_translator)) {
				return false;
			}
			_translator.setObjectName(ui_language);
		}
		return true;
	}

	int Skeleton::boot()
	{
		return boot(QUrl(QStringLiteral("qrc:/Dummy.qml")));
	}

	int Skeleton::command()
	{
		return EXIT_FAILURE;
	}

	int Skeleton::boot(const QUrl& url)
	{
		if (!_root_object) {
			_qml_engine.addImportPath("qrc:/");

			if (!translate()) {
				qCritical() << "Unexpected translator installation error";
				return -1;
			}

			QQmlApplicationEngine& application_engine(_qml_engine);
			QQmlContext& root_context(*_qml_engine.rootContext());

			if (!std::all_of(_bone_list.begin(), _bone_list.end(), [](Bone* bone) -> bool { return bone->registerMetaObject(); })) {
				qCritical() << "Register error(Meta object)";
				return -1;
			} else if (!std::all_of(_bone_list.begin(), _bone_list.end(), [](Bone* bone) -> bool { return bone->registerMetaType(); })) {
				qCritical() << "Register error(Meta type)";
				return -1;
			} else if (!std::all_of(_bone_list.begin(), _bone_list.end(), [](Bone* bone) -> bool { return bone->registerMetaTypeConverter(); })) {
				qCritical() << "Register error(Meta type converter)";
				return -1;
			} else if (!std::all_of(_bone_list.begin(), _bone_list.end(), [](Bone* bone) -> bool { return bone->registerMetaTypeStreamOperator(); })) {
				qCritical() << "Register error(Meta type stream operator)";
				return -1;
			} else if (!std::all_of(_bone_list.begin(), _bone_list.end(), [](Bone* bone) -> bool { return bone->registerQMLType(); })) {
				qCritical() << "Register error(QML type)";
				return -1;
			} else if (!std::all_of(_bone_list.begin(), _bone_list.end(), [](Bone* bone) -> bool { return bone->registerQMLSingletonType(); })) {
				qCritical() << "Register error(QML singleton type)";
				return -1;
			} else if (!std::all_of(_bone_list.begin(), _bone_list.end(), [](Bone* bone) -> bool { return bone->registerAceAction(); })) {
				qCritical() << "Register error(Ace action)";
				return -1;
			} else if (!std::all_of(_bone_list.begin(), _bone_list.end(), [](Bone* bone) -> bool { return bone->registerAceScheme(); })) {
				qCritical() << "Register error(Ace scheme)";
				return -1;
			} else if (!std::all_of(_bone_list.begin(), _bone_list.end(), [&root_context](Bone* bone) -> bool { return bone->tweakRootContext(root_context); })) {
				qCritical() << "Tweak error (root context)";
				return -1;
			} else if (!std::all_of(_bone_list.begin(), _bone_list.end(), [&application_engine](Bone* bone) -> bool { return bone->tweakApplicationEngine(application_engine); })) {
				qCritical() << "Tweak error (application engine)";
				return -1;
			}

			if (!config()) {
				qCritical() << "Unexpected config error";
				return -1;
			} else if (!service()) {
				qCritical() << "Unexpected platform specific initialization error";
				return -1;
			} else if (!tune()) {
				qCritical() << "Unexpected UI tunning issue";
				return -1;
			}
			Skeleton& self(*this);
			std::for_each
			(
				_bone_list.begin(), _bone_list.end(),
				[&self]
				(Bone* bone) -> void
				{
					self._qml_engine.rootContext()->setContextProperty
					(
						bone->makeTitle().append("Extension").toUtf8().data(),
						QVariant::fromValue(bone)
					);
					return bone->onAttach(self);
				}
			);
			Environment::GlobalInstance().updateModel();

			root_context.setContextProperty("argumentList", _cli_parser.positionalArguments());
			root_context.setContextProperty("environmentModel", &Environment::GlobalInstance());
			root_context.setContextProperty("preferenceModel", &Core::System::Preference::GlobalInstance());

			/// @todo Использовать враппер GlobalContext с реализацией env, arg, preference, ...
			root_context.setContextObject(this);
		} else {
			/// @todo _qml_engine.unload();
		}

		if (url.isEmpty()) {
			_root_object = this;
			return command();
		}

		_qml_engine.load(url);
		QObject* root_object(_root_object = _qml_engine.rootObjects().constFirst());
		if (!_root_object) {
			qCritical() << "Unexpected QML engine error";
			return -1;
		} else if (
					!std::all_of
					(
						_bone_list.begin(), _bone_list.end(),
						[&root_object]
						(Bone* bone) -> bool
						{
							return bone->tweakRootObject(*root_object);
						}
					)
		) {
			_root_object = nullptr;
			qCritical() << "Tweak error (root object)";
			return -1;
		}
		return exec();
	}

	int Skeleton::run()
	{
		Cadabra::Queen::ActiveInstance = this;
		Core::System::Native* native(Core::System::Native::Instance());
		qCritical(
					"Skeleton(%s:%s) is tearing up on %s...",
					qUtf8Printable(_target_name),
					qUtf8Printable(_framework_version),
					qUtf8Printable(QDateTime::currentDateTime().toString())
		);
		qCritical("Machine Name: %s", qUtf8Printable(native->getProductName()));
		qCritical("Machine Identifier: %s", qUtf8Printable(native->getMachineIdentifier()));
		qCritical("Host Name: %s", qUtf8Printable(native->getHostName()));
		qCritical("Boot Identifier: %s", qUtf8Printable(native->getBootIdentifier()));
		qCritical("Native Identifier: %s", qUtf8Printable(Core::System::Native::Instance()->getNativeIdentifier()));
		qCritical("Data Directory: %s", qUtf8Printable(native->getDataDirectory().absolutePath()));

		if (!extend()) {
			qCritical() << "Unexpected extend error";
			return EXIT_FAILURE;
		}
		int retval(EXIT_SUCCESS);
		try {
			retval = boot();
		} catch (const std::exception& e) {
			retval = EXIT_FAILURE;
			qCritical() << "Unexpected boot exception:" << e.what();
		}
		qCritical(
					"Skeleton(%s:%s) is tearing down with %d code on %s... ",
					qUtf8Printable(_target_name),
					qUtf8Printable(_framework_version),
					retval,
					qUtf8Printable(QDateTime::currentDateTime().toString())
		);
		Cadabra::Queen::ActiveInstance = nullptr;
		return retval;
	}

	QFuture<void> Skeleton::disclose(Cupboard& cupboard)
	{
		std::for_each
		(
			_bone_list.begin(), _bone_list.end(),
			[&cupboard]
			(Bone* bone) -> void
			{
				bone->onPreDisclose(cupboard);
			}
		);

		QList<Bone*>& bone_list(_bone_list);
		return QtConcurrent::run
		(
			[&bone_list, &cupboard]() -> void
			{
				std::for_each
				(
					bone_list.begin(), bone_list.end(),
					[&cupboard]
					(Bone* bone) -> void
					{
						bone->onPostDisclose(cupboard);
					}
				);
			}
		);
	}

	QQmlListProperty<Bone> Skeleton::readBoneList()
	{
		return _bone_list;
	}

	void Skeleton::rotateLog(QFile& rotation_file)
	{
		if (!_log_file.isOpen()) {
			return ;
		} else if (rotation_file.fileName().isEmpty()) {
			_log_file.close();
			_log_file.open(QFile::WriteOnly | QFile::Truncate);
			return ;
		} else if (!rotation_file.isOpen()) {
			return ;
		}
		rotation_file.close();
		const QString rotation_filename(rotation_file.fileName());
		rotation_file.remove();
		_log_file.flush();
		_log_file.close();
		if (_log_file.rename(rotation_filename)) {
			rotation_file.open(QFile::ReadOnly);
		}
		_log_file.open(QFile::WriteOnly | QFile::Append);
	}

	const QString& Skeleton::getApplicationName() const
	{
		return _application_name;
	}

	const QString& Skeleton::getTargetName() const
	{
		return _target_name;
	}

	const QString& Skeleton::getPlatformName() const
	{
		return _platform_name;
	}

	const QString& Skeleton::getFrameworkVersion() const
	{
		return _framework_version;
	}

	const quint32& Skeleton::getFrameworkSerial() const
	{
		return _framework_serial;
	}

	const QQmlEngine& Skeleton::getQMLEngine() const
	{
		return _qml_engine;
	}

	const QList<Bone*>& Skeleton::getBoneList() const
	{
		return _bone_list;
	}

	QString Skeleton::getTranslatorLanguage() const
	{
		return _translator.objectName();
	}

	QString Skeleton::getBootMode() const
	{
		return _cli_parser.isSet(_boot_mode_option) ? _cli_parser.value(_boot_mode_option) : "";
	}

	QStringList Skeleton::getArgumentList() const
	{
		return _cli_parser.positionalArguments();
	}

	QString Skeleton::getCommandArgument(const QString& key, const QString& default_value) const
	{
		return _cli_parser.isSet(key) ? _cli_parser.value(key) : default_value;
	}

	Core::System::Preference* Skeleton::getPreference() const
	{
		Core::System::Preference* retval(&Core::System::Preference::GlobalInstance());
		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}

	QVariant Skeleton::getPreferenceOption(const QString& key, const QVariant& default_value) const
	{
		return Core::System::Preference::GlobalInstance().getOption(key, default_value);
	}

	Environment* Skeleton::getEnvironment() const
	{
		Environment* retval(&(Environment::GlobalInstance()));
		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}

	QVariant Skeleton::getEnvironmentVariable(const QString& key, const QVariant& default_value) const
	{
		return Environment::GlobalInstance().getVariable(key, default_value);
	}

	void Skeleton::addCommandLineOption(const QCommandLineOption& option)
	{
		_cli_parser.addOption(option);
	}

	bool Skeleton::setPreferenceOption(const QString& key, const QVariant& value)
	{
		return Core::System::Preference::GlobalInstance().setOption(key, value), true;
	}

	bool Skeleton::setEnvironmentVariable(const QString& key, const QVariant& value)
	{
		return Environment::GlobalInstance().setVariable(key, value);
	}

	void Skeleton::requestClose()
	{
		QQuickWindow* window(qobject_cast<QQuickWindow*>(_root_object));
		if (window) {
			emit window->closing(nullptr);
		}
	}

	void Skeleton::fileLog(QString file_name, QString function_name, int code_line, QString message)
	{
		QTextStream stream(&_log_file);
		stream <<  message;
		if (!function_name.isEmpty()) {
			stream << " in " << function_name;
		}
		if (!file_name.isEmpty()) {
			stream << " at " << file_name.isEmpty() << ":" << code_line;
		}
		stream << Qt::endl;
	}

	bool Skeleton::hasLogFile() const
	{
		return _log_file.isWritable();
	}
} }
