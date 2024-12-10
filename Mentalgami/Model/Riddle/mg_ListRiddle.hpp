#ifndef Cadabra_Queen_Mentalgami_Model_Question_ListRiddle_hpp
#define Cadabra_Queen_Mentalgami_Model_Question_ListRiddle_hpp

#include "../mg_AnswerModel.hpp"
#include "../mg_TemplatedRiddle.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class ListRiddle : public TemplatedRiddle<QSet<QString>>
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class PreferableComponent : std::int64_t
		{
			Empty = 1,
			CheckBox = 2,
			RadioButton = 3
		};

	private:
		class Validator : public RiddleValidator
		{
			/** @name Procedures */
			/** @{ */
			public:
				virtual bool validate(const QVariant& value) const override
				{
					return value.canConvert<QStringList>();
				}
			/** @} */
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(PreferableComponent)
		Q_PROPERTY(QAbstractListModel* answerModel READ getAnswerModel NOTIFY answerModelChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ListRiddle(const QString& text = QString(), const QString& descriptor = QString(), Validator* validator = new Validator());
		virtual ~ListRiddle() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QSharedPointer<Model::AnswerModel> _answer_model;
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
		virtual QString getType() const override;
		QAbstractListModel* getAnswerModel() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void answerModelChanged();
	/** @} */
	};
} } } }

#endif
