#ifndef Cadabra_Queen_Conversation_Data_Setup_Revision1Setup_hpp
#define Cadabra_Queen_Conversation_Data_Setup_Revision1Setup_hpp

#include "../../../Repository/Data/AbstractSetup.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	class Revision1Setup : public Repository::Data::AbstractSetup
	{
	/** @name Constructors */
	/** @{ */
	public:
		Revision1Setup();
		virtual ~Revision1Setup() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool go(Core::Data::Provider& data_provider) const override;
	/** @} */

	/** @name Factories */
	/** @{ */
	protected:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
