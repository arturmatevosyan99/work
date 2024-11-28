#ifndef Cadabra_Queen_Conversation_Model_Message_PictureMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_PictureMessage_hpp

#include "AttachmentMessage.hpp"

#include <QImage>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class PictureMessage : public AttachmentMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QSize originalSize READ getOriginalSize NOTIFY imported)
		Q_PROPERTY(QSize thumbnailSize READ getThumbnailSize NOTIFY imported)
		Q_PROPERTY(QString thumbnailBase64 READ getThumbnailBase64 NOTIFY imported)
		Q_PROPERTY(bool hasThumbnail READ hasThumbnail NOTIFY imported)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static PictureMessage Thumbnail(const QImage& image);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		PictureMessage(QObject* parent = nullptr);
		PictureMessage(const PictureMessage& origin);
		virtual ~PictureMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QSize _original_size;
		QSize _thumbnail_size;
		QString _thumbnail_base64;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& body) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;

		const QSize& getOriginalSize() const;
		const QSize& getThumbnailSize() const;
		const QString& getThumbnailBase64() const;
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
		bool hasThumbnail() const;
	/** @} */
	};
} } } }

#endif
