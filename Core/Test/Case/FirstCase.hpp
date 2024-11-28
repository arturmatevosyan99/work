#ifndef Cadabra_Queen_Core_Test_FirstCase_hpp
#define Cadabra_Queen_Core_Test_FirstCase_hpp

#include "../AbstractCase.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class FirstCase : public AbstractCase
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		FirstCase() = default;
		virtual ~FirstCase() override = default;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool run() override;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		virtual void initTestCase() override;
		virtual void initTestCase_data() override;
		virtual void cleanupTestCase() override;
		virtual void init() override;
		virtual void cleanup() override;
	/** @} */
	};
} } } }

#endif
