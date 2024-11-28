#include "Avatar.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Avatar::Avatar(QObject* parent, const QString& alias)
	:
		QObject(parent),

		_realm(new Realm(nullptr)),
		_session(new Session(nullptr)),
		_home(new Directory(nullptr)),
		_alias(alias)
	{
		QObject::connect(_home, &AbstractApplication::opened, _home, &Directory::index);

		const QVariant subject_profile(_realm->getPreference()->getOption("queen_avatar_profile"));

		if (subject_profile.canConvert<Meta::Profile>()) {
			_subject_profile = QSharedPointer<Meta::Profile>::create(subject_profile.value<Meta::Profile>());
		} else {
			_subject_profile = QSharedPointer<Meta::Profile>::create();
		}

		_session->realize(_realm);
	}

	Avatar::~Avatar()
	{
		delete _home;
		delete _session;
		delete _realm;
	}

	bool Avatar::reviveSession()
	{
		return _session->hasAttached() ? true : _session->prolongate(_realm);
	}

	bool Avatar::pointEnd(const QUrl& endpoint)
	{
		if (_session->getPoolId() > 0) {
			return false;
		}
		_realm->getEndpoint()->setURL(endpoint);
		return true;
	}

	bool Avatar::pickUpProfile()
	{
		if (!_session->isActive()) {
			return false;
		}
		Core::Net::CRPDelay call(_session->pickUp());
		call->handle(std::bind(&Avatar::onPickProfileUp, this, std::placeholders::_1));
		return true;
	}

	bool Avatar::indexHome()
	{
		if (!_session->isActive() || !_subject_profile || _subject_profile->_home_object_id == Meta::Object::DeadlockId) {
			return false;
		} else if (!_home->hasDescriptor()) {
			if (!_session->attach(_home, _subject_profile->_home_object_id, true)) {
				qWarning() << "Attach home model error";
				return false;
			}
			return true;
		}
		_home->index();
		return true;
	}

	Realm* Avatar::getRealm()
	{
		return _realm;
	}

	Session* Avatar::getSession()
	{
		return _session;
	}

	Directory* Avatar::getHome()
	{
		return _home;
	}

	const QString& Avatar::getAlias() const
	{
		return _alias;
	}

	const Avatar::Flags& Avatar::getFlagSet() const
	{
		return _flag_set;
	}

	const Meta::Object::Id& Avatar::getHomeObjectId() const
	{
		return _subject_profile ? _subject_profile->_home_object_id : Meta::Object::DeadlockId;
	}

	void Avatar::setAlias(const QString& value)
	{
		if (_alias != value) {
			_alias = value;
			emit aliasChanged();
		}
	}

	void Avatar::setFlagSet(const Flags& value)
	{
		if (_flag_set != value) {
			_flag_set = value;
			emit flagsetChanged();
		}
	}

	void Avatar::onPickProfileUp(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			/// @log
			reaction.log("Avatar::onPickedProfileUp");
			return ;
		} else if (!(*reaction).isObject()) {
			qWarning("Unknown data structure, could not parse");
			return ;
		}
/*
		const Meta::ObjectId home_object_id(Meta::Essence::ToId((*reaction).toObject().value("home_object_id"), true));
		Meta::Profile(home_object_id);
*/
		QSet<Meta::Profile::Role> role_set(_subject_profile->parse((*reaction).toObject()));
		if (!role_set.isEmpty()) {
			_realm->getPreference()->setOption("queen_avatar_profile", QVariant::fromValue(*_subject_profile));
		}

		if (role_set.contains(Meta::Profile::Role::HomeObjectId)) {
			indexHome();
		}
	}
} } } }
