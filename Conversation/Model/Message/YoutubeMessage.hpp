#ifndef Cadabra_Queen_Conversation_Model_Message_YoutubeMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_YoutubeMessage_hpp

#include "../AbstractMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class YoutubeMessage : public AbstractMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString videoId READ getVideoId NOTIFY imported)
		Q_PROPERTY(QUrl hyperlink READ getHyperlink NOTIFY imported)
		Q_PROPERTY(QUrl thumbnailHQ READ getThumbnailHQ NOTIFY imported)
		Q_PROPERTY(QUrl thumbnailMQ READ getThumbnailMQ NOTIFY imported)
		Q_PROPERTY(QUrl thumbnailSQ READ getThumbnailSQ NOTIFY imported)
		Q_PROPERTY(QString note READ getNote NOTIFY imported)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		YoutubeMessage(QObject* parent = nullptr);
		virtual ~YoutubeMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _video_id;
		QString _custom_note;
		bool _note_load;
		bool _cover_load;
		bool _hyperlink;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
		QString getVideoId() const;
		QUrl getHyperlink() const;
		QUrl getThumbnailHQ() const;
		QUrl getThumbnailMQ() const;
		QUrl getThumbnailSQ() const;
		QString getNote() const;
	/** @} */

	/** @name Setters */
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
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasHyperlink() const;
		bool needCover() const;
		bool needNote() const;
	/** @} */
	};
} } } }

#endif
