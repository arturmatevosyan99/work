#ifndef Cadabra_Queen_Conversation_Model_AbstractTrack_hpp
#define Cadabra_Queen_Conversation_Model_AbstractTrack_hpp

#include "Application/SmartCourse.hpp"
#include "../Meta/Track.hpp"
#include "../../Repository/Model/AbstractDescriptor.hpp"

#include <QPointer>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class AbstractTrack : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::Model::AbstractDescriptor* courseDescriptor READ getCourseDescriptor NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::SmartCare::Meta::Track::Scheme scheme READ getScheme NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd schemeQd READ getSchemeQd NOTIFY imported)
		Q_PROPERTY(QString previewText READ makePreviewText NOTIFY imported)
		Q_PROPERTY(QString key READ getKey NOTIFY imported)
		Q_PROPERTY(int revisionCode READ getRevisionCode NOTIFY imported)
		Q_PROPERTY(int languageCode READ getLanguageCode NOTIFY imported)
		Q_PROPERTY(QString title READ getTitle NOTIFY imported)
		Q_PROPERTY(QString description READ getDescription NOTIFY imported)
		Q_PROPERTY(QString flag READ getFlag NOTIFY imported)
		Q_PROPERTY(QString contentType READ getContentType NOTIFY imported)
		Q_PROPERTY(int contentSize READ getContentSize NOTIFY imported)
		Q_PROPERTY(int contentLength READ getContentLength NOTIFY imported)
		Q_PROPERTY(QDateTime lifeDateTime READ getLifeDateTime NOTIFY filed)
		Q_PROPERTY(QString file READ getFile NOTIFY filed)
		Q_PROPERTY(QDateTime fileDateTime READ getFileDateTime NOTIFY imported)
		Q_PROPERTY(QJsonObject meta READ getMeta NOTIFY imported)
		Q_PROPERTY(QDateTime metaDateTime READ getMetaDateTime NOTIFY imported)
		Q_PROPERTY(double remoteProgress READ getRemoteProgress NOTIFY progressed)
		Q_PROPERTY(QDateTime remoteProgressDateTime READ getRemoteProgressDateTime NOTIFY progressed)
		Q_PROPERTY(double localProgress READ getLocalProgress NOTIFY progressed)
		Q_PROPERTY(QDateTime localProgressDateTime READ getLocalProgressDateTime NOTIFY progressed)
		Q_PROPERTY(double actualProgress READ getActualProgress NOTIFY progressed)
		Q_PROPERTY(bool isImported READ isImported NOTIFY imported)
		Q_PROPERTY(bool isSynchronized READ isSynchronized NOTIFY progressed)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractTrack(QObject* parent = nullptr);
		virtual ~AbstractTrack() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QPointer<Repository::Model::AbstractDescriptor> _course_descriptor;
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

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool import(SmartCourse* course_application, const Cadabra::Queen::SmartCare::Meta::Track& track_meta);
		Q_INVOKABLE bool import(Cadabra::Queen::SmartCare::Model::SmartCourse* course_application, const QString& key, qint16 revision_code, qint16 language_code, const QString& title, const QString& description, const QString& artwork, const QString& flag, qint32 weight, const QString& content_type, qint32 content_size, qint32 content_length, const QDateTime& life_time, const QString& file, const QDateTime& file_time, const QJsonObject& meta, const QDateTime& meta_time, double remote_progress, const QDateTime& remote_progress_time, double local_progress, const QDateTime& local_progress_time);
		Q_INVOKABLE bool sign(const QString& path, const QDateTime& time);
		Q_INVOKABLE bool update(double value, const QDateTime& time);
		Q_INVOKABLE bool progress(double value);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QString makePreviewText() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Repository::Model::AbstractDescriptor* getCourseDescriptor() const;
		Meta::Track::SchemeIdent getSchemeIdent() const;
		Cadabra::Queen::Core::Meta::Qd getSchemeQd() const;
		Meta::Track::Scheme getScheme() const;

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

		const double& getActualProgress() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void filed();
		void imported();
		void progressed();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasCourseDescriptor() const;
		bool isImported() const;
		bool isSynchronized() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
