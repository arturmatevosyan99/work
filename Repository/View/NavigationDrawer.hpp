#ifndef Cadabra_Queen_Repository_View_NavigationDrawer_hpp
#define Cadabra_Queen_Repository_View_NavigationDrawer_hpp

#include <QQuickItem>

namespace Cadabra { namespace Queen { namespace Repository { namespace View {
	class NavigationDrawer : public QQuickItem
	{
	/** @name Enum */
	/** @{ */
	public:		
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit NavigationDrawer(QQuickItem* parent = nullptr);
		virtual ~NavigationDrawer() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:		
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Setters */
	/** @{ */
	public:		
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:		
	/** @} */
	};
} } } }

#endif
