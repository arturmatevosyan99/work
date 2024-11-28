#include "WebApplication.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	WebApplication::WebApplication(QObject* parent)
	:
		Repository::Model::AbstractApplication(parent)
	{
	}

	WebApplication::~WebApplication()
	{
	}

	void WebApplication::fetch(const QString& version)
	{
		if (!hasDescriptor()) {
			return ;
		}
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&WebApplication::onFetched));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&WebApplication::onFetched));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Fetch",
					{
						{"name", version}
					}
		);
	}

	void WebApplication::sign(const QString& version)
	{
		if (!hasDescriptor()) {
			return ;
		}
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&WebApplication::onSigned));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&WebApplication::onSigned));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Sign",
					{
						{"name", version}
					}
		);
	}

	void WebApplication::onFetched(quint8 domain, quint32 code, QString message)
	{
		emit denied();
	}

	void WebApplication::onFetched(QJsonValue payload)
	{
		if (!payload.isString()) {
			emit denied();
			return ;
		}
		emit accessed(QUrl(payload.toString()), "");
	}

	void WebApplication::onSigned(quint8 domain, quint32 code, QString message)
	{
		emit denied();
	}

	void WebApplication::onSigned(QJsonValue payload)
	{
		if (!payload.isObject()) {
			emit denied();
			return ;
		}
		/// @todo cache : resource_locator, life_time, token_map
		const QJsonObject object(payload.toObject());
		emit accessed(object.value("resource_locator").toString(""), object.value("view_preset").toString(""));
	}

	bool WebApplication::onHandle()
	{
		return true;
	}

	bool WebApplication::onOpen()
	{
		return true;
	}

	bool WebApplication::onSetup()
	{
		return true;
	}
} } } }
