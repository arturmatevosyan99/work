#include "Infrastructure.hpp"

#include "../../../Core/Data/Predicate/ConditionPredicate.hpp"
#include "../Descriptor/SessionDescriptor.hpp"

#include <QQmlEngine>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Infrastructure::Infrastructure(QObject* parent)
	:
		AbstractApplication(parent)
	{

	}

	Infrastructure::~Infrastructure()
	{
		/// @todo отцепится от descriptor'a и получения AceAction
	}

	Repository::Model::ObjectProxy* Infrastructure::getObjectProxy()
	{
		return &_model;
	}

	QAbstractItemModel* Infrastructure::getObjectModel()
	{
		return &_model;
	}

	ObjectPath* Infrastructure::getObjectPath()
	{
		return _model.getPath();
	}

	Cadabra::Queen::Repository::Model::AbstractApplication* Infrastructure::getApplication(Cadabra::Queen::Core::Meta::Qd object_id)
	{
		CacheMap::Iterator c(_cache.find(object_id));
		if (_cache.end() == c) {
			/// @todo
			c = _cache.insert(object_id, QSharedPointer<AbstractApplication>::create(nullptr));
			getDescriptor()->getRealm()->getSessionDescriptor()->getSession()->attach(c.value().get(), object_id, true);
		}
		QQmlEngine::setObjectOwnership(c->get(), QQmlEngine::CppOwnership);

		return c->get();
	}

	void Infrastructure::customEvent(QEvent* event)
	{
		return AbstractApplication::customEvent(event);
	}

	bool Infrastructure::onHandle()
	{
		const QVariantList object_filter_list(getDescriptor()->getRealm()->getPreference()->getOption("repository_infrastructure_object_filter", false).toList());
		for (const QVariant& filter : object_filter_list) {
			if (static_cast<QMetaType::Type>(filter.type()) == QMetaType::QString) {
				_model.tokenize(filter.toString(), false);
			}
		}
		_model.realize(getDescriptor()->getRealm());
		_model.addPathFilter(ObjectProxy::PathFilter::ObjectPath);
		_model.addOrder(Repository::Meta::Object::Role::Name, Qt::SortOrder::AscendingOrder);
		_model.setRefreshMode(Repository::Model::AbstractProxy::RefreshFlag::DynamicToken);
		_model.setRefreshMode(Repository::Model::AbstractProxy::RefreshFlag::PermanentToken);

		QObject::connect(&_model, &Repository::Model::AbstractProxy::dynamicTokenChanged, this, &Infrastructure::onModelTokenChanged);

		return true;
	}

	bool Infrastructure::onOpen()
	{
		return _model.load();
	}

	bool Infrastructure::onSetup()
	{
		return false;
	}

	bool Infrastructure::onClose()
	{
		_cache.clear();
		_model.unload();

		return true;
	}

	bool Infrastructure::onTeardown()
	{
		QObject::disconnect(&_model, &Repository::Model::AbstractProxy::dynamicTokenChanged, this, &Infrastructure::onModelTokenChanged);

		return true;
	}

	void Infrastructure::onModelTokenChanged(int index, bool onoff)
	{
		QVariantList object_filter_list;
		for (const Repository::Meta::EssenceToken& filter : _model.getDynamicTokenVector()) {
			object_filter_list.push_back(QVariant::fromValue(filter.dump()));
		}
		getDescriptor()->getRealm()->getPreference()->setOption("repository_infrastructure_object_filter", object_filter_list);
	}
} } } }
