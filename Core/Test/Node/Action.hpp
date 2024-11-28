#ifndef Cadabra_Queen_Core_Test_Node_Action_hpp
#define Cadabra_Queen_Core_Test_Node_Action_hpp

#include "AbstractWaiter.hpp"

#include "../Node.hpp"

#include <QObject>

#include <QList>
#include <QJSValue>
#include <QQmlListProperty>

#include <QMutex>
#include <QWaitCondition>
#include <QAtomicInteger>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class Assertion;

	class Action : public Node
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
		Q_PROPERTY(QVector<Cadabra::Queen::Core::Test::Assertion*> assertions READ getAssertionVector WRITE setAssertionVector NOTIFY assertionVectorChanged)
		Q_PROPERTY(int waitTimeout READ getWaitTimeout WRITE setWaitTimeout NOTIFY waitTimeoutChanged)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Test::AbstractWaiter> waitList READ makeWaitPropertyList CONSTANT)
		Q_PROPERTY(bool condition READ getCondition WRITE setCondition NOTIFY conditionChanged)
		Q_CLASSINFO("DefaultProperty", "waitList")
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Action(const QString& description = "Empty description");
		virtual ~Action() override;
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QVector<Assertion*> _assertion_vector;
		QVector<Assertion*> _input_assertion_vector;

		QMutex _wait_mutex;
		QWaitCondition _wait_codition;
		QAtomicInteger<bool> _wait_atom;
		quint32 _wait_timeout;
		QList<AbstractWaiter*> _wait_list;

		/// @note Данное поле необходимо для реализации условных переходов
		/// @todo Хорошо бы придумать механизм, защищающий модель от бесконечных циклов
		bool _condition;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void run();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QQmlListProperty<AbstractWaiter> makeWaitPropertyList();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		quint32 getWaitTimeout() const;
		const QVector<Assertion*>& getAssertionVector() const;
		const QVector<Assertion*>& getInputAssertionVector() const;
		bool getCondition() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void setWaitTimeout(quint32 value);

		void addAssertion(Assertion* assertion);
		void addInputAssertion(Assertion* inputAssertion);

		void setAssertionVector(const QVector<Assertion*>& assertions);
		void setInputAssertionVector(const QVector<Assertion*>& inputAssertions);
		void setCondition(bool condition);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void waitTimeoutChanged();
		void assertionVectorChanged();
		void conditionChanged();
		void acted();
		void snoozed();
	/** @} */

	/** @name Signals */
	/** @{ */
	private slots:
		void awake();
	/** @} */
	};
} } } }

#endif
