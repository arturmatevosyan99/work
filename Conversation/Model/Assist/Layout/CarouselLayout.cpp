#include "CarouselLayout.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	CarouselLayout::CarouselLayout(QObject* parent)
	:
		AbstractLayout(-8034428181974876154, parent)
	{
	}

	QString CarouselLayout::getPreviewText() const
	{
		return "🎡";
	}
} } } }
