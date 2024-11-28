#include "../WindowManager.hpp"

#include "../../System/Native/Linux.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	void WindowManager::keepScreen(bool onoff)
	{

	}

	void WindowManager::orientScreen(Qt::ScreenOrientations mask)
	{

	}

	QRectF WindowManager::getSafeAreaRect() const
	{
		QRectF retval(0, 0, 0, 0);

		return retval;
	}

	int WindowManager::getTopPadding() const
	{
		/// @todo Landscape/Portrait
		return getSafeAreaRect().top();
	}

	int WindowManager::getBottomPadding() const
	{
		/// @todo Landscape/Portrait
		return getSafeAreaRect().bottom();
	}
} } } }
