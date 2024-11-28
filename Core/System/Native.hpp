#ifndef Cadabra_Queen_Core_System_Native_hpp
#define Cadabra_Queen_Core_System_Native_hpp

#include <QObject>

#include "../../Body.hpp"
#include "../Core.hpp"

#include "Alert/StateAlert.hpp"

#include <QDir>
#include <QFile>
#include <QRectF>
#include <QString>
#include <QClipboard>
#include <QQuickItem>
#include <QQmlEngine>
#include <QtGlobal>

#include <functional>
#include <type_traits>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class Native : public Queen::Body
	{
	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		using Platform = Queen::Body::Platform;
		using Device = Queen::Body::Device;
		using FactoryMethod = std::function<Native*()>;

		enum class AlertProperty
		{
			ActionSheet = 0b000001,
			ActionCancel = 0b000010
		};
		Q_DECLARE_FLAGS(AlertFlags, AlertProperty)
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
		static inline Native* Singleton = nullptr;

	public:
		static const FactoryMethod Factory;

		template<typename N>
		static typename std::enable_if<std::is_base_of<Native, N>::value, Native*>::type Instance()
		{
			static N instance;
			Native* pointer(&instance);
			QQmlEngine::setObjectOwnership(pointer, QQmlEngine::CppOwnership);
			if (!Native::Singleton) {
				Native::Singleton = pointer;
			}
			return pointer;
		}
		static Native* Instance();
		static QObject* QInstance(QQmlEngine* engine, QJSEngine* script_engine);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_FLAG(AlertFlags)
		Q_PROPERTY(ApplicationState applicationState READ getApplicationState NOTIFY applicationStateChanged)
		Q_PROPERTY(QRectF screenKeyboardRectangle READ getScreenKeyboardRectangle NOTIFY screenKeyboardChanged)
		Q_PROPERTY(qreal screenKeyboardScreenHeight READ getScreenKeyboardScreenHeight NOTIFY screenKeyboardChanged)
		Q_PROPERTY(qreal defaultFontScale READ getDefaultFontScale CONSTANT)
		Q_PROPERTY(bool screenKeyboardVisible READ isScreenKeyboardVisible NOTIFY screenKeyboardDisplayed)
	/** @} */

	/** @name Constructors */
	/** @{ */
	protected:
		explicit Native(Platform platform = Platform::Unknown, const QString& bundle_identifier = "systems.cadabra.queen", const QString& machine_identifier = QUuid::createUuid().toString());
		virtual ~Native();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Core::ApplicationState _application_state;
		QClipboard* _clipboard;
		StateAlert* _oom_state;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool filterEvent(QObject* receiver, QEvent* event);

		virtual Q_INVOKABLE bool removeFile(QUrl url);
		virtual Q_INVOKABLE bool openFile(QUrl url);
		virtual Q_INVOKABLE bool shareFile(QUrl url);
		virtual Q_INVOKABLE void openAlbum();
		virtual Q_INVOKABLE void openCamera();
		virtual Q_INVOKABLE void copyToClipboard(const QString& text);
		virtual Q_INVOKABLE void setupInputMethodEventFilter(QQuickItem* item);
		virtual Q_INVOKABLE void requestApplicationRate();
		virtual Q_INVOKABLE void checkApplicationUpdate();
		virtual Q_INVOKABLE void runApplicationUpdate();
		virtual Q_INVOKABLE void vibrate();
		virtual Q_INVOKABLE void alertInput(const QString& title, const QString& placeholder = "", AlertFlags flag_set = {}, const QString& ident ="");
		virtual Q_INVOKABLE void alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set = {}, const QString& ident = "");
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QStringList getRunCommandLine() const;
		virtual ApplicationState getApplicationState() const;
		virtual QRectF getScreenKeyboardRectangle() const;
		virtual qreal getScreenKeyboardScreenHeight() const;
		virtual int getScreenDensity() const;
		virtual qreal getDefaultFontScale() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void exceptionThrown(QString message);
		void memoryExhausted();
		void motionEnded();
		void clipboardChanged();
		void albumItemPicked(QUrl filename);
		void albumDiscarded();
		void inputAlertDone(QString value, bool success, QString ident);
		void actionAlertDone(int index, QString ident);
		void exitRequested();
		void backRequested();
		void screenKeyboardChanged();
		void screenKeyboardDisplayed();
		void applicationStateChanged(Cadabra::Queen::Core::ApplicationState previous, Cadabra::Queen::Core::ApplicationState current);
		void applicationUpdated(bool ready);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isScreenKeyboardVisible() const;
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		void onMemoryExhausted();
		void onException(QString message);
		void onApplicationStateChanged(Qt::ApplicationState state);
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Core::System::Native::AlertFlags)
Q_DECLARE_METATYPE(Cadabra::Queen::Core::System::Native::AlertFlags)

#endif
