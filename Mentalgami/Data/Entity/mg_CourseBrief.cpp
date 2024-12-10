#include "mg_CourseBrief.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Data {
	CourseBrief::CourseBrief(Repository::Meta::ObjectId object_id)
	:
		Core::Data::AbstractEntity("Mentalgami_CourseBrief", QString::number(object_id)),

		title("title", Field::Type::Text, false),
		description("description", Field::Type::Text, true),
		author("author", Field::Type::Text, true),
		copyright("copyright", Field::Type::Text, true),
		artwork("artwork", Field::Type::Text, false),
		revision_code("revision_code", Field::Type::Int, false),
		language_code("language_code", Field::Type::Int, false),
		release_datetime("release_datetime", Field::Type::Timestamp, true),
		key("key", Field::Type::Text, false),
		count("count", Field::Type::Integer, false),
		meta("meta", Field::Type::Document, true),
		subject_id("subject_id", Field::Type::Integer, false),

		subject({{std::ref(subject_id), &Repository::Data::Subject::id}}, this)
	{
		addField(title);
		addField(description);
		addField(author);
		addField(copyright);
		addField(artwork);
		addField(revision_code);
		addField(language_code);
		addField(release_datetime);
		addField(key);
		addField(count);
		addField(meta);
		addField(subject_id);

		setPrimaryKey({revision_code, language_code, subject_id, key});

		addForeignKey(subject);
	}

	void CourseBrief::onSelect()
	{

	}

	void CourseBrief::onCreate()
	{
	}

	void CourseBrief::onForeign()
	{
	}
} } } }
