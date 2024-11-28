/**
 @file CRPCall.hpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Core_Net_Call_CRPCall_hpp
#define Cadabra_Queen_Core_Net_Call_CRPCall_hpp

#include "../ServiceCall.hpp"

#include <QMap>
#include <QVector>
#include <QString>
#include <QJsonObject>
#include <QJsonValue>
#include <functional>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class CRPCall : public ServiceCall
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CRPCall(Core::Net::Endpoint* endpoint, const QString& reflector, const QString& path);
		CRPCall(Core::Net::Endpoint* endpoint, const QString& reflector);
		virtual ~CRPCall() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		const QString _path;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void act(const QString& interface, const QString& method, const QJsonObject& payload);
		virtual void act(const QString& method, const QJsonObject& payload);
		virtual void act(qint64 handle, const QString& interface, const QString& method, const QJsonObject& payload);
		virtual void act(qint64 handle, const QString& method, const QJsonObject& payload);

	private:
		void act(const QJsonObject& action);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void reflected();
		void reacted(QJsonValue payload);
		void excepted(quint8 domain, quint32 code, QString message);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onComplete() override;
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;

		virtual void onReaction(const QSharedPointer<QNetworkReply>& reply, const QUuid& id, qsizetype index, quint8 domain, quint32 code, const QString& message);
		virtual void onReaction(const QSharedPointer<QNetworkReply>& reply, const QUuid& id, qsizetype index, const QJsonValue& payload);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
