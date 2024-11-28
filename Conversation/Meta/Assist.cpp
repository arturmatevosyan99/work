/**
 @file Assist.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "Assist.hpp"

#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Meta {
	Assist::Assist()
	:
		_scheme(0),
		_uid(QUuid::createUuid()),
		_target_layout(Assist::TargetLayout::Default),
		_source_group(Assist::SourceGroup::Default)
	{

	}

	Assist::Assist(Scheme scheme, const QUuid& uid, const QJsonObject& body, const QSet<Repository::Meta::Subject::Id>& affected_subject_id_set, Assist::TargetLayout target_layout)
	:
		_scheme(scheme),
		_uid(uid),
		_body(body),
		_affected_subject_id_set(affected_subject_id_set),
		_target_layout(target_layout),
		_source_group(Assist::SourceGroup::Default)
	{

	}

	Assist::Assist(const QJsonObject& object)
	:
		_scheme(0),
		_target_layout(Assist::TargetLayout::Default),
		_source_group(Assist::SourceGroup::Default)
	{
		QJsonObject::ConstIterator b(object.find("body"));
		QJsonObject::ConstIterator m(object.find("meta"));

		if (b == object.constEnd() || !b->isObject() || m == object.constEnd() || !m->isObject()) {
			return ;
		}
		const QJsonObject meta(m->toObject());
		QJsonObject::ConstIterator s(meta.find("scheme"));
		QJsonObject::ConstIterator as(meta.find("affected_subjects"));
		QJsonObject::ConstIterator l(meta.find("target_layout"));
		QJsonObject::ConstIterator u(meta.find("uid"));
		if (s == meta.constEnd() || !s->isString() || as == meta.constEnd() || !as->isArray()) {
			return ;
		}

		_scheme = Repository::Meta::Essence::ToId(*s, true);
		if (0 == _scheme) {
			return ;
		}

		const QJsonArray affected_subjects(as->toArray());
		for (QJsonArray::ConstIterator as = affected_subjects.constBegin(); as != affected_subjects.constEnd(); ++as) {
			_affected_subject_id_set.insert(Repository::Meta::Essence::ToId(*as, true));
		}
		_affected_subject_id_set.remove(0);

		if (l != meta.constEnd()) {
			_target_layout = static_cast<TargetLayout>(l->toString("0").toLatin1().at(0));
		}

		if (u != meta.constEnd()) {
			_uid = QUuid::fromString(u->toString(""));
		} else {
			_uid = QUuid::createUuid();
		}

		_body = b->toObject();
	}

	Assist::Assist(const Assist& origin)
	:
		_scheme(origin._scheme),
		_uid(origin._uid),
		_body(origin._body),
		_affected_subject_id_set(origin._affected_subject_id_set),
		_target_layout(origin._target_layout),
		_source_group(origin._source_group)
	{

	}

	Assist::Assist(Assist&& origin)
	:
		_scheme(std::move(origin._scheme)),
		_uid(std::move(origin._uid)),
		_body(std::move(origin._body)),
		_affected_subject_id_set(std::move(origin._affected_subject_id_set)),
		_target_layout(std::move(origin._target_layout)),
		_source_group(std::move(origin._source_group))
	{

	}

	Assist::~Assist()
	{

	}

	Core::Meta::Qd Assist::makeSchemeQd() const
	{
		return Core::Meta::Qd(_scheme);
	}

	QList<Core::Meta::Qd> Assist::makeAffectedSubjectQdList() const
	{
		QList<Core::Meta::Qd> retval;
		if (!_affected_subject_id_set.isEmpty()) {
			for (QSet<Repository::Meta::Subject::Id>::ConstIterator i(_affected_subject_id_set.constBegin()); i != _affected_subject_id_set.constEnd(); ++i) {
				retval.push_back(Core::Meta::Qd(*i));
			}
		}
		return retval;
	}

	QVariantList Assist::makeAffectedSubjectQdVariantList() const
	{
		QVariantList retval;
		if (!_affected_subject_id_set.isEmpty()) {
			for (const Repository::Meta::Subject::Id& id : _affected_subject_id_set) {
				retval.push_back(Core::Meta::Qd(id));
			}
		}
		return retval;
	}

	Assist::operator QJsonObject() const
	{
		 QJsonArray affected_subjects;
		 for (const Cadabra::Queen::Repository::Meta::Subject::Id& id : _affected_subject_id_set) {
			affected_subjects.push_back(QString::number(id));
		 }

		 return QJsonObject{
							{"body", _body},
							{"meta",
									QJsonObject{
										{"scheme", QString::number(_scheme)},
										{"uid", _uid.toString(QUuid::StringFormat::WithoutBraces)},
										{"target_layout", static_cast<char>(_target_layout)},
										{"affected_subjects", affected_subjects}
									}
							}
		 };
	}

	Assist& Assist::operator=(const Assist& rhs)
	{
		_scheme = rhs._scheme;
		_uid = rhs._uid;
		_body = rhs._body;
		_affected_subject_id_set = rhs._affected_subject_id_set;
		_target_layout = rhs._target_layout;

		return *this;
	}

	Assist& Assist::operator=(Assist&& rhs)
	{
		_scheme = std::move(rhs._scheme);
		_uid = std::move(rhs._uid);
		_body = std::move(rhs._body);
		_affected_subject_id_set = std::move(rhs._affected_subject_id_set);
		_target_layout = std::move(rhs._target_layout);

		return *this;
	}

	void Assist::setSourceGroup(SourceGroup group)
	{
		if (group != _source_group) {
			const SourceGroup backup(_source_group);
			_source_group = group;
			emit sourceGroupChanged(backup);
		}
	}

	const Cadabra::Queen::Conversation::Meta::Assist::Scheme& Assist::getScheme() const
	{
		return _scheme;
	}

	const QUuid& Assist::getUId() const
	{
		return _uid;
	}

	const QJsonObject& Assist::getBody() const
	{
		return _body;
	}

	const QSet<Cadabra::Queen::Repository::Meta::Subject::Id>& Assist::getAffectedSubjectIdSet() const
	{
		return _affected_subject_id_set;
	}

	const Assist::TargetLayout& Assist::getTargetLayout() const
	{
		return _target_layout;
	}

	const Assist::SourceGroup& Assist::getSourceGroup() const
	{
		return _source_group;
	}

	bool Assist::isValid() const
	{
		return 0 != _scheme;
	}
} } } }
