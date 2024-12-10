/**
 @file Track.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_SmartCare_Meta_Track_hpp
#define Cadabra_Queen_SmartCare_Meta_Track_hpp

#include "../../Repository/Meta/Essence.hpp"
#include "../../Repository/Meta/Subject.hpp"

#include <QObject>
#include <QSharedPointer>
#include <QJsonObject>
#include <QDateTime>
#include <QUuid>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Meta {
	/**
	 * @brief Track meta information
	 */
	class Track
	{
	/** @name Aliases */
	/** @{ */
	public:
		/**
		 * @brief Field role in a data model
		 */
		enum class Role : int
		{
			Key = Qt::UserRole + 1,
			RevisionCode,
			LanguageCode,
			Title,
			Description,
			Artwork,
			Flag,
			Weight,
			ContentType,
			ContentSize,
			ContentLength,
			LifeTimestamp,
			File,
			FileTimestamp,
			Meta,
			MetaTimestamp,
			RemoteProgress,
			RemoteProgressTimestamp,
			LocalProgress,
			LocalProgressTimestamp,
		};

		/**
		 * @brief Track special flag
		 */
		enum class Flag : char
		{
			Obsolete = 'o',
			Disabled = 'd',
			Hidden = 'h',
			Automatic = 'a',
			Unpaid = 'P',
			Mistimed = 'T',
			Disordered = 'O',
			Denied = 'D'
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_ENUM(Role)
		Q_PROPERTY(QString key READ getKey)
		Q_PROPERTY(qint16 revisionCode READ getRevisionCode)
		Q_PROPERTY(qint16 languageCode READ getLanguageCode)
		Q_PROPERTY(QString title READ getTitle)
		Q_PROPERTY(QString description READ getDescription)
		Q_PROPERTY(QString artwork READ getArtwork)
		Q_PROPERTY(QString flag READ getFlag)
		Q_PROPERTY(QString contentType READ getContentType)
		Q_PROPERTY(qsizetype contentSize READ getContentSize)
		Q_PROPERTY(qsizetype contentLength READ getContentLength)
		Q_PROPERTY(QString file READ getFile)
		Q_PROPERTY(QDateTime fileDateTime READ getFileDateTime)
		Q_PROPERTY(QJsonObject meta READ getMeta)
		Q_PROPERTY(QDateTime metaDateTime READ getMetaDateTime)
		Q_PROPERTY(double remoteProgress READ getRemoteProgress)
		Q_PROPERTY(QDateTime remoteProgressDateTime READ getRemoteProgressDateTime)
		Q_PROPERTY(double localProgress READ getLocalProgress)
		Q_PROPERTY(QDateTime localProgressDateTime READ getLocalProgressDateTime)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using Key = QString;
		using Scheme = Repository::Meta::Essence::Scheme;
		using SchemeIdent = Repository::Meta::Essence::SchemeIdent;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Track();
		Track(const QString& key, qint16 revision_code, qint16 language_code, const QString& title, const QString& description, const QString& artwork, const QString& flag, qint32 weight, const QString& content_type, qint32 content_size, qint32 content_length, const QDateTime& life_time, const QString& file, const QDateTime& file_time, const QJsonObject& meta, const QDateTime& meta_time, double remote_progress, const QDateTime& remote_progress_time, double local_progress = 0, const QDateTime& local_progress_time = QDateTime());
		Track(const Track& origin);
		Track(Track&& origin);
		~Track();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _key;
		qint16 _revision_code;
		qint16 _language_code;
		QString _title;
		QString _description;
		QString _artwork;
		QString _flag;
		qint32 _weight;
		QString _content_type;
		qint32 _content_size;
		qint32 _content_length;
		QDateTime _life_time;
		QString _file;
		QDateTime _file_time;
		QJsonObject _meta;
		QDateTime _meta_time;
		double _remote_progress;
		QDateTime _remote_progress_time;
		double _local_progress;
		QDateTime _local_progress_time;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator QJsonObject() const;
		Track& operator=(const Track& rhs);
		Track& operator=(Track&& rhs);
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QVariant get(int role) const;
		QVariant get(Role role) const;

		const QString& getKey() const;
		const qint16& getRevisionCode() const;
		const qint16& getLanguageCode() const;

		const QString& getTitle() const;
		const QString& getDescription() const;
		const QString& getArtwork() const;

		const QString& getFlag() const;
		const qint32& getWeight() const;

		const QString& getContentType() const;
		const qint32& getContentSize() const;
		const qint32& getContentLength() const;

		const QDateTime& getLifeDateTime() const;
		const QString& getFile() const;
		const QDateTime& getFileDateTime() const;

		const QJsonObject& getMeta() const;
		const QDateTime& getMetaDateTime() const;

		const double& getRemoteProgress() const;
		const QDateTime& getRemoteProgressDateTime() const;

		const double& getLocalProgress() const;
		const QDateTime& getLocalProgressDateTime() const;
	/** @} */

	/** @name Formatters */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name Templates */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
