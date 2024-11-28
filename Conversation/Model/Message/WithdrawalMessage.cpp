#include "WithdrawalMessage.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	WithdrawalMessage::WithdrawalMessage(QObject* parent)
	:
		AbstractMessage(parent)
	{
	}

	QJsonObject WithdrawalMessage::exportBody() const
	{
		return QJsonObject();
	}

	bool WithdrawalMessage::importBody(const QJsonObject&)
	{		
		return true;
	}

	QString WithdrawalMessage::getPreviewText() const
	{
		return tr("Erased message");
	}

	bool WithdrawalMessage::isErasable() const
	{
		return false;
	}

	bool WithdrawalMessage::hasClipboard() const
	{
		return false;
	}
} } } }
