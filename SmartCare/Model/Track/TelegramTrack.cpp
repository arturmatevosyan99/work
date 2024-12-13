#include "TelegramTrack.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	TelegramTrack::TelegramTrack(QObject* parent)
	:
		AbstractTrack(parent)
	{

	}

	QString TelegramTrack::makePreviewText() const
	{
		return QString("[%1]").arg(tr("Telegram channel"));
	}
} } } }
