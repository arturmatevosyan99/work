#include "mg_RTFTrack.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	RTFTrack::RTFTrack(QObject* parent)
	:
		AbstractTrack(parent)
	{

	}

	QString RTFTrack::makePreviewText() const
	{
		return QString("[%1]").arg(tr("Richtext document"));
	}
} } } }
