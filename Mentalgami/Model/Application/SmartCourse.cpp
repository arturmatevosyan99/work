#include "SmartCourse.hpp"

#include "../../../Core/Data/Expression/RoundExpression.hpp"

#include "../../../Qaos/MakeUnique.hpp"

#include <QtGlobal>
#include <QString>
#include <QSqlRecord>
#include <functional>
#include <cmath>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	SmartCourse::SmartCourse(QObject* parent)
	:
		Repository::Model::AbstractApplication(parent),

		_license_life_time(0),
		_over_view_time(0),
		_index_semaphore(1),
		_progress_semaphore(1)
	{
	}

	SmartCourse::~SmartCourse()
	{
		_index_semaphore.release(1);
		_progress_semaphore.release(1);
	}

	bool SmartCourse::index()
	{
		if (hasFullIndex()) {
			return true;
		}
		const QString index_key(getIndexKey());
		return !index_key.isEmpty() ? index(index_key) : false;
	}

	bool SmartCourse::index(const QString& key, qint32 limit)
	{
		if (key.isEmpty() || limit <= 0) {
			return true;
		} else if (_index_semaphore.tryAcquire(1)) {
			emit indexed();
			PlaylistInterface::indexSequence(key, limit);
			return true;
		}
		return false;
	}

	bool SmartCourse::progress()
	{
		if (!_progress_semaphore.tryAcquire(1)) {
			return false;
		}
		PlaylistInterface::pausePlayback(getProgressMap(), getProgressDateTime());
		emit progressed();
		return true;
	}

	bool SmartCourse::progress(const QString& key, double value)
	{
		if (_brief_data->isEmpty()) {
			return false;
		} else if (qIsNaN(value)) {
			return false;
		}

		Data::CourseTrack data(getDescriptor()->getObjectId());
		for (const Core::Data::AbstractEntity::FieldMap::mapped_type& field : data.getFieldMap()) {
			field.get().setGenerated(false);
		}
		data.local_progress.setGenerated(true);
		data.local_progress.setValue(value);
		data.local_progress_timestamp.setGenerated(true);
		data.local_progress_timestamp.setValue(QDateTime::currentDateTimeUtc());

		Core::Data::Factory data_factory(getDescriptor()->getRealm()->getDataProvider()->makeFactory());
		if (!data_factory.update(
									&data,
									Qaos::MakeUnique<Core::Data::CompoundPredicate>
									(
										std::initializer_list<QSharedPointer<const Core::Data::WherePredicate>>
										{
											QSharedPointer<Core::Data::ConditionPredicate>::create
											(
												data.key,
												Core::Data::Field
												(
													Core::Data::Field::Type::Text,
													key
												),
												Core::Data::ConditionPredicate::Condition::Equal
											),
											QSharedPointer<Core::Data::ConditionPredicate>::create
											(
												data.revision_code,
												Core::Data::Field(
													Core::Data::Field::Type::Int,
													_brief_data->revision_code.value().toInt()
												),
												Core::Data::ConditionPredicate::Condition::Equal
											),
											QSharedPointer<Core::Data::ConditionPredicate>::create
											(
												data.language_code,
												Core::Data::Field
												(
													Core::Data::Field::Type::Int,
													_brief_data->language_code.value().toInt()
												),
												Core::Data::ConditionPredicate::Condition::Equal
											),
											QSharedPointer<Core::Data::CompoundPredicate>::create
											(
												std::initializer_list<QSharedPointer<const Core::Data::WherePredicate>>
												{
													QSharedPointer<Core::Data::ConditionPredicate>::create
													(
														data.local_progress,
														Core::Data::Field(Core::Data::Field::Type::Real, QVariant()),
														Core::Data::ConditionPredicate::Condition::Is
													),
													QSharedPointer<Core::Data::ConditionPredicate>::create
													(
														data.local_progress,
														Core::Data::Field
														(
															Core::Data::Field::Type::Real,
															value
														),
														Core::Data::ConditionPredicate::Condition::Less
													)
												},
												Core::Data::CompoundPredicate::Logic::Or
											)
										},
										Core::Data::CompoundPredicate::Logic::And
									).get()
								 )
		) {
			return false;
		} else if (data_factory.getAffectedRowsCount() <= 0) {
			return false;
		}
		return progress();
	}

	Core::Net::CRPPromise SmartCourse::file(const QString& key)
	{
		if (
			key.isEmpty()
			||
			_brief_data->isEmpty()
			||
			_brief_data->subject_id.isNull()
			||
			_brief_data->revision_code.isNull()
			||
			_brief_data->language_code.isNull()
		) {
			return Core::Net::CRPPromise{};
		}
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::URLInterface",
			"Cadabra::Ace::Repository::Model::Sign",
			{
				{"name", key}
			}
		).detach(
				std::bind
				(
					&SmartCourse::onFile, this,
					std::placeholders::_1,
					_brief_data->subject_id.value().toLongLong(),
					_brief_data->revision_code.value().toInt(),
					_brief_data->language_code.value().toInt(),
					key
				)
		);
	}

	bool SmartCourse::resetBriefModel(qint16 revision_code, qint16 language_code)
	{
		if (!_brief_data) {
			return false;
		} else if (_track_data->isOpened() && !_track_data->isEmpty() && _track_data->first()) {
			if (_track_data->revision_code.value().toLongLong() != revision_code || _track_data->language_code.value().toLongLong() != language_code) {
				_track_data->close();
			}
		}

		Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());

		Repository::Meta::Subject::Id subject_id(getDescriptor()->getSubjectId());
		const QVariant opt_subject_vid(getDescriptor()->opt({"subject_id"}));
		if (opt_subject_vid.isValid()) {
			if (opt_subject_vid.canConvert(QMetaType::QString)) {
				subject_id = Repository::Meta::Essence::ToId(opt_subject_vid.toString());
			} else if (opt_subject_vid.canConvert(QMetaType::LongLong)) {
				subject_id = Repository::Meta::Essence::ToId(opt_subject_vid.toLongLong());
			} else {
				qWarning() << "Opt subject id is missed or malformed";
				return false;
			}
			if (getDescriptor()->getSubjectId() != subject_id) {
				Data::CourseTrack track_data(getDescriptor()->getObjectId());
				Core::Data::Factory data_factory(data_provider->makeFactory());
				if (!data_factory.remove(
										&track_data,
										Qaos::MakeUnique<Core::Data::ConditionPredicate>
										(
											track_data.subject_id,
											Core::Data::Field(Core::Data::Field::Type::Integer, subject_id),
											Core::Data::ConditionPredicate::Condition::Equal
										).get()
				)) {
					qWarning() << "Clean error";
					return false;
				}
			}
		}

		_brief_data->resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::And);
		_brief_data->addPredicate(
									QSharedPointer<Core::Data::ConditionPredicate>::create
									(
										_brief_data->subject_id,
										Core::Data::Field(Core::Data::Field::Type::Integer, subject_id),
										Core::Data::ConditionPredicate::Condition::Equal
									)
		);
		if (language_code > 0) {
			_brief_data->addPredicate(
										QSharedPointer<Core::Data::ConditionPredicate>::create
										(
											_brief_data->language_code,
											Core::Data::Field(Core::Data::Field::Type::Int, language_code),
											Core::Data::ConditionPredicate::Condition::Equal
										)
			);
		}
		if (revision_code > 0) {
			_brief_data->addPredicate(
										QSharedPointer<Core::Data::ConditionPredicate>::create
										(
											_brief_data->revision_code,
											Core::Data::Field(Core::Data::Field::Type::Int, revision_code),
											Core::Data::ConditionPredicate::Condition::Equal
										)
			);
		}

		_license_life_time = 0;
		_over_view_time = 0;

		if (!data_provider->open(_brief_data.get(), {},  Core::Data::Provider::RowsetType::Regular)) {
			return false;
		} else if (!_brief_data->isEmpty()) {
			const QVariantMap meta(_brief_data->meta.value().toJsonObject().toVariantMap());
			_license_life_time = meta.value("license_life_time", 0).toLongLong();
			_over_view_time = meta.value("over_view_time", 0).toLongLong();
		}
		return true;
	}

	bool SmartCourse::resetTrackModel()
	{
		if (!_track_data || !_brief_data || _brief_data->isEmpty()) {
			return false;
		}

		_track_data->resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::And);
		_track_data->addPredicate(
									QSharedPointer<Core::Data::ConditionPredicate>::create
									(
										_track_data->subject_id,
										Core::Data::Field(Core::Data::Field::Type::Integer, _brief_data->subject_id.value().toLongLong()),
										Core::Data::ConditionPredicate::Condition::Equal
									)
		);
		_track_data->addPredicate(
									QSharedPointer<Core::Data::ConditionPredicate>::create
									(
										_track_data->revision_code,
										Core::Data::Field(Core::Data::Field::Type::Int, _brief_data->revision_code.value().toInt()),
										Core::Data::ConditionPredicate::Condition::Equal
									)
		);
		_track_data->addPredicate(
									QSharedPointer<Core::Data::ConditionPredicate>::create
									(
										_track_data->language_code,
										Core::Data::Field(Core::Data::Field::Type::Int, _brief_data->language_code.value().toInt()),
										Core::Data::ConditionPredicate::Condition::Equal
									)
		);

		return getDescriptor()->getRealm()->getDataProvider()->open(_track_data.get(), {Core::Data::Provider::OpenMode::Reverse}, Core::Data::Provider::RowsetType::Tele);
	}

	bool SmartCourse::track(const QVector<Repository::Model::Track>& vector, TrackMode mode)
	{
		if (vector.isEmpty()) {
			return true;
		}

		Core::Data::Factory data_factory(getDescriptor()->getRealm()->getDataProvider()->makeFactory());
		if (!data_factory.transact(Core::Data::Factory::TransactionPolicy::AutoRollback)) {
			return false;
		}

		std::function<bool(Core::Data::AbstractEntity*)> mutator;
		Core::Data::Rowset::Mutation mutation(Core::Data::Rowset::Mutation::Ignore);
		switch (mode) {
			case TrackMode::Ignore:
				mutator = std::bind((bool(Core::Data::Factory::*)(Core::Data::AbstractEntity*, Core::Data::Factory::InsertionPolicy))&Core::Data::Factory::insert, &data_factory, std::placeholders::_1, Core::Data::Factory::InsertionPolicy::Ignore);
				break;

			case TrackMode::Insert:
				mutator = std::bind((bool(Core::Data::Factory::*)(Core::Data::AbstractEntity*, Core::Data::Factory::InsertionPolicy))&Core::Data::Factory::insert, &data_factory, std::placeholders::_1, Core::Data::Factory::InsertionPolicy::Regular);
				mutation = Core::Data::Rowset::Mutation::Insert;
				break;

			case TrackMode::Upsert:
				mutator = std::bind((bool(Core::Data::Factory::*)(Core::Data::AbstractEntity*, Core::Data::Factory::InsertionPolicy))&Core::Data::Factory::insert, &data_factory, std::placeholders::_1, Core::Data::Factory::InsertionPolicy::Replace);
				/// @???
//				mutation = Core::Data::Rowset::Mutation::Insert;
				break;

			case TrackMode::Update:
				mutator = std::bind((bool(Core::Data::Factory::*)(Core::Data::AbstractEntity*))&Core::Data::Factory::update, &data_factory, std::placeholders::_1);
				mutation = Core::Data::Rowset::Mutation::Update;
				break;

			default:
				return false;
		}

		const QVariant revision_code(_brief_data->revision_code.value());
		const QVariant language_code(_brief_data->language_code.value());

		Core::Data::Rowset::HintTable hint_table;
		for (const QVector<Repository::Model::Track>::value_type& track : vector) {
			QString flag_string;
			for (const Repository::Model::Track::Flag& flag : track.flagset) {
				flag_string += static_cast<char>(flag);
			}
			Data::CourseTrack data(getDescriptor()->getObjectId());
			data.file.setGenerated(false);
			data.life_timestamp.setGenerated(false);
			data.local_progress.setGenerated(false);
			data.local_progress_timestamp.setGenerated(false);
			data.subject_id.setValue(_brief_data->subject_id.value().toLongLong());
			data.revision_code.setValue(revision_code);
			data.language_code.setValue(language_code);
			data.key.setValue(track.key);
			data.flag.setValue(flag_string);
			data.title.setValue(track.title);
			data.description.setValue(track.description);
			data.artwork.setValue(track.artwork);
			data.weight.setValue(track.weight > 0 ? QVariant::fromValue(track.weight) : QVariant::fromValue(nullptr));
			data.content_type.setValue(track.content_type);
			data.content_length.setValue(track.content_length);
			data.content_size.setValue(track.content_size);
			data.file_timestamp.setValue(track.file_datetime);
			data.meta.setValue(QJsonDocument(QJsonObject::fromVariantMap(track.meta_map)));
			data.meta_timestamp.setValue(track.meta_datetime);
			data.remote_progress.setValue(track.progress);
			data.remote_progress_timestamp.setValue(track.progress_datetime);

			if (!mutator(&data)) {
				return false;
			}
			if (data_factory.getAffectedRowsCount() > 0 && !_track_data->isEmpty()) {
				QSqlRecord hint_record(_track_data->record(nullptr));
				hint_record.setGenerated("subject_id", true);
				hint_record.setValue("subject_id", data.subject_id.value());

				hint_record.setGenerated("key", true);
				hint_record.setValue("key", data.key.value());

				hint_record.setGenerated("revision_code", true);
				hint_record.setValue("revision_code", data.revision_code.value());

				hint_record.setGenerated("langauge_code", true);
				hint_record.setValue("language_code", data.language_code.value());

				hint_record.setGenerated("weight", true);
				hint_record.setValue("weight", data.weight.value());

				hint_table.insert(mutation, hint_record);
			}
		}

		if (!data_factory.commit()) {
			return false;
		} else if (!_track_data->isOpened()) {
			if (!resetTrackModel()) {
				return false;
			}
		} else if (!_track_data->reopen(hint_table)) {
			qCritical("Tracklist open error");
			return false;
		} else if (!hint_table.isEmpty()) {
			emit indexed();
			emit progressed();
		}
		return true;
	}

	QAbstractListModel* SmartCourse::getTrackModel() const
	{
		return _track_data.get();
	}

	QAbstractListModel* SmartCourse::getBriefModel() const
	{
		return _brief_data.get();
	}

	Core::Data::AbstractEntity* SmartCourse::getTrackEntity() const
	{
		return _track_data.get();
	}

	Core::Data::AbstractEntity* SmartCourse::getBriefEntity() const
	{
		return _brief_data.get();
	}

	qsizetype SmartCourse::getIndexSize() const
	{
		return !_track_data || _track_data->isEmpty() ? 0 : getDescriptor()->getRealm()->getDataProvider()->count(_track_data.get(), true);
	}

	qsizetype SmartCourse::getOverviewSize() const
	{
		return !_brief_data || _brief_data->isEmpty() ? 0 : _brief_data->count.value().toLongLong();
	}

	QFlags<SmartCourse::IndexFlag> SmartCourse::getIndexState() const
	{
		/// @todo cache
		QFlags<SmartCourse::IndexFlag> retval;
		retval.setFlag(IndexFlag::Loading, _index_semaphore.available() < 1);
		retval.setFlag(IndexFlag::Partial, !hasFullIndex());
		return retval;
	}

	QFlags<SmartCourse::ProgressFlag> SmartCourse::getProgressState() const
	{
		/// @todo cache
		QFlags<SmartCourse::ProgressFlag> retval;
		retval.setFlag(ProgressFlag::Committing, _progress_semaphore.available() < 1);
		retval.setFlag(ProgressFlag::Modified, hasLocalProgress());
		return retval;
	}

	QDateTime SmartCourse::getProgressDateTime() const
	{
		if (!hasDescriptor()) {
			return {};
		} else if (!_brief_data->isOpened() || _brief_data->isEmpty()) {
			return {};
		}

		const Repository::Meta::Subject::Id subject_id(_brief_data->subject_id.value().toLongLong());
		if (getDescriptor()->getSubjectId() != subject_id) {
			return {};
		}

		const QScopedPointer<Data::CourseTrack> data(new Data::CourseTrack(getDescriptor()->getObjectId()));
		data->local_progress.setExpression(new Core::Data::RoundExpression(3));
		data->remote_progress.setExpression(new Core::Data::RoundExpression(3));
		data->resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::And);
		data->addPredicate(
							QSharedPointer<Core::Data::ConditionPredicate>::create
							(
								data->subject_id,
								Core::Data::Field(Core::Data::Field::Type::Integer, subject_id),
								Core::Data::ConditionPredicate::Condition::Equal
							)
		);
		data->addPredicate(
							QSharedPointer<Core::Data::ConditionPredicate>::create
							(
								data->remote_progress,
								Core::Data::Field(Core::Data::Field::Type::Integer, 0),
								Core::Data::ConditionPredicate::Condition::Greater
							)
		);
		data->addOrder(data->remote_progress_timestamp, Qt::SortOrder::DescendingOrder);
		data->setRowLimit(1);
		if (data->open(getDescriptor()->getRealm()->getDataProvider()) && !data->isEmpty()) {
			return data->remote_progress_timestamp.value().toDateTime();
		}
		return QDateTime::fromSecsSinceEpoch(_over_view_time, Qt::TimeSpec::UTC);
	}

	QMap<QString, double> SmartCourse::getProgressMap() const
	{
		if (!hasDescriptor()) {
			return {};
		} else if (!_brief_data->isOpened() || _brief_data->isEmpty()) {
			return {};
		}

		const Repository::Meta::Subject::Id subject_id(_brief_data->subject_id.value().toLongLong());
		if (getDescriptor()->getSubjectId() != subject_id) {
			return {};
		}

		const QScopedPointer<Data::CourseTrack> data(new Data::CourseTrack(getDescriptor()->getObjectId()));

		data->local_progress.setExpression(new Core::Data::RoundExpression(3));
		data->remote_progress.setExpression(new Core::Data::RoundExpression(3));

		data->addPredicate(
							QSharedPointer<Core::Data::ConditionPredicate>::create
							(
								data->subject_id,
								Core::Data::Field(Core::Data::Field::Type::Integer, subject_id),
								Core::Data::ConditionPredicate::Condition::Equal
							)
		);
		data->addPredicate(
							QSharedPointer<Core::Data::ConditionPredicate>::create
							(
								data->local_progress,
								data->remote_progress,
								Core::Data::ConditionPredicate::Condition::Greater
							)
		);

		Cadabra::Queen::Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());
		if (!data->open(data_provider)) {
			return {};
		} else if (data->isEmpty()) {
			return {};
		}

		QMap<QString, double> retval;
		do {
			retval.insert(data->key.value().toString(), data->local_progress.value().toDouble() - std::numeric_limits<double>::epsilon());
		} while (data->next());

		return retval;
	}

	QString SmartCourse::getIndexKey() const
	{
		if (_brief_data->isEmpty()) {
			return "";
		}

		Data::CourseTrack data(getDescriptor()->getObjectId());
		data.addPredicate
		(
			QSharedPointer<Core::Data::ConditionPredicate>::create
			(
				data.revision_code,
				Core::Data::Field(Core::Data::Field::Type::Int, _brief_data->revision_code.value().toInt()),
				Core::Data::ConditionPredicate::Condition::Equal
			)
		);
		data.addPredicate
		(
			QSharedPointer<Core::Data::ConditionPredicate>::create
			(
				data.language_code,
				Core::Data::Field(Core::Data::Field::Type::Int, _brief_data->language_code.value().toInt()),
				Core::Data::ConditionPredicate::Condition::Equal
			)
		);
		data.addOrder(data.weight, Qt::SortOrder::DescendingOrder);
		data.addOrder(data.key, Qt::SortOrder::DescendingOrder);
		data.setRowLimit(1);

		return (data.open(getDescriptor()->getRealm()->getDataProvider()) && !data.isEmpty()) ? data.key.value().toString() : _brief_data->key.value().toString();
	}

	void SmartCourse::onSummaryOverviewed(quint8 domain, quint32 code, const QString& message)
	{
		qWarning() << "Summary error:" << domain << code << message;
		emit exceptionThrew(MessageLevel::WarningMessage, tr("Could not make an overview of a playlist"));
	}
	void SmartCourse::onSummaryOverviewed(const Repository::Model::Playmeta& play_meta)
	{
		qDebug() << "Rrevision:" << play_meta.revision_code << "language:" << play_meta.language_code << "starts at:" << play_meta.key << "total count:" << play_meta.count;

		Data::CourseBrief data(getDescriptor()->getObjectId());
		data.artwork.setValue(play_meta.artwork);
		data.title.setValue(play_meta.title);
		data.description.setValue(play_meta.description);
		data.author.setValue(play_meta.author);
		data.copyright.setValue(play_meta.copyright);
		data.revision_code.setValue(play_meta.revision_code);
		data.language_code.setValue(play_meta.language_code);
		data.release_datetime.setValue(play_meta.release_datetime);
		data.count.setValue(play_meta.count);
		data.key.setValue(play_meta.key);
		data.meta.setValue(QJsonDocument(QJsonObject::fromVariantMap(play_meta.map)));
		if (play_meta.map.contains("license_subject_id")) {
			const QVariant opt_subject_vid(getDescriptor()->opt({"subject_id"}));
			Repository::Meta::Subject::Id opt_subject_id(Repository::Meta::Subject::DummyId);
			if (!opt_subject_vid.isValid() || opt_subject_vid.isNull()) {
				opt_subject_id = getDescriptor()->getSubjectId();
			} else if (opt_subject_vid.canConvert(QMetaType::QString)) {
				opt_subject_id = Repository::Meta::Essence::ToId(opt_subject_vid.toString());
			} else if (opt_subject_vid.canConvert(QMetaType::LongLong)) {
				opt_subject_id = Repository::Meta::Essence::ToId(opt_subject_vid.toLongLong());
			}
			const Repository::Meta::Subject::Id license_subject_id(Repository::Meta::Essence::ToId(play_meta.map.value("license_subject_id", "0").toString(), true));
			if (opt_subject_id != license_subject_id) {
				qWarning() << "On behalf of @" << opt_subject_id << "when licensed for" << license_subject_id << "in application opened with" << getDescriptor()->getSubjectId();
				return ;
			}
			data.subject_id.setValue(opt_subject_id);
		} else {
			data.subject_id.setValue(getDescriptor()->getSubjectId());
		}

		Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());
		Core::Data::Factory data_factory(data_provider->makeFactory());
		if (!data_factory.insert(&data, Core::Data::Factory::InsertionPolicy::Replace)) {
			qCritical("Upsert overview error");
			return ;
		} else if (!resetBriefModel(play_meta.revision_code, play_meta.language_code)) {
			qCritical("Overview model error");
			return ;
		} else if (_brief_data->isEmpty()) {
			qCritical("Overview dataset unexpected state");
			return ;
		}
		emit overviewed();
		if (_brief_data->count.value().toLongLong() > 0 && _track_data->isEmpty()) {
			if (!index()) {
				qCritical("Index error");
				return ;
			}
		} else if (!progress()) {
			qCritical("Progress update error");
			return ;
		}
	}

	void SmartCourse::onSequenceIndexed(quint8 domain, quint32 code, const QString& message)
	{
		{
			QSemaphoreReleaser semphore_releaser(_index_semaphore, 1);
			qWarning() << "Error:" << domain << code << message;
			emit exceptionThrew(MessageLevel::WarningMessage, tr("Playlist could not be listed"));
		}
		emit indexed();
		/// @todo detect network error to stop attemp
//		index();
	}
	void SmartCourse::onSequenceIndexed(const QVector<Repository::Model::Track>& track_vector)
	{
		{
			QSemaphoreReleaser semphore_releaser(_index_semaphore, 1);
			qDebug() << "Index tracklist:" << track(track_vector);
		}
		emit indexed();
	}

	void SmartCourse::onPlaybackPaused(quint8 domain, quint32 code, const QString& message)
	{
		{
			QSemaphoreReleaser semphore_releaser(_progress_semaphore, 1);
			qWarning() << "Error:" << domain << code << message;
			emit exceptionThrew(MessageLevel::WarningMessage, tr("Progress could not be tracked"));
		}
		emit progressed();
		/// @todo detect network error to stop attemp
//		progress();
	}
	void SmartCourse::onPlaybackPaused(const QVector<Repository::Model::Track>& track_vector)
	{
		{
			QSemaphoreReleaser semphore_releaser(_progress_semaphore, 1);
			qDebug() << "Playback tacklist:" << track(track_vector, TrackMode::Update);
		}
		emit progressed();
	}

	void SmartCourse::onFetched(Repository::Model::URLInterface::Type type, const QUrl& url)
	{
		qDebug() << "Ignoring fetch...";
	}

	void SmartCourse::onSigned(Type type, const QUrl& url, const QDateTime& life_time, const QVariantMap& token_map, const QString& view_preset)
	{
		qDebug() << "SmartCourse::onSigned() Ignoring...";
	}

	void SmartCourse::onFile(const Core::Net::CRPReaction& reaction, Repository::Meta::Subject::Id subject_id, int revision_code, int language_code, QString key)
	{
		if (!reaction) {
			/// @???
			return reaction.log("SmartCourse::onFile()");
		} else if (!(*reaction).isObject()) {
			return onSigned(Type::Unaccessible, {}, {}, {}, "");
		}

		Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());
		Data::CourseTrack data(getDescriptor()->getObjectId());
		data.resetCompoundPredicate(Core::Data::CompoundPredicate::Logic::And);
		data.addPredicate(
							QSharedPointer<Core::Data::ConditionPredicate>::create
							(
								data.subject_id,
								Core::Data::Field(Core::Data::Field::Type::Integer, getDescriptor()->getSubjectId()),
								Core::Data::ConditionPredicate::Condition::Equal
							)
		);
		data.addPredicate(
							QSharedPointer<Core::Data::ConditionPredicate>::create
							(
								data.key,
								Core::Data::Field(Core::Data::Field::Type::Text, key),
								Core::Data::ConditionPredicate::Condition::Equal
							)
		);
		data.addPredicate(
							QSharedPointer<Core::Data::ConditionPredicate>::create
							(
								data.revision_code,
								Core::Data::Field(Core::Data::Field::Type::Int, revision_code),
								Core::Data::ConditionPredicate::Condition::Equal
							)
		);
		data.addPredicate(
							QSharedPointer<Core::Data::ConditionPredicate>::create
							(
								data.language_code,
								Core::Data::Field(Core::Data::Field::Type::Int, language_code),
								Core::Data::ConditionPredicate::Condition::Equal
							)
		);
		if (!data.open(data_provider)) {
			qWarning("Nothing to update");
			return ;
		} else if (data.isEmpty()) {
			return ;
		}

		const int weight(data.weight.value().toInt());
		data.close();

		const QJsonObject object((*reaction).toObject());
		const QString resource_locator(object.value("resource_locator").toString(""));
		const QDateTime life_datetime(QDateTime::fromSecsSinceEpoch(object.value("life_time").toInt(0), Qt::UTC));
		const QVariantMap token_map(object.value("token_map").toObject({}).toVariantMap());
		const QString view_preset(object.value("view_preset").toString(""));

		AbstractApplication* self(this);
		static QMap<QString, std::function<bool(QString&)>> TokenTransformMap =
		{
			{
				"capec-objectlicense-serialnumber",
				[self](QString& input) -> bool
				{
					input
						.replace("-", ":")
						.prepend(":")
						.prepend(self->getDescriptor()->getRealm()->getEndpoint()->getHost())
						.prepend("crn:cc:license:capec:");
					return true;
				}
			}
		};

		QUrlQuery url_query;
		for (QVariantMap::ConstIterator t = token_map.constBegin(); t != token_map.constEnd(); ++t) {
			QString token_value(t.value().toString());
			url_query.addQueryItem
			(
				t.key(),
				TokenTransformMap.value(
										t.key(), [](QString& input) -> bool { return true; }
				)(token_value) ? token_value : t.value().toString()
			);
		}
		QUrl url(resource_locator);
		url.setQuery(url_query);

		QSqlRecord record(_track_data->record(false));
		record.setGenerated("file", true);
		record.setValue("file", url.toString());
		record.setGenerated("life_timestamp", true);
		record.setValue("life_timestamp", life_datetime);

		Core::Data::Factory data_factory(data_provider->makeFactory());
		if (!data_factory.update(
									_track_data.get(),
									record,
									Qaos::MakeUnique<Core::Data::CompoundPredicate>(
										std::initializer_list<QSharedPointer<const Core::Data::WherePredicate>>
										{
											QSharedPointer<Core::Data::ConditionPredicate>::create
											(
												_track_data->subject_id,
												Core::Data::Field(Core::Data::Field::Type::Integer, subject_id),
												Core::Data::ConditionPredicate::Condition::Equal
											),
											QSharedPointer<Core::Data::ConditionPredicate>::create
											(
												_track_data->key,
												Core::Data::Field(Core::Data::Field::Type::Text, key),
												Core::Data::ConditionPredicate::Condition::Equal
											),
											QSharedPointer<Core::Data::ConditionPredicate>::create
											(
												_track_data->revision_code,
												Core::Data::Field(Core::Data::Field::Type::Int, revision_code),
												Core::Data::ConditionPredicate::Condition::Equal
											),
											QSharedPointer<Core::Data::ConditionPredicate>::create
											(
												_track_data->language_code,
												Core::Data::Field(Core::Data::Field::Type::Int, language_code),
												Core::Data::ConditionPredicate::Condition::Equal
											),
											QSharedPointer<Core::Data::ConditionPredicate>::create
											(
												_track_data->weight,
												Core::Data::Field(Core::Data::Field::Type::Int, weight),
												Core::Data::ConditionPredicate::Condition::Equal
											)
										},
										Core::Data::CompoundPredicate::Logic::And
									).get()
								 )
		) {
			qCritical("Update error");
		} else if (data_factory.getAffectedRowsCount() > 0) {
			Core::Data::Rowset::HintTable hint_table;
			QSqlRecord update_record(_track_data->record(nullptr));
			update_record.setGenerated("subject_id", true);
			update_record.setValue("subject_id", subject_id);

			update_record.setGenerated("key", true);
			update_record.setValue("key", key);

			update_record.setGenerated("revision_code", true);
			update_record.setValue("revision_code", revision_code);

			update_record.setGenerated("langauge_code", true);
			update_record.setValue("language_code", language_code);

			update_record.setGenerated("weight", true);
			update_record.setValue("weight", weight);

			hint_table.insert(Core::Data::Rowset::Mutation::Update, update_record);
			if (!_track_data->reopen(hint_table)) {
				qCritical("Tracklist reopen error");
			} else {
				onSigned(SchemeType(url), url, life_datetime, token_map, view_preset);
			}
		}
	}

	void SmartCourse::customEvent(QEvent* event)
	{

	}

	bool SmartCourse::onHandle()
	{
		_brief_data.reset(new Data::CourseBrief(getDescriptor()->getObjectId()));
		_brief_data->addOrder(_brief_data->revision_code, Qt::SortOrder::DescendingOrder);

		_track_data.reset(new Data::CourseTrack(getDescriptor()->getObjectId()));
		_track_data->addOrder(_track_data->weight, Qt::SortOrder::AscendingOrder);
		_track_data->addOrder(_track_data->key, Qt::SortOrder::AscendingOrder);

		return true;
	}

	bool SmartCourse::onOpen()
	{
		if (!resetBriefModel()) {
			qFatal("Brief model error");
			return false;
		} else if (_brief_data->isEmpty()) { /// Еще даже нет обзора
			PlaylistInterface::overviewSummary();
		} else {
			emit overviewed();
			if (!resetTrackModel()) {
				qFatal("Track model error");
				return false;
			} else if (!hasActiveLicense()) { /// < Лицензия закончилась, нужно попробовать получить новую
				PlaylistInterface::overviewSummary();
				return true;
			} else if (_track_data->isEmpty() && _brief_data->count.value().toLongLong() > 0) { /// < Ничего не затречили еще, но в brief есть ненулевое количество
				if (!index(getIndexKey())) {
					qCritical("Index error");
					return false;
				}
			} else if (hasLocalProgress()) { /// < На всякий случай попробуем синхронизировать прогрес
				if (!progress()) {
					qCritical("Update progress error");
					return false;
				}
			}
		}
		return true;
	}

	bool SmartCourse::onSetup()
	{
		Core::Data::Factory data_factory(getDescriptor()->getRealm()->getDataProvider()->makeFactory());
		if (
			!data_factory.create(QScopedPointer<Data::CourseBrief>(new Data::CourseBrief(getDescriptor()->getObjectId())).get(), true)
			||
			!data_factory.create(QScopedPointer<Data::CourseTrack>(new Data::CourseTrack(getDescriptor()->getObjectId())).get(), true)
		) {
			qFatal("Data creation error");
			return false;
		}
		return true;
	}

	bool SmartCourse::onSetdown()
	{
		Core::Data::Factory data_factory(getDescriptor()->getRealm()->getDataProvider()->makeFactory());
		if (
			!data_factory.drop(QScopedPointer<Data::CourseBrief>(new Data::CourseBrief(getDescriptor()->getObjectId())).get(), true)
			||
			!data_factory.drop(QScopedPointer<Data::CourseTrack>(new Data::CourseTrack(getDescriptor()->getObjectId())).get(), true)
		) {
			qWarning("Data destruction error");
			return false;
		}
		return true;
	}

	bool SmartCourse::onClose()
	{
		if (_brief_data) {
			_brief_data->close();
		}
		if (_track_data) {
			_track_data->close();
		}
		emit overviewed();
		emit indexed();
		return true;
	}

	bool SmartCourse::onTeardown()
	{
		return true;
	}

	bool SmartCourse::hasFullIndex() const
	{
		const qsizetype brief_size(getOverviewSize());
		return brief_size == 0 ? false : getIndexSize() == brief_size;
	}

	bool SmartCourse::hasLocalProgress() const
	{
		if (!hasDescriptor()) {
			/// @xxx
			return false;
		} else if (!_brief_data->isOpened() || _brief_data->isEmpty()) {
			/// @xxx
			return false;
		}

		const Repository::Meta::Subject::Id subject_id(_brief_data->subject_id.value().toLongLong());
		if (getDescriptor()->getSubjectId() != subject_id) {
			return false;
		}

		Data::CourseTrack data(getDescriptor()->getObjectId());
		data.setRowLimit(1);
		data.local_progress.setExpression(new Core::Data::RoundExpression(3));
		data.remote_progress.setExpression(new Core::Data::RoundExpression(3));
		data.addPredicate(
							QSharedPointer<Core::Data::ConditionPredicate>::create
							(
								data.subject_id,
								Core::Data::Field(Core::Data::Field::Type::Integer, subject_id),
								Core::Data::ConditionPredicate::Condition::Equal
							)
		);
		data.addPredicate(
							QSharedPointer<Core::Data::ConditionPredicate>::create
							(
								data.local_progress,
								data.remote_progress,
								Core::Data::ConditionPredicate::Condition::Greater
							)
		);
		Cadabra::Queen::Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());
		return data.open(data_provider) ? !data.isEmpty() : false;
	}

	bool SmartCourse::hasActiveLicense() const
	{
		return _license_life_time > QDateTime::currentSecsSinceEpoch();
	}
} } } }
