/**
 @file Endpoint.hpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Core_Net_Endpoint_hpp
#define Cadabra_Queen_Core_Net_Endpoint_hpp

#include <QObject>
#include <QLatin1String>
#include <QString>
#include <QUrl>
#include <QSet>
#include <QIODevice>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QDataStream>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class Endpoint : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Side
		{
			Client,
			Server
		};
	/** @} */
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Side)
		Q_PROPERTY(QString host READ getHost NOTIFY hostChanged)
		Q_PROPERTY(int port READ getPort NOTIFY portChanged)
		Q_PROPERTY(bool secure READ isSecured NOTIFY secureChanged)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static Endpoint* Instance(const QUrl& url);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Endpoint(QObject* parent = nullptr);
		Endpoint(const QString& host, int port, bool secure);
		Endpoint(const QString& url);
		Endpoint(const QUrl& url);
		virtual ~Endpoint() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QNetworkAccessManager _access_manager;

		QString _host;
		int _port;
		bool _secure;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE virtual void reach();
		QNetworkReply* get(const QNetworkRequest& request);
		QNetworkReply* del(const QNetworkRequest& request);
		QNetworkReply* post(const QNetworkRequest& request, QIODevice* content);
		QNetworkReply* post(const QNetworkRequest& request, QHttpMultiPart* content);
		QNetworkReply* post(const QNetworkRequest& request, const QByteArray& content);
		QNetworkReply* put(const QNetworkRequest& request, QIODevice* content);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QUrl makeBaseURL() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getHost() const;
		int getPort() const;
		QNetworkProxy getProxy() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setURL(const QUrl& value);
		void setURL(const QString& value);
		void setPort(int port);
		void setHost(const QString& host);
		void setSecure(bool onoff);
		void setProxy(const QNetworkProxy& proxy);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void hostChanged();
		void portChanged();
		void secureChanged();
		void proxyChanged();

		/**
		 * @brief Signal "Versions are not compatible"
		 * @param side(Side) Server or Client needs an upgrade
		 */
		void outdated(Cadabra::Queen::Core::Net::Endpoint::Side side);
		/**
		 * @brief Signal "Backend is reached and fully operates"
		 */
		void reached();
		/**
		 * @brief Signal "Backend is not available because of a maintenance or a downtime"
		 */
		void maintenanced();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		void onReached();
		void onOutdated(Side side);
		void onMaintenanced();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isSecured() const;
		bool isProxied() const;
	/** @} */


	/** @name Friends */
	/** @{ */
		friend QDataStream& operator<<(QDataStream& out, const Endpoint& endpoint);
		friend QDataStream& operator>>(QDataStream& in, Endpoint& endpoint);
	/** @} */
	};
} } } }

#endif
