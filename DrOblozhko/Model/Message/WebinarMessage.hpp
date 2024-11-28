#ifndef Cadabra_Queen_DrOblozhko_Model_Message_WebinarMessage_hpp
#define Cadabra_Queen_DrOblozhko_Model_Message_WebinarMessage_hpp

#include "../../../Conversation/Model/Message/AppletMessage.hpp"

#include <QDateTime>

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	class WebinarMessage : public Conversation::Model::AppletMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString hostName READ getHostName NOTIFY imported)
		Q_PROPERTY(QDateTime broadcastDateTime READ getBroadcastTimePoint NOTIFY imported)
		Q_PROPERTY(bool active READ isActive NOTIFY activated)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		WebinarMessage(QObject* parent = nullptr);
		virtual ~WebinarMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _host_name;
		QDateTime _broadcast_time_point;
		bool _active;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void fetch();
		Q_INVOKABLE void sign();

	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;

	private:
		void activate(bool onoff = true);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QString getHostName() const;
		QDateTime getBroadcastTimePoint() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:		
		void activated(bool onoff);
		void accessed(QUrl url);
		void denied();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onFetched(quint8 domain, quint32 code, QString message);
		void onFetched(QJsonValue payload);

		void onSigned(quint8 domain, quint32 code, QString message);
		void onSigned(QJsonValue payload);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isActive() const;
		bool isFetched() const;
	/** @} */
	};
} } } }

#endif
