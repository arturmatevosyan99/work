#include "PetrI.hpp"

#include "Node/Port.hpp"
#include "Node/Teleport.hpp"
#include "../../Skeleton.hpp"

#include <QDebug>
#include <QRandomGenerator>
#include <QQmlEngine>
#include <QQmlContext>

#include <chrono>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	Node* PetrI::Item(QQmlListProperty<Node>* list, int index)
	{
		NodePropertyListTuple* list_tuple(reinterpret_cast<NodePropertyListTuple*>(list->data));
		if (!list_tuple) {
			return nullptr;
		} else if (index < list_tuple->assertion.length()) {
			return list_tuple->assertion.at(index);
		} else if ((index -= list_tuple->assertion.length()) < list_tuple->action.length()) {
			return list_tuple->action.at(index);
		} else if ((index -= list_tuple->action.length()) < list_tuple->port.length()) {
			return list_tuple->port.at(index);
		}

		return nullptr;
	}

	int PetrI::Size(QQmlListProperty<Node>* list)
	{
		int retval(0);
		QQmlListReference assertion_list_reference(list->object, "assertions");
		if (assertion_list_reference.canCount()) {
			retval += assertion_list_reference.count();
		}

		QQmlListReference action_list_reference(list->object, "actions");
		if (action_list_reference.canCount()) {
			retval += action_list_reference.count();
		}

		QQmlListReference port_list_reference(list->object, "ports");
		if (port_list_reference.canCount()) {
			retval += port_list_reference.count();
		}

		return retval;
	}

	void PetrI::Clear(QQmlListProperty<Node>* list)
	{
		QQmlListReference assertion_list_reference(list->object, "assertions");
		if (assertion_list_reference.canClear()) {
			assertion_list_reference.clear();
		}

		QQmlListReference action_list_reference(list->object, "actions");
		if (action_list_reference.canClear()) {
			action_list_reference.clear();
		}

		QQmlListReference port_list_reference(list->object, "ports");
		if (port_list_reference.canClear()) {
			port_list_reference.clear();
		}
	}

	void PetrI::Append(QQmlListProperty<Node>* list, Node* item)
	{
		Assertion* assertion(qobject_cast<Assertion*>(item));
		if (assertion) {
			QQmlListReference list_reference(list->object, "assertions");
			if (list_reference.canAppend()) {
				list_reference.append(assertion);
			}
			return;
		}

		Action* action(qobject_cast<Action*>(item));
		if (action) {
			QQmlListReference list_reference(list->object, "actions");
			if (list_reference.canAppend()) {
				list_reference.append(action);
			}
			return;
		}

		Port* port(qobject_cast<Port*>(item));
		if (port) {
			QQmlListReference list_reference(list->object, "ports");
			if (list_reference.canAppend()) {
				list_reference.append(port);
			}
			return;
		}
	}

	PetrI::PetrI(QObject* parent, PetrI* origin)
	:
		QObject(parent),

		_origin(origin ? origin : this),

		_context(nullptr),

		_preassertion(nullptr),
		_current_token(nullptr),

		_port_property_list(_node_property_list_tuple.port),
		_assertion_property_list(_node_property_list_tuple.assertion),
		_action_property_list(_node_property_list_tuple.action),

		_autofire(true),
		_headless(true)
	{
		if (this != _origin) {
			QObject::connect(_origin, &PetrI::currentTokenChanged, this, &PetrI::currentTokenChanged);
		}
	}

	PetrI::~PetrI()
	{

	}

	bool PetrI::fire()
	{
		/// @brief Если забыли добавить финальный Assertion, например
		repair();

		for (Port* port : _port_property_list) {
			QPointer<PetrI> petr_i(port->getPetrI());
			if (!petr_i) {
				qWarning() << "This port is broken:" << port->getSource().toString();
				return false;
			} else if (!petr_i->_preassertion) {
				qWarning() << "This port does not contain preAssertion node:" << port->getSource().toString();
				return false;
			} else if (_preassertion == nullptr) {
				/// @note По умолчанию (если узлов в корневой сети нет) ставим токены на preAssertion первой обработанной сети
				/// Пока не понятно, понадобится ли в дальнейшем возможность указать произвольный Assertion для позиционирования токена
				_preassertion = petr_i->_preassertion;
				for (Token* token : _token_property_list) {
					token->addPosition(_preassertion);
				}
				_preaction_property_list.merge(petr_i->_preaction_property_list);
				_assertion_property_list.merge(petr_i->_assertion_property_list);
				_action_property_list.merge(petr_i->_action_property_list);
			} else {
				QScopedPointer<Assertion> final_assertion(petr_i->_preassertion);
				petr_i->_preassertion = nullptr;
				for (Assertion* assertion : _assertion_property_list) {
					if (assertion->getActionVector().isEmpty()) {
						QScopedPointer<Action> teleport(new Teleport());
						teleport->moveToThread(thread());
						teleport->setDescription("Teleport from '" + assertion->getDescription() + "' to '" + final_assertion->getDescription() + "'");
						addArc(assertion, teleport.get());
						addArc(teleport.get(), final_assertion.get());
						_action_property_list.push_back(teleport.take());
					}
				}
				_action_property_list.merge(petr_i->_preaction_property_list);
				_assertion_property_list.push_back(final_assertion.take());
				_assertion_property_list.merge(petr_i->_assertion_property_list);
				_action_property_list.merge(petr_i->_action_property_list);
			}
		}

		return !_autofire ? true : fire(getPreActionPropertyList(), true) && fire(getActionPropertyList(), false);
	}

	bool PetrI::fire(const Core::Meta::TemplatedPropertyList<Action>& action_list, bool charge)
	{
		/// @todo Имеет смысл сначала планировать маршрут ("генерировать путевой лист") для каждого токена, а эмитить сигналы позже
		/// @todo В процессе составления маршрутов было бы полезно рисовать граф передвижений

		qDebug() << (charge ? "Charge!" : "Fire!");

		try {
			QQmlEngine* engine(QQmlEngine::contextForObject(this)->engine());

			if (charge) {
				/// @brief Run initial assert
				for (int length = (_preassertion ? _preassertion->getAssertPropertyList().length() : 0), i = 0; i < length; ++i) {
					if (_preassertion->getAssertPropertyList().at(i)->getTargetAction() == nullptr) {
						if (engine->isInterrupted()) {
							return true;
						}
						_preassertion->getAssertPropertyList().at(i)->beginRun();
						break;
					}
				}
			}

//			for (int i = 0; i < _token_property_list.length(); i++) {
//				_passed_action_hash.insert(_token_property_list.at(i), QVector<Action*>());
//			}

			while (true) {
				QHash<Token*, QVector<Action*>> upcoming_action_hash;
				for (int i = 0; i < _token_property_list.length(); i++) {
					setCurrentToken(_token_property_list.at(i));
					upcoming_action_hash.insert(_token_property_list.at(i), makeUpcomingActionVector(action_list, _token_property_list.at(i)));
				}

				QHash<Token*, QVector<Action*>>::iterator it = upcoming_action_hash.begin();
				while (it != upcoming_action_hash.end()) {
					it.value().isEmpty() ? it = upcoming_action_hash.erase(it) : ++it;
				}

				if (upcoming_action_hash.isEmpty()) {
					if (std::all_of(_token_property_list.begin(), _token_property_list.end(), [](Token* token) { return token->stopAt(nullptr); })) {
						qDebug() << "There are no more available tokens";
					} else {
						qFatal("At least one token by the end has an unvisited stop what normally treats as a fatal error");
						return false;
					}
					break;
				}

				/// @todo Реализовать последовательный/параллельный обход сети в зависимости от значения параметра _headless
				setCurrentToken(upcoming_action_hash.keys().at(QRandomGenerator::global()->bounded(upcoming_action_hash.count())));

				Action* selected_preaction(upcoming_action_hash.value(_current_token).at(QRandomGenerator::global()->bounded(upcoming_action_hash.value(_current_token).length())));
				if (_current_token->stopAt(nullptr) || _current_token->stopAt(selected_preaction)) {
					/// @brief Run start asserts
					for (int i = 0; i < selected_preaction->getInputAssertionVector().length(); i++) {
						for (int j = 0; j < selected_preaction->getInputAssertionVector().at(i)->getAssertPropertyList().length(); j++) {
							if (selected_preaction->getInputAssertionVector().at(i)->getAssertPropertyList().at(j)->getTargetAction() == selected_preaction) {
								if (engine->isInterrupted()) {
									return true;
								} else if (!selected_preaction->getInputAssertionVector().at(i)->getAssertPropertyList().at(j)->getDescription().isEmpty()) {
									qDebug() << _current_token->getDescription() + ": " + selected_preaction->getInputAssertionVector().at(i)->getAssertPropertyList().at(j)->getDescription();
								} else {
									qDebug() << _current_token->getDescription() + ": " + selected_preaction->getInputAssertionVector().at(i)->getDescription();
								}
								selected_preaction->getInputAssertionVector().at(i)->getAssertPropertyList().at(j)->beginRun();
							}
						}
					}

					if (engine->isInterrupted()) {
						return true;
					} else {
						/// @btw How much is the fish?
						qDebug() << _current_token->getDescription() + ": " + selected_preaction->getDescription();
						selected_preaction->run();
					}

					/// @brief Run finish asserts
					for (int i = 0; i < selected_preaction->getAssertionVector().length(); i++) {
						for (int j = 0; j < selected_preaction->getAssertionVector().at(i)->getAssertPropertyList().length(); j++) {
							if (selected_preaction->getAssertionVector().at(i)->getAssertPropertyList().at(j)->getTargetAction() == selected_preaction) {
								if (engine->isInterrupted()) {
									return true;
								} else if (!selected_preaction->getAssertionVector().at(i)->getAssertPropertyList().at(j)->getDescription().isEmpty()) {
									qDebug() << _current_token->getDescription() + ": " + selected_preaction->getAssertionVector().at(i)->getAssertPropertyList().at(j)->getDescription();
								} else {
									qDebug() << _current_token->getDescription() + ": " + selected_preaction->getAssertionVector().at(i)->getDescription();
								}
								selected_preaction->getAssertionVector().at(i)->getAssertPropertyList().at(j)->endRun();
							}
						}
					}
				} else {
					qDebug() << "Token" << _current_token->getDescription() << "bypassed an Action" << selected_preaction->getDescription() << "...";
				}
				_current_token->setPositionVector(selected_preaction->getAssertionVector());
//				_passed_action_hash[_current_token].append(selected_preaction);
			}
		} catch (...) {
			if (charge) {
				qFatal("Unhandled exception on charge");
			} else {
				qFatal("Unhandled exception on fire");
			}
		}
		return true;
	}

	void PetrI::printNodeInformation(Node* node) const
	{
		qDebug() << node->getIdentifier() + " - " + node->getDescription();
	}

	void PetrI::printEnabledTransitions(Node* node) const
	{
		qDebug() << "Enabled transitions:";
		if (const Assertion* assertion = qobject_cast<const Assertion*>(node)) {
			if (!assertion->getActionVector().isEmpty()) {
				for (int i = 0; i < assertion->getActionVector().length(); i++) {
					printNodeInformation(assertion->getActionVector().at(i));
				}
			} else {
				qDebug() << "There are no available transitions!";
			}
		} else if (const Action* action = qobject_cast<const Action*>(node)) {
			if (action->getAssertionVector().isEmpty()) {
				for (int i = 0; i < action->getAssertionVector().length(); i++) {
					printNodeInformation(action->getAssertionVector().at(i));
				}
			} else {
				qDebug() << "There are no available transitions!";
			}
		}
	}

	void PetrI::repair()
	{
		/// @note Данный метод отслеживает и исправляет невалидную ситуацию, когда сеть оканчивается на Action
		/// @todo Необходимо реализовать отдельный метод для общей валидации, который будет проверять топологию всей сети
		for (Action* action : _action_property_list) {
			if (action->getAssertionVector().isEmpty()) {
				qWarning() << "Dummy Assertion is going to be append to the end otherwise net will be malformed";
				QScopedPointer<Assertion> leaf_assertion(new Assertion("Automatically generated assertion"));
				QScopedPointer<Assert> leaf_assert(new Assert(action));
				leaf_assertion->moveToThread(thread());
				leaf_assert->moveToThread(thread());
				leaf_assertion->addAssert(leaf_assert.take());
				addArc(action, leaf_assertion.get());
				_assertion_property_list.push_back(leaf_assertion.take());
			}
		}
	}

	QQmlListProperty<Token> PetrI::makeTokenPropertyList()
	{
		return _token_property_list;
	}

	QQmlListProperty<Action> PetrI::makePreActionPropertyList()
	{
		return _preaction_property_list;
	}

	QQmlListProperty<Port> PetrI::makePortPropertyList()
	{
		return _port_property_list;
	}

	QQmlListProperty<Action> PetrI::makeActionPropertyList()
	{
		return _action_property_list;
	}

	QQmlListProperty<Assertion> PetrI::makeAssertionPropertyList()
	{
		return _assertion_property_list;
	}

	QQmlListProperty<Node> PetrI::makeNodePropertyList()
	{
		return QQmlListProperty<Node>(this, &_node_property_list_tuple, &PetrI::Append, &PetrI::Size, &PetrI::Item, &PetrI::Clear);
	}

	QVector<Action*> PetrI::makeUpcomingActionVector(const Core::Meta::TemplatedPropertyList<Action>& actions, Token* token) const
	{
		QVector<Action*> retval;
		for (int a = 0, i = 0; i < actions.length(); i++) {
			if (actions.at(i)->getCondition()) {
				/**
				 * @brief Простая "защита" от зацикливаний сети при наличии висячих узлов
				 * Необходимость в ней пропадет, когда научимся каким-либо образом обрабатывать ReferenceError'ы QML/JS-движка
				 */
				if (actions.at(i)->getInputAssertionVector().isEmpty()) {
					continue;
				}
				for (int j = 0; j < actions.at(i)->getInputAssertionVector().length(); j++) {
					if (token->getPositionVector().contains(actions.at(i)->getInputAssertionVector().at(j))) {
//						if (!_passed_action_hash.value(token).contains(actions.at(i))) {
							a++;
//							break;
//						}
					}
				}
				if (actions.at(i)->getInputAssertionVector().length() == a) {
					retval.append(actions.at(i));
				}
				a = 0;
			}
		}
		return retval;
	}

	QVector<Token*> PetrI::makeAvailableTokenVector(Action* action) const
	{
		QVector<Token*> retval;
		for (int a = 0, i = 0; i < _token_property_list.length(); i++) {
			for (int j = 0; j < action->getInputAssertionVector().length(); j++) {
				if (_token_property_list.at(i)->getPositionVector().contains(action->getInputAssertionVector().at(j))) {
					++a;
				}
			}

			if (action->getInputAssertionVector().length() == a) {
				retval.append(_token_property_list.at(i));
			}
			a = 0;
		}
		return retval;
	}

	PetrI* PetrI::getOrigin() const
	{
		return _origin;
	}

	Context* PetrI::getContext() const
	{
		return _origin ? _origin->_context : _context;
	}

	Assertion* PetrI::getPreAssertion() const
	{
		return _preassertion;
	}

	Token* PetrI::getCurrentToken() const
	{
		return _origin ? _origin->_current_token : _current_token;
	}

	const Core::Meta::TemplatedPropertyList<Assertion>& PetrI::getAssertionPropertyList() const
	{
		return _assertion_property_list;
	}

	const Core::Meta::TemplatedPropertyList<Action>& PetrI::getPreActionPropertyList() const
	{
		return _preaction_property_list;
	}

	const Core::Meta::TemplatedPropertyList<Action>& PetrI::getActionPropertyList() const
	{
		return _action_property_list;
	}

	const Core::Meta::TemplatedPropertyList<Port>& PetrI::getPortPropertyList() const
	{
		return _port_property_list;
	}

	bool PetrI::getAutoFire() const
	{
		return _autofire;
	}

	bool PetrI::getHeadless() const
	{
		return _headless;
	}

	bool PetrI::resetOrigin(PetrI* origin)
	{
		if (!origin || _origin != this || _origin == origin) {
			return false;
		} else if (_origin) {
			QObject::disconnect(_origin, &PetrI::currentTokenChanged, this, &PetrI::currentTokenChanged);
			QObject::disconnect(_origin, &PetrI::contextChanged, this, &PetrI::contextChanged);
		}
		_origin = origin;
		QObject::connect(_origin, &PetrI::currentTokenChanged, this, &PetrI::currentTokenChanged);
		QObject::connect(_origin, &PetrI::contextChanged, this, &PetrI::contextChanged);
		emit originChanged();
		emit contextChanged();
		return true;
	}

	Assertion* PetrI::addAssertion(Assertion *assertion)
	{
		_assertion_property_list.push_back(assertion);
		return assertion;
	}

	Action* PetrI::addAction(Action* action)
	{
		_action_property_list.push_back(action);
		return action;
	}

	void PetrI::addToken(Token* token)
	{
		token->addPosition(_preassertion);
		_token_property_list.push_back(token);
	}

	void PetrI::addArc(Action *action, Assertion *assertion)
	{
		action->addAssertion(assertion);
	}

	void PetrI::addArc(Assertion* assertion, Action* action)
	{
		assertion->addAction(action);
		action->addInputAssertion(assertion);
	}

	void PetrI::setContext(Context* context)
	{
		if (context != _context) {
			_context = context;
			emit contextChanged();
		}
	}

	void PetrI::setPreAssertion(Assertion* preassertion)
	{
		if (preassertion != _preassertion) {
			_preassertion = preassertion;
			emit preAssertionChanged();
		}
	}

	void PetrI::setAutoFire(bool onoff)
	{
		if (_autofire != onoff) {
			_autofire = onoff;
			emit autoFireChanged();
		}
	}

	void PetrI::setHeadless(bool onoff)
	{
		if (_headless != onoff) {
			_headless = onoff;
			emit headlessChanged();
		}
	}

	void PetrI::setCurrentToken(Token* token, bool force)
	{
		if (_current_token != token || force) {
			_current_token = token;
			QMetaObject::invokeMethod(this, "currentTokenChanged", Qt::BlockingQueuedConnection);
		}
	}

	bool PetrI::isOrigin() const
	{
		return this == _origin;
	}
} } } }
