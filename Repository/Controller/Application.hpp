#ifndef Cadabra_Queen_Repository_Controller_Application_hpp
#define Cadabra_Queen_Repository_Controller_Application_hpp

#include <QQuickItem>

namespace Cadabra { namespace Queen { namespace Repository { namespace Controller {
	/**
	 * @brief The Application Controller
	 * @todo Rename to AbstractApplication (need to deal somehow with conflict between Model::AbstractApplication)
	 */
	class Application : public QQuickItem
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
		Application(QQuickItem* parent = nullptr);
		virtual ~Application() = default;
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
