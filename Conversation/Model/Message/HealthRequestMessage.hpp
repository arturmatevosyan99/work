#ifndef Cadabra_Queen_Conversation_Model_Message_HealthRequestMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_HealthRequestMessage_hpp

#include "../AbstractMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class HealthRequestMessage : public AbstractMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString comment READ getComment WRITE setComment NOTIFY commentChanged)
		Q_PROPERTY(QString permission READ getPermission WRITE setPermission NOTIFY permissionChanged)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class KitFamily : char
		{
			Unknown = 0,
			Apple = 'A',
			Google = 'G'
		};
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		HealthRequestMessage(QObject* parent = nullptr);
		virtual ~HealthRequestMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _comment;
		QString _permission;
		Repository::Meta::Subject::Id _subject_id;
		QString _local_permission;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		Q_INVOKABLE void requestAccess();

		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:		
		virtual QString getPreviewText() const override;
		QString getComment() const;
		QString getPermission() const;
		Repository::Meta::Subject::Id getSubjectId() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setComment(const QString& text);
		void setPermission(const QString& permission);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void commentChanged();
		void permissionChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onRegistered();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		Q_INVOKABLE bool hasPermission() const;
	/** @} */
	};
} } } }

#endif
