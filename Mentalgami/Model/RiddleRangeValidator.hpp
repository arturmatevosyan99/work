#ifndef Cadabra_Queen_Mentalgami_Model_RiddleRangeValidator_hpp
#define Cadabra_Queen_Mentalgami_Model_RiddleRangeValidator_hpp

#include "AbstractRangeValidator.hpp"

#include <QObject>
#include <QVariant>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	template <typename T>
	class RiddleRangeValidator : public AbstractRangeValidator
	{
	/** @name Constructors */
	/** @{ */
	public:
		RiddleRangeValidator()
		:
			AbstractRangeValidator(),

			_default_effect(true)
		{

		}

		virtual ~RiddleRangeValidator() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		bool _default_effect;
		QSet<QPair<T, T>> _limit_set;

		T _default_value;
		QPair<T, T> _border_pair;
		T _step_size;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QVariant getDefaultValue() const override
		{
			return QVariant::fromValue(_default_value);
		}

		virtual QVariant getMinimum() const override
		{
			return QVariant::fromValue(_border_pair.first);
		}

		virtual QVariant getMaximum() const override
		{
			return QVariant::fromValue(_border_pair.second);
		}

		virtual QVariant getStepSize() const override
		{
			return QVariant::fromValue(_step_size);
		}
	/** @} */
	};
} } } }

#endif
