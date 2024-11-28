/**
 @file CRPDeferredCall.hpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Core_Net_Call_CRPDeferredCall_hpp
#define Cadabra_Queen_Core_Net_Call_CRPDeferredCall_hpp

#include "CRPCall.hpp"

#include "CRPFuture.hpp"

#include <QMap>
#include <QVector>
#include <QString>
#include <QPointer>
#include <QJsonObject>
#include <QJsonValue>
#include <functional>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class CRPDeferredCall : public CRPCall
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QUuid NullUID;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CRPDeferredCall(Core::Net::Endpoint* endpoint, const QString& reflector, const QString& path);
		CRPDeferredCall(Core::Net::Endpoint* endpoint, const QString& reflector);
		virtual ~CRPDeferredCall() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QMap<QUuid, QVector<QSharedPointer<CRPFuture>>> _future_map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void act();
		void act(CRPFuture&& future);
		void act(QVector<QSharedPointer<CRPFuture>>&& vector);
		virtual void act(const QString& interface, const QString& method, const QJsonObject& payload) override;
		virtual void act(const QString& method, const QJsonObject& payload) override;
		virtual void act(qint64 handle, const QString& interface, const QString& method, const QJsonObject& payload) override;
		virtual void act(qint64 handle, const QString& method, const QJsonObject& payload) override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
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
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onReaction(const QSharedPointer<QNetworkReply>& reply, const QUuid& id, qsizetype index, quint8 domain, quint32 code, const QString& message) override;
		virtual void onReaction(const QSharedPointer<QNetworkReply>& reply, const QUuid& id, qsizetype index, const QJsonValue& payload) override;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
