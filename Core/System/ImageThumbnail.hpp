#ifndef Cadabra_Queen_Core_System_ImageThumbnail_hpp
#define Cadabra_Queen_Core_System_ImageThumbnail_hpp

#include <QObject>
#include <QByteArray>
#include <QBuffer>
#include <QFutureWatcher>
#include <QPointer>
#include <QImage>
#include <QImageReader>
#include <QFile>
#include <QString>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class ImageThumbnail : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Format {
			JPEG,
			PNG,
			HEIF,
			GIF
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Format)
		Q_PROPERTY(QBuffer* buffer READ getBuffer NOTIFY flushed)
		Q_PROPERTY(Format format MEMBER _format)
		Q_PROPERTY(int quality MEMBER _quality)
		Q_PROPERTY(int limit MEMBER _limit)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QMap<QString, ImageThumbnail::Format> FormatMap;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ImageThumbnail(QObject* parent = nullptr);
		virtual ~ImageThumbnail() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Format _format;
		int _quality;
		int _limit;

		QImage _image;
		QByteArray _byte_array;
		QPointer<QBuffer> _buffer;
		QFutureWatcher<bool> _watcher;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool enqueue(const QUrl& source_url, bool source_autoremove);
		Q_INVOKABLE void cancel();
		Q_INVOKABLE bool flush();

		bool process(const QUrl& source_url, bool source_autoremove);
		bool process(const QImage& image);
		bool process(QIODevice& device);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QImage& getImage() const;
		QBuffer* getBuffer() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setQuality(int quality);
		void setLimit(int limit);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void started();
		void finished();
		void flushed();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		Q_INVOKABLE bool isBusy() const;
		Q_INVOKABLE bool isReady() const;
	/** @} */
	};
} } } }

#endif
