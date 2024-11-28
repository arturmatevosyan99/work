#ifndef Cadabra_Queen_Core_Test_Node_Waiter_ObjectWaiter_hpp
#define Cadabra_Queen_Core_Test_Node_Waiter_ObjectWaiter_hpp

#include "WaiterCallback.hpp"

#include "../AbstractWaiter.hpp"

#include "../../../Meta/PropertyList.hpp"

#include <QByteArray>
#include <QMap>
#include <QMetaMethod>
#include <QPointer>
#include <QQmlParserStatus>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class ObjectWaiter : public AbstractWaiter, public QQmlParserStatus
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QObject* target READ getTarget WRITE setTarget NOTIFY targetChanged)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Test::WaiterCallback> callbackList READ makeCallbackPropertyList CONSTANT)

		Q_CLASSINFO("DefaultProperty", "callbackList")
		Q_INTERFACES(QQmlParserStatus)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ObjectWaiter(QObject* parent = nullptr);
		virtual ~ObjectWaiter() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QPointer<QObject> _target;
		QMap<QByteArray, QMetaMethod> _slot_map;

		QSet<QString> _callback_history_set;
		QMap<QString, QPointer<WaiterCallback>> _callback_map;
		Core::Meta::TemplatedPropertyList<WaiterCallback> _callback_property_list;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		bool bind();
		bool disconnect();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QQmlListProperty<Cadabra::Queen::Core::Test::WaiterCallback> makeCallbackPropertyList();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QObject* getTarget();
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setTarget(QObject* target);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void classBegin() override;
		virtual void componentComplete() override;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void targetChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onSet(bool onoff);
	/** @} */
	};
} } } }

#endif
