#include "GenericQuiz.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	GenericQuiz::GenericQuiz(QObject* parent)
	:
		Repository::Model::AbstractApplication(parent),

		_riddle_list(new Data::RiddleList(this)),
		_approximate_length(0)
	{

	}

	GenericQuiz::~GenericQuiz()
	{

	}

	void GenericQuiz::name()
	{
		QuizInterface::name();
	}

	void GenericQuiz::ask(int limit, int offset)
	{
		QuizInterface::ask(limit, offset)->react(std::bind(&GenericQuiz::onAsk, this, limit, offset, std::placeholders::_1));
	}

	void GenericQuiz::answer(AbstractRiddle* riddle)
	{
		riddle->setBusy(true);
		QuizInterface::answer(riddle)->react(std::bind(&GenericQuiz::onAnswer, this, riddle, std::placeholders::_1));
	}

	const QString& GenericQuiz::getTitle() const
	{
		return _title;
	}

	QAbstractListModel* GenericQuiz::getRiddleList() const
	{
		return _riddle_list;
	}

	int GenericQuiz::getApproximateLength() const
	{
		return _approximate_length;
	}

	void GenericQuiz::setTitle(const QString& title)
	{
		if (_title != title) {
			_title = title;
			emit titled();
		}
	}

	bool GenericQuiz::onHandle()
	{
		return true;
	}

	bool GenericQuiz::onOpen()
	{
		return true;
	}

	void GenericQuiz::onNamed(quint8 domain, quint32 code, const QString& message)
	{
		qWarning() << "Name error:" << domain << code << message;
	}

	void GenericQuiz::onNamed(const QString& retval)
	{
		if (_title != retval) {
			_title = retval;
			emit titled();
		}
	}

	void GenericQuiz::onAsk(int limit, int offset, const Core::Net::CRPReaction& reaction)
	{
		if (reaction) {
			if (!(*reaction).isArray()) {
				qWarning() << "Ask error: the received data is not an array";
				emit askFaulted("The received data is malformed");
				return ;
			}

			const QJsonArray pair((*reaction).toArray());

			if (pair.size() != 2) {
				qWarning() << "Ask error: the received data is not a pair";
				emit askFaulted("The received data is malformed");
				return ;
			}

			_approximate_length = _riddle_list->rowCount() + pair.at(1).toInt();
			emit approximateLengthChanged();

			emit asked(_riddle_list->append(Data::RiddleList::Parse(pair.at(0))));
		} else {
			qWarning() << "Ask error:" << reaction.getDomain() << reaction.getCode() << reaction.getMessage();
			emit askFaulted(reaction.getMessage());
		}
	}

	void GenericQuiz::onAnswer(AbstractRiddle* riddle, const Core::Net::CRPReaction& reaction)
	{
		if (reaction) {
			if (!(*reaction).isArray()) {
				qWarning() << "Answer error: the received data is not an array";
				emit answerFaulted("The received data is malformed");
				riddle->setBusy(false);
				return ;
			}

			const QJsonArray pair((*reaction).toArray());

			if (pair.size() != 2) {
				qWarning() << "Answer error: the received data is not a pair";
				emit answerFaulted("The received data is malformed");
				riddle->setBusy(false);
				return ;
			}

			_riddle_list->append(Data::RiddleList::Parse(pair.at(0)));
			emit answered(pair.at(1).toBool());
		} else {
			qWarning() << "Answer error:" << reaction.getDomain() << reaction.getCode() << reaction.getMessage();
			emit answerFaulted(reaction.getMessage());
		}

		riddle->setBusy(false);
	}

	bool GenericQuiz::isEmpty() const
	{
		return _riddle_list->isEmpty();
	}
} } } }
