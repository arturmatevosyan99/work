#ifndef Cadabra_Queen_Core_Net_Call_ITunesGrab_hpp
#define Cadabra_Queen_Core_Net_Call_ITunesGrab_hpp

#include "../ServiceCall.hpp"

#include "../../Meta/ITunesItem.hpp"

#include <QObject>
#include <QJsonObject>
#include <QString>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class ITunesGrab : public ServiceCall
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
		static Endpoint Endpoint;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ITunesGrab();
		virtual ~ITunesGrab() override = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool lookup(const QString& bundle_id);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void lookedup(Cadabra::Queen::Core::Meta::ITunesItem bundle);
		void faulted(QString error);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:		
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	private:
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
	/** @} */
	};
} } } }

#endif
