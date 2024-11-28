#include "VerticalLayout.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	VerticalLayout::VerticalLayout(QObject* parent)
	:
		AbstractLayout(-8034428181974876159, parent)
	{
	}

	QString VerticalLayout::getPreviewText() const
	{
		return "🗼";
	}
} } } }
