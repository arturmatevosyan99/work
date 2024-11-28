/**
 @file Linux.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Linux.hpp"

#include <QScreen>
#include <QGuiApplication>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	const Native::FactoryMethod Native::Factory = &(Native::Instance<Linux>);

	Linux::Linux(QObject* parent)
	:
		Native(
				Platform::Linux,
				"systems.cadabra.queen",
				QString::fromLatin1(QSysInfo::machineUniqueId())
		)
	{
	}

	Linux::~Linux()
	{

	}

	bool Linux::filterEvent(QObject* receiver, QEvent* event)
	{
		if (event->type() == QEvent::Close) {
			emit exitRequested();
			return true;
		}
		return false;
	}

	bool Linux::shareFile(QUrl url)
	{
		return false;
	}

	void Linux::openAlbum()
	{
	}

	void Linux::openCamera()
	{
	}

	bool Linux::openFile(QUrl url)
	{
		bool retval(false);

		return retval;
	}

	void Linux::checkApplicationUpdate()
	{
		/// @todo from cube
		return ;
	}

	void Linux::runApplicationUpdate()
	{
		/// @todo from cube
		return ;
	}

	void Linux::vibrate()
	{

	}

	void Linux::alertInput(const QString& title, const QString& placeholder, AlertFlags flag_set, const QString& ident)
	{
		return ;
	}

	void Linux::alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set, const QString& ident)
	{
		return ;
	}

	qreal Linux::getScreenKeyboardScreenHeight() const
	{
		return 0;
	}

	int Linux::getScreenDensity() const
	{
		return QGuiApplication::primaryScreen()->physicalDotsPerInch();
	}

	qreal Linux::getDefaultFontScale() const
	{
		return 1; // return QGuiApplication::font().pointSize();
	}
} } } }
