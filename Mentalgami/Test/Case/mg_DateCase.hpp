#ifndef Cadabra_Queen_Qaos_Test_Case_DateCase_hpp
#define Cadabra_Queen_Qaos_Test_Case_DateCase_hpp

#include "../../../Core/Test/AbstractCase.hpp"

#include "../../../Qaos/Date.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Test {
	class DateCase : public Core::Test::AbstractCase
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		DateCase() = default;
		virtual ~DateCase() override = default;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool run() override;

	private:
		void equalCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result);
		void nonEqualCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result);
		void greaterCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result);
		void greaterOrEqualCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result);
		void lessCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result);
		void lessOrEqualCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result);
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
