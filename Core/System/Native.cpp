/**
 @file Native.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Native.hpp"

#include "../../Body.hpp"

#include "Alert.hpp"
#include "Alert/StateAlert.hpp"

#include <QGuiApplication>
#include <QScreen>
#include <QInputMethod>
#include <QFile>
#include <QSoundEffect>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	Native* Native::Instance()
	{
		return Native::Singleton;
	}

	QObject* Native::QInstance(QQmlEngine*, QJSEngine*)
	{
		return Native::Singleton;
	}

	Native::Native(Platform platform, const QString& bundle_identifier, const QString& machine_identifier)
	:
		Queen::Body(platform, bundle_identifier, machine_identifier),

		_application_state(static_cast<ApplicationState>(qGuiApp->applicationState())),
		_clipboard(QGuiApplication::clipboard()),
		_oom_state(
					new StateAlert
					(
						this,
						"OUT_OF_MEMORY",
						QObject::tr("System memory is exhausted"),
						QObject::tr("Please, free some memory by stopping background applications"),
						this,
						StateAlert::Level::Error
					)
		)
	{
		QObject::connect(this, &Native::memoryExhausted, this, &Native::onMemoryExhausted);
		QObject::connect(this, &Native::exceptionThrown, this, &Native::onException);
		QObject::connect(_clipboard, &QClipboard::dataChanged, this, &Native::clipboardChanged);
		QObject::connect(QGuiApplication::inputMethod(), &QInputMethod::keyboardRectangleChanged, this, &Native::screenKeyboardChanged);
		QObject::connect(QGuiApplication::inputMethod(), &QInputMethod::visibleChanged, this, &Native::screenKeyboardDisplayed);
		QObject::connect(qGuiApp, &QGuiApplication::applicationStateChanged, this, &Native::onApplicationStateChanged);
	}

	Native::~Native()
	{

	}

	bool Native::filterEvent(QObject* receiver, QEvent* event)
	{
		return false;
	}

	bool Native::removeFile(QUrl url)
	{
		return url.isLocalFile() ? QFile::remove(url.toLocalFile()) : false;
	}

	bool Native::openFile(QUrl url)
	{
		return false;
	}

	bool Native::shareFile(QUrl url)
	{
		return false;
	}

	void Native::openAlbum()
	{
		return ;
	}

	void Native::openCamera()
	{
		return ;
	}

	void Native::copyToClipboard(const QString& text)
	{
		_clipboard->setText(text, QClipboard::Clipboard);
	}

	void Native::setupInputMethodEventFilter(QQuickItem* item)
	{
		/// @todo Hack для случаев когда виртуальная клавиатура поднимает вверх все вью
/*
		class ImAdHoc : public QObject {
		Q_OBJECT
		protected:
			bool eventFilter(QObject* object, QEvent* event) override
			{
				if (event->type() != QEvent::InputMethodQuery) {
					return QObject::eventFilter(object, event);

				}
				QInputMethodQueryEvent* imEvt = static_cast<QInputMethodQueryEvent*>(event);
				if (imEvt->queries() == Qt::InputMethodQuery::ImCursorRectangle) {
					imEvt->setValue(Qt::InputMethodQuery::ImCursorRectangle, QRectF());
					return true;
				}
			}
		};

		static thread_local ImFixer imf;
		item->installEventFilter(&imf);
*/
	}

	void Native::requestApplicationRate()
	{
		return ;
	}

	void Native::checkApplicationUpdate()
	{
		return ;
	}

	void Native::runApplicationUpdate()
	{
		return ;
	}

	void Native::vibrate()
	{
		return ;
	}

	void Native::alertInput(const QString& title, const QString& placeholder, AlertFlags flag_set, const QString& ident)
	{
		return ;
	}

	void Native::alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set, const QString& ident)
	{
		return ;
	}

	QStringList Native::getRunCommandLine() const
	{
		return Skeleton::arguments();
	}

	Core::ApplicationState Native::getApplicationState() const
	{
		return _application_state;
	}

	QRectF Native::getScreenKeyboardRectangle() const
	{
		return QGuiApplication::inputMethod() ? QGuiApplication::inputMethod()->keyboardRectangle() : QRectF();
	}

	qreal Native::getScreenKeyboardScreenHeight() const
	{
		return 0;
	}

	int Native::getScreenDensity() const
	{
		return 1;
	}

	qreal Native::getDefaultFontScale() const
	{
		return 1;
	}

	bool Native::isScreenKeyboardVisible() const
	{
		return QGuiApplication::inputMethod() ? QGuiApplication::inputMethod()->isVisible() : false;
	}

	void Native::onMemoryExhausted()
	{
		Alert::Instance()->enqueue(_oom_state);
	}

	void Native::onException(QString message)
	{
		qWarning() << "Native exception thrown:" << message;
	}

	void Native::onApplicationStateChanged(Qt::ApplicationState state)
	{
		ApplicationState application_state(static_cast<ApplicationState>(state));
		if (_application_state != application_state) {
			emit applicationStateChanged(_application_state, application_state);
			_application_state = application_state;
		}
	}
} } } }
