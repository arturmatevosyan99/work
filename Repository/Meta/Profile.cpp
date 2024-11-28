#include "Profile.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	Profile::Profile(Meta::ObjectId home_object_id)
	:
		_home_object_id(home_object_id)
	{

	}

	Profile::~Profile()
	{

	}

	QSet<Profile::Role> Profile::parse(const QJsonObject& object)
	{
		QSet<Role> retval;
		const Meta::ObjectId home_object_id(Essence::ToId(object.value("home_object_id"), true));

		if (_home_object_id != home_object_id) {
			_home_object_id = home_object_id;
			/// @todo emit homeObjectIdChanged();
		}

		retval.insert(Role::HomeObjectId);
		return retval;
	}

	const Core::Meta::Qd Profile::getHomeObjectId() const
	{
		return _home_object_id;
	}

	void Profile::setHomeObjectId(const Core::Meta::Qd& id)
	{
		_home_object_id = id;
	}

	QDataStream& operator<<(QDataStream& out, const Profile& profile)
	{
		out << profile._home_object_id;
		return out;
	}

	QDataStream& operator>>(QDataStream& in, Profile& profile)
	{
		in >> profile._home_object_id;
		return in;
	}

	QDebug operator<<(QDebug debug, const Profile& profile)
	{
		QDebugStateSaver saver(debug);
		debug.nospace() << profile._home_object_id;

		return debug;
	}
} } } }
