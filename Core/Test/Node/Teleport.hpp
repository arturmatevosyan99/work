#ifndef Cadabra_Queen_Core_Test_Node_Teleport_hpp
#define Cadabra_Queen_Core_Test_Node_Teleport_hpp

#include "Action.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class Teleport : public Action
	{
	/** @name Constructors */
	/** @{ */
	public:
		Teleport(const QString& description = "Empty teleport description");
		virtual ~Teleport() override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void run() override;
	/** @} */
	};
} } } }

#endif
