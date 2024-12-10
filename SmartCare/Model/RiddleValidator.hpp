#ifndef Cadabra_Queen_SmartCare_Model_RiddleValidator_hpp
#define Cadabra_Queen_SmartCare_Model_RiddleValidator_hpp

#include <QObject>
#include <QVariant>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class RiddleValidator : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		RiddleValidator() = default;
		virtual ~RiddleValidator() = default;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool import(const QJsonObject& object);
		Q_INVOKABLE virtual bool validate(const QVariant& value) const;
	/** @} */
	};
} } } }

#endif
