#ifndef Cadabra_Queen_SmartCare_Model_Application_Quiz_BMIQuiz_hpp
#define Cadabra_Queen_SmartCare_Model_Application_Quiz_BMIQuiz_hpp

#include "../GenericQuiz.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class BMIQuiz : public GenericQuiz
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		BMIQuiz(QObject* parent = nullptr);
		virtual ~BMIQuiz() override;
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
