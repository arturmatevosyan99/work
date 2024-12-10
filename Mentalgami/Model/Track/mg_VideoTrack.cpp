#include "mg_VideoTrack.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	VideoTrack::VideoTrack(QObject* parent)
	:
		AbstractTrack(parent)
	{

	}

	QString VideoTrack::makePreviewText() const
	{
		return QString("[%1]").arg(tr("Video file"));
	}
} } } }
