#ifndef Cadabra_Queen_Conversation_Data_Entity_ChannelSwap_hpp
#define Cadabra_Queen_Conversation_Data_Entity_ChannelSwap_hpp

#include "ChannelMessage.hpp"

#include "../../../Core/Meta/Qd.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	class ChannelSwap : public ChannelMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ChannelSwap(Repository::Meta::ObjectId object_id, const Core::Meta::Qd& scheme_qd);
		virtual ~ChannelSwap() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		const Core::Meta::Qd _scheme_qd;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void resetCompoundPredicate(Core::Data::CompoundPredicate::Logic logic) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Core::Meta::Qd& getSchemeQd() const;
	/** @} */
	};
} } } }

#endif
