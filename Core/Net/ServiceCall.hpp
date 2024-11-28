#ifndef Cadabra_Queen_Core_Net_ServiceCall_hpp
#define Cadabra_Queen_Core_Net_ServiceCall_hpp

#include "Endpoint.hpp"

#include <QObject>
#include <QFlags>
#include <QString>
#include <QVector>
#include <QSet>
#include <QIODevice>
#include <QByteArray>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class ServiceCall : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum ObserverFlag
		{
			Finish   = 0b000001,
			Error    = 0b000010,
			Redirect = 0b000100,
			Download = 0b001000,
			Upload   = 0b010000,
			SSLError = 0b100000
		};
		enum FaultFlag
		{
			None    = 0b000000,
			General = 0b000010,
			Network = 0b000100,
			SSL     = 0b100000
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(ObserverFlag)
		Q_ENUM(FaultFlag)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ServiceCall(const QUrl& endpoint);
		ServiceCall(Core::Net::Endpoint* endpoint = nullptr);
		ServiceCall(Core::Net::Endpoint* endpoint, const QString& resource);
		virtual ~ServiceCall();
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QString makeUserAgent() const;

	private:
		QNetworkRequest makeRequest(const QUrlQuery& query = QUrlQuery()) const;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Core::Net::Endpoint* _endpoint;
		QString _virtual_scheme;
		QString _virtual_host;
		int _virtual_port;
		QString _resource;
		QString _user_agent;
		int _transfer_timeout;
		QMap<QString, QString> _xeader_map;
		QSet<QNetworkReply*> _set;

	protected:
		QFlags<ObserverFlag> _protected_observer_flag_set;

	private:
		QFlags<ObserverFlag> _public_observer_flag_set;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		void observe(QNetworkReply* reply);

	protected:
		QJsonObject parse(QSharedPointer<QNetworkReply> reply);

		void get(const QUrl& url);

	public:
		void get(const QUrlQuery& query = QUrlQuery());
		void del(const QUrlQuery& query = QUrlQuery());
		void post(QIODevice* content, const QString& type = "plain/text", const QUrlQuery& query = QUrlQuery());
		void post(QHttpMultiPart* content, const QUrlQuery& query = QUrlQuery());
		void post(const QByteArray& content, const QString& type = "plain/text", const QUrlQuery& query = QUrlQuery());
		void post(const QJsonDocument& document, const QUrlQuery& query = QUrlQuery());
		void put(QIODevice* content, const QString& type = "plain/text", const QUrlQuery& query = QUrlQuery());
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Core::Net::Endpoint* getEndpoint();
		const QString& getResource() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setEndpoint(Core::Net::Endpoint* endpoint);
		void setVirtualServer(const QString& scheme = "", const QString& host = "", int port = 0);
		void setVirtualScheme(const QString& scheme = "");
		void setVirtualHost(const QString& host = "");
		void setVirtualPort(int port = 0);
		void setResource(const QString& resource);
		void setTransferTimeout(int value = QNetworkRequest::DefaultTransferTimeoutConstant);
		void setXeader(const QString& key, const QString& value);

		void setObserver(QFlags<ObserverFlag> flag_set);

	protected:
		void setUserAgent(const QString& agent);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void finished();
		void redirected(QUrl url);
		void faulted(FaultFlag flag_set);
		void downloaded(qint64 current, qint64 total);
		void uploaded(qint64 current, qint64 total);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onDestroyed(QObject* object_raw);
		void onFinished(QNetworkReply* reply);
		void onError(QNetworkReply* reply_raw, QNetworkReply::NetworkError error);
		void onError(QNetworkReply* reply_raw, const QList<QSslError>& ssl_error_list);
		void onRedirected(QNetworkReply* reply, const QUrl& url);
		void onLoaded(QNetworkReply* reply, bool inout, qint64 received, qint64 total);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onComplete();
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply);
		virtual void onFault(QSharedPointer<QNetworkReply> reply);
		virtual void onError(const QScopedPointer<QNetworkReply>& reply, QNetworkReply::NetworkError error);
		virtual void onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url);
		virtual void onDownload(const QScopedPointer<QNetworkReply>& reply, qint64 current, qint64 total);
		virtual void onUpload(const QScopedPointer<QNetworkReply>& reply, qint64 current, qint64 total);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasEndpoint() const;
	/** @} */
	};
} } } }

#endif
