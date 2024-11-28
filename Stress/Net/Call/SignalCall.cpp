#include "SignalCall.hpp"

#include <QString>
#include <QUrlQuery>

namespace Cadabra { namespace Queen { namespace Stress { namespace Net {
	SignalCall::SignalCall(Core::Net::Endpoint* endpoint)
	:
		Core::Net::ServiceCall(endpoint, "/stress/death/signal"),

		_counter(0)
	{
	}

	SignalCall::~SignalCall()
	{
	}

	bool SignalCall::call(int no)
	{
		get(QUrlQuery(QString("no=%1").arg(no)));
		++_counter;
		return true;
	}

	void SignalCall::onSuccess(QSharedPointer<QNetworkReply> reply)
	{
		--_counter;
		if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
			emit faulted("Could not send a signal to the endpoint");
		} else {
			emit completed(reply->url().hasQuery() ? (QUrlQuery(reply->url().query()).queryItemValue("no").toInt()) : 0);
		}
	}

	void SignalCall::onFault(QSharedPointer<QNetworkReply> reply)
	{
		--_counter;
		if (QNetworkReply::NetworkError::OperationCanceledError == reply->error()) {
			return ;
		}
		emit faulted(QString("Error: %1").arg(reply->errorString()));
	}

	bool SignalCall::isIdle() const
	{
		return _counter <= 0;
	}
} } } }
