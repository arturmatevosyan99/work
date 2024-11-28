#ifndef Cadabra_Queen_Repository_Model_Application_Interface_ProfileInterface_hpp
#define Cadabra_Queen_Repository_Model_Application_Interface_ProfileInterface_hpp

#include "../AbstractInterface.hpp"

#include "../../../Meta/Profile.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	/**
	 * @brief The Profile Interface
	 * @todo SignOut, Verify, TokenUpdate
	 */
	class ProfileInterface : public virtual Repository::Model::AbstractInterface
	{
	/** @name Aliases */
	/** @{ */
	protected:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ProfileInterface() = default;
		virtual ~ProfileInterface() = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Core::Net::CRPDelay pickUp();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onPickedUp(const Meta::Profile& profile);

	private:
		void onPickUp(const Core::Net::CRPReaction& reaction);
	/** @} */
	};
} } } }

Q_DECLARE_INTERFACE(Cadabra::Queen::Repository::Model::ProfileInterface, "cadabra.queen.repository.model.profile/1.0");

#endif
