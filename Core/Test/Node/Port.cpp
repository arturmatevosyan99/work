#include "Port.hpp"

#include "../Case/PetriCase.hpp"

#include <QtQml>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	Port::Port(const QString& description)
	:
		Node(QString("Port &%1").arg(reinterpret_cast<quintptr>(this)), description),
		_origin(nullptr)
	{
	}

	Port::~Port()
	{

	}

	bool Port::dock()
	{
		return (_case.isNull() || !_case->hasInstance()) ? false : qobject_cast<AbstractCase*>(_case)->run(_source.toString(), Core::TestDialect::QML);
	}

	PetrI* Port::getPetrI()
	{
		return _case ? _case->getPetrI() : nullptr;
	}

	PetrI* Port::getOriginPetrI()
	{
		return _origin;
	}

	const QUrl& Port::getSource() const
	{
		return _source;
	}

	void Port::setSource(const QUrl& url)
	{
		_source = url;
	}

	void Port::classBegin()
	{

	}

	void Port::componentComplete()
	{
		if (!_case.isNull()) {
			_case->deleteLater();
		}

		_origin = qobject_cast<PetrI*>(parent());
		_case = new PetriCase(this);
		QQmlEngine::setContextForObject(_case, QtQml::qmlContext(this));
	}
} } } }
