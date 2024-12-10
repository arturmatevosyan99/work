#ifndef Cadabra_Queen_SmartCare_Model_AbstractRiddle_hpp
#define Cadabra_Queen_SmartCare_Model_AbstractRiddle_hpp

#include "../Meta/RiddleInformation.hpp"
#include "RiddleValidator.hpp"

#include <QObject>
#include <QJsonValue>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class AbstractRiddle : public QObject
	{
	friend class GenericQuiz;

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString text READ getText NOTIFY textChanged)
		Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged)

		Q_PROPERTY(int component READ getComponent NOTIFY componentChanged)
		Q_PROPERTY(Cadabra::Queen::SmartCare::Meta::RiddleInformation::Specifications specifications READ getSpecifications NOTIFY specificationsChanged)

		Q_PROPERTY(QVector<QUrl> mediaList READ getMediaList WRITE setMediaList NOTIFY mediaListChanged)

		Q_PROPERTY(Cadabra::Queen::SmartCare::Model::RiddleValidator* validator READ getValidator CONSTANT)
		Q_PROPERTY(QVariant answer READ exportAnswer WRITE importAnswer NOTIFY answerChanged)

		Q_PROPERTY(bool isReadOnly READ isReadOnly NOTIFY specificationsChanged)
		Q_PROPERTY(bool isBusy READ isBusy NOTIFY busyChanged)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static bool isInt(const QJsonValue& value);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractRiddle(const QString& text = QString(), const QString& description = QString(), RiddleValidator* validator = new RiddleValidator());
		virtual ~AbstractRiddle() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _text;
		QString _description;

		qint64 _key;
		qint64 _component;
		QFlags<Meta::RiddleInformation::Specification> _specifications;

		QVector<QUrl> _media_list;

		RiddleValidator* _validator;
		bool _is_busy;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		/// @note Экспортируем/импортируем данные Riddle'а
		QJsonObject dump(bool all_inclusive = true) const;
		Q_INVOKABLE bool parse(const QJsonObject& body, bool all_inclusive = true);

		/// @note Экспортируем/импортируем только данные, специфические для конкретного Riddle'а
		virtual QJsonObject exportBody() const = 0;
		virtual bool importBody(const QJsonObject& body) = 0;

		/// @note Экспортируем/импортируем ответ Riddle'а
		virtual QVariant exportAnswer() const = 0;
		virtual bool importAnswer(const QVariant& answer) = 0;

		/// @note Валидируем и, в случае успешной валидации, импортируем ответ
		Q_INVOKABLE bool validate(const QVariant& answer);

		/// @note Возвращаем деволтное значение для типа ответа
		Q_INVOKABLE virtual QVariant makeDefault() const = 0;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getText() const;
		const QString& getDescription() const;

		int getComponent() const;
		QFlags<Meta::RiddleInformation::Specification> getSpecifications() const;

		const QVector<QUrl>& getMediaList() const;

		RiddleValidator* getValidator() const;
		Cadabra::Queen::Core::Meta::Qd getScheme() const;

		virtual QString getType() const = 0;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setText(const QString& text);
		void setDescription(const QString& description);

		void setSpecifications(const QFlags<Meta::RiddleInformation::Specification>& specifications);

		void setMediaList(const QVector<QUrl>& media_list);

	protected:
		virtual void setBusy(bool busy);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void textChanged();
		void descriptionChanged();

		void componentChanged();
		void specificationsChanged();

		void mediaListChanged();

		void answerChanged();
		void busyChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isReadOnly() const;
		virtual bool isBusy() const;
	/** @} */
	};
} } } }

#endif
