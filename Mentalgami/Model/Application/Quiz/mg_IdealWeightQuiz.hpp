#ifndef Cadabra_Queen_Mentalgami_Model_Application_Quiz_IdealWeightQuiz_hpp
#define Cadabra_Queen_Mentalgami_Model_Application_Quiz_IdealWeightQuiz_hpp

#include "../mg_GenericQuiz.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class IdealWeightQuiz : public GenericQuiz
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		IdealWeightQuiz(QObject* parent = nullptr);
		virtual ~IdealWeightQuiz() override;
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
