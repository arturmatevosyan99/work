#ifndef Cadabra_Queen_Mentalgami_Model_Application_SmartCourse_hpp
#define Cadabra_Queen_Mentalgami_Model_Application_SmartCourse_hpp

#include "../../../Repository/Model/AbstractApplication.hpp"

#include "../../../Repository/Model/Application/Interface/PlaylistInterface.hpp"
#include "../../../Repository/Model/Application/Interface/URLInterface.hpp"

#include "../../Data/Entity/mg_CourseBrief.hpp"
#include "../../Data/Entity/mg_CourseTrack.hpp"

#include <QSemaphore>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	/**
	 * @brief Playlist for interactive course
	 * @todo Дать возможность пирнудительно реиндексировать(полезно для режима супервайзера)
	 */
	class SmartCourse : public Repository::Model::AbstractApplication, Cadabra::Queen::Repository::Model::URLInterface, Cadabra::Queen::Repository::Model::PlaylistInterface
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class IndexFlag
		{
			Complete = 0b00,
			Partial = 0b01,
			Loading = 0b10
		};
		Q_DECLARE_FLAGS(IndexState, IndexFlag)

		enum class ProgressFlag
		{
			Actual = 0b00,
			Modified = 0b01,
			Committing = 0b10
		};
		Q_DECLARE_FLAGS(ProgressState, ProgressFlag)

		enum class TrackMode
		{
			Update = 'u',
			Upsert = 'U',
			Ignore = 'i',
			Insert = 'I'
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_INTERFACES(Cadabra::Queen::Repository::Model::URLInterface Cadabra::Queen::Repository::Model::PlaylistInterface)
		Q_FLAGS(IndexState)
		Q_FLAGS(ProgressState)
		/**
		 * @brief Available playlists for an opened course
		 */
		Q_PROPERTY(QAbstractListModel* briefModel READ getBriefModel NOTIFY opened)
		/**
		 * @brief Current/active playlist
		 */
		Q_PROPERTY(QAbstractListModel* trackModel READ getTrackModel NOTIFY opened)
		/**
		 * @brief Indexed items count
		 */
		Q_PROPERTY(int indexSize READ getIndexSize NOTIFY indexed)
		/**
		 * @brief Total items count
		 */
		Q_PROPERTY(int overviewSize READ getOverviewSize NOTIFY overviewed)
		/**
		 * @brief ?
		 */
		Q_PROPERTY(Cadabra::Queen::Mentalgami::Model::SmartCourse::IndexState indexState READ getIndexState NOTIFY indexed)
		/**
		 * @brief ?
		 */
		Q_PROPERTY(Cadabra::Queen::Mentalgami::Model::SmartCourse::ProgressState progressState READ getProgressState NOTIFY progressed)
		/**
		 * @brief ?
		 */
		Q_PROPERTY(bool activeLicense READ hasActiveLicense NOTIFY overviewed)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SmartCourse(QObject* parent = nullptr);
		virtual ~SmartCourse() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QSharedPointer<Data::CourseBrief> _brief_data;
		QSharedPointer<Data::CourseTrack> _track_data;

		qint64 _license_life_time;
		qint64 _over_view_time;

		QSemaphore _index_semaphore;
		QSemaphore _progress_semaphore;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		/**
		 * @brief index
		 * @return
		 */
		Q_INVOKABLE bool index();

		/**
		 * @brief index
		 * @param key
		 * @param limit
		 * @return
		 */
		Q_INVOKABLE bool index(const QString& key, qint32 limit = 10);

		/**
		 * @brief progress
		 * @return
		 */
		Q_INVOKABLE bool progress();

		/**
		 * @brief progress
		 * @param key
		 * @param value
		 * @return
		 */
		Q_INVOKABLE bool progress(const QString& key, double value);

		/**
		 * @brief file
		 * @param key
		 * @return
		 */
		Q_INVOKABLE Cadabra::Queen::Core::Net::CRPPromise file(const QString& key);

	private:
		bool resetBriefModel(qint16 revision_code = 0, qint16 language_code = 0);
		bool resetTrackModel();
		bool track(const QVector<Repository::Model::Track>& vector, TrackMode mode = TrackMode::Upsert);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QAbstractListModel* getTrackModel() const;
		QAbstractListModel* getBriefModel() const;

		Core::Data::AbstractEntity* getTrackEntity() const;
		Core::Data::AbstractEntity* getBriefEntity() const;

		QDateTime getProgressDateTime() const;
		QMap<QString, double> getProgressMap() const;
		QString getIndexKey() const;

		qsizetype getIndexSize() const;
		qsizetype getOverviewSize() const;

		QFlags<IndexFlag> getIndexState() const;
		QFlags<ProgressFlag> getProgressState() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void synchronized();
		void overviewed();
		void indexed();
		void progressed();
		void fetched();
		void denied();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void customEvent(QEvent* event) override;
		virtual bool onHandle() override;
		virtual bool onOpen() override;
		virtual bool onSetup() override;
		virtual bool onSetdown() override;
		virtual bool onClose() override;
		virtual bool onTeardown() override;

		virtual void onSummaryOverviewed(quint8 domain, quint32 code, const QString& message) override;
		virtual void onSummaryOverviewed(const Repository::Model::Playmeta& play_meta) override;

		virtual void onSequenceIndexed(quint8 domain, quint32 code, const QString& message) override;
		virtual void onSequenceIndexed(const QVector<Repository::Model::Track>& track_vector) override;

		virtual void onPlaybackPaused(quint8 domain, quint32 code, const QString& message) override;
		virtual void onPlaybackPaused(const QVector<Repository::Model::Track>& track_vector) override;

		virtual void onFetched(Repository::Model::URLInterface::Type type, const QUrl& url) override;
		virtual void onSigned(Type type, const QUrl& url, const QDateTime& life_time, const QVariantMap& token_map, const QString& view_preset) override;

	private:
		void onFile(const Core::Net::CRPReaction& reaction, Repository::Meta::Subject::Id subject_id, int revision_code, int language_code, QString key);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasFullIndex() const;
		bool hasLocalProgress() const;
		bool hasActiveLicense() const;
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Mentalgami::Model::SmartCourse::IndexState)
Q_DECLARE_METATYPE(Cadabra::Queen::Mentalgami::Model::SmartCourse::IndexState)
Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Mentalgami::Model::SmartCourse::ProgressState)
Q_DECLARE_METATYPE(Cadabra::Queen::Mentalgami::Model::SmartCourse::ProgressState)

#endif
