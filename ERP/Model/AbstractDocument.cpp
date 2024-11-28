#include "AbstractDocument.hpp"

namespace Cadabra { namespace Queen { namespace ERP { namespace Model {
    AbstractDocument::AbstractDocument(QObject* parent)
    :
		QObject(parent),

		_state(ERP::DocumentState::Unknown),
		_direction(ERP::DocumentDirection::Unknown),
		_id(0),
		_no(0)
    {

	}

	QJsonObject AbstractDocument::compose() const
	{
		if (0 == _id) {
			return {};
		}
		return {
				{"id", QString::number(_id)},
				{"no", QString::number(_no)},
				{"journal_id", QString::number(_journal_id)},
				{"provoker_id", QString::number(_provoker_subject_id)},
				{"state", static_cast<quint8>(_state)},
				{"direction", static_cast<quint8>(_direction)},
				{"issue_timestamp", _issue_timestamp.toSecsSinceEpoch()},
				{"modification_timestamp", _modification_timestamp.toSecsSinceEpoch()},
//				{"scheme", },
				{"title", _title},
				{"document", dump()},
				{"comment", _comment}
		};
	}

	bool AbstractDocument::parse(const QJsonObject& body)
	{
		/// @todo check scheme
		_journal_id = body["journal_id"].toString("0").toLongLong();
		_id = body["id"].toString("0").toLongLong();
		_no = body["no"].toString("0").toLongLong();
		_issue_timestamp = QDateTime::fromSecsSinceEpoch(body["issue_timestamp"].toInt(0), Qt::UTC);
		_modification_timestamp = QDateTime::fromSecsSinceEpoch(body["modification_timestamp"].toInt(0), Qt::UTC);
		if (_no == 0) {
			_no = _id;
		}
		_provoker_subject_id = body["provoker_id"].toString("0").toLongLong();
		setTitle(body["title"].toString());
		setComment(body["comment"].toString());
		setState(static_cast<DocumentState>(body["state"].toInt(48)));
		_direction = static_cast<DocumentDirection>(body["direction"].toInt(48));
		if (scan(body.value("document").toObject())) {
			emit parsed();
			return true;
		}
		_state = ERP::DocumentState::Unknown;
		_direction = ERP::DocumentDirection::Unknown;
		_id = 0;
		_journal_id = 0;
		return false;
	}

	QJsonObject AbstractDocument::dump() const
	{
		return {};
	}

	bool AbstractDocument::scan(const QJsonObject& body)
	{
		return false;
	}

	QString AbstractDocument::getDescription() const
	{
		return tr("Document") + ((_no > 0 && _no != !_id) ?  " №" + QString::number(_no) : " #" + QString::number(_id));
	}

	Repository::Meta::Object::Id AbstractDocument::getJournalId() const
	{
		return _journal_id;
	}

	Repository::Meta::Essence::Id AbstractDocument::getId() const
	{
		return _id;
	}

	Repository::Meta::Essence::Id AbstractDocument::getNo() const
	{
		return _no;
	}

	Repository::Meta::Subject::Id AbstractDocument::getProvokerSubjectId() const
	{
		return _provoker_subject_id;
	}

	QString AbstractDocument::getTitle() const
	{
		return _title;
	}

	QString AbstractDocument::getComment() const
	{
		return _comment;
	}

	ERP::DocumentState AbstractDocument::getState() const
	{
		return _state;
	}

	ERP::DocumentDirection AbstractDocument::getDirection() const
	{
		return _direction;
	}

	const QDateTime& AbstractDocument::getIssueDate() const
	{
		return _issue_timestamp;
	}

	const QDateTime& AbstractDocument::getModificationDate() const
	{
		return _modification_timestamp;
	}

	void AbstractDocument::setTitle(const QString& title)
	{
		if (_title != title) {
			_title = title;
			emit titleChanged();
		}
	}

	void AbstractDocument::setComment(const QString& comment)
	{
		if (_comment != comment) {
			_comment = comment;
			emit commentChanged();
		}
	}

	void AbstractDocument::setState(const ERP::DocumentState& state)
	{
		if (_state != state) {
			_state = state;
			emit stateChanged();
		}
	}

	bool AbstractDocument::isHeld() const
	{
		return ERP::DocumentState::Held == _state;
	}
} } } }
