#include "ImageThumbnail.hpp"

#include <QtConcurrent>
#include <QThreadPool>
#include <QBuffer>
#include <QByteArray>
#include <QImageReader>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	const QMap<QString, ImageThumbnail::Format> ImageThumbnail::FormatMap = {
																			{"JPG", ImageThumbnail::Format::JPEG},
																			{"JPEG", ImageThumbnail::Format::JPEG},
																			{"PNG", ImageThumbnail::Format::PNG},
																			{"GIF", ImageThumbnail::Format::GIF},
																			{"HEIF", ImageThumbnail::Format::HEIF},
																			{"HEIC", ImageThumbnail::Format::HEIF}
	};

	ImageThumbnail::ImageThumbnail(QObject* parent)
	:
		QObject(parent),

		_format(Format::JPEG),
		_quality(-1),
		_limit(1280),
		_buffer(new QBuffer(&_byte_array))
	{
		_buffer->open(QIODevice::WriteOnly);
		QObject::connect(&_watcher, &QFutureWatcher<bool>::finished, this, &ImageThumbnail::finished);
		QObject::connect(&_watcher, &QFutureWatcher<bool>::canceled, this, &ImageThumbnail::finished);
	}

	ImageThumbnail::~ImageThumbnail()
	{
		_buffer->deleteLater();
		_buffer.clear();
	}

	bool ImageThumbnail::enqueue(const QUrl& source_url, bool source_autoremove)
	{
		if (isBusy()) {
			return false;
		}
		emit started();
		_watcher.setFuture(QtConcurrent::run(this, &ImageThumbnail::process, source_url, source_autoremove));
		return true;
	}

	void ImageThumbnail::cancel()
	{
		_image = QImage();
		if (_watcher.isRunning()) {
			_watcher.cancel();
		}
		_buffer->seek(0);
		_byte_array.clear();
	}

	bool ImageThumbnail::flush()
	{
		if (_byte_array.isEmpty()) {
			return false;
		}
		_buffer->seek(0);
		emit flushed();
		return true;
	}

	bool ImageThumbnail::process(const QUrl& source_url, bool source_autoremove)
	{		
		QFile source_file;
		if (source_url.scheme().compare("fd") == 0) {
			if (!source_file.open(source_url.path().toInt(), QIODevice::ReadOnly, QFileDevice::AutoCloseHandle)) {
				qWarning() << "Open error:" << source_url;
				return false;
			}
			source_autoremove = false;
		} else {
			source_file.setFileName(source_url.isLocalFile() ? source_url.toLocalFile() : source_url.toString());
			if (!source_file.open(QIODevice::ReadOnly)) {
				qWarning() << "Open error:" << source_url;
				return false;
			}
		}

		const bool retval(process(source_file));

		if (source_autoremove && source_file.isOpen()) {
			source_file.close();
			source_file.remove();
		}

		return retval;
	}

	bool ImageThumbnail::process(const QImage& image)
	{
		qDebug() << "Process" << image.format() << "image" << image.size().width() << "x" << image.size().height();

		_buffer->seek(0);
		_byte_array.clear();

		if (image.isNull()) {
			return false;
		} else if (qMax(image.width(), image.height()) > _limit) {
			if (image.width() > image.height()) {
				_image = image.scaledToWidth(_limit, Qt::TransformationMode::SmoothTransformation);
			} else if (image.height() > image.width()) {
				_image = image.scaledToHeight(_limit, Qt::TransformationMode::SmoothTransformation);
			} else if (image.height() == image.width()) {
				_image = image.scaled(_limit, _limit, Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation);
			}
		}

		QString format("JPEG");
		switch (_format) {
			case ImageThumbnail::Format::GIF: format = "GIF"; break;
			case ImageThumbnail::Format::JPEG: format = "JPEG"; break;
			case ImageThumbnail::Format::HEIF: format = "HEIF"; break;
			case ImageThumbnail::Format::PNG: format = "PNG"; break;
		}

		return _image.save(_buffer, format.toLatin1().data(), (_quality > 100 || _quality < 0) ? -1 : _quality);
	}

	bool ImageThumbnail::process(QIODevice& device)
	{
		QImageReader image_reader(&device);
		image_reader.setAutoDetectImageFormat(true);
		qDebug() << "Process" << image_reader.format() << "/" <<image_reader.subType() << "image" << image_reader.size().width() << "x" << image_reader.size().height();

		_buffer->seek(0);
		_byte_array.clear();

		if (!image_reader.canRead() && (image_reader.size().height() <= 0 || image_reader.size().width() <= 0)) {
			_image = QImage();
		} else {
			_image = image_reader.read();
		}

		if (_image.isNull()) {
			return false;
		} else if (qMax(_image.width(), _image.height()) > _limit) {
			if (_image.width() > _image.height()) {
				_image = _image.scaledToWidth(_limit, Qt::TransformationMode::SmoothTransformation);
			} else if (_image.height() > _image.width()) {
				_image = _image.scaledToHeight(_limit, Qt::TransformationMode::SmoothTransformation);
			} else if (_image.height() == _image.width()) {
				_image = _image.scaled(_limit, _limit, Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation);
			}
		}

		QString format("JPEG");
		switch (_format) {
			case ImageThumbnail::Format::GIF: format = "GIF"; break;
			case ImageThumbnail::Format::JPEG: format = "JPEG"; break;
			case ImageThumbnail::Format::HEIF: format = "HEIF"; break;
			case ImageThumbnail::Format::PNG: format = "PNG"; break;
		}

		return _image.save(_buffer, format.toLatin1().data(), (_quality > 100 || _quality < 0) ? -1 : _quality);
	}

	const QImage& ImageThumbnail::getImage() const
	{
		return _image;
	}

	QBuffer* ImageThumbnail::getBuffer() const
	{
		return _buffer.data();
	}

	void ImageThumbnail::setQuality(int quality)
	{
		_quality = quality;
	}

	void ImageThumbnail::setLimit(int limit)
	{
		_limit = limit;
	}

	bool ImageThumbnail::isBusy() const
	{
		return _watcher.isRunning();
	}

	bool ImageThumbnail::isReady() const
	{
		return !_image.isNull();
	}
} } } }
