#ifndef Cadabra_Queen_Mentalgami_Model_Question_NumericRiddle_hpp
#define Cadabra_Queen_Mentalgami_Model_Question_NumericRiddle_hpp

#include "../RiddleRangeValidator.hpp"
#include "../TemplatedRiddle.hpp"

#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class NumericRiddle : public TemplatedRiddle<double>
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class PreferableComponent : std::int64_t
		{
			Tumbler = 1,
			SpinBox = 2,
			Slider = 3,
			TextField = 4
		};

	private:
		class Validator : public RiddleRangeValidator<double>
		{
			/** @name Procedures */
			/** @{ */
			public:
				virtual bool import(const QJsonObject& object) override
				{
					const QJsonValue default_effect(object["default_effect"]);
					if (default_effect.isBool()) {
						_default_effect = default_effect.toBool();
					} else {
						qWarning() << "NumericRiddle::Validator::import > Something is wrong with the 'default_effect' parameter, the value has not changed";
						return false;
					}

					const QJsonValue limit_set(object["limit_set"]);
					if (limit_set.isArray()) {
						const QJsonArray limit_set_array(limit_set.toArray());
						for (QJsonArray::ConstIterator i = limit_set_array.constBegin(); i != limit_set_array.constEnd(); i++) {
							if (i->isArray()) {
								const QJsonArray number_pair(i->toArray());
								if (number_pair.at(0).isDouble() && number_pair.at(1).isDouble()) {
									_limit_set.insert(qMakePair(number_pair.at(0).toDouble(), number_pair.at(1).toDouble()));
								} else {
									qWarning() << "NumericRiddle::Validator::import > Something is wrong with the elements of the pair of the 'limit_set' parameter, the pair is not inserted";
									return false;
								}
							} else {
								qWarning() << "NumericRiddle::Validator::import > Something is wrong with the element of the 'limit_set', it is not inserted";
								return false;
							}
						}
					} else {
						qWarning() << "NumericRiddle::Validator::import > Something is wrong with the 'limit_set' parameter, new elements are not inserted";
						return false;
					}

					const QJsonValue border_pair(object["border_pair"]);
					if (border_pair.isArray()) {
						const QJsonArray border_pair_array(border_pair.toArray());
						if (border_pair_array.at(0).isDouble() && border_pair_array.at(1).isDouble()) {
							_border_pair = qMakePair(border_pair_array.at(0).toDouble(), border_pair_array.at(1).toDouble());
							emit borderChanged();
						} else {
							qWarning() << "NumericRiddle::Validator::import > Something is wrong with the elements of the 'border_pair' parameter, the value has not changed";
							return false;
						}
					} else {
						qWarning() << "NumericRiddle::Validator::import > Something is wrong with the 'border_pair' parameter, the value has not changed";
						return false;
					}

					const QJsonValue default_value(object["default_value"]);
					if (default_value.isDouble()) {
						_default_value = default_value.toDouble();
						emit defaultValueChanged();
					} else {
						qWarning() << "NumericRiddle::Validator::import > Something is wrong with the 'default_value' parameter, the value has not changed";
						return false;
					}

					const QJsonValue step_size(object["step_size"]);
					if (step_size.isDouble()) {
						_step_size = step_size.toDouble();
						emit stepSizeChanged();
					} else {
						qWarning() << "NumericRiddle::Validator::import > Something is wrong with the 'step_size' parameter, the value has not changed";
						return false;
					}

					return true;
				}

				virtual bool validate(const QVariant& value) const override
				{
					if (value.canConvert<double>()) {
						bool valid_flag(false);
						for (const QPair<double, double>& pair : qAsConst(_limit_set)) {
							if (_default_effect) {
								if ((pair.first <= value.toDouble()) && (value.toDouble() <= pair.second)) {
									valid_flag = true;
									break;
								}
							} else {
								valid_flag = true;
								if ((pair.first <= value.toDouble()) && (value.toDouble() <= pair.second)) {
									valid_flag = false;
									break;
								}
							}
						}
						return valid_flag;
					} else {
						return false;
					}
				}
			/** @} */
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(PreferableComponent)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		NumericRiddle(const QString& text = QString(), const QString& descriptor = QString(), Validator* validator = new Validator());
		virtual ~NumericRiddle() override = default;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getType() const override;
	/** @} */
	};
} } } }

#endif
