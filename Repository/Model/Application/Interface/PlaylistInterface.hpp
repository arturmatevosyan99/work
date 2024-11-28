#ifndef Cadabra_Queen_Repository_Model_Application_Interface_PlaylistInterface_hpp
#define Cadabra_Queen_Repository_Model_Application_Interface_PlaylistInterface_hpp

#include "../AbstractInterface.hpp"

#include <QtPlugin>

#include <QSet>
#include <QMap>
#include <QString>
#include <QJsonObject>
#include <QDateTime>
#include <QMimeType>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	struct Playmeta
	{
		QString title;
		QString description;
		QString author;
		QString copyright;
		QString artwork;
		qint16 revision_code;
		qint16 language_code;
		QDateTime release_datetime;
		qsizetype count;
		QString key;
		QMap<QString, QVariant> map;

		operator QJsonObject() const;
	};

	struct Track
	{
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

		QString key;
		QSet<Flag> flagset;

		/// @brief release datetime
		QDateTime file_datetime;

		QString title;
		QString description;
		QString artwork;

		/// @brief Order
		qint32 weight;

		QString content_type; /// < How about QMimeType?
		qint32 content_length;  /// < negative(-1) means unknown, zerp means broken or empty, bytes if positive
		qint32 content_size; /// < negative(-1) means unknown, zero means endless(a live stream); positive means metered content in any unit type (page, second, etc)

		/// @brief Anything: genre, tags, section, thumbnails(sq, hq)
		QMap<QString, QVariant> meta_map;
		QDateTime meta_datetime;

		/// @brief Negative means ratio, positive means position
		double progress;
		QDateTime progress_datetime;

		operator QJsonObject() const;
	};

	class PlaylistInterface : public virtual Repository::Model::AbstractInterface
	{
	/** @name Statics */
	/** @{ */
	public:
		static Playmeta OverviewSummary(const QJsonObject& input);
		static QVector<Track> IndexSequence(const QJsonArray& input);
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		PlaylistInterface() = default;
		virtual ~PlaylistInterface() = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Core::Net::CRPDelay overviewSummary();
		Core::Net::CRPDelay indexSequence(const QString& key, qint32 limit = 0);
		Core::Net::CRPDelay pausePlayback(const QMap<QString, double>& map, const QDateTime& datetime);
		Core::Net::CRPDelay pausePlayback(const QString& key, double progress, const QDateTime& datetime);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onSummaryOverview(const Core::Net::CRPReaction& reaction);
		virtual void onSummaryOverviewed(quint8 domain, quint32 code, const QString& message);
		virtual void onSummaryOverviewed(const Playmeta& data);

		virtual void onSequenceIndex(const Core::Net::CRPReaction& reaction);
		virtual void onSequenceIndexed(quint8 domain, quint32 code, const QString& message);
		virtual void onSequenceIndexed(const QVector<Track>& tack_vector);

		virtual void onPlaybackPause(const Core::Net::CRPReaction& reaction);
		virtual void onPlaybackPaused(quint8 domain, quint32 code, const QString& message);
		virtual void onPlaybackPaused(const QVector<Track>& tack_vector);
	/** @} */
	};

	inline uint qHash(Track::Flag key, uint seed)
	{
		return ::qHash(static_cast<quint8>(key), seed);
	}
} } } }

Q_DECLARE_INTERFACE(Cadabra::Queen::Repository::Model::PlaylistInterface, "cadabra.queen.repository.model.playlist/1.0");

#endif
