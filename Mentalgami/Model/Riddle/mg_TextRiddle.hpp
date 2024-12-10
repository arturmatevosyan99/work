#ifndef Cadabra_Queen_Mentalgami_Model_Question_TextRiddle_hpp
#define Cadabra_Queen_Mentalgami_Model_Question_TextRiddle_hpp

#include "../mg_TemplatedRiddle.hpp"
#include "../mg_RiddleValidator.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class TextRiddle : public TemplatedRiddle<QString>
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class PreferableComponent : std::int64_t
		{
			TextField = 1,
			TextArea = 2
		};

	private:
		class Validator : public RiddleValidator
		{
			/** @name Propreties */
			/** @{ */
			private:
				QScopedPointer<QString> _validator;
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
					} else if (validator.isString()) {
						_validator.reset(new QString(validator.toString()));
						return true;
					} else {
						qWarning() << "TextRiddle::Validator::import > Something is wrong with the 'validator' parameter, the value has not changed";
						return false;
					}
				}

				virtual bool validate(const QVariant& value) const override
				{
					if (_validator.isNull()) {
						return true;
					}

					if (value.canConvert<QString>()) {
						QRegularExpression re(*_validator);
						QRegularExpressionMatch match = re.match(value.toString());
						return match.hasMatch();
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
		Q_PROPERTY(QString placeholder READ getPlaceholder NOTIFY placeholderChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TextRiddle(const QString& text = QString(), const QString& descriptor = QString(), Validator* validator = new Validator());
		virtual ~TextRiddle() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _placeholder;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& body) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getPlaceholder() const;
		virtual QString getType() const override;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void placeholderChanged();
	/** @} */
	};
} } } }

#endif
