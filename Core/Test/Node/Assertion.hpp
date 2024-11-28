#ifndef Cadabra_Queen_Core_Test_Node_Assertion_hpp
#define Cadabra_Queen_Core_Test_Node_Assertion_hpp

#include "../Node.hpp"

#include "Action.hpp"
#include "../Assert.hpp"
#include "../../Meta/PropertyList.hpp"

#include <QObject>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class Action;
	class Assert;

	class Assertion: public Node
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QVector<Cadabra::Queen::Core::Test::Action*> actions READ getActionVector WRITE setActionVector NOTIFY actionVectorChanged)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Test::Assert> asserts READ makeAssertPropertyList CONSTANT)
		Q_CLASSINFO("DefaultProperty", "asserts");
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Assertion(const QString& description = "Empty description");
		virtual ~Assertion() override;
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QVector<Action*> _action_vector;
		Core::Meta::TemplatedPropertyList<Assert> _assert_property_list;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QQmlListProperty<Assert> makeAssertPropertyList();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QVector<Action*>& getActionVector() const;
		const Core::Meta::TemplatedPropertyList<Assert>& getAssertPropertyList() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void addAction(Action* action);
		void addAssert(Assert* assert);

		void setActionVector(const QVector<Action*>& actions);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void actionVectorChanged();
	/** @} */
	};
} } } }

#endif
