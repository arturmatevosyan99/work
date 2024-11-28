#include "CommandMessage.hpp"

#include <QJsonObject>
#include <QGuiApplication>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	CommandMessage::CommandMessage(QObject* parent)
	:
		AbstractMessage(parent)
	{
	}

	QJsonObject CommandMessage::exportBody() const
	{
		return QJsonObject{
							{"argument", _argument},
							{"output", _output},
							{"action", _action},
							{"success", _success}
		};
	}

	bool CommandMessage::importBody(const QJsonObject& body)
	{
		_argument = body.value("argument").toString(body.value("command").toString()); /// < backward compability
		_output = body.value("output").toString("");
		_action = body.value("action").toString("");
		_success = body.value("success").toInt(0);

		return true;
	}

	bool CommandMessage::clipBody() const
	{
		QGuiApplication::clipboard()->setText(_argument);
		return false;
	}

	QString CommandMessage::getPreviewText() const
	{
		return _action.isEmpty() ? QString("[%1]").arg(tr("Chat Command")) : _action;
	}

	const QString& CommandMessage::getArgument() const
	{
		return _argument;
	}

	const QString& CommandMessage::getOutput() const
	{
		return _output;
	}

	const QString& CommandMessage::getAction() const
	{
		return _action;
	}

	const int& CommandMessage::getSuccess() const
	{
		return _success;
	}

	bool CommandMessage::hasClipboard() const
	{
		return true;
	}

	bool CommandMessage::hasAction() const
	{
		return !_action.isEmpty();
	}
} } } }
