#include "CRPCall.hpp"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <QUuid>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	CRPCall::CRPCall(Core::Net::Endpoint* endpoint, const QString& reflector, const QString& path)
	:
		ServiceCall(endpoint, reflector),

		_path(path)
	{
	}

	CRPCall::CRPCall(Core::Net::Endpoint* endpoint, const QString& reflector)
	:
		ServiceCall(endpoint, reflector),

		_path("")
	{
	}

	CRPCall::~CRPCall()
	{

	}

	void CRPCall::act(const QString& interface, const QString& method, const QJsonObject& payload)
	{
		act(
			{
				{"moniker", ""},
				{
					"reflector",
					QJsonObject
					{
						{"interface", interface},
						{"method", method}
					},
				},
				{"payload", payload}
			}
		);
	}

	void CRPCall::act(const QString& method, const QJsonObject& payload)
	{
		act(
			{
				{"moniker", ""},
				{"reflector", method},
				{"payload", payload}
			}
		);
	}

	void CRPCall::act(qint64 handle, const QString& interface, const QString& method, const QJsonObject& payload)
	{
		act(
			{
				{"moniker", QString::number(handle)},
				{
					"reflector",
					QJsonObject
					{
						{"interface", interface},
						{"method", method}
					},
				},
				{"payload", payload}
			}
		);
	}

	void CRPCall::act(qint64 handle, const QString& method, const QJsonObject& payload)
	{
		act(
			{
				{"moniker", QString::number(handle)},
				{"reflector", method},
				{"payload", payload}
			}
		);
	}

	void CRPCall::act(const QJsonObject& action)
	{
		if (!hasEndpoint()) {
			emit excepted(0, 0, "No valid endpoint");
			qWarning("[0, 0] No valid endpoint");
			return ;
		}
		post(
				QJsonDocument
				{
					QJsonObject
					{
						{"timepoint", QString::number(QDateTime::currentSecsSinceEpoch())},
						{"dialect", 1},
						{"id", QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces)},
						{"path", _path},
						{"body", QJsonArray{action}}
					}
				}
		);
	}

	void CRPCall::onComplete()
	{
		emit reflected();
	}

	void CRPCall::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		QJsonParseError error;
		const QJsonDocument document(QJsonDocument::fromJson(reply->readAll(), &error));
		if (error.error != QJsonParseError::NoError || !document.isObject()) {
			onReaction(reply, QUuid{}, 0, 0, 1, error.errorString().prepend("Data error: "));
			return ;
		}

		const QJsonObject response(document.object());

		const QUuid id(response.value("id").toString(""));

		QJsonObject::const_iterator b(response.find("body"));
		if (response.constEnd() == b || !b->isArray()) {
			onReaction(reply, id, 0, 0, 2, "Body is not an array of reactions");
			return ;
		}

		const QJsonArray body(b.value().toArray());
		for (int r = 0, count = body.count(); r < count; ++r) {
			const QJsonObject reaction(body[r].toObject());
			QJsonObject::const_iterator s(reaction.find("state"));
			if (reaction.constEnd() == s || !s->isDouble()) {
				onReaction(reply, id, r, 0, 3, "Reaction has not have a state");
				continue;
			}

			QJsonObject::const_iterator p(reaction.find("payload"));
			const quint8 state(static_cast<quint8>(s.value().toInt()));
			if (0 == state) {
				if (reaction.constEnd() == p) {
					onReaction(reply, id, r, 0, 4, "Payload is missed");
					continue;
				}
				onReaction(reply, id, r, p.value());
			} else {
				const QJsonObject error(p.value().toObject());
				if (reaction.constEnd() == p) {
					onReaction(reply, id, r, state, 0, "Error is defective because payload is missed");
				} else {
					onReaction(reply, id, r, state, error.value("code").toInt(-1), error.value("message").toString());
				}
			}
		}
	}

	void CRPCall::onFault(QSharedPointer<QNetworkReply> reply)
	{
		onReaction(reply, QUuid{}, 0, 0, 1000 + static_cast<quint32>(reply->error()), reply->errorString().prepend("Network error: "));
	}

	void CRPCall::onReaction(const QSharedPointer<QNetworkReply>& reply, const QUuid& id, qsizetype index, quint8 domain, quint32 code, const QString& message)
	{
		/// @todo Если UID == null Вытащить UID у reply->request();
		qWarning("[%d, %d] %s", domain, code, qUtf8Printable(message));
		emit excepted(domain, code, message);
	}

	void CRPCall::onReaction(const QSharedPointer<QNetworkReply>& reply, const QUuid& id, qsizetype index, const QJsonValue& payload)
	{
		emit reacted(payload);
	}
} } } }
