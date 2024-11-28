/**
 @file Windows.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Windows.hpp"

#include <QScreen>
#include <QGuiApplication>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	const Native::FactoryMethod Native::Factory = &(Native::Instance<Windows>);

	Windows::Windows(QObject* parent)
	:
		Native(
				Platform::Windows,
				"systems.cadabra.queen",
				QString::fromLatin1(QSysInfo::machineUniqueId())
		)
	{
	}

	Windows::~Windows()
	{

	}

	bool Windows::filterEvent(QObject* receiver, QEvent* event)
	{
		if (event->type() == QEvent::Close) {
			emit exitRequested();
			return true;
		}
		return false;
	}

	bool Windows::shareFile(QUrl url)
	{
		return false;
	}

	void Windows::openAlbum()
	{
	}

	void Windows::openCamera()
	{
	}

	bool Windows::openFile(QUrl url)
	{
		bool retval(false);

		return retval;
	}

	void Windows::checkApplicationUpdate()
	{
		/// @todo from cube
		return ;
	}

	void Windows::runApplicationUpdate()
	{
		/// @todo from cube
		return ;
	}

	void Windows::vibrate()
	{

	}

	void Windows::alertInput(const QString& title, const QString& placeholder, AlertFlags flag_set, const QString& ident)
	{
		return ;
	}

	void Windows::alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set, const QString& ident)
	{
		return ;
	}

	qreal Windows::getScreenKeyboardScreenHeight() const
	{
		return 0;
	}

	int Windows::getScreenDensity() const
	{
		return QGuiApplication::primaryScreen()->physicalDotsPerInch();
	}

	qreal Windows::getDefaultFontScale() const
	{
		return 1; // return QGuiApplication::font().pointSize();
	}
} } } }
