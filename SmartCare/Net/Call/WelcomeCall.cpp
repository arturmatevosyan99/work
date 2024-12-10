#include "WelcomeCall.hpp"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocale>
#include <QTimeZone>
#include <QDateTime>

#include "../../../Repository/Meta/Essence.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Net {
	WelcomeCall::WelcomeCall(Core::Net::Endpoint* endpoint)
	:
		Core::Net::ServiceCall(endpoint, "/smartcare/access/welcome"),

		_map_model(this)
	{

	}

	void WelcomeCall::request(const QString& alias)
	{
		QUrlQuery query;
		query.addQueryItem("alias", alias);
		get(query);
	}

	QAbstractListModel& WelcomeCall::getModel()
	{
		return _map_model;
	}

	const WelcomeCall::MapModel::Map& WelcomeCall::getMap() const
	{
		return _map_model.getMap();
	}

	void WelcomeCall::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		const MapModel::Down down(_map_model.cleanUp());
		const QByteArray data(reply->readAll());

		QJsonParseError error;
		const QJsonDocument document(QJsonDocument::fromJson(data, &error));
		if (error.error != QJsonParseError::NoError) {
			emit responded(error.errorString());
			return ;
		} else if (!document.isObject()) {
			emit responded("Malformed data");
			return ;
		}
		const QJsonObject object(document.object());
		if (object.isEmpty()) {
			/// @xxx Возможно это особенный кейс, когда доступных WelcomeQuiz попросту нет на данный момент
			emit responded("");
			return ;
		}
		for (QJsonObject::ConstIterator o = object.constBegin(); o != object.constEnd(); ++o) {
			if (_map_model.getMap().insert(Repository::Meta::Essence::ToId(o.key(), true), o.value().toString("")).key().isZero()) {
				_map_model.getMap().remove(0);
			}
		}
		emit responded(_map_model.isEmpty() ? "Malformed data" : "");
	}

	void WelcomeCall::onFault(QSharedPointer<QNetworkReply> reply)
	{
		emit responded(reply->errorString());
	}
} } } }
