#ifndef Cadabra_Queen_Core_Net_Call_MediaGrab_hpp
#define Cadabra_Queen_Core_Net_Call_MediaGrab_hpp

#include "../ServiceCall.hpp"

#include <QObject>
#include <QVariant>
#include <QString>
#include <QSize>
#include <QUrl>
#include <QMap>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class MediaGrab : public ServiceCall
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			Alias = Qt::UserRole + 1,
			Size, /// < Bytes
			Duration, /// < Seconds
			URL,
			VideoResolution,
			VideoCodec,
			AudioQuality,
			AudioCodec
		};
	/** @} */

	public:
		static const QMap<QString, QSize> VideoResolutionMap;

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Role)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		MediaGrab(Core::Net::Endpoint* endpoint, QString resource);
		MediaGrab(Core::Net::Endpoint* endpoint);
		MediaGrab(const QUrl& url);
		virtual ~MediaGrab() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QMap<QString, QString> _title_map;
		QMap<QString, QString> _description_map;
		QMap<QString, QUrl> _thumbnail_map;
		QMap<QString, QUrl> _stream_map;
		QMap<QString, QVariant> _variant_map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool fetch() = 0;
		virtual bool fetch(bool allow_cache) final;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QVariant getStreamRoleValue(qsizetype index, int role) const;

		QString getStreamKey(qsizetype index) const;

		QString getStreamAlias(const QString& key = "") const;
		qsizetype getStreamSize(const QString& key = "") const;
		qsizetype getStreamDuration(const QString& key = "") const;
		QUrl getStreamURL(const QString& key = "") const;
		QString getStreamVideoResolution(const QString& key = "") const;
		QString getStreamVideoCodec(const QString& key = "") const;
		QString getStreamAudioQuality(const QString& key = "") const;
		QString getStreamAudioCodec(const QString& key = "") const;


		QUrl getSource() const;
		QString getTitle() const;
		QString getDescription() const;
		QUrl getThumbnail() const;
		qsizetype getCount() const;

		const QMap<QString, QString>& getTitleMap() const;
		const QMap<QString, QString>& getDescriptionMap() const;
		const QMap<QString, QUrl>& getThumbnailMap() const;
		const QMap<QString, QUrl>& getStreamMap() const;
		const QMap<QString, QVariant>& getVariantMap() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	protected:
		void addTitle(const QString& language_code, const QString& title);
		void addDescription(const QString& language_code, const QString& description);
		void addThumbnail(const QString& language_code, const QUrl& url);
		void addStream(const QString&  key, const QUrl& url);
		void addVariant(const QString& key, const QVariant& value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void fetched();
		void faulted(QString error);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	private:
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
	/** @} */
	};
} } } }

#endif
