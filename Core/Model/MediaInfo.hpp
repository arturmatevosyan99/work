#ifndef Cadabra_Queen_Core_Model_MediaInfo_hpp
#define Cadabra_Queen_Core_Model_MediaInfo_hpp

#include <QAbstractListModel>

#include "../Net/Call/MediaGrab.hpp"

#include <QScopedPointer>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Core { namespace Model {
	class MediaInfo : public QAbstractListModel
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Service
		{
			Unknown = '0',
			Youtube = 'Y',
			Vimeo = 'V',
			Cube = 'C',
			HTTP = 'H'
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Service)
		Q_PROPERTY(Service service READ getService NOTIFY grabbed)
		Q_PROPERTY(QString title READ getTitle NOTIFY grabbed)
		Q_PROPERTY(QString description READ getDescription NOTIFY grabbed)
		Q_PROPERTY(QUrl thumbnail READ getThumbnail NOTIFY grabbed)
		Q_PROPERTY(QUrl rawSource READ getRawSource WRITE setRawSource NOTIFY rawSourceChanged)
		Q_PROPERTY(QUrl defaultSource READ getDefaultSource NOTIFY defaultSourceChanged)
		Q_PROPERTY(QString defaultAlias READ getDefaultAlias WRITE setDefaultAlias NOTIFY defaultAliasChanged)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit MediaInfo(QObject* parent = nullptr);
		virtual ~MediaInfo();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QScopedPointer<Net::MediaGrab> _grabber;
		Service _service;
		QUrl _raw_source;
		QString _default_alias;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const MediaInfo::Service& getService() const;

		QString getTitle() const;
		QString getDescription() const;
		QUrl getThumbnail() const;

		const QUrl& getRawSource() const;
		QUrl getDefaultSource() const;
		QString getDefaultAlias() const;

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setRawSource(const QUrl& source);
		void setDefaultAlias(const QString& alias);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void grabbed();
		void rawSourceChanged();
		void defaultAliasChanged();
		void defaultSourceChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onGrabbed();
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};

	class DefaultGrab : public Net::MediaGrab
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit DefaultGrab(const QUrl& url);
		virtual ~DefaultGrab() override = default;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool fetch() override;
	/** @ */
	};
} } } }

#endif
