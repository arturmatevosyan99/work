#include "StatefullDescriptor.hpp"

#include "../Action/HandleAction.hpp"

#include <QCoreApplication>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	StatefullDescriptor::StatefullDescriptor(const SessionDescriptor* session_descriptor, QSharedPointer<const Meta::Object> meta, Meta::Object::Handle handle, bool singleton, const QVariant& option)
	:
		AbstractDescriptor(session_descriptor ? session_descriptor->getRealm() : nullptr, meta),

		_session_descriptor(session_descriptor),
		_singleton(singleton),
		_option(option),
		_handle(handle),
		_moniker(!_session_descriptor ? "" : QString("/%1/%2/%3").arg(_session_descriptor->getPoolId()).arg(static_cast<QString>(_session_descriptor->getPoolHandle())).arg(static_cast<QString>(_handle)))
	{

	}

	StatefullDescriptor::~StatefullDescriptor()
	{

	}

	QUrlQuery StatefullDescriptor::makeMoniker() const
	{
		QUrlQuery retval;
		retval.addQueryItem("session_id", QString::number(_session_descriptor->getPoolId()));
		retval.addQueryItem("session_handle", _session_descriptor->getPoolHandle());
		retval.addQueryItem("application_handle", _handle);

		return retval;
	}

	QVariant StatefullDescriptor::opt(const QStringList& path) const
	{
		QStringListIterator p(path);
		if (!p.hasNext() || !_option.canConvert<QVariantMap>()) {
			return AbstractDescriptor::opt(path);
		}

		QVariant retval;
		do {
			retval = _option.value<QAssociativeIterable>().value(p.next());
		} while (p.hasNext() && retval.canConvert<QVariantMap>());
		return retval;
	}

	bool StatefullDescriptor::bind(const SessionDescriptor* session_descriptor, Meta::Object::Handle handle)
	{
		if (!session_descriptor || _session_descriptor != session_descriptor || handle) {
			_handle = handle;
			_moniker = QString("/%1/%2/%3").arg(_session_descriptor->getPoolId()).arg(static_cast<QString>(_session_descriptor->getPoolHandle())).arg(static_cast<QString>(_handle));

			AceEvent event("Cadabra::Queen::Repository::Model::Handle", Meta::RealmTarget(Meta::Essence::Target::Object, 0));
			event.import(QJsonValue(static_cast<int>(HandleAction::Cause::Revalidation)));
			QCoreApplication::sendEvent(this, &event);
			return true;
		}
		return false;
	}

	const Repository::Meta::Object::Handle& StatefullDescriptor::getHandle() const
	{
		return _handle;
	}

	const Meta::Object::ApplicationScheme& StatefullDescriptor::getScheme() const
	{
		return getObject()->getApplicationScheme();
	}

	SubjectKey StatefullDescriptor::getSubjectKey()
	{
		return !_session_descriptor ? SubjectKey() : SubjectKey(_session_descriptor->getRealm()->getHierarchy()->getSubjectModel()->getSubject(getSubjectId()));
	}

	const Meta::SubjectId& StatefullDescriptor::getSubjectId() const
	{
		static const Cadabra::Queen::Repository::Meta::SubjectId dummy(0);
		return !_session_descriptor ? dummy : _session_descriptor->getSubjectId();
	}

	const QString& StatefullDescriptor::getMoniker() const
	{
		return _moniker;
	}

	bool StatefullDescriptor::isSingleton() const
	{
		return _singleton;
	}
} } } }
