#include "AuthorizationMessage.hpp"

#include "../../../Repository/Model/Descriptor/SessionDescriptor.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	AuthorizationMessage::AuthorizationMessage(QObject* parent)
	:
		AbstractMessage(parent),

		_force(false),
		_hard_reset(false),
		_comment(""),
		_id(0),
		_handle(0),
		_scheme("")
	{

	}

	bool AuthorizationMessage::authorize()
	{
		if (!hasChannelDescriptor()) {
			return false;
		} else if (0 == _id) {
			getChannelDescriptor()->getRealm()->getSessionDescriptor()->getSession()->invalidate();
		} else {
			getChannelDescriptor()->getRealm()->getSessionDescriptor()->getSession()->revalidate(_id, _handle, _scheme);
		}
		return true;
	}

	QJsonObject AuthorizationMessage::exportBody() const
	{
		return {
					{"force", _force},
					{"hard_reset", _hard_reset},
					{"comment", _comment},					
					{"id", QString::number(_id)},
					{"handle", QString::number(_handle)},
					{"scheme", _scheme}
		};
	}

	bool AuthorizationMessage::importBody(const QJsonObject& body)
	{
		_force = body["force"].toBool(false);
		_hard_reset = body["hard_reset"].toBool(false); /// < Удалить локальное хранилище
		_comment = body["comment"].toString();
		_id = Repository::Meta::Essence::ToId(body["id"]);
		_handle = Repository::Meta::Object::ToHandle(body["handle"]);
		_scheme = body["scheme"].toString("0");

		if (_force) {
			_force = false;
			authorize();
		}
		return true;
	}

	QString AuthorizationMessage::getPreviewText() const
	{
		return QString("[%1]").arg(tr("Authorization"));
	}
} } } }
