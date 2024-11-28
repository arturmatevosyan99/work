#include "AbstractDescriptor.hpp"

#include <QCoreApplication>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	AbstractDescriptor::AbstractDescriptor(Realm* realm, QSharedPointer<const Meta::Object> object)
	:
		QObject(nullptr),

		_realm(realm),
		_object(object)
	{

	}

	QVariant AbstractDescriptor::opt(const QStringList& path) const
	{
		/// @todo Стандартные опции общего назначения из мета-данных
		return QVariant();
	}

	Model::Realm* AbstractDescriptor::getRealm() const
	{
		return _realm.data();
	}

	const Meta::ObjectId& AbstractDescriptor::getObjectId() const
	{
		return _object->getId();
	}

	const QString& AbstractDescriptor::getObjectName() const
	{
		return _object->getName();
	}

	const QString AbstractDescriptor::getObjectSimplifiedName() const
	{
		return _object->getName().simplified();
	}

	QUrl AbstractDescriptor::getObjectPictureURL() const
	{
		return !_object->getPicture().isEmpty() ? QUrl::fromLocalFile(_realm->getStoragePath().filePath(_object->getPicture() + ".picture")) : QUrl();
	}

	const Meta::Object::ApplicationScheme& AbstractDescriptor::getObjectApplicationScheme() const
	{
		return _object->getApplicationScheme();
	}

	QSharedPointer<const Meta::Object> AbstractDescriptor::getObject() const
	{
		return _object;
	}

	bool AbstractDescriptor::isOwner(Core::Meta::Qd subject_qd) const
	{
		return subject_qd == getSubjectId();
	}

	bool AbstractDescriptor::hasRealm() const
	{
		return !_realm.isNull();
	}
} } } }
