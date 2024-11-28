#include "GridLayout.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	GridLayout::GridLayout(QObject* parent)
	:
		AbstractLayout(-8034428181974876160, parent)
	{
	}

	QString GridLayout::getPreviewText() const
	{
		return "🧮";
	}
} } } }
