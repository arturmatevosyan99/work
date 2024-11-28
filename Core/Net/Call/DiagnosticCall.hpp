#ifndef Cadabra_Queen_Core_Net_Call_DiagnosticCall_hpp
#define Cadabra_Queen_Core_Net_Call_DiagnosticCall_hpp

#include "../ServiceCall.hpp"

#include <QIODevice>
#include <QPointer>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class DiagnosticCall : public ServiceCall
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
		static Endpoint SOSEndpoint;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		DiagnosticCall();
		virtual ~DiagnosticCall() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QPointer<QIODevice> _input;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool upload(QIODevice* io_device);
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
		void loaded(double progress);
		void faulted(const QString& error);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
		virtual void onUpload(const QScopedPointer<QNetworkReply>& reply, qint64 out, qint64 total) override;
		virtual void onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url) override;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:

	/** @} */
	};
} } } }

#endif
