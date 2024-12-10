#include "CalendarTrack.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	CalendarTrack::CalendarTrack(QObject* parent)
	:
		AbstractTrack(parent)
	{

	}

	QString CalendarTrack::makePreviewText() const
	{
		return QString("[%1]").arg(tr("Calendar event"));
	}
} } } }
