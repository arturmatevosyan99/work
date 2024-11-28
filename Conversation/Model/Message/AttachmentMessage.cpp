#include "AttachmentMessage.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	AttachmentMessage::AttachmentMessage(QObject* parent, Meta::Message::SchemeIdent scheme)
	:
		AbstractMessage(parent),

		_file_state(AttachmentMessage::FileState::Unknown),
		_file_loader(nullptr)
	{

	}

	bool AttachmentMessage::cache(bool force)
	{
		const QSharedPointer<Repository::Net::ObjectFile> file
		(
			getChannelDescriptor()->getRealm()->getHierarchy()->loadObjectFile
			(
				_object_id,
				_file_name,
				force
				? Repository::Model::Hierarchy::LoadMode::CacheForce
				: Repository::Model::Hierarchy::LoadMode::TimeLess
			)
		);
		if (!file) { /// < Уже загружен
			validateCache();
			return false;
		}
		_file_state = AttachmentMessage::FileState::Loading;
		_file_loader = file;
		QObject::connect(_file_loader.get(), &Repository::Net::ObjectFile::saved, this, &AttachmentMessage::onFileSaved);
		return true;
	}

	QJsonObject AttachmentMessage::exportBody() const
	{
		return {
				{"object_id", _object_id},
				{"file_name", _file_name},
				{"file_description", _file_description}
		};
	}

	bool AttachmentMessage::importBody(const QJsonObject& body)
	{
		_object_id = body["object_id"].toString("0").toLongLong();
		_file_name = body["file_name"].toString();
		_file_description = body["file_description"].toString("");
		validateCache();

		return true;
	}

	bool AttachmentMessage::validateCache()
	{
		if (_object_id > 0 && getChannelDescriptor()->getRealm()->getHierarchy()->hasObjectFile(_object_id, _file_name)) {
			if (_file_state != AttachmentMessage::FileState::Cached) {
				_file_state = AttachmentMessage::FileState::Cached;
				emit cached();
			}
			return true;
		} else if (getAuthorSubjectId() == getChannelDescriptor()->getSubjectId()) {
			const QString draft_file_name(getUId().toString(QUuid::StringFormat::WithoutBraces) + ".draft");
			if (getChannelDescriptor()->getRealm()->getHierarchy()->hasObjectFile(_object_id, draft_file_name)) {
				if (QFile::rename(getChannelDescriptor()->getRealm()->getHierarchy()->getObjectFileLocation(_object_id, draft_file_name).toLocalFile(),
								  getChannelDescriptor()->getRealm()->getHierarchy()->getObjectFileLocation(_object_id, _file_name).toLocalFile())) {
					if (_file_state != AttachmentMessage::FileState::Cached) {
						_file_state = AttachmentMessage::FileState::Cached;
						emit cached();
					}
					return true;
				}
			}			
		}
		_file_state =  AttachmentMessage::FileState::Unknown;
		return false;
	}

	QString AttachmentMessage::getPreviewText() const
	{
		return QString("[%1]").arg(tr("Attachment"));
	}

	Repository::Meta::ObjectId AttachmentMessage::getObjectId() const
	{
		return _object_id;
	}

	QString AttachmentMessage::getFileName() const
	{
		return _file_name;
	}

	QString AttachmentMessage::getFileDescription() const
	{
		return _file_description;
	}

	AttachmentMessage::FileState AttachmentMessage::getFileState() const
	{
		return _file_state;
	}

	QUrl AttachmentMessage::getCachePath() const
	{
		return AttachmentMessage::FileState::Cached == _file_state
		? getChannelDescriptor()->getRealm()->getHierarchy()->getObjectFileLocation(_object_id, _file_name)
		: QUrl();
	}

	void AttachmentMessage::onFileSaved(QString file_name, int size)
	{
		QObject::disconnect(_file_loader.get(), &Repository::Net::ObjectFile::saved, this, &AttachmentMessage::onFileSaved);
		qDebug() << "File saved at:" << file_name << "(" << size << " bytes)";
		_file_state = (size > 0) ? AttachmentMessage::FileState::Cached : AttachmentMessage::FileState::Unknown;
		emit cached();
		_file_loader.reset();
	}

	bool AttachmentMessage::isCached() const
	{
		return FileState::Cached == _file_state;
	}

	bool AttachmentMessage::isCaching() const
	{
		return FileState::Loading == _file_state;
	}

	bool AttachmentMessage::isDescribed() const
	{
		return !_file_description.isEmpty();
	}
} } } }
