#ifndef Cadabra_Queen_Conversation_Model_Message_PushRequestMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_PushRequestMessage_hpp

#include "../AbstractMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class PushRequestMessage : public AbstractMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString comment READ getComment WRITE setComment NOTIFY commentChanged)
		Q_PROPERTY(QString agent READ getAgent NOTIFY tokenChanged)
		Q_PROPERTY(QString token READ getToken WRITE setToken NOTIFY tokenChanged)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class ServiceFamily : char
		{
			Unknown = 0,
			APN = 'A',
			GCN = 'G', /// deprecated
			FCM = 'F'
		};
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		PushRequestMessage(QObject* parent = nullptr);
		virtual ~PushRequestMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _comment;
		QString _agent;
		Core::System::NotificationCenter::Family _family;
		QString _token;
		Repository::Meta::Subject::Id _subject_id;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		Q_INVOKABLE void requestToken();

		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
		QString getComment() const;
		QString getAgent() const;
		QString getToken() const;
		Repository::Meta::Subject::Id getSubjectId() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setComment(const QString& text);
		void setToken(const QString& token);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void commentChanged();
		void tokenChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onRegistered(QString token);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		Q_INVOKABLE bool hasToken() const;
	/** @} */
	};
} } } }

#endif
