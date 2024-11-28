#ifndef Cadabra_Queen_Mentalgami_Model_Question_TemplatedRiddle_hpp
#define Cadabra_Queen_Mentalgami_Model_Question_TemplatedRiddle_hpp

#include "AbstractRiddle.hpp"

#include "../../Qaos/Date.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	template <typename T>
	class TemplatedRiddle : public AbstractRiddle
	{
	/** @name Constructors */
	/** @{ */
	public:
		TemplatedRiddle(const QString& text, const QString& description, RiddleValidator* validator = new RiddleValidator())
		:
			AbstractRiddle(text, description, validator)
		{

		}

		virtual ~TemplatedRiddle() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		T _answer;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual QVariant makeDefault() const override
		{
			return QVariant::fromValue(T{});
		}

		virtual QJsonObject exportBody() const override
		{
			return QJsonObject();
		}

		virtual bool importBody(const QJsonObject& body) override
		{
			return true;
		}

		virtual QVariant exportAnswer() const override
		{
			return _answer;
		}

		virtual bool importAnswer(const QVariant& answer) override
		{
			if (_answer != answer) {
				_answer = answer;
				emit answerChanged();
			}

			return true;
		}
	/** @} */
	};

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<QString>::importAnswer(const QVariant& answer);

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<Qaos::Date>::importAnswer(const QVariant& answer);

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<bool>::importAnswer(const QVariant& answer);

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<std::nullptr_t>::importAnswer(const QVariant& answer);

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<double>::importAnswer(const QVariant& answer);

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<QSet<QString>>::importAnswer(const QVariant& answer);

	template<>
	bool Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<int>::importAnswer(const QVariant& answer);

	template<>
	QVariant Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<Qaos::Date>::exportAnswer() const;

	template<>
	QVariant Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<std::nullptr_t>::exportAnswer() const;

	template<>
	QVariant Cadabra::Queen::Mentalgami::Model::TemplatedRiddle<QSet<QString>>::exportAnswer() const;
} } } }

#endif
