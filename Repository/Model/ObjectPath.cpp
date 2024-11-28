/**
 @file ObjectPath.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "ObjectPath.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	const QSharedPointer<const Meta::Object> ObjectPath::RootObject = QSharedPointer<const Meta::Object>::create(1, 1, "0", "Root", false, true, "", 555, 0, 0, 0);

	ObjectPath::ObjectPath(ObjectMap* map)
	:
		QObject(nullptr),

		_map(map)
	{
		if (nullptr != map) {
			_vector.push_back(ObjectPath::RootObject);
		}
	}

	ObjectPath::ObjectPath(const ObjectPath& origin)
	:
		QObject(nullptr),

		_map(origin._map),
		_vector(origin._vector)
	{
	}

	ObjectPath::ObjectPath(ObjectPath&& origin)
	:
		QObject(nullptr),

		_map(std::move(origin._map)),
		_vector(std::move(origin._vector))
	{
	}

	bool ObjectPath::go(Meta::Object::Id id)
	{
		/// @todo Каким-либо образом придти к заданному id(может быть пройти путь заново)
		return false;
	}

	bool ObjectPath::go(QSharedPointer<const Meta::Object> object)
	{
		/// @todo Каким-либо образом придти к заданному id(может быть пройти путь заново)
		return false;
	}

	bool ObjectPath::go(Shortcut shortcut)
	{
		if (nullptr == _map || _vector.size() <= 1) {
			return false;
		}

		switch (shortcut) {
			case Shortcut::Back:
				_vector.removeLast();
				break;

			case Shortcut::Root:
				if (_vector.length() > 1) {
					_vector.resize(1);
				} else {
					return false;
				}
				break;

			default:
				return false;
		}

		emit changed();
		return true;
	}

	bool ObjectPath::backward()
	{
		return go(Shortcut::Back);
	}

	bool ObjectPath::forward(Cadabra::Queen::Core::Meta::Qd object_qd)
	{
		if (!hasMap() || _vector.last()->getId() == object_qd) {
			return false;
		}

		const QSharedPointer<const Meta::Object> candidate(_map->getObject(object_qd));
		if (nullptr == candidate) {
			return false;
		} else if (candidate->getParentId() != _vector.last()->getId()) {
			return false;
		}
		_vector.push_back(candidate);

		emit changed();
		return true;
	}

	QString ObjectPath::breadCrumbs() const
	{
		QString retval;
		for (int o = 1; o < _vector.size(); ++o) {
			retval.append("/").append(_vector.at(o)->getLocalizedName());
		}
		return retval.isEmpty() ? "/" : retval;
	}

	const QVector<QSharedPointer<const Meta::Object>>& ObjectPath::getVector() const
	{
		return _vector;
	}

	int ObjectPath::getDepth() const
	{
		return _vector.length();
	}

	QString ObjectPath::getLocalizedName() const
	{
		return hasMap() ? _vector.last()->getLocalizedName() : "";
	}

	void ObjectPath::setMap(ObjectMap* map)
	{
		_map = map;
		if (nullptr != _map) {
			_vector = QVector({ObjectPath::RootObject});
			emit changed();
		} else if (_vector.isEmpty()) {
			_vector.clear();
			emit changed();
		}
	}

	bool ObjectPath::hasMap() const
	{
		return !_vector.isEmpty() && nullptr != _map;
	}

	QDebug operator<<(QDebug debug, const ObjectPath& object_path)
	{
		debug.nospace() << object_path.breadCrumbs();

		return debug;
	}
} } } }
