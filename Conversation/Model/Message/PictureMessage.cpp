#include "PictureMessage.hpp"

#include "../../../Core/System/ImageThumbnail.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	PictureMessage PictureMessage::Thumbnail(const QImage& image)
	{
		PictureMessage retval;

		Core::System::ImageThumbnail thumbnail;
		thumbnail.setQuality(50);
		thumbnail.setLimit(320);
		if (!thumbnail.process(image)) {
			return retval;
		}

		retval._content = Content::Picture;
		retval._original_size = image.size();
		retval._thumbnail_size = thumbnail.getImage().size();
		retval._thumbnail_base64 = QString::fromLatin1(thumbnail.getBuffer()->buffer().toBase64());

		return retval;
	}

	PictureMessage::PictureMessage(QObject* parent)
	:
		AttachmentMessage(parent, -8034428181974876157),

		_original_size(0, 0),
		_thumbnail_size(0, 0)
	{

	}

	PictureMessage::PictureMessage(const PictureMessage& origin)
	:
		AttachmentMessage(origin.parent(), -8034428181974876157),

		_original_size(origin._original_size),
		_thumbnail_size(origin._thumbnail_size),
		_thumbnail_base64(origin._thumbnail_base64)
	{
		_content = origin._content;
	}

	QJsonObject PictureMessage::exportBody() const
	{
		QJsonObject retval(AttachmentMessage::exportBody());

		retval["original_height"] = _original_size.height();
		retval["original_width"] = _original_size.width();

		retval["thumbnail_height"] = _thumbnail_size.height();
		retval["thumbnail_width"] = _thumbnail_size.width();

		retval["thumbnail_base64"] = _thumbnail_base64;

		return retval;
	}

	bool PictureMessage::importBody(const QJsonObject& body)
	{
		_original_size.setHeight(body.value("original_height").toInt(0));
		_original_size.setWidth(body.value("original_width").toInt(0));

		_thumbnail_size.setHeight(body.value("thumbnail_height").toInt(0));
		_thumbnail_size.setWidth(body.value("thumbnail_width").toInt(0));

		_thumbnail_base64 = body.value("thumbnail_base64").toString("");

		if (_original_size.isNull()) {
			_content = Content::Picture;
		}

		return AttachmentMessage::importBody(body);
	}

	QString PictureMessage::getPreviewText() const
	{
		return QString("[%1]").arg(tr("Picture"));
	}

	const QSize& PictureMessage::getOriginalSize() const
	{
		return _original_size;
	}

	const QSize& PictureMessage::getThumbnailSize() const
	{
		return _thumbnail_size;
	}

	const QString& PictureMessage::getThumbnailBase64() const
	{
		return _thumbnail_base64;
	}

	bool PictureMessage::hasThumbnail() const
	{
		return !_thumbnail_size.isEmpty() && !_thumbnail_base64.isEmpty();
	}
} } } }
