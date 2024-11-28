/**
 @file StatusBar.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "StatusBar.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	QObject* StatusBar::QInstance(QQmlEngine* engine, QJSEngine* scriptEngine)
	{
		Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)

		StatusBar* retval(Cadabra::Queen::Core::View::StatusBar::Instance());

		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}

	StatusBar::StatusBar(QObject* parent)
	:
		QObject(parent),

		_theme(StatusBar::Dark)
	{

	}

	QColor StatusBar::getColor()
	{
		return _color;
	}

	StatusBar::Theme StatusBar::getTheme()
	{
		return _theme;
	}

	void StatusBar::setColor(const QColor& color)
	{
		if (_color != color) {
			_color = color;
			emit colorChanged();
		}
	}

	void StatusBar::setTheme(Theme theme)
	{
		if (_theme != theme) {
			_theme = theme;
			emit themeChanged();
		}
	}
} } } }
