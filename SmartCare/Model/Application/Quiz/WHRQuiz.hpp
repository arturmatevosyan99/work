#ifndef Cadabra_Queen_SmartCare_Model_Application_Quiz_WHRQuiz_hpp
#define Cadabra_Queen_SmartCare_Model_Application_Quiz_WHRQuiz_hpp

#include "../GenericQuiz.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class WHRQuiz : public GenericQuiz
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		WHRQuiz(QObject* parent = nullptr);
		virtual ~WHRQuiz() override;
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
