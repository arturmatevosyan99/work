#ifndef Cadabra_Queen_Core_Test_Node_AbstractWaiter_hpp
#define Cadabra_Queen_Core_Test_Node_AbstractWaiter_hpp

#include <QObject>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class AbstractWaiter : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractWaiter(QObject* parent = nullptr);
		virtual ~AbstractWaiter() override = default;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void set(bool onoff);
		void done();
	/** @} */

	/** @name Slots */
	/** @{ */
	public slots:
	/** @} */
	};
} } } }

#endif
