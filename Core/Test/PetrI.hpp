#ifndef Cadabra_Queen_Core_Test_PetrI_hpp
#define Cadabra_Queen_Core_Test_PetrI_hpp

#include "Token.hpp"
#include "Context.hpp"
#include "Node/Port.hpp"
#include "Node/Assertion.hpp"
#include "Node/Action.hpp"
#include "../../Core/Meta/PropertyList.hpp"

#include <QObject>
#include <QQmlListProperty>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class PetrI : public QObject
	{
	/** @name Statics */
	/** @{ */
	public:
		static Node* Item(QQmlListProperty<Node>* list, int index);
		static int Size(QQmlListProperty<Node>* list);
		static void Clear(QQmlListProperty<Node>* list);
		static void Append(QQmlListProperty<Node>* list, Node* item);
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		struct NodePropertyListTuple
		{
			Core::Meta::TemplatedPropertyList<Port> port;
			Core::Meta::TemplatedPropertyList<Assertion> assertion;
			Core::Meta::TemplatedPropertyList<Action> action;
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Core::Test::PetrI* origin READ getOrigin NOTIFY originChanged)
		Q_PROPERTY(Cadabra::Queen::Core::Test::Context* context READ getContext WRITE setContext NOTIFY contextChanged)
		Q_PROPERTY(Cadabra::Queen::Core::Test::Assertion* preAssertion READ getPreAssertion WRITE setPreAssertion NOTIFY preAssertionChanged)
		Q_PROPERTY(Cadabra::Queen::Core::Test::Token* currentToken READ getCurrentToken NOTIFY currentTokenChanged)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Test::Token> tokens READ makeTokenPropertyList CONSTANT)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Test::Action> preActions READ makePreActionPropertyList CONSTANT)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Test::Port> ports READ makePortPropertyList CONSTANT)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Test::Assertion> assertions READ makeAssertionPropertyList CONSTANT)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Test::Action> actions READ makeActionPropertyList CONSTANT)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Test::Node> nodes READ makeNodePropertyList CONSTANT)
		Q_PROPERTY(bool autoFire READ getAutoFire WRITE setAutoFire NOTIFY autoFireChanged)
		Q_PROPERTY(bool headless READ getHeadless WRITE setHeadless NOTIFY headlessChanged)
		Q_CLASSINFO("DefaultProperty", "nodes")
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		PetrI(QObject* parent = nullptr, PetrI* origin = nullptr);
		virtual ~PetrI() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		PetrI* _origin;

		Context* _context;

		Assertion* _preassertion;
		Core::Meta::TemplatedPropertyList<Action> _preaction_property_list;

		Token* _current_token;
		Core::Meta::TemplatedPropertyList<Token> _token_property_list;

		/// @note Временно отключена история передвижений (чтобы можно было реализовать в моделях циклы)
//		QHash<Token*, QVector<Action*>> _passed_action_hash;

		NodePropertyListTuple _node_property_list_tuple;
		Core::Meta::TemplatedPropertyList<Port>& _port_property_list;
		Core::Meta::TemplatedPropertyList<Assertion>& _assertion_property_list;
		Core::Meta::TemplatedPropertyList<Action>& _action_property_list;

		bool _autofire;
		bool _headless;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool fire();

	private:
		bool fire(const Core::Meta::TemplatedPropertyList<Action>& action_property_list, bool charge);
		void printNodeInformation(Node* node) const;
		void printEnabledTransitions(Node* node) const;
		void repair();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QQmlListProperty<Token> makeTokenPropertyList();
		QQmlListProperty<Action> makePreActionPropertyList();
		QQmlListProperty<Port> makePortPropertyList();
		QQmlListProperty<Action> makeActionPropertyList();
		QQmlListProperty<Assertion> makeAssertionPropertyList();
		QQmlListProperty<Node> makeNodePropertyList();

	private:
		QVector<Action*> makeUpcomingActionVector(const Core::Meta::TemplatedPropertyList<Action>& actions, Token* token) const;
		QVector<Token*> makeAvailableTokenVector(Action* action) const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		PetrI* getOrigin() const;

		Context* getContext() const;

		Assertion* getPreAssertion() const;
		Token* getCurrentToken() const;

		const Core::Meta::TemplatedPropertyList<Action>& getPreActionPropertyList() const;
		const Core::Meta::TemplatedPropertyList<Port>& getPortPropertyList() const;
		const Core::Meta::TemplatedPropertyList<Action>& getActionPropertyList() const;
		const Core::Meta::TemplatedPropertyList<Assertion>& getAssertionPropertyList() const;

		bool getAutoFire() const;
		bool getHeadless() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		bool resetOrigin(PetrI* origin);

		Assertion* addAssertion(Assertion* assertion);
		Action* addAction(Action* action);
		void addToken(Token* token);
		void addArc(Action* action, Assertion* assertion);
		void addArc(Assertion* assertion, Action* action);

		void setContext(Context* context);
		void setPreAssertion(Assertion* pre_assertion);

		void setAutoFire(bool onoff);
		void setHeadless(bool onoff);

	private:
		void setCurrentToken(Token* token, bool force = false);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void originChanged();

		void contextChanged();

		void preAssertionChanged();
		void currentTokenChanged();

		void autoFireChanged();
		void headlessChanged();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isOrigin() const;
	/** @} */
	};
} } } }

#endif
