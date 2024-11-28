#ifndef Cadabra_Queen_Repository_Model_Descriptor_SessionDescriptor_hpp
#define Cadabra_Queen_Repository_Model_Descriptor_SessionDescriptor_hpp

#include "../AbstractDescriptor.hpp"

#include "../AceEvent.hpp"

#include "../Application/Session.hpp"

#include "../../Meta/Essence.hpp"
#include "../../Meta/Subject.hpp"
#include "../../Meta/Session.hpp"

#include <QList>
#include <QMultiMap>
#include <QSharedPointer>
#include <QScopedPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class SessionDescriptor : public AbstractDescriptor
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using CacheMap = QMultiMap<Repository::Meta::ObjectId, StatefullDescriptor*>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit SessionDescriptor(Realm* realm, Session* session);
		virtual ~SessionDescriptor() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Model::Session* _session;
		Meta::Session::Id _pool_id;
		Meta::Session::Handle _pool_handle;
		Meta::Subject::AvatarScheme _avatar_scheme;
		QString _moniker;
		CacheMap _cache;
		QList<QVariant> _history_stack;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QUrlQuery makeMoniker() const override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		/**
		 * @brief Identification
		 * @param id
		 * @param handle
		 * @param scheme
		 * @return
		 */
		bool identify(Meta::Session::Id id, Meta::Session::Handle handle, Meta::Subject::AvatarScheme scheme);
		/**
		 * @brief Setup pool id and handle from scratch and initialize the Realm via ::authorize(0)
		 * Very first identification and initialization
		 * @param id
		 * @param handle
		 * @param scheme
		 * @return
		 */
		bool authorize(Meta::Session::Id id, Meta::Session::Handle handle, Meta::Subject::AvatarScheme scheme);
		/**
		 * @brief Change handle of the same pool and initialize the Realm
		 * Possible if the descriptor was identified earlier but Realm was not initialized yet
		 * @param delta
		 * @param increment
		 * @return
		 */
		bool reauthorize(Meta::Session::Handle delta = 0, bool increment = true);
		/**
		 * @brief Change id, handle and detect consuquences automatically
		 * Possible if Realm was initialized earlier and will be reinitialized automatically(if needed) when
		 * application cache is definitely out of date and is going to be cleanup completely if pool has been changed
		 * or reattaching will be accomplished, if not, but for all applications in the cache
		 * @param id
		 * @param handle
		 * @param scheme
		 * @return
		 */
		bool reauthorize(Meta::Session::Id id, Meta::Session::Handle handle, Meta::Subject::AvatarScheme scheme);
		/**
		 * @brief Deauthorize me!
		 * @return
		 */
		bool deauthorize();

		StatefullDescriptor* handle(const Meta::Object::Id& object_id, const Meta::Object::Handle& application_handle, const Meta::Object::ApplicationScheme& application_scheme, const QString& name = "Untitled", bool singleton = false, const QVariant& option = QVariant());
		StatefullDescriptor* handle(StatefullDescriptor* descriptor, const Meta::Object::Handle& handle);
		void marshal(AceEvent& event);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Model::Session* getSession() const;
		virtual const QString& getMoniker() const override;
		Meta::Session::Id getPoolId() const;
		Meta::Session::Handle getPoolHandle() const;
		virtual Model::SubjectKey getSubjectKey() override;
		virtual const Meta::Subject::Id& getSubjectId() const override;
		Meta::Subject::AvatarScheme getAvatarScheme() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	protected:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasCache() const;
	/** @} */
	};
} } } }

#endif
