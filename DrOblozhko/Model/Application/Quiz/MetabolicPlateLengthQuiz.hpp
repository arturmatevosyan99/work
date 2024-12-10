#ifndef Cadabra_Queen_DrOblozhko_Model_Application_Quiz_MetabolicPlateLengthQuiz_hpp
#define Cadabra_Queen_DrOblozhko_Model_Application_Quiz_MetabolicPlateLengthQuiz_hpp

#include "../../../../SmartCare/Model/Application/GenericQuiz.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	class MetabolicPlateLengthQuiz : public SmartCare::Model::GenericQuiz
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		MetabolicPlateLengthQuiz(QObject* parent = nullptr);
		virtual ~MetabolicPlateLengthQuiz() override;
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
