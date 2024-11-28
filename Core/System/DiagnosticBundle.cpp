#include "DiagnosticBundle.hpp"

#include <QtConcurrent>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	DiagnosticBundle::DiagnosticBundle(QObject* parent)
	:
		Queen::Cupboard(parent),

		_skeleton(Queen::Skeleton::ActiveInstance())
	{
		QObject::connect(&_discloser, &QFutureWatcher<void>::finished, this, &DiagnosticBundle::onCollected);
		QObject::connect(&_uploader, &Net::DiagnosticCall::loaded, this, &DiagnosticBundle::loaded);
		QObject::connect(&_uploader, &Net::DiagnosticCall::faulted, this, &DiagnosticBundle::faulted);
	}

	DiagnosticBundle::~DiagnosticBundle()
	{
		_discloser.cancel();
	}

	bool DiagnosticBundle::collect()
	{
		if (!_skeleton) {
			return false;
		}

		_discloser.setFuture(_skeleton->disclose(*this));
		if (_discloser.isCanceled()) {
			emit faulted(tr("Diagnostic collecting is canceled"));
			return false;
		}

		return true;
	}

	bool DiagnosticBundle::upload()
	{
		return _uploader.upload(&(pack()));
	}

	void DiagnosticBundle::onCollected()
	{
		if (_discloser.isFinished() && !_discloser.isCanceled()) {
			emit collected();
		} else {
			emit faulted(tr("Diagnostic collector error"));
		}
	}
} } } }
