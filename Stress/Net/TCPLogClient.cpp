#include "TCPLogClient.hpp"

#include <QTextStream>

namespace Cadabra { namespace Queen { namespace Stress { namespace Net {
	TCPLogClient::TCPLogClient(LogFormat format, QObject* parent)
	:
		System::LogClient(format, parent)
	{
/*
		QObject::connect(&_socket, &QTcpSocket::connected,this, &TCPLogClient::onConnected);
		QObject::connect(&_socket, &QTcpSocket::readyRead,this, &TCPLogClient::onRead);
*/
		QObject::connect(&_socket, &QTcpSocket::disconnected,this, &TCPLogClient::onDisconnected);
		QObject::connect(&_socket, &QTcpSocket::bytesWritten, this, &TCPLogClient::onDataWritten);
	}

	TCPLogClient::~TCPLogClient()
	{
		QObject::disconnect(&_socket);

		const qint64 bytes_count(_socket.bytesToWrite());
		if (bytes_count > 0) {
			qWarning() << "Lost bytes:" << bytes_count;
		}
	}

	bool TCPLogClient::connect(const QString& hostname, quint16 port)
	{
		_socket.connectToHost(hostname, port, QIODevice::WriteOnly);
		if (!_socket.waitForConnected(5000)) {
			qWarning() << "Connection error:" << _socket.errorString();
			return false;
		}
		return true;
	}

	bool TCPLogClient::write(const QByteArray& message)
	{
		if (message.isEmpty()) {
			return true;
		} else if (!_socket.isWritable()) {
			return false;
		}

		quint8 zero(0);
		qint64 total(0);
		do {
			const qint64 shot(_socket.write(message.constData() + total, message.size() - total));
			if (shot < 0) {
				qWarning() << "Write error:" << _socket.error() << " total =" << total << "shot =" << shot << "size" << message.size();
				return false;
			} else if (shot == 0) {
				qWarning() << "Zero write:" << ++zero;
				if (zero > 10) {
					return false;
				}
			} else {
				total += shot;
			}
		} while (total < message.size());
		return true;
	}

	void TCPLogClient::onDisconnected()
	{
		qWarning() << "Socket state:" <<  _socket.state() << "error:" << _socket.error();
	}

	void TCPLogClient::onDataWritten(qint64 count)
	{
//		qDebug() << "TCPLogClient::onDataWritten() Count: " << count;
	}
} } } }
