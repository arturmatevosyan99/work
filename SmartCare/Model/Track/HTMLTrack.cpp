#include "HTMLTrack.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	HTMLTrack::HTMLTrack(QObject* parent)
	:
		AbstractTrack(parent)
	{

	}

	QString HTMLTrack::makePreviewText() const
	{
		return QString("[%1]").arg(tr("Web page"));
	}
} } } }