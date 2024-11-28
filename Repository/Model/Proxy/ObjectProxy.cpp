#include "ObjectProxy.hpp"

#include "../../Model/Iterator/ObjectIterator.hpp"
#include "../../Data/Entity/Record/Object.hpp"
#include "../../Net/Cursor/ObjectCursor.hpp"

#include "../../Meta/ApplicationRender.hpp"
#include "../../../Core/Meta/Scheme.hpp"

#include <QSharedPointer>
#include <QCoreApplication>
#include <QEvent>
#include <QList>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	ObjectProxy::ObjectProxy(Realm* parent)
	:
		AbstractProxy(parent),

		_path(nullptr)
	{
		QObject::connect(&_path, &ObjectPath::changed, std::bind(&AbstractProxy::refresh, this, AbstractProxy::RefreshFlag::PermanentToken));
	}

	ObjectProxy::~ObjectProxy()
	{

	}

	Model::AbstractIterator* ObjectProxy::allocateIterator() const
	{
		return !hasRealm() ? nullptr : new Model::ObjectIterator(getRealm()->getHierarchy()->getObjectModel());
	}

	Data::AbstractRecord* ObjectProxy::allocateRecord() const
	{
		QScopedPointer<Data::Object> retval(new Data::Object());
		QObject::connect(getRealm()->getHierarchy()->getObjectModel(), &ObjectMap::patched, retval.get(), &Data::Object::onMutated);
		return retval.take();
	}

	Net::AbstractCursor* ObjectProxy::allocateCursor() const
	{
		return nullptr; // return !hasRealm() ? nullptr : new Net::ObjectCursor(getRealm()->getHierarchy()->getObjectModel());
	}

	bool ObjectProxy::filterIterator(Model::AbstractIterator* iterator) const
	{
		if (!AbstractProxy::filterIterator(iterator)) {
			return false;
		} else if (_path.hasMap()) {
			iterator->filter(Meta::EssenceExpression("path", _path.getVector().last()->getId()));
		}
		return true;
	}

	bool ObjectProxy::filterRecord(Data::AbstractRecord* record) const
	{
		if (!AbstractProxy::filterRecord(record)) {
			return false;
		} else if (_path.hasMap()) {
			record->filter(Meta::EssenceExpression("path", _path.getVector().last()->getId()));
		}
		return true;
	}

	bool ObjectProxy::filterCursor(Net::AbstractCursor* cursor) const
	{
		return false;
	}

	AbstractMap* ObjectProxy::getSourceModel() const
	{
		return hasRealm() ? getRealm()->getHierarchy()->getObjectModel() : nullptr;
	}

	ObjectPath* ObjectProxy::getPath()
	{
		return &_path;
	}

	void ObjectProxy::addOrder(Meta::Object::Role role, Qt::SortOrder order)
	{
		AbstractProxy::addOrder(role, order, true);
	}

	void ObjectProxy::addPathFilter(PathFilter filter)
	{
		/// @todo Проинициализировать ObjectPath с учетом chroot
		_path.setMap(hasRealm() && PathFilter::ObjectPath == filter ? getRealm()->getHierarchy()->getObjectModel() : nullptr);
	}

	void ObjectProxy::addPathFilter(const QString& path)
	{
		addToken(Meta::EssenceToken::Type::Path, path, true);
	}

	void ObjectProxy::addParentIdFilter(const Meta::Object::Id& object_id)
	{
		addToken(Meta::EssenceToken::Type::Path, QString::number(object_id), true);
	}

	void ObjectProxy::addApplicationFilter(const Meta::Object::ApplicationScheme& scheme_ident)
	{
		addToken(Meta::EssenceToken::Type::Attribute, QString("application_ident=%1").arg(scheme_ident), true);
	}

	void ObjectProxy::addApplicationFilter(const QString& scheme_alias, bool ident_convert)
	{
		if (ident_convert) {
			QList<Core::Meta::Scheme<Meta::ApplicationRender>::Iterator> list(Core::Meta::Scheme<Meta::ApplicationRender>::List(scheme_alias));
			for (const Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Iterator& i : list) {
				addToken(Meta::EssenceToken::Type::Attribute, QString("application_ident=%1").arg(i.key()), true);
			}
		} else {
			addToken(Meta::EssenceToken::Type::Attribute, QString("application_alias='%1'").arg(scheme_alias), true);
		}
	}

	void ObjectProxy::addObjectFilter(const QString& name)
	{
		addToken(Meta::EssenceToken::Type::Attribute, QString("object_name='%1'").arg(name), true);
	}

	void ObjectProxy::addObjectFilter(const Meta::Object::Id& id)
	{
		addToken(Meta::EssenceToken::Type::Attribute, QString("object_id=%1").arg(id), true);
	}

	void ObjectProxy::addOwnerFilter(const QString& name)
	{
		addToken(Meta::EssenceToken::Type::Attribute, QString("owner_name='%1'").arg(name), true);
	}

	void ObjectProxy::addOwnerFilter(const Meta::Object::Id& id)
	{
		addToken(Meta::EssenceToken::Type::Attribute, QString("owner_id=%1").arg(id), true);
	}

	void ObjectProxy::addOwnersFilter(const QString& name)
	{
		addToken(Meta::EssenceToken::Type::Attribute, QString("owners_name='%1'").arg(name), true);
	}

	void ObjectProxy::addOwnersFilter(const Meta::Group::Id& id)
	{
		addToken(Meta::EssenceToken::Type::Attribute, QString("owners_id=%1").arg(id), true);
	}

} } } }
