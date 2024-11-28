#ifndef Cadabra_Queen_Core_Test_Node_Waiter_GroupWaiter_hpp
#define Cadabra_Queen_Core_Test_Node_Waiter_GroupWaiter_hpp

#include "../AbstractWaiter.hpp"

#include <QMutex>
#include <QSet>
#include <QList>
#include <QQmlListProperty>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class GroupWaiter : public AbstractWaiter
	{
	/** @name Statics */
	/** @{ */
	public:
		static AbstractWaiter* Item(QQmlListProperty<AbstractWaiter>* list, int index);
		static int Size(QQmlListProperty<AbstractWaiter>* list);
		static void Clear(QQmlListProperty<AbstractWaiter>* list);
		static void Append(QQmlListProperty<AbstractWaiter>* list, AbstractWaiter* item);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Test::AbstractWaiter> waiterList READ makePropertyList CONSTANT)
		Q_CLASSINFO("DefaultProperty", "waiterList")
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		GroupWaiter(QObject* parent = nullptr);
		virtual ~GroupWaiter() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QList<AbstractWaiter*> _waiter_list;
		QSet<AbstractWaiter*> _idle_set;
		QMutex _mutex;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void reset(bool signal = false);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QQmlListProperty<AbstractWaiter> makePropertyList();
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
		void onPartiallyDone();
	/** @} */
	};
} } } }

#endif
