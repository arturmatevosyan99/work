#ifndef Cadabra_Queen_SmartCare_Model_Question_BoolRiddle_hpp
#define Cadabra_Queen_SmartCare_Model_Question_BoolRiddle_hpp

#include "../TemplatedRiddle.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class BoolRiddle : public TemplatedRiddle<bool>
	{
	/** @name Classes */
	/** @{ */
	private:
		class Validator : public RiddleValidator
		{
			/** @name Propreties */
			/** @{ */
			private:
				QScopedPointer<bool> _validator;
			/** @} */

			/** @name Procedures */
			/** @{ */
			public:
				virtual bool import(const QJsonObject& object) override
				{
					const QJsonValue validator(object["validator"]);

					if (validator.isNull()) {
						_validator.reset();
						return true;
					} else if (validator.isBool()) {
						_validator.reset(new bool(validator.toBool()));
						return true;
					} else {
						_validator.reset();
						qWarning() << "BoolRiddle::Validator::import > Something is wrong with the 'validator' parameter, the value has not changed";
						return false;
					}
				}

				virtual bool validate(const QVariant& value) const override
				{
					if (_validator.isNull()) {
						return true;
					}

					if (value.canConvert<bool>()) {
						return value.toBool() == *_validator;
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
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		BoolRiddle(const QString& text = QString(), const QString& descriptor = QString(), Validator* validator = new Validator());
		virtual ~BoolRiddle() override = default;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getType() const override;
	/** @} */
	};
} } } }

#endif
