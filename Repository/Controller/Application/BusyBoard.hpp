#ifndef Cadabra_Queen_Repository_Controller_Application_BusyBoard_hpp
#define Cadabra_Queen_Repository_Controller_Application_BusyBoard_hpp

#include "../Application.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Controller {
	class BusyBoard : public Application
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		BusyBoard(QQuickItem* parent = nullptr);
		virtual ~BusyBoard() = default;
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

	/** @name Signals */
	/** @{ */
	private:
	/** @} */
	};
} } } }

#endif
