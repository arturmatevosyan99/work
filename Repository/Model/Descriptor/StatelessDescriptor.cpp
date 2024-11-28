#include "StatelessDescriptor.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	StatelessDescriptor::StatelessDescriptor(const SessionDescriptor* session_descriptor, QSharedPointer<const Meta::Object> meta)
	:
		AbstractDescriptor(session_descriptor ? session_descriptor->getRealm() : nullptr, meta),
		_moniker(nullptr == session_descriptor ? "" : QString("/%1/%2#%3").arg(session_descriptor->getPoolId()).arg(static_cast<QString>(session_descriptor->getPoolHandle())).arg(meta->getId())),
		_session_descriptor(session_descriptor)
	{

	}

	StatelessDescriptor::~StatelessDescriptor()
	{
		setParent(nullptr);
	}

	QUrlQuery StatelessDescriptor::makeMoniker() const
	{
		QUrlQuery retval;
		retval.addQueryItem("session_id", QString::number(_session_descriptor->getPoolId()));
		retval.addQueryItem("session_handle", _session_descriptor->getPoolHandle());
		retval.addQueryItem("object_id", QString::number(getObjectId()));

		return retval;
	}

	Cadabra::Queen::Repository::Model::SubjectKey StatelessDescriptor::getSubjectKey()
	{
		return SubjectKey();
	}

	const Cadabra::Queen::Repository::Meta::SubjectId& StatelessDescriptor::getSubjectId() const
	{
		static const Cadabra::Queen::Repository::Meta::SubjectId dummy(0);
		return dummy;
	}

	const QString& StatelessDescriptor::getMoniker() const
	{
		return _moniker;
	}
} } } }
/*
	const Cadabra::Queen::Repository::Meta::Object::ApplicationScheme& StatefullDescriptor::getScheme() const
	{
		return getObject()->getApplicationScheme();
	}

	const Cadabra::Queen::Repository::Meta::SubjectId& StatefullDescriptor::getSubjectId() const
	{
		static const Cadabra::Queen::Repository::Meta::SubjectId dummy(0);
		return !_session_descriptor ? dummy : _session_descriptor->getSubjectId();
	}

	const QString& StatefullDescriptor::getMoniker() const
	{
		return _moniker;
	}
*/
