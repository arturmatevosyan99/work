#ifndef Cadabra_Queen_Mentalgami_Model_Riddle_IntegerRiddle_hpp
#define Cadabra_Queen_Mentalgami_Model_Riddle_IntegerRiddle_hpp

#include "../RiddleRangeValidator.hpp"
#include "../TemplatedRiddle.hpp"

#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class IntegerRiddle : public TemplatedRiddle<int>
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
		class Validator : public RiddleRangeValidator<int>
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
						qWarning() << "IntegerRiddle::Validator::import > Something is wrong with the 'default_effect' parameter, the value has not changed";
						return false;
					}

					const QJsonValue limit_set(object["limit_set"]);
					if (limit_set.isArray()) {
						const QJsonArray limit_set_array(limit_set.toArray());
						for (QJsonArray::ConstIterator i = limit_set_array.constBegin(); i != limit_set_array.constEnd(); i++) {
							if (i->isArray()) {
								const QJsonArray number_pair(i->toArray());
								if (isInt(number_pair.at(0)) && isInt(number_pair.at(1))) {
									_limit_set.insert(qMakePair(number_pair.at(0).toInt(), number_pair.at(1).toInt()));
								} else {
									qWarning() << "IntegerRiddle::Validator::import > Something is wrong with the elements of the pair of the 'limit_set' parameter, the pair is not inserted";
									return false;
								}
							} else {
								qWarning() << "IntegerRiddle::Validator::import > Something is wrong with the element of the 'limit_set', it is not inserted";
								return false;
							}
						}
					} else {
						qWarning() << "IntegerRiddle::Validator::import > Something is wrong with the 'limit_set' parameter, new elements are not inserted";
						return false;
					}

					const QJsonValue border_pair(object["border_pair"]);
					if (border_pair.isArray()) {
						const QJsonArray border_pair_array(border_pair.toArray());
						if (isInt(border_pair_array.at(0)) && isInt(border_pair_array.at(1))) {
							_border_pair = qMakePair(border_pair_array.at(0).toInt(), border_pair_array.at(1).toInt());
							emit borderChanged();
						} else {
							qWarning() << "IntegerRiddle::Validator::import > Something is wrong with the elements of the 'border_pair' parameter, the value has not changed";
							return false;
						}
					} else {
						qWarning() << "IntegerRiddle::Validator::import > Something is wrong with the 'border_pair' parameter, the value has not changed";
						return false;
					}

					const QJsonValue default_value(object["default_value"]);
					if (isInt(default_value)) {
						_default_value = default_value.toInt();
						emit defaultValueChanged();
					} else {
						qWarning() << "IntegerRiddle::Validator::import > Something is wrong with the 'default_value' parameter, the value has not changed";
						return false;
					}

					const QJsonValue step_size(object["step_size"]);
					if (isInt(step_size)) {
						_step_size = step_size.toInt();
						emit stepSizeChanged();
					} else {
						qWarning() << "IntegerRiddle::Validator::import > Something is wrong with the 'step_size' parameter, the value has not changed";
						return false;
					}

					return true;
				}

				virtual bool validate(const QVariant& value) const override
				{
					if (value.canConvert<int>()) {
						bool valid_flag(false);
						for (const QPair<int, int>& pair : qAsConst(_limit_set)) {
							if (_default_effect) {
								if ((pair.first <= value.toInt()) && (value.toInt() <= pair.second)) {
									valid_flag = true;
									break;
								}
							} else {
								valid_flag = true;
								if ((pair.first <= value.toInt()) && (value.toInt() <= pair.second)) {
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
		IntegerRiddle(const QString& text = QString(), const QString& descriptor = QString(), Validator* validator = new Validator());
		virtual ~IntegerRiddle() override = default;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getType() const override;
	/** @} */
	};
} } } }

#endif
