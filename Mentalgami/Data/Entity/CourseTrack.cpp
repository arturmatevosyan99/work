#include "CourseTrack.hpp"

#include "../../Meta/Track.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Data {
	CourseTrack::CourseTrack(Repository::Meta::ObjectId object_id)
	:
		Core::Data::AbstractEntity("Mentalgami_CourseTrack", QString::number(object_id)),

		subject_id("subject_id", Field::Type::Integer, false),
		key("key", Field::Type::Text, false),
		revision_code("revision_code", Field::Type::Int, false),
		language_code("language_code", Field::Type::Int, false),
		flag("flag", Field::Type::Text, true),
		title("title", Field::Type::Text, false),
		description("description", Field::Type::Text, true),
		artwork("artwork", Field::Type::Text, false),
		weight("weight", Field::Type::Int, true),
		content_type("content_type", Field::Type::Text, false),
		content_length("content_length", Field::Type::Int, false),
		content_size("content_size", Field::Type::Int, false),
		file("file", Field::Type::Text, true),
		file_timestamp("file_timestamp", Field::Type::Timestamp, false),
		life_timestamp("life_timestamp", Field::Type::Timestamp, true),
		meta("meta", Field::Type::Document, true),
		meta_timestamp("meta_timestamp", Field::Type::Timestamp, true),
		local_progress("local_progress", Field::Type::Real, true),
		local_progress_timestamp("local_progress_timestamp", Field::Type::Timestamp, true),
		remote_progress("remote_progress", Field::Type::Real, true),
		remote_progress_timestamp("remote_progress_timestamp", Field::Type::Timestamp, true),

		subject({{std::ref(subject_id), &Repository::Data::Subject::id}}, this)
	{
		addField(subject_id);
		addField(key);
		addField(revision_code);
		addField(language_code);
		addField(flag);
		addField(title);
		addField(description);
		addField(artwork);
		addField(weight);
		addField(content_type);
		addField(content_length);
		addField(content_size);
		addField(file);
		addField(file_timestamp);
		addField(life_timestamp);
		addField(meta);
		addField(meta_timestamp);
		addField(local_progress);
		addField(local_progress_timestamp);
		addField(remote_progress);
		addField(remote_progress_timestamp);

		setPrimaryKey({weight, key, revision_code, language_code, subject_id});
		setPrimaryIndex({weight, key});

		addForeignKey(subject);
	}

	void CourseTrack::onSelect()
	{

	}

	void CourseTrack::onCreate()
	{

	}

	void CourseTrack::onForeign()
	{

	}

	Qt::ItemFlags CourseTrack::flags(const QModelIndex& index) const
	{
		if (!index.isValid()) {
			return Qt::NoItemFlags;
		}
		Qt::ItemFlags retval(Core::Data::AbstractEntity::flags(index));
		retval.setFlag(Qt::ItemIsEnabled, get(index, getNameTable().value("flag", 0)).toString().isEmpty());
		return retval;
	}
} } } }
