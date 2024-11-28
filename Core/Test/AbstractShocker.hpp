#ifndef Cadabra_Queen_Core_Test_AbstractShocker_hpp
#define Cadabra_Queen_Core_Test_AbstractShocker_hpp

#include <QObject>

#include <QMutex>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class AbstractShocker : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractShocker(QObject* parent = nullptr);
		virtual ~AbstractShocker() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QMutex _onoff;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE virtual bool turn(bool onoff) final;

	protected:
		virtual bool turnOn() = 0;
		virtual bool turnOff() = 0;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
