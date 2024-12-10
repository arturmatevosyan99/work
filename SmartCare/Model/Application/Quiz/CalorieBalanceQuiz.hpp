#ifndef Cadabra_Queen_SmartCare_Model_Application_Quiz_CalorieBalanceQuiz_hpp
#define Cadabra_Queen_SmartCare_Model_Application_Quiz_CalorieBalanceQuiz_hpp

#include "../GenericQuiz.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class CalorieBalanceQuiz : public GenericQuiz
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CalorieBalanceQuiz(QObject* parent = nullptr);
		virtual ~CalorieBalanceQuiz() override;
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual bool onHandle() override;
		virtual bool onOpen() override;
	/** @} */
	};
} } } }

#endif
