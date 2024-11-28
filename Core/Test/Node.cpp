#include "Node.hpp"

#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	Node::Node(const QString& identifier, const QString& description)
	:
		QObject(nullptr),

		_identifier(identifier),
		_description(description)
	{

	}

	Node::~Node()
	{
//		qDebug() << "Destroying" << _identifier << "...";
	}

	const QString& Node::getIdentifier() const
	{
		return _identifier;
	}

	const QString& Node::getDescription() const
	{
		return _description;
	}

	void Node::setDescription(const QString& description)
	{
		if (description != _description) {
			_description = description;
			emit descriptionChanged();
		}
	}
} } } }
