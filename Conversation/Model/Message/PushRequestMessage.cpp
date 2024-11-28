#include "PushRequestMessage.hpp"

#include "../../../Skeleton.hpp"
#include "../../../Core/System/Native.hpp"
#include "../../../Core/System/NotificationCenter.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	PushRequestMessage::PushRequestMessage(QObject* parent)
	:
		AbstractMessage(parent),

		_family(Core::System::NotificationCenter::Family::Unknown)
	{
		QObject::connect
		(
			Core::System::NotificationCenter::Instance(),
			&Core::System::NotificationCenter::registered,

			this,
			&PushRequestMessage::onRegistered
		);
	}

	void PushRequestMessage::requestToken()
	{
		if (!hasToken() && hasChannelDescriptor()) {
			Core::System::NotificationCenter::Instance()->requestToken();
		}
	}

	QJsonObject PushRequestMessage::exportBody() const
	{
		return
		{
			{"comment", _comment},
			{"agent", _agent},
			{"token", _token},
			{"service", static_cast<char>(_family)},
			{"subject_id", QString::number(_subject_id)}
		};
	}

	bool PushRequestMessage::importBody(const QJsonObject& body)
	{
		_agent = body["agent"].toString("");
		setComment(body["comment"].toString());
		setToken(body["token"].toString());
		_subject_id = body["subject_id"].toString("0").toLongLong();

		return true;
	}

	QString PushRequestMessage::getPreviewText() const
	{
		return QString("[%1]").arg(tr("Push Request"));
	}

	QString PushRequestMessage::getComment() const
	{
		return _comment;
	}

	QString PushRequestMessage::getAgent() const
	{
		return _agent;
	}

	QString PushRequestMessage::getToken() const
	{
		return _token;
	}

	Repository::Meta::Subject::Id PushRequestMessage::getSubjectId() const
	{
		return _subject_id;
	}

	void PushRequestMessage::setComment(const QString& comment)
	{
		if (_comment != comment) {
			_comment = comment;
			emit commentChanged();
		}
	}

	void PushRequestMessage::setToken(const QString& token)
	{
		if (_token != token) {
			_token = token;
			emit tokenChanged();
		}
	}

	bool PushRequestMessage::hasToken() const
	{
		return !_token.isEmpty();
	}

	void PushRequestMessage::onRegistered(QString token)
	{
		if (_token != token && !hasToken() && getChannelDescriptor()->getSubjectId() == _subject_id) {
			_agent = Core::System::Native::Instance()->getBundleIdentifier();
			_family = Core::System::NotificationCenter::Instance()->getFamily();
			_token = Core::System::NotificationCenter::Instance()->getToken();
			emit tokenChanged();
			emit modified();
		}
	}
} } } }
