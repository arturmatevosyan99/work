#include "Cleanup.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	Cleanup::Cleanup(QObject* parent)
	:
		Cleanup(0, "", parent)
	{
	}

	Cleanup::Cleanup(Repository::Meta::Object::Id object_id, const QString alias, QObject* parent)
	:
		Repository::Model::AbstractJob(-8663136378210484223, parent),

		_reception_id(object_id),
		_mode(alias)
	{

	}

	Cleanup::~Cleanup()
	{
	}

	void Cleanup::setReceptionId(Repository::Meta::Object::Id object_id)
	{
		_reception_id = object_id;
	}

	void Cleanup::setModeAlias(const QString& alias)
	{
		_mode = alias;
	}

	QJsonObject Cleanup::makeEnvironment() const
	{
		return {
				{"mode", _mode},
				{"reception_id", QString::number(_reception_id)}
		};
	}
} } } }
