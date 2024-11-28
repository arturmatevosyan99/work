#include "AbstractAssist.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	AbstractAssist::AbstractAssist(const Meta::Assist::Scheme scheme, QObject* parent)
	:
		QObject(parent),

		_channel_application(nullptr),
		_scheme(scheme),
		_target_layout(Meta::Assist::TargetLayout::Default),
		_is_imported(false)
	{

	}

	bool AbstractAssist::import(GroupChannel* channel_application, const Cadabra::Queen::Conversation::Meta::Assist& assist)
	{
		return (assist.getScheme() != _scheme)
												? false
												: import(channel_application, assist.getUId(), assist.getBody(), assist.getAffectedSubjectIdSet(), assist.getTargetLayout());
	}

	bool AbstractAssist::import(GroupChannel* channel_application, const QUuid& uid, const QJsonObject& body, const QSet<Repository::Meta::Subject::Id>& affected_subject_id_set, Meta::Assist::TargetLayout target_layout)
	{
		if (!channel_application || !channel_application->hasDescriptor() || (!_affected_subject_id_set.empty() && !_affected_subject_id_set.contains(channel_application->getDescriptor()->getSubjectId()))) {
			return false;
		}

		_channel_application = channel_application;
		_affected_subject_id_set = affected_subject_id_set;
		_target_layout = target_layout;
		_uid = uid;
		if (!importBody(body)) {
			_is_imported = false;
			return false;
		}

		_is_imported = true;
		emit imported();
		return true;
	}

	bool AbstractAssist::import(Cadabra::Queen::Conversation::Model::GroupChannel* channel_application, const QUuid& uid, const QJsonObject& body, const QVariantList& affected_subject_variant_list, Cadabra::Queen::Conversation::Meta::Assist::TargetLayout target_layout)
	{
		QSet<Repository::Meta::Subject::Id> affected_subject_id_set;
		for (const QVariant& subject_qd : affected_subject_variant_list) {
			affected_subject_id_set.insert(subject_qd.toLongLong());
		}
		return import(channel_application, uid, body, affected_subject_id_set, target_layout);
	}

	Cadabra::Queen::Conversation::Meta::Assist AbstractAssist::exportMeta() const
	{
		return Meta::Assist(_scheme, _uid, exportBody(), _affected_subject_id_set, getTargetLayout());
	}

	Cadabra::Queen::Conversation::Model::GroupChannel* AbstractAssist::getChannelApplication() const
	{
		return _channel_application;
	}

	Cadabra::Queen::Conversation::Meta::Assist::Scheme AbstractAssist::getScheme() const
	{
		return _scheme;
	}

	Cadabra::Queen::Core::Meta::Qd AbstractAssist::getSchemeQd() const
	{
		return _scheme;
	}

	const QSet<Cadabra::Queen::Repository::Meta::Subject::Id>& AbstractAssist::getAffectedSubjectIdSet() const
	{
		return _affected_subject_id_set;
	}

	const QUuid& AbstractAssist::getUId() const
	{
		return _uid;
	}

	Meta::Assist::TargetLayout AbstractAssist::getTargetLayout() const
	{
		return Meta::Assist::TargetLayout::Default != _target_layout ? _target_layout : getDefaultTargetLayout();
	}

	Meta::Assist::TargetLayout AbstractAssist::getDefaultTargetLayout() const
	{
		return Meta::Assist::TargetLayout::Default;
	}

	QString AbstractAssist::getPreview() const
	{
		return !_is_imported ? "..." : "!!!";
	}

	QString AbstractAssist::getPreviewText() const
	{
		return "...";
	}

	QList<Core::Meta::Qd> AbstractAssist::makeAffectedSubjectQdList() const
	{
		QList<Core::Meta::Qd> retval;
		if (!_affected_subject_id_set.isEmpty()) {
			for (QSet<Repository::Meta::Subject::Id>::ConstIterator i(_affected_subject_id_set.constBegin()); i != _affected_subject_id_set.constEnd(); ++i) {
				retval.push_back(Core::Meta::Qd(*i));
			}
		}
		return retval;
	}

	bool AbstractAssist::hasChannelApplication() const
	{
		return nullptr != _channel_application;
	}

	bool AbstractAssist::isImported() const
	{
		return _is_imported;
	}
} } } }
