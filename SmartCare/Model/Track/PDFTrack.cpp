#include "PDFTrack.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	PDFTrack::PDFTrack(QObject* parent)
	:
		AbstractTrack(parent)
	{

	}

	QString PDFTrack::makePreviewText() const
	{
		return QString("[%1]").arg(tr("Portable document"));
	}
} } } }
