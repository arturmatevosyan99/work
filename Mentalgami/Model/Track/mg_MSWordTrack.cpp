#include "mg_MSWordTrack.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	MSWordTrack::MSWordTrack(QObject* parent)
	:
		AbstractTrack(parent)
	{

	}

	QString MSWordTrack::makePreviewText() const
	{
		return QString("[%1]").arg(tr("MS Word document"));
	}
} } } }
