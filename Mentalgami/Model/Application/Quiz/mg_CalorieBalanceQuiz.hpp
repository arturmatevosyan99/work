#ifndef Cadabra_Queen_Mentalgami_Model_Application_Quiz_CalorieBalanceQuiz_hpp
#define Cadabra_Queen_Mentalgami_Model_Application_Quiz_CalorieBalanceQuiz_hpp

#include "../mg_GenericQuiz.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
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
