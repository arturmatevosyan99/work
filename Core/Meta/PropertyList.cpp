/**
 @file PropertyList.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "PropertyList.hpp"

#include <QEvent>
#include <QObject>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	AbstractPropertyList::AbstractPropertyList(QObject* parent)
	:
		QObject(parent)
	{

	}

	AbstractPropertyList::~AbstractPropertyList()
	{

	}

	bool AbstractPropertyList::eventFilter(QObject* object, QEvent* event)
	{
		if (event->type() == QEvent::UpdateRequest) {
			emit updated();
			return true;
		}
		return false;
	}
} } } }
