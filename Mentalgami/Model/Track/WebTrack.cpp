#include "WebTrack.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	WebTrack::WebTrack(QObject* parent)
	:
		AbstractTrack(parent)
	{

	}

	QString WebTrack::makePreviewText() const
	{
		return QString("[%1]").arg(tr("Web application"));
	}
} } } }
