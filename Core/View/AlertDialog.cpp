#include "AlertDialog.hpp"

#include "../System/Native.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	AlertDialog::AlertDialog(QObject* parent)
	:
		QObject(parent),

		_state(nullptr)
	{

	}

	AlertDialog::~AlertDialog()
	{

	}

	bool AlertDialog::show(const QString& state_name)
	{
		if (_connection) {
			return false;
		}

		QStringList list;
		AlertState* state(nullptr);
		if (!state_name.isEmpty()) {
			for (AlertState*& s : _state_property_list) {
				if (s->getName() == state_name) {
					state = s;
				}
			}
		} else if (!_state && !_state_property_list.isEmpty()) {
			state = _state_property_list.first();
		} else {
			state = _state;
		}

		if (!state) {
			return false;
		}
		const QStringList action_title_list(state->makeActionTitleList());
		if (action_title_list.isEmpty()) {
			return false;
		}

		_state = state;
		_connection = QObject::connect(System::Native::Instance(), &System::Native::actionAlertDone, std::bind(&AlertDialog::onDone, this, std::placeholders::_1, std::placeholders::_2, _state));
		System::Native::Instance()->alertAction(_state->getTitle(), _state->getText(), action_title_list, {}, QString::number(reinterpret_cast<quintptr>(_state)));

		return true;
	}

	QQmlListProperty<AlertState> AlertDialog::makeStatePropertyList()
	{
		return _state_property_list;
	}

	const QString& AlertDialog::getState() const
	{
		static const QString dummy_name;
		return _state ? _state->getName() : _state_property_list.isEmpty() ? dummy_name : _state_property_list.first()->getName();
	}

	void AlertDialog::setState(const QString& value)
	{
		if ((_state && _state->getName() == value) || _state_property_list.isEmpty()) {
			return ;
		}
		for (AlertState*& state : _state_property_list) {
			if (state->getName() == value) {
				_state = state;
				emit stateChanged();
			}
		}
	}

	void AlertDialog::onDone(int index, QString ident, AlertState* state)
	{
		if (ident != QString::number(reinterpret_cast<quintptr>(state)) || !QObject::disconnect(_connection)) {
			return ;
		}
		_connection = QMetaObject::Connection();
		AlertAction* action(state->getAction(index));
		if (action) {
			emit action->done();
		}
	}

	AlertState::AlertState(QObject* parent)
	:
		QObject(parent)
	{

	}

	AlertState::~AlertState()
	{

	}

	QQmlListProperty<AlertAction> AlertState::makeActionPropertyList()
	{
		return _action_property_list;
	}

	QStringList AlertState::makeActionTitleList() const
	{
		QStringList retval;
		for (AlertAction* const& action : _action_property_list) {
			if (action->isEnabled()) {
				switch (action->getStyle()) {
					case AlertAction::Style::Cancel:
						retval.append("^" + action->getText());
						break;

					case AlertAction::Style::Discard:
						retval.append("!" + action->getText());
						break;

					default:
						retval.append(action->getText());
				}
			}
		}
		return retval;
	}

	const QString& AlertState::getName() const
	{
		return _name;
	}

	const QString& AlertState::getTitle() const
	{
		return _title;
	}

	const QString& AlertState::getText() const
	{
		return _text;
	}

	AlertAction* AlertState::getAction(int index) const
	{
		for (int a = 0; a < _action_property_list.length(); ++a) {
			AlertAction* const& retval(_action_property_list.at(a));
			if (retval->isEnabled() && 0 == index--) {
				return retval;
			}
		}
		return nullptr;
	}

	void AlertState::setName(const QString& value)
	{
		if (value != _name) {
			_name = value;
			emit nameChanged();
		}
	}

	void AlertState::setTitle(const QString& value)
	{
		if (value != _title) {
			_title = value;
			emit titleChanged();
		}
	}

	void AlertState::setText(const QString& value)
	{
		if (value != _text) {
			_text = value;
			emit textChanged();
		}
	}

	AlertAction::AlertAction(QObject* parent)
	:
		QObject(parent),

		_style(Style::Default),
		_is_enabled(true)
	{

	}

	AlertAction::~AlertAction()
	{

	}

	void AlertAction::enable(bool onoff)
	{
		if (onoff !=_is_enabled) {
			_is_enabled = onoff;
			emit enableChanged();
		}
	}

	const QString& AlertAction::getText() const
	{
		return _text;
	}

	const QString& AlertAction::getLabel() const
	{
		return _label;
	}

	const AlertAction::Style& AlertAction::getStyle() const
	{
		return _style;
	}

	void AlertAction::setText(const QString& value)
	{
		if (value != _text) {
			_text = value;
			emit textChanged();
		}
	}

	void AlertAction::setLabel(const QString& value)
	{
		if (value != _label) {
			_label = value;
			emit labelChanged();
		}
	}

	void AlertAction::setStyle(AlertAction::Style value)
	{
		if (value != _style) {
			_style = value;
			emit styleChanged();
		}
	}

	bool AlertAction::isEnabled() const
	{
		return _is_enabled;
	}
} } } }
