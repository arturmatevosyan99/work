#include "Context.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	Context::Context(QObject* parent)
	:
		QObject(parent)
	{

	}

	Context::~Context()
	{

	}

	QVariant Context::getProperty(const QString& name, const QVariant& default_value)
	{
		QVariant return_value(QObject::property(name.toLocal8Bit().data()));
		return return_value.isValid() ? return_value : default_value;
	}

	void Context::setProperty(const QString& name, const QVariant& value)
	{
		QObject::setProperty(name.toLocal8Bit().data(), value);
	}

	bool Context::hasProperty(const QString& name) const
	{
		return QObject::property(name.toLocal8Bit().data()).isValid();
	}
} } } }
