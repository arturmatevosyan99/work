#include "Webinar.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	Webinar::Webinar(QObject* parent)
	:
		Repository::Model::AbstractApplication(parent),
		_type(Type::Unknown),
		_source("file:///dev/null"),
		_title("")
	{
	}

	Webinar::~Webinar()
	{
	}

	void Webinar::fetch()
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&Webinar::onFetched));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&Webinar::onFetched));
		call->act(
					"Cadabra::Ace::Repository::Model::URLInterface",
					"Cadabra::Ace::Repository::Model::Fetch",
					{
					}
		);
	}

	void Webinar::page()
	{
		QSharedPointer<Core::Net::CRPCall> call(makeCall());
		connect(call.get(), qOverload<QJsonValue>(&Core::Net::CRPCall::reacted), this, qOverload<QJsonValue>(&Webinar::onPaged));
		connect(call.get(), qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted), this, qOverload<quint8, quint32, QString>(&Webinar::onFetched));
		call->act(
					"Cadabra::Ace::Mentalgami::Model::ContentInterface",
					"Cadabra::Ace::Mentalgami::Model::Page",
					{
					}
		);
	}

	Webinar::Type Webinar::getType() const
	{
		return _type;
	}

	QUrl Webinar::getSource() const
	{
		return _source;
	}

	QString Webinar::getTitle() const
	{
		return _title;
	}

	bool Webinar::onHandle()
	{
		return true;
	}

	bool Webinar::onOpen()
	{
		return true;
	}

	bool Webinar::onSetup()
	{
		return true;
	}

	void Webinar::customEvent(QEvent* event)
	{

	}

	void Webinar::onFetched(quint8 domain, quint32 code, QString message)
	{

	}

	void Webinar::onPaged(QJsonValue payload)
	{
		 if (!payload.isObject()) {
			 emit denied();
			 return ;
		 }

		 QJsonObject object(payload.toObject());
		 _title = object.value("title").toString("");
		 onFetched(object.value("url"));
	}

	void Webinar::onFetched(QJsonValue payload)
	{		
		if (!payload.isString()) {
			emit denied();
			return ;
		}
		_source = payload.toString("file:///dev/null");
		if (_source.scheme().startsWith("youtube://")) {
			_type = Type::YouTube;
		} else if (_source.scheme().startsWith("cube://")) {
			_type = Type::CadabraTube;
		} else if (_source.scheme().compare("http") == 0 || _source.scheme().compare("https") == 0) {
			if (_source.path().endsWith(".mp4") || _source.path().endsWith(".avi") || _source.path().endsWith(".mpeg") || _source.path().endsWith(".m3u8")) {
				_type = Type::File;
			} else {
				_type = Type::HyperLink;
			}
		} else if (_source.scheme().startsWith("rtsp://")) {
			_type = Type::RTSP;
		} else {
			_type = Type::Unknown;
		}

		emit fetched();
	}
} } } }
