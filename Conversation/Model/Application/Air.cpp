#include "Air.hpp"

#include "../../../Core/Data/Predicate/ConditionPredicate.hpp"
#include "../../../Repository/Model/Descriptor/SessionDescriptor.hpp"

#include "../../../Conversation/Model/Job/Spread.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	Air::Air(QObject* parent)
	:
		Repository::Model::AbstractApplication(parent)
	{

	}

	Air::~Air()
	{
		/// @todo отцепится от descriptor'a и получения AceAction
	}

	Cadabra::Queen::Repository::Model::AbstractJob* Air::spread(const QString& text_message)
	{
		if (!_model.hasActiveModel()) {
			return nullptr;
		}
		QSharedPointer<Conversation::Model::Spread> retval(new Conversation::Model::Spread());
		retval->setMessageText(text_message);
		retval->addChannelTokenVector(_model.getPermanentTokenVector());
		retval->addChannelTokenVector(_model.getDynamicTokenVector());
		if (!getSession()->launch(retval)) {
			return nullptr;
		}
		QQmlEngine::setObjectOwnership(retval.data(), QQmlEngine::CppOwnership);
		return retval.data();
	}

	Repository::Model::ObjectProxy* Air::getObjectProxy()
	{
		return &_model;
	}

	QAbstractItemModel* Air::getObjectModel()
	{
		return &_model;
	}

	Cadabra::Queen::Conversation::Model::GroupChannel* Air::getChannel(Cadabra::Queen::Core::Meta::Qd channel_id, bool auto_build)
	{
		CacheMap::Iterator c(_cache.find(channel_id));
		if (_cache.end() == c) {
			if (auto_build) {
				c = _cache.insert(channel_id, QSharedPointer<GroupChannel>::create(nullptr));
				getDescriptor()->getRealm()->getSessionDescriptor()->getSession()->attach(c.value().get(), channel_id, true);
			} else {
				return nullptr;
			}
		}
		QQmlEngine::setObjectOwnership(c->get(), QQmlEngine::CppOwnership);

		return c->get();
	}

	void Air::customEvent(QEvent* event)
	{
		if (event->type() == Repository::Model::AceEvent::Action<BroadcastAction>::RegisterEventType(0)) {
			return onAction(Repository::Model::AceEvent::TypeCast<BroadcastAction>(static_cast<Repository::Model::AceEvent*>(event)));
		}
		return AbstractApplication::customEvent(event);
	}

	bool Air::onHandle()
	{
		const QVariantList object_filter_list(getDescriptor()->getRealm()->getPreference()->getOption("conversation_air_object_filter", QVariantList{}).toList());
		for (const QVariant& filter : object_filter_list) {
			if (static_cast<QMetaType::Type>(filter.type()) == QMetaType::QString) {
				_model.tokenize(filter.toString(), false);
			}
		}
		if (object_filter_list.isEmpty()) {
			_model.tokenize("~1day", false);
		}

		_model.realize(getDescriptor()->getRealm(), true);
		_model.addApplicationFilter("*.*.*channel", true);
		_model.setRefreshMode(Repository::Model::AbstractProxy::RefreshFlag::DynamicToken);
		_model.setRefreshMode(Repository::Model::AbstractProxy::RefreshFlag::PermanentToken);
		_model.addOrder(Repository::Meta::Object::Role::ModificationDateTime, Qt::SortOrder::DescendingOrder);
		if (getDescriptor()->getRealm()->getPreference()->getOption("conversation_air_live", false).toBool()) {
			_model.setLive(true);
		}
		if (getDescriptor()->getRealm()->getPreference()->getOption("conversation_air_reverse", false).toBool()) {
			_model.setReverse(true);
		}

		QObject::connect(&_model, &Repository::Model::AbstractProxy::dynamicTokenChanged, this, &Air::onModelTokenChanged);
		QObject::connect(&_model, &Repository::Model::AbstractProxy::liveChanged, this, &Air::onModelLiveChanged);
		QObject::connect(&_model, &Repository::Model::AbstractProxy::reverseChanged, this, &Air::onModelReverseChanged);

		return true;
	}

	bool Air::onOpen()
	{
		return _model.load();
	}

	bool Air::onSetup()
	{
		return false;
	}

	bool Air::onClose()
	{
		_cache.clear();
		_model.unload();

		return true;
	}

	bool Air::onTeardown()
	{
		QObject::disconnect(&_model, &Repository::Model::AbstractProxy::dynamicTokenChanged, this, &Air::onModelTokenChanged);
		QObject::disconnect(&_model, &Repository::Model::AbstractProxy::liveChanged, this, &Air::onModelLiveChanged);
		QObject::disconnect(&_model, &Repository::Model::AbstractProxy::reverseChanged, this, &Air::onModelReverseChanged);

		return true;
	}

	void Air::onAction(BroadcastAction* action)
	{
/*
		qDebug() << "BroadcastAction is fired with log_id" << action->getLogId();
		enqueue(action->getLogId());
*/
	}

	void Air::onModelLiveChanged()
	{
		getDescriptor()->getRealm()->getPreference()->setOption("conversation_air_live", _model.isLive());
	}

	void Air::onModelReverseChanged()
	{
		getDescriptor()->getRealm()->getPreference()->setOption("conversation_air_reverse", _model.isReverse());
	}

	void Air::onModelTokenChanged(int index, bool onoff)
	{
		QVariantList object_filter_list;
		for (const Repository::Meta::EssenceToken& filter : _model.getDynamicTokenVector()) {
			object_filter_list.push_back(QVariant::fromValue(filter.dump()));
		}
		getDescriptor()->getRealm()->getPreference()->setOption("conversation_air_object_filter", object_filter_list);
	}
} } } }
