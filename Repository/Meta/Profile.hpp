#ifndef Cadabra_Queen_Repository_Meta_Profile_hpp
#define Cadabra_Queen_Repository_Meta_Profile_hpp

#include "Object.hpp"

#include <QObject>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class Profile
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd homeObjectId READ getHomeObjectId WRITE setHomeObjectId)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class Role : int
		{
			HomeObjectId = Qt::UserRole + 1
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Profile(Meta::ObjectId home_object_id = Meta::Object::DeadlockId);
		~Profile();
	/** @} */

	/** @name Properties */
	/** @{ */
		Meta::ObjectId _home_object_id;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		QSet<Role> parse(const QJsonObject& object);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Core::Meta::Qd getHomeObjectId() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setHomeObjectId(const Core::Meta::Qd& home_object_id);
	/** @} */

	/** @name Friends */
	/** @{ */
		friend QDataStream& operator<<(QDataStream& out, const Profile& profile);
		friend QDataStream& operator>>(QDataStream& in, Profile& profile);
		friend QDebug operator<<(QDebug debug, const Profile& profile);
	/** @} */
	};

	inline uint qHash(const Profile::Role& key, uint seed)
	{
		return ::qHash(static_cast<uint>(key), seed);
	}
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Meta::Profile)
Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Meta::Profile::Role)
Q_DECLARE_METATYPE(QSharedPointer<Cadabra::Queen::Repository::Meta::Profile>)

#endif
