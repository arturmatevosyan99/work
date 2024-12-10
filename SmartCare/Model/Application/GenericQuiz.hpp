#ifndef Cadabra_Queen_SmartCare_Model_Application_GenericQuiz_hpp
#define Cadabra_Queen_SmartCare_Model_Application_GenericQuiz_hpp

#include "../../../Repository/Model/AbstractApplication.hpp"

#include "Interface/QuizInterface.hpp"
#include "../../Data/RiddleList.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class GenericQuiz : public Repository::Model::AbstractApplication, Cadabra::Queen::SmartCare::Model::QuizInterface
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_INTERFACES(Cadabra::Queen::SmartCare::Model::QuizInterface)
		Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titled)
		Q_PROPERTY(QAbstractListModel* riddleList READ getRiddleList CONSTANT)
		Q_PROPERTY(int approximateLength READ getApproximateLength NOTIFY approximateLengthChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		GenericQuiz(QObject* parent = nullptr);
		virtual ~GenericQuiz() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _title;
		Data::RiddleList* const _riddle_list;
		int _approximate_length;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void name();
		Q_INVOKABLE void ask(int limit = 1, int offset = 0);
		Q_INVOKABLE void answer(Cadabra::Queen::SmartCare::Model::AbstractRiddle* riddle);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getTitle() const;
		QAbstractListModel* getRiddleList() const;
		int getApproximateLength() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setTitle(const QString& title);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void titled();
		void asked(bool there_is_more);
		void askFaulted(const QString& message);
		void answered(bool there_is_more);
		void answerFaulted(const QString& message);
		void approximateLengthChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual bool onHandle() override;
		virtual bool onOpen() override;

		virtual void onNamed(quint8 domain, quint32 code, const QString& message) override;
		virtual void onNamed(const QString& retval) override;

	private:
		void onAsk(int limit, int offset, const Core::Net::CRPReaction& reaction);
		void onAnswer(AbstractRiddle* riddle, const Core::Net::CRPReaction& reaction);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		Q_INVOKABLE bool isEmpty() const;
	/** @} */
	};
} } } }

#endif
