#include "MSWordTrack.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
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
