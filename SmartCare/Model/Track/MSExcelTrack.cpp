#include "MSExcelTrack.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	MSExcelTrack::MSExcelTrack(QObject* parent)
	:
		AbstractTrack(parent)
	{

	}

	QString MSExcelTrack::makePreviewText() const
	{
		return QString("[%1]").arg(tr("MS Excel spreadsheet"));
	}
} } } }
