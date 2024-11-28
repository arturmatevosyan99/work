/**
 @file BSD.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "BSD.hpp"

#include <QScreen>
#include <QGuiApplication>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	const Native::FactoryMethod Native::Factory = &(Native::Instance<BSD>);

	BSD::BSD(QObject* parent)
	:
		Native(
				Platform::Berkley,
				"systems.cadabra.queen",
				QString::fromLatin1(QSysInfo::machineUniqueId())
		)
	{
	}

	BSD::~BSD()
	{

	}

	bool BSD::filterEvent(QObject* receiver, QEvent* event)
	{
		if (event->type() == QEvent::Close) {
			emit exitRequested();
			return true;
		}
		return false;
	}

	bool BSD::shareFile(QUrl url)
	{
		return false;
	}

	void BSD::openAlbum()
	{
	}

	void BSD::openCamera()
	{
	}

	bool BSD::openFile(QUrl url)
	{
		bool retval(false);

		return retval;
	}

	void BSD::checkApplicationUpdate()
	{
		/// @todo from cube
		return ;
	}

	void BSD::runApplicationUpdate()
	{
		/// @todo from cube
		return ;
	}

	void BSD::vibrate()
	{

	}

	void BSD::alertInput(const QString& title, const QString& placeholder, AlertFlags flag_set, const QString& ident)
	{
		return ;
	}

	void BSD::alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set, const QString& ident)
	{
		return ;
	}

	qreal BSD::getScreenKeyboardScreenHeight() const
	{
		return 0;
	}

	int BSD::getScreenDensity() const
	{
		return QGuiApplication::primaryScreen()->physicalDotsPerInch();
	}

	qreal BSD::getDefaultFontScale() const
	{
		return 1; // return QGuiApplication::font().pointSize();
	}
} } } }
