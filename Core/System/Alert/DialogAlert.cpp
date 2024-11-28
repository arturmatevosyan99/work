/**
 @file DialogAlert.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "DialogAlert.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	DialogAlert::DialogAlert(QObject* parent, const QString& name, const QString& title, Level level)
	:
		AbstractAlert(parent, name, title, level)
	{

	}

	DialogAlert::DialogAlert(const DialogAlert& origin)
	:
		AbstractAlert(origin)
	{

	}

	DialogAlert::~DialogAlert()
	{

	}

	bool DialogAlert::isModal() const
	{
		return Alert::Instance()->isModal(this);
	}
} } } }
