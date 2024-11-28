#ifndef Cadabra_Queen_Core_System_UserTracker_hpp
#define Cadabra_Queen_Core_System_UserTracker_hpp

#include <QObject>
#include <QQmlEngine>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class UserTracker : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static bool Lock(bool onoff = true);
		static UserTracker* Instance();
		static QObject* QInstance(QQmlEngine* engine, QJSEngine* scriptEngine);

		static bool PurchaseEvent(const QString& payment_system, const QString& sku, double quantity, long price, const QString& currency);
		static bool CustomEvent(const QString& message);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	private:
		explicit UserTracker(bool auto_activation = false);
		virtual ~UserTracker() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	public:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void activate();
		Q_INVOKABLE void deactivate();
		Q_INVOKABLE bool logPurchaseEvent(const QString& payment_system, const QString& sku, double quantity, long price, const QString& currency);
		Q_INVOKABLE bool logCustomEvent(const QString& message);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals*/
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
