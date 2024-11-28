#ifndef Cadabra_Queen_Stress_Net_TCPLogClient_hpp
#define Cadabra_Queen_Stress_Net_TCPLogClient_hpp

#include "../System/LogClient.hpp"

#include <QTcpSocket>

namespace Cadabra { namespace Queen { namespace Stress { namespace Net {
	class TCPLogClient : public System::LogClient
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TCPLogClient(LogFormat format, QObject* parent = nullptr);
		virtual ~TCPLogClient() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QTcpSocket _socket;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool connect(const QString& hostname, quint16 port);

	protected:
		virtual bool write(const QByteArray& message) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onDisconnected();
		void onDataWritten(qint64 count);
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
