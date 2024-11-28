/**
 @file Android.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "../Native.hpp"

#include <QObject>

#include <QtAndroid>
#include <QAndroidJniEnvironment>


namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class AndroidListener : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AndroidListener(QObject* dialog);
		virtual ~AndroidListener() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		QAndroidJniObject _jni_object;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator jobject();
	/** @} */
	};

	class DialogClickListener : public AndroidListener
	{
	/** @name Statics */
	/** @{ */
	private:
		static void Back(JNIEnv* environment, jobject self, jlong native, jint index);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		DialogClickListener(QObject* dialog);
		virtual ~DialogClickListener() = default;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void done(int index);
	/** @} */
	};

	class DialogCancelListener : public AndroidListener
	{
	/** @name Statics */
	/** @{ */
	private:
		static void Back(JNIEnv* environment, jobject self, jlong native);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		DialogCancelListener(QObject* parent = nullptr);
		virtual ~DialogCancelListener() = default;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void done();
	/** @} */
	};

	class Android : public Native
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Version : quint8
		{
			L = 21, // < 5.0 (Lollipop)
			M = 23, /// < 6.0
			N = 24, /// < 7.0
			O = 26, /// < 8.0
			P = 28, /// < 9.0
			Q = 29, /// < 10.0
			R = 30, /// < 11.0
			S = 31, /// < 12
			S_V2 = 32, /// < 12
			Tiramisu = 33 /// < 13.0
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
		static QString BundleIdentifier();
		static QStringList CommandLine();
		static QString NativeId();
		static int DotsPerInch();
		static qreal DefaultFontScale();
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Android();
		virtual ~Android() override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool filterEvent(QObject* receiver, QEvent* event) override;

		virtual Q_INVOKABLE bool openFile(QUrl url) override;
		virtual Q_INVOKABLE bool shareFile(QUrl url) override;
		virtual Q_INVOKABLE void openAlbum() override;
		virtual Q_INVOKABLE void openCamera() override;
		virtual Q_INVOKABLE void requestApplicationRate() override;
		virtual Q_INVOKABLE void checkApplicationUpdate() override;
		virtual Q_INVOKABLE void runApplicationUpdate() override;
		virtual Q_INVOKABLE void vibrate() override;
		virtual Q_INVOKABLE void alertInput(const QString& title, const QString& placeholder = "", AlertFlags flag_set = {}, const QString& ident ="") override;
		virtual Q_INVOKABLE void alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set = {}, const QString& ident = "") override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QStringList getRunCommandLine() const override;
		virtual qreal getScreenKeyboardScreenHeight() const override;
		virtual int getScreenDensity() const override;
		virtual qreal getDefaultFontScale() const override;
	/** @} */
	};
} } } }
