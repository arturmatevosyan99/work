/**
 @file ObjectList.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "ObjectList.hpp"

#include <QQmlEngine>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {	
	ObjectList::ObjectList(QObject* parent)
	{

	}

	ObjectList::~ObjectList()
	{

	}

	QQmlListProperty<QObject> ObjectList::makeList()
	{
		return _list;
	}

	QList<QObject*> ObjectList::getAll(const QString& key)
	{
		return _list.index(key);
	}

	QObject* ObjectList::getFirst(const QString& key)
	{
		QObject* retval(_list.map(key));
		if (retval) {
			QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		}
		return retval;
	}
} } } }
