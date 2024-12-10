#ifndef Cadabra_Queen_Mentalgami_Model_Application_Quiz_BMIQuiz_hpp
#define Cadabra_Queen_Mentalgami_Model_Application_Quiz_BMIQuiz_hpp

#include "../mg_GenericQuiz.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
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
