#include "../StatusBar.hpp"

#include "../../System/Native/Linux.hpp"

#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	class LinuxStatusBar : public StatusBar
	{
	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		LinuxStatusBar(QObject* parent = nullptr)
		:
			StatusBar(parent)
		{
			_color = "transparent";

			QObject::connect(this, &StatusBar::colorChanged, this, &LinuxStatusBar::onColorChanged);
			QObject::connect(this, &StatusBar::themeChanged, this, &LinuxStatusBar::onThemeChanged);

			onColorChanged();
			onThemeChanged();
		}
		virtual ~LinuxStatusBar() override = default;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onColorChanged()
		{
		}

		void onThemeChanged()
		{
		}
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual int getHeight() override
		{
			return 0;
		}
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool hasCutOut() override
		{
			return false;
		}
	/** @} */
	};

	StatusBar* StatusBar::Instance()
	{
		static Cadabra::Queen::Core::View::LinuxStatusBar retval;
		return &retval;
	}
} } } }
