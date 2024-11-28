#include "AppletMessage.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	AppletMessage::AppletMessage(QObject* parent, Meta::Message::SchemeIdent scheme)
	:
		AbstractMessage(parent),

		_id(0),
		_application_scheme(""),
		_name(""),
		_description("")
	{

	}

	QJsonObject AppletMessage::exportBody() const
	{
		return {
				{"id", QString::number(_id)},
				{"application_scheme", _application_scheme},
				{"name", _name},
				{"description", _description}
		};
	}

	bool AppletMessage::importBody(const QJsonObject& body)
	{
		_id = Repository::Meta::Essence::ToId(body.value("id").toString("0"));
		_application_scheme = body.value("application_scheme").toString();
		_name = body.value("name").toString();
		_description = body.value("description").toString("");

		return true;
	}

	QString AppletMessage::getPreviewText() const
	{
		return _description.isEmpty() ? QString("[%1]").arg(tr("Interactive Application")) : _description;
	}

	Repository::Meta::ObjectId AppletMessage::getId() const
	{
		return _id;
	}

	Cadabra::Queen::Core::Meta::Qd AppletMessage::getQd() const
	{
		return _id;
	}

	Repository::Meta::Object::ApplicationScheme AppletMessage::getApplicationScheme() const
	{
		return _application_scheme;
	}

	const QString& AppletMessage::getName() const
	{
		return _name;
	}

	const QString& AppletMessage::getDescription() const
	{
		return _description;
	}
} } } }
