#ifndef Cadabra_Queen_Mentalgami_Model_AbstractRangeValidator_hpp
#define Cadabra_Queen_Mentalgami_Model_AbstractRangeValidator_hpp

#include "mg_RiddleValidator.hpp"

#include <QObject>
#include <QVariant>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class AbstractRangeValidator : public RiddleValidator
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QVariant defaultValue READ getDefaultValue NOTIFY defaultValueChanged)
		Q_PROPERTY(QVariant minimum READ getMinimum NOTIFY borderChanged)
		Q_PROPERTY(QVariant maximum READ getMaximum NOTIFY borderChanged)
		Q_PROPERTY(QVariant stepSize READ getStepSize NOTIFY stepSizeChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		using RiddleValidator::RiddleValidator;
		virtual ~AbstractRangeValidator() override = default;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QVariant getDefaultValue() const = 0;
		virtual QVariant getMinimum() const = 0;
		virtual QVariant getMaximum() const = 0;
		virtual QVariant getStepSize() const = 0;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void defaultValueChanged();
		void borderChanged();
		void stepSizeChanged();
	/** @} */
	};
} } } }

#endif
