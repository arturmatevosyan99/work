/**
 @file AbstractApplication.cpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Repository_Model_AbstractApplication_hpp
#define Cadabra_Queen_Repository_Model_AbstractApplication_hpp

#include "AbstractDescriptor.hpp"
#include "Application/AbstractInterface.hpp"

#include "../../Core/Core.hpp"
#include "../../Core/System/Alert/StateAlert.hpp"

#include <QSharedPointer>
#include <QObject>
#include <QString>
#include <QSet>
#include <QEvent>
#include <QTimer>
#include <QAtomicInteger>
#include <QtWebSockets/QWebSocket>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Session;
	class AbstractApplication : public QObject, public virtual AbstractInterface
	{
	/** @name Aliases */
	/** @{ */
	public:
		using MessageLevel = Core::ApplicationMessage;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::Model::AbstractDescriptor* descriptor READ getDescriptor NOTIFY descriptorChanged)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Session* session READ getSession NOTIFY descriptorChanged)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Object::ApplicationScheme scheme READ getScheme CONSTANT)
		Q_PROPERTY(bool hasUpstream READ hasUpstream NOTIFY upstreamed)
		Q_PROPERTY(bool isOpened READ hasDescriptor NOTIFY descriptorChanged)
		Q_PROPERTY(bool isIdle READ isIdle NOTIFY callmapChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractApplication(QObject* parent = nullptr, bool upstream = false);
		virtual ~AbstractApplication() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		AbstractDescriptor* _descriptor;
		qulonglong _ack;

	protected:
		bool _upstream_onoff;
		QWebSocket _upstream_socket;
		QTimer* _upstream_pinger;
		QAtomicInteger<quint8> _lost_pinger;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE virtual bool open(Cadabra::Queen::Repository::Model::AbstractDescriptor* descriptor);
		Q_INVOKABLE virtual bool clean(Cadabra::Queen::Repository::Model::AbstractDescriptor* descriptor);
		Q_INVOKABLE virtual bool reopen();
		Q_INVOKABLE virtual bool close();

	protected:
		void upStream(bool onoff);
		Core::System::StateAlert* setState(const QString& name, const QString& title, const QString& description, Core::System::StateAlert::Level level = Core::System::StateAlert::Level::Warning);
		bool resetState(const QString& name);

	private:
		void dequeue();
		void acknowledge();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		AbstractDescriptor* getDescriptor() const override;
		Meta::Object::ApplicationSchemeIdent getSchemeIdent() const;
		Meta::Object::ApplicationScheme getScheme() const;
		Session* getSession() const;
		QVariant getMetaValue(const QString& key) const;

		Q_INVOKABLE QUrl getFileEndpoint(const QString& file_name) const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void resetPingInterval(int timeout = 0);
		bool setMetaValue(const QString& key, const QVariant& value);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onDequeued(quint8 domain, quint32 code, QString message);
		void onDequeued(QJsonValue payload);
		void onAcknowledge(quint8 domain, quint32 code, QString message);
		void onAcknowledge(QJsonValue payload);
		void onUpstreamData(const QString& message);
		void onUpstreamData(const QByteArray& message);
		void onUpstreamConnect();
		void onUpstreamDisconnect();
		void onUpstreamState(QAbstractSocket::SocketState state);
		void onUpstreamError(const QAbstractSocket::SocketError& error);
		void onUpstreamChange();
		void onPong(quint64 roundtrip, const QByteArray& payload);
		void onPingTime();
		void onDisposed();
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void opened();
		void reopened();
		void unopened();
		void closed();
		void upstreamed(bool onoff);
		void descriptorChanged();
		void callmapChanged(bool idle);
		void exceptionThrew(Cadabra::Queen::Core::ApplicationMessage level, QString message);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual bool onHandle();
		virtual bool onOpen();
		virtual bool onReopen();
		virtual bool onClose();
		virtual bool onTeardown();
		virtual bool onSetup();
		virtual bool onSetdown();
		virtual void onCall(bool start) override;

		virtual void onSignal(quint16 signal, QDataStream& stream);
		virtual void onQueueEvent(const QJsonValue& body);
		virtual void onExceptionCaught(Core::ApplicationMessage level, QString message);
		virtual bool eventFilter(QObject *object, QEvent *event) override;
		virtual void customEvent(QEvent* event) override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasDescriptor() const;
		bool hasUpstream() const;
		bool isUpstream() const;
		bool isIdle() const;
	/** @} */
	};
} } } }

#endif
