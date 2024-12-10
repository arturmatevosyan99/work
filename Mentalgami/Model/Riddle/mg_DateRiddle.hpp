#ifndef Cadabra_Queen_Mentalgami_Model_Question_DateRiddle_hpp
#define Cadabra_Queen_Mentalgami_Model_Question_DateRiddle_hpp

#include "../mg_RiddleRangeValidator.hpp"
#include "../mg_TemplatedRiddle.hpp"

#include "../../../Qaos/Date.hpp"

#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class DateRiddle : public TemplatedRiddle<Qaos::Date>
	{
	/** @name Classes */
	/** @{ */
	private:
		class Validator : public RiddleRangeValidator<Qaos::Date>
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
						qWarning() << "DateRiddle::Validator::import > Something is wrong with the 'default_effect' parameter, the value has not changed";
						return false;
					}

					const QJsonValue limit_set(object["limit_set"]);
					if (limit_set.isArray()) {
						const QJsonArray limit_set_array(limit_set.toArray());
						for (QJsonArray::ConstIterator i = limit_set_array.constBegin(); i != limit_set_array.constEnd(); i++) {
							if (i->isArray()) {
								const QJsonArray date_object_pair(i->toArray());
								if (date_object_pair.at(0).isObject() && date_object_pair.at(1).isObject()) {
									const QJsonObject first_date_object(date_object_pair.at(0).toObject());
									const QJsonObject second_date_object(date_object_pair.at(1).toObject());
									if (isDateObjectValid(first_date_object) && isDateObjectValid(second_date_object)) {
										const Qaos::Date first_date(first_date_object["day"].toInt(), first_date_object["month"].toInt(), first_date_object["year"].toInt());
										const Qaos::Date second_date(second_date_object["day"].toInt(), second_date_object["month"].toInt(), second_date_object["year"].toInt());
										_limit_set.insert(qMakePair(first_date, second_date));
									} else {
										qWarning() << "DateRiddle::Validator::import > Something is wrong with the date objects of the 'limit_set' parameter, the value has not changed";
										return false;
									}
								} else {
									qWarning() << "DateRiddle::Validator::import > Something is wrong with the elements of the pair of the 'limit_set' parameter, the pair is not inserted";
									return false;
								}
							} else {
								qWarning() << "DateRiddle::Validator::import > Something is wrong with the element of the 'limit_set', it is not inserted";
								return false;
							}
						}
					} else {
						qWarning() << "DateRiddle::Validator::import > Something is wrong with the 'limit_set' parameter, new elements are not inserted";
						return false;
					}

					const QJsonValue border_pair(object["border_pair"]);
					if (border_pair.isArray()) {
						const QJsonArray date_object_pair(border_pair.toArray());
						if (date_object_pair.at(0).isObject() && date_object_pair.at(1).isObject()) {
							const QJsonObject first_date_object(date_object_pair.at(0).toObject());
							const QJsonObject second_date_object(date_object_pair.at(1).toObject());
							if (isDateObjectValid(first_date_object) && isDateObjectValid(second_date_object)) {
								const Qaos::Date first_date(first_date_object["day"].toInt(), first_date_object["month"].toInt(), first_date_object["year"].toInt());
								const Qaos::Date second_date(second_date_object["day"].toInt(), second_date_object["month"].toInt(), second_date_object["year"].toInt());
								_border_pair = qMakePair(first_date, second_date);
								emit borderChanged();
							} else {
								qWarning() << "DateRiddle::Validator::import > Something is wrong with the date objects of the 'border_pair' parameter, the value has not changed";
								return false;
							}
						} else {
							qWarning() << "DateRiddle::Validator::import > Something is wrong with the elements of the 'border_pair' parameter, the value has not changed";
							return false;
						}
					} else {
						qWarning() << "DateRiddle::Validator::import > Something is wrong with the 'border_pair' parameter, the value has not changed";
						return false;
					}

					const QJsonValue default_value(object["default_value"]);
					if (default_value.isObject()) {
						const QJsonObject date_object(default_value.toObject());
						if (isDateObjectValid(date_object)) {
							_default_value = Qaos::Date(date_object["day"].toInt(), date_object["month"].toInt(), date_object["year"].toInt());
							emit defaultValueChanged();
						} else {
							qWarning() << "DateRiddle::Validator::import > Something is wrong with the date object of the 'default_value' parameter, the value has not changed";
							return false;
						}
					} else {
						qWarning() << "DateRiddle::Validator::import > Something is wrong with the 'default_value' parameter, the value has not changed";
						return false;
					}

					const QJsonValue step_size(object["step_size"]);
					if (step_size.isObject()) {
						const QJsonObject date_object(step_size.toObject());
						if (isDateObjectValid(date_object)) {
							_default_value = Qaos::Date(date_object["day"].toInt(), date_object["month"].toInt(), date_object["year"].toInt());
							emit stepSizeChanged();
						} else {
							qWarning() << "DateRiddle::Validator::import > Something is wrong with the date object of the 'step_size' parameter, the value has not changed";
							return false;
						}
					} else {
						qWarning() << "DateRiddle::Validator::import > Something is wrong with the 'step_size' parameter, the value has not changed";
						return false;
					}

					return true;
				}

				virtual bool validate(const QVariant& value) const override
				{
					Qaos::Date date;

					if (value.isValid() && value.canConvert<Qaos::Date>()) {
						date = value.value<Qaos::Date>();
					} else if (value.isValid() && value.canConvert<QDate>()) {
						date = Qaos::Date(value.value<QDate>().day(), value.value<QDate>().month(), value.value<QDate>().year());
					} else {
						return false;
					}

					bool valid_flag(false);

					for (const QPair<Qaos::Date, Qaos::Date>& pair : qAsConst(_limit_set)) {
						if (_default_effect) {
							if ((pair.first <= date) && (date <= pair.second)) {
								valid_flag = true;
								break;
							}
						} else {
							valid_flag = true;
							if ((pair.first <= date) && (date <= pair.second)) {
								valid_flag = false;
								break;
							}
						}
					}

					return valid_flag;
				}

			private:
				bool isDateObjectValid(const QJsonObject& object) const
				{
					return (object.size() == 3) && isInt(object["day"]) && isInt(object["month"]) && isInt(object["year"]);
				}
			/** @} */
		};

	/** @} */
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		DateRiddle(const QString& text = QString(), const QString& descriptor = QString(), Validator* validator = new Validator());
		virtual ~DateRiddle() override = default;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getType() const override;
	/** @} */
	};
} } } }

#endif
