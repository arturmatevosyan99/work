#include "Hierarchy.hpp"

#include "Realm.hpp"
#include "Descriptor/SessionDescriptor.hpp"
#include "Descriptor/StatelessDescriptor.hpp"

#include "../Meta/ApplicationRender.hpp"

#include "../Data/Entity/Extension.hpp"
#include "../Data/Entity/Sync.hpp"
#include "../Data/Entity/Record/Object.hpp"
#include "../Data/Entity/Record/Group.hpp"
#include "../Data/Entity/Record/Subject.hpp"
#include "../Data/Entity/Record/Tag.hpp"
#include "../Data/Entity/Tag/ObjectTag.hpp"
#include "../Data/Entity/Tag/GroupTag.hpp"
#include "../Data/Entity/Tag/SubjectTag.hpp"


#include "../../Qaos/MakeUnique.hpp"

#include <QFile>
#include <QBuffer>
#include <QByteArray>
#include <QTemporaryFile>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Hierarchy::Hierarchy(const Realm* const realm)
	:
		QAbstractItemModel(nullptr),

		_realm(realm)
	{

	}

	Hierarchy::~Hierarchy()
	{
		/// @todo sync back Object(access_timestamp, meta)
	}

	void Hierarchy::disassemble()
	{
		QObject::disconnect(_realm, &Realm::deinitialized, this, &Hierarchy::disassemble);

		_group_model.unload();
		_subject_model.unload();
		_object_model.unload();

		_roaster.unload();

		_file_map.clear();
	}

	bool Hierarchy::assemble()
	{
		if (!_realm) {
			return false;
		}

		Data::Object object_data;
		Data::Sync sync_data;
		Data::Group group_data;
		Data::Subject subject_data;

		Data::Tag tag_data;
		Data::ObjectTag object_tag_data;
		Data::GroupTag group_tag_data;
		Data::SubjectTag subject_tag_data;

		/// @brief Временное решение для кэширования картинок субъектов
		_file_map.insert(qMakePair(1, QString("0.s.0.pic")), QSharedPointer<Net::ObjectFile>::create(nullptr, 0, 0, 1)).value()->load(Qaos::MakeUnique<QFile>(":/Repository/Asset/Image/PatientPicture.jpg").get());
		_file_map.insert(qMakePair(1, QString("2.s.0.pic")), QSharedPointer<Net::ObjectFile>::create(nullptr, 0, 0, 1)).value()->load(Qaos::MakeUnique<QFile>(":/Repository/Asset/Image/OblozhkoPicture.jpg").get());
		_file_map.insert(qMakePair(1, QString("3.s.0.pic")), QSharedPointer<Net::ObjectFile>::create(nullptr, 0, 0, 1)).value()->load(Qaos::MakeUnique<QFile>(":/Repository/Asset/Image/AngelinaPicture.jpg").get());

		const bool lazy_realm(_realm->getSynchronizationMode() == Realm::SynchronizationMode::Lazy);

		if (!_roaster.load(_realm->getDataProvider())) {
			qCritical("Could not load Roaster");
			return false;
		} else if (!_group_model.load(&_roaster, lazy_realm)) {
			qCritical("Could not load Group model");
			return false;
		} else if (!_subject_model.load(&_roaster, lazy_realm)) {
			qCritical("Could not load Subject model");
			return false;
		} else if (!_object_model.load(&_roaster, lazy_realm)) {
			qCritical("Could not load Object model");
			return false;
		}

		QObject::connect(_realm, &Realm::deinitialized, this, &Hierarchy::disassemble);
		return true;
	}

	QSharedPointer<Net::ObjectFile> Hierarchy::loadObjectFile(Meta::Object::Id id, const QString& file_name, LoadMode mode)
	{
		const QFile file(getObjectCachePath(id, true).absoluteFilePath(file_name));
		if ((file.exists() && file.size() > 0) && (static_cast<quint8>(mode) & static_cast<quint8>(LoadMode::CacheForce)) == 0) {
			return nullptr;
		}

		const QPair<Meta::Object::Id, QString> key(qMakePair(id, file_name));
		FileMap::Iterator f(_file_map.find(key));
		if (_file_map.end() != f) {
			return f.value();
		}

		QSharedPointer<Net::ObjectFile> retval
		(
			new Net::ObjectFile
			(
				_realm->getEndpoint(),
				_realm->getSessionDescriptor()->getPoolId(),
				_realm->getSessionDescriptor()->getPoolHandle(),
				id
			),
			&QObject::deleteLater
		);
		if ((static_cast<quint8>(mode) & static_cast<quint8>(LoadMode::TimeLess)) != 0) {
			retval->setTransferTimeout(0);
		}
		f = _file_map.insert(key, retval);
		QObject::connect
		(
			retval.get(),
			qOverload<QString>(&Net::ObjectFile::downloaded),

			std::bind
			(
				(void(Hierarchy::*)(FileMap::iterator, QString))&Hierarchy::onObjectFileLoaded,
				this, f, std::placeholders::_1
			)
		);
		QObject::connect
		(
			retval.get(),
			qOverload<QNetworkReply::NetworkError, QString>(&Net::ObjectFile::faulted),

			std::bind
			(
				(void(Hierarchy::*)(FileMap::iterator, QNetworkReply::NetworkError, QString))&Hierarchy::onObjectFileLoaded,
				this, f, std::placeholders::_1, std::placeholders::_2
			)
		);
		retval->download(file_name);
		return retval;
	}

	QSharedPointer<Net::ObjectFile> Hierarchy::loadSubjectPicture(Meta::Subject::Id id, LoadMode mode)
	{
		return loadObjectFile(1, QString("%1.%2.%3.pic").arg((id > 3) ? 0 : id).arg(static_cast<char>(Meta::Essence::Target::Subject)).arg("0"), mode);
	}

	bool Hierarchy::cacheObjectFile(Meta::Object::Id id, const QString& file_name, QBuffer* input)
	{
		if (!input) {
			return false;
		}
//		const qint64 original_position(input->pos());
//		input->seek(0);
		QFile file(getObjectCachePath(id, true).absoluteFilePath(file_name));
		file.open(QIODevice::WriteOnly);
		const qint64 byte_count(file.write(input->data()));
		qDebug("Caching realm object's file : %s(%lld bytes)", qPrintable(file.fileName()), byte_count);
		file.close();
//		input->seek(original_position);
		return true;
	}

	bool Hierarchy::discardObjectFile(Meta::Object::Id id, const QString& file_name)
	{
		return QFile::remove(getObjectCachePath(id, true).absoluteFilePath(file_name));
	}

	Roaster& Hierarchy::getRoaster()
	{
		return _roaster;
	}

	const Roaster& Hierarchy::getRoaster() const
	{
		return _roaster;
	}

	ObjectMap* Hierarchy::getObjectModel()
	{
		return &_object_model;
	}

	const ObjectMap* Hierarchy::getObjectModel() const
	{
		return &_object_model;
	}

	SubjectMap* Hierarchy::getSubjectModel()
	{
		return &_subject_model;
	}

	const SubjectMap* Hierarchy::getSubjectModel() const
	{
		return &_subject_model;
	}

	GroupMap* Hierarchy::getGroupModel()
	{
		return &_group_model;
	}

	const GroupMap* Hierarchy::getGroupModel() const
	{
		return &_group_model;
	}

	const Meta::Essence::Id& Hierarchy::getLastEssenceId(Meta::Essence::Target target, bool force)
	{
		if (force || !_last_id_map.contains(target)) {
			Cadabra::Queen::Core::Data::Provider* data_provider(_realm->getDataProvider());

			switch (target) {
				case Meta::Essence::Target::Object:
				{
					QSharedPointer<Data::Object> data(new Data::Object());
					data->setRowLimit(1);
					data->addOrder(data->id, Qt::SortOrder::DescendingOrder);
					return (_last_id_map[Meta::Essence::Target::Object] = (!data->open(data_provider) || data->isEmpty()) ? 0 : data->id.value().toLongLong());
				} break;

				case Meta::Essence::Target::Subject:
				{
					QSharedPointer<Data::Subject> data(new Data::Subject());
					data->setRowLimit(1);
					data->addOrder(data->id, Qt::SortOrder::DescendingOrder);
					return (_last_id_map[Meta::Essence::Target::Subject] = (!data->open(data_provider) || data->isEmpty()) ? 0 : data->id.value().toLongLong());
				} break;

				case Meta::Essence::Target::Group:
				{
					QSharedPointer<Data::Group> data(new Data::Group());
					data->setRowLimit(1);
					data->addOrder(data->id, Qt::SortOrder::DescendingOrder);
					return (_last_id_map[Meta::Essence::Target::Group] = (!data->open(data_provider) || data->isEmpty()) ? 0 : data->id.value().toLongLong());
				} break;

				default:
				{
					static const Meta::Essence::Id nil(0);
					return nil;
				}
			}
		}
		return _last_id_map.find(target).value();
	}

	const Meta::Essence::Id& Hierarchy::getLastObjectId(bool force)
	{
		return getLastEssenceId(Meta::Essence::Target::Object, force);
	}

	const Meta::Essence::Id& Hierarchy::getLastSubjectId(bool force)
	{
		return getLastEssenceId(Meta::Essence::Target::Subject, force);
	}

	const Meta::Essence::Id& Hierarchy::getLastGroupId(bool force)
	{
		return getLastEssenceId(Meta::Essence::Target::Group, force);
	}

	QUrl Hierarchy::getObjectCacheLocation(Meta::Object::Id id) const
	{
		QDir path(QString("%1/%2").arg(_realm->getStoragePath().absolutePath()).arg(id));
		return QUrl::fromLocalFile(path.absolutePath());
	}

	QDir Hierarchy::getObjectCachePath(Meta::Object::Id id, bool auto_mkdir) const
	{
		const QString object_directory(QString::number(id));
		QDir path(_realm->getStoragePath());
		if (auto_mkdir && !path.exists(object_directory)) {
			path.mkdir(object_directory);
		}
		path.cd(object_directory);
		return path;
	}

	QUrl Hierarchy::getObjectFileLocation(Meta::Object::Id id, const QString& file_name) const
	{
		return QUrl::fromLocalFile(QString("%1/%2/%3").arg(_realm->getStoragePath().absolutePath()).arg(id).arg(file_name));
	}

	Model::SubjectKey Hierarchy::keySubject(Cadabra::Queen::Core::Meta::Qd qd)
	{
		return _subject_model.getSubject(qd.toInt64()).toWeakRef();
	}

/*
	Model::GroupKey Hierarchy::keyGroup(Cadabra::Queen::Core::Meta::Qd qd)
	{

	}
*/
/*
	Model::ObjectKey Hierarchy::keyObject(Cadabra::Queen::Core::Meta::Qd qd)
	{

	}
*/

	QModelIndex Hierarchy::index(int row, int column, const QModelIndex& parent) const
	{
		return QModelIndex{};
	}

	QModelIndex Hierarchy::parent(const QModelIndex& child) const
	{
		return QModelIndex{};
	}

	int Hierarchy::rowCount(const QModelIndex& parent) const
	{
		return 0;
	}

	int Hierarchy::columnCount(const QModelIndex& parent) const
	{
		return 0;
	}

	QHash<int, QByteArray> Hierarchy::roleNames() const
	{
		return
		{
			{Role::EssenceTarget, "essenceTarget"},
			{Role::EssenceId, "essenceId"},
			{Role::EssenceName, "essenceName"},
			{Role::EssenceDescription, "essenceDescription"},
			{Role::EssencePicture, "essencePicture"},
			{Role::EssenceCreationDateTime, "essenceCreationDateTime"},
			{Role::EssenceModificationDateTime, "essenceModificationDateTime"},
			{Role::EssenceMeta, "essenceMeta"}
		};
	}

	QVariant Hierarchy::data(const QModelIndex &index, int role) const
	{
		return QVariant{};
	}

	bool Hierarchy::upsert(Meta::Essence::Target target, Meta::Essence::Id id, const QDateTime& modification_datetime, const QJsonObject& dump)
	{
		switch (target) {
			case Meta::Essence::Target::Group: return _group_model.apply(id, modification_datetime, dump);
			case Meta::Essence::Target::Subject: return _subject_model.apply(id, modification_datetime, dump);
			case Meta::Essence::Target::Object: return _object_model.apply(id, modification_datetime, dump);
			default: return false;
		}
	}

	bool Hierarchy::tag(Meta::Essence::Target target, Meta::Essence::Id target_id, Meta::Essence::Id tag_id, bool onoff)
	{
		switch (target) {
			case Meta::Essence::Target::Group:
				return _group_model.tag(target_id, tag_id, onoff);
			case Meta::Essence::Target::Subject:
				return _subject_model.tag(target_id, tag_id, onoff);
			case Meta::Essence::Target::Object:
				return _object_model.tag(target_id, tag_id, onoff);
			default:
				return false;
		}
	}

	bool Hierarchy::remove(Meta::Essence::Target target, Meta::Essence::Id id)
	{
		switch (target) {
			case Meta::Essence::Target::Group:
				return _group_model.remove(id);
			case Meta::Essence::Target::Subject:
				return _subject_model.remove(id);
			case Meta::Essence::Target::Object: {
				const QSharedPointer<const Meta::Object> meta(_object_model.getObject(id));
				if (!meta) {
					return false;
				}
				const QSharedPointer<AbstractApplication> application(Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Produce(Core::Meta::Qd(meta->getApplicationScheme())));
				if (application) {
					const QScopedPointer<StatelessDescriptor> descriptor(new StatelessDescriptor(_realm->getSessionDescriptor(), meta));
					application->clean(descriptor.get());
				}
				return _object_model.remove(id);
			}
			default:
				return false;
		}
	}

	void Hierarchy::onObjectFileLoaded(FileMap::iterator iterator, QNetworkReply::NetworkError error, QString message)
	{
		const Meta::Object::Id object_id(iterator.key().first);
		const QString file_name(iterator.key().second);
		_file_map.erase(iterator);
		qWarning("Load object(%lld) file(%s) error(%d): %s", object_id, qPrintable(file_name), error, qPrintable(message));
		emit objectFileCached(object_id, file_name, false);
	}

	void Hierarchy::onObjectFileLoaded(FileMap::iterator iterator, QString filename)
	{
		const Meta::Object::Id object_id(iterator.key().first);
		const QString file_name(iterator.key().second);
		const QSharedPointer<Net::ObjectFile> file(iterator.value());
		_file_map.erase(iterator);
		if (!file->save(getObjectCachePath(object_id).absoluteFilePath(file_name))) {
			qWarning("Save object(%lld) file(%s) error", object_id, qPrintable(file_name));
			emit objectFileCached(object_id, file_name, false);
		} else {
			emit objectFileCached(object_id, file_name, true);
		}
	}

	bool Hierarchy::hasGroup(Meta::Group::Id id) const
	{
		return _group_model.isExist(id);
	}

	bool Hierarchy::hasSubject(Meta::Subject::Id id) const
	{
		return _subject_model.isExist(id);
	}

	bool Hierarchy::hasObject(Meta::Object::Id id) const
	{
		return _object_model.isExist(id);
	}

	bool Hierarchy::hasObjectFile(Meta::Subject::Id id, const QString& file_name) const
	{
		return QFile::exists(getObjectCachePath(id).absoluteFilePath(file_name));
	}

} } } }
