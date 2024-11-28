#include "CRPDeferredCall.hpp"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <QUuid>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	const QUuid CRPDeferredCall::NullUID = {};

	CRPDeferredCall::CRPDeferredCall(Core::Net::Endpoint* endpoint, const QString& reflector, const QString& path)
	:
		CRPCall(endpoint, reflector, path)
	{
	}

	CRPDeferredCall::CRPDeferredCall(Core::Net::Endpoint* endpoint, const QString& reflector)
	:
		CRPCall(endpoint, reflector)
	{
	}

	CRPDeferredCall::~CRPDeferredCall()
	{

	}

	void CRPDeferredCall::act()
	{
		QMap<QUuid, QVector<QSharedPointer<Core::Net::CRPFuture>>>::Iterator d(_future_map.find(NullUID));
		QVector<QSharedPointer<CRPFuture>> vector;
		d->swap(vector);
		act(std::move(vector));
	}

	void CRPDeferredCall::act(CRPFuture&& future)
	{
		_future_map[CRPDeferredCall::NullUID].append
		(
			QSharedPointer<Core::Net::CRPFuture>::create(std::move(future))
		);
	}

	void CRPDeferredCall::act(QVector<QSharedPointer<CRPFuture>>&& vector)
	{
		if (!hasEndpoint()) {
			emit excepted(0, 0, "No valid endpoint");
			qWarning("[0, 0] No valid endpoint");
			_future_map[NullUID].append(vector);
			return ;
		} else if (vector.isEmpty()) {
			qWarning("[0, 0] Nothing to call");
			return ;
		}

		QMap<QUuid, QVector<QSharedPointer<CRPFuture>>>::Iterator d(_future_map.insert(QUuid::createUuid(), {}));
		d->swap(vector);

		QJsonArray body;
		std::for_each
		(
			d->constBegin(), d->constEnd(),
			[&body]
			(const QSharedPointer<Core::Net::CRPFuture>& future)
			{
				if (future->progress()) {
					body.push_back(future->getAction());
				} else {
					body.push_back({/* skip somehow later on */});
				}
			}
		);
		const QString CRP_UID(d.key().toString(QUuid::StringFormat::WithoutBraces));
		setXeader("CRP-UID", CRP_UID);
		post(
				QJsonDocument
				{
					QJsonObject
					{
						{"timepoint", QString::number(QDateTime::currentSecsSinceEpoch())},
						{"dialect", 1},
						{"id", CRP_UID},
						{"path", _path},
						{"body", body}
					}
				}
		);
	}

	void CRPDeferredCall::act(const QString& interface, const QString& method, const QJsonObject& payload)
	{
		act({interface, method, payload});
	}

	void CRPDeferredCall::act(const QString& method, const QJsonObject& payload)
	{
		act({method, payload});
	}

	void CRPDeferredCall::act(qint64 handle, const QString& interface, const QString& method, const QJsonObject& payload)
	{
		act({interface, method, payload, handle});
	}

	void CRPDeferredCall::act(qint64 handle, const QString& method, const QJsonObject& payload)
	{
		act({method, payload, handle});
	}

	void CRPDeferredCall::onReaction(const QSharedPointer<QNetworkReply>& reply, const QUuid& id, qsizetype index, quint8 domain, quint32 code, const QString& message)
	{
		CRPCall::onReaction(reply, id, index, domain, code, message);

		QVector<QSharedPointer<CRPFuture>> future_vector;
		QMap<QUuid, QVector<QSharedPointer<CRPFuture>>>::Iterator f(id.isNull() ? _future_map.end() : _future_map.find(id));
		do {
			if (_future_map.end() == f) {
				const QUuid CRP_UID(reply->request().rawHeader("X-CRP-UID"));
				if (
					CRP_UID.isNull()
					||
					(f = _future_map.find(CRP_UID)) == _future_map.end()
				) {
					break;
				}
			}
			future_vector.swap(f.value());
			_future_map.erase(f);
		} while (false);

		if (index > 0 && future_vector.size() > --index) {
			future_vector.at(index)->happen(domain, code, message);
		} else std::for_each (
			future_vector.constBegin(), future_vector.constEnd(),
			[&domain, &code, &message]
			(const QSharedPointer<Core::Net::CRPFuture>& future)
			{
				future->happen(domain, code, message);
			}
		);
	}

	void CRPDeferredCall::onReaction(const QSharedPointer<QNetworkReply>& reply, const QUuid& id, qsizetype index, const QJsonValue& payload)
	{
		CRPCall::onReaction(reply, id, index, payload);

		QVector<QSharedPointer<CRPFuture>> future_vector;
		QMap<QUuid, QVector<QSharedPointer<CRPFuture>>>::Iterator f(id.isNull() ? _future_map.end() : _future_map.find(id));
		do {
			if (_future_map.end() == f) {
				const QUuid CRP_UID(reply->request().rawHeader("X-CRP-UID"));
				if (
					CRP_UID.isNull()
					||
					(f = _future_map.find(CRP_UID)) == _future_map.end()
				) {
					break;
				}
			}
			future_vector.swap(f.value());
			_future_map.erase(f);
		} while (false);

		if (index > 0 && future_vector.size() > --index) {
			future_vector.at(index)->happen(payload);
		} else std::for_each (
			future_vector.constBegin(), future_vector.constEnd(),
			[&payload]
			(const QSharedPointer<Core::Net::CRPFuture>& future)
			{
				future->happen(payload);
			}
		);
	}
} } } }
