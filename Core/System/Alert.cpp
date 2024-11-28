/**
 @file Alert.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "Alert.hpp"

#include "Alert/DialogAlert.hpp"
#include "Alert/StateAlert.hpp"
#include "Alert/SoundAlert.hpp"
#include "Alert/InputAlert.hpp"
#include "Alert/ActionAlert.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	Alert* Alert::Instance()
	{
		static Alert retval;
		return &retval;
	}

	QObject* Alert::QInstance(QQmlEngine* engine, QJSEngine* scriptEngine)
	{
		Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)

		Alert* retval(Cadabra::Queen::Core::System::Alert::Instance());

		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}

	Alert::Alert(QObject* parent)
	:
		QAbstractListModel(parent),

		_headless(false),

		_audio_thread(new QThread())
	{
		_audio_thread->start();
		QObject::connect
		(
			_audio_thread,
			&QThread::finished,

			_audio_thread,
			&QObject::deleteLater
		);
	}

	Alert::~Alert()
	{
		_audio_thread->quit();
		_audio_thread->exit();
	}

	bool Alert::flush()
	{
		bool retval(false);

		while (!_dialog_queue.isEmpty()) {
			DialogAlert* alert(_dialog_queue.head());
			if (!alert) {
				_dialog_queue.removeFirst();
			}
			retval |= true;

			InputAlert* input(qobject_cast<InputAlert*>(alert));
			if (input) {
				dequeue(input);
				continue;
			}

			ActionAlert* action(qobject_cast<ActionAlert*>(alert));
			if (action) {
				dequeue(action);
				continue;
			}

			/// @xxx Unknown type of alert
			queue(alert);
		}

		while (!_state_queue.isEmpty()) {
			retval |= true;
			dequeue(_state_queue.head());
		}

		return retval;
	}

	AbstractAlert* Alert::find(Type type, const QString& name) const
	{
		AbstractAlert* retval(nullptr);
		if (Type::State == type) {
			QQueue<StateAlert*>::ConstIterator s
			(
				std::find_if
				(
					_state_queue.constBegin(),
					_state_queue.constEnd(),
					[&name](StateAlert* state)
					{
						return state && state->objectName() == name;
					}
				)
			);
			retval = _state_queue.constEnd() == s ? nullptr : *s;
		} else if (Type::Input == type || Type::Action == type) {
			QQueue<DialogAlert*>::ConstIterator d
			(
				std::find_if
				(
					_dialog_queue.constBegin(),
					_dialog_queue.constEnd(),
					[&type, &name](DialogAlert* dialog)
					{
						if (Type::Input == type && !qobject_cast<InputAlert*>(dialog)) {
							return false;
						} else if (Type::Action == type && !qobject_cast<ActionAlert*>(dialog)) {
							return false;
						} else if (dialog->objectName() != name) {
							return false;
						}
						return true;
					}
				)
			);
			retval = _dialog_queue.constEnd() == d ? nullptr : *d;
		}
		return retval;
	}

	void Alert::sound(const QString& name, DialogAlert::Level level)
	{
		QScopedPointer<SoundAlert> sound(new SoundAlert(nullptr, name, level));
		qWarning("Sound: +%s(%p)", qUtf8Printable(sound->objectName()), sound.data());
		sound->moveToThread(_audio_thread);
		/// @todo Beaty deletion via enqueue/dequeue
		sound.take()->warn();
	}

	StateAlert* Alert::state(const QString& name, const QString& title, const QString& description, QObject* source, DialogAlert::Level level)
	{
		QScopedPointer<StateAlert> retval(new StateAlert(this, name, title, description, source, level));
		QQmlEngine::setObjectOwnership(retval.get(), QQmlEngine::CppOwnership);
		return enqueue(retval.get()) ? retval.take() : nullptr;
	}

	InputAlert* Alert::input(const QString& name, const QString& title, const QString& placeholder, int input_flagset)
	{
		QScopedPointer<InputAlert> retval
		(
			new InputAlert
			(
				this,
				name,
				title,
				placeholder,
				static_cast<InputAlert::Flags>(input_flagset)
			)
		);
		QQmlEngine::setObjectOwnership(retval.get(), QQmlEngine::CppOwnership);
		enqueue(retval.get());
		return retval.take();
	}

	ActionAlert* Alert::action(const QString& name, const QString& title, const QString& text, int option_flagset, int action_flagset, DialogAlert::Level level)
	{
		QScopedPointer<ActionAlert> retval
		(
			new ActionAlert
			(
				this,
				name,
				title,
				text,
				static_cast<AlertOption::Flags>(option_flagset),
				static_cast<ActionAlert::Flags>(action_flagset),
				level
			)
		);
		QQmlEngine::setObjectOwnership(retval.get(), QQmlEngine::CppOwnership);
		enqueue(retval.get());
		return retval.take();
	}

	bool Alert::enqueue(StateAlert* state)
	{
		for (StateAlert*& s : _state_queue) {
			if (state->getSource() == s->getSource() && state->objectName() == s->objectName()) {
				return false;
			}
		}
		const int offset(_state_queue.length());
		beginInsertRows({}, offset, offset);
		qWarning("State: +%s(%p)", qUtf8Printable(state->objectName()), state);
		_state_queue.enqueue(state);
		endInsertRows();
		emit stated(state, true);
		if (!_headless/* || state->getFlagSet().testFlag(InputAlert::Flag::HeadFull)*/) {
			head(state);
		}
		return true;
	}

	bool Alert::enqueue(SoundAlert* sound)
	{
		/// @todo
		return false;
	}

	bool Alert::enqueue(InputAlert* input)
	{
		if (!input) {
			return false;
		} else if (Alert::Instance()->isModal(input)) {
			return true;
		}
		const int offset(_state_queue.size() + _dialog_queue.size());
		beginInsertRows({}, offset, offset);
		qWarning("Dialog: +%s(%p)", qUtf8Printable(input->objectName()), input);
		_dialog_queue.enqueue(input);
		endInsertRows();
		if (_dialog_queue.head() == input) {
			emit inputed(input, true);
			if (!_headless || input->getFlagSet().testFlag(InputAlert::Flag::HeadFull)) {
				head(input);
			}
			emit input->modalChanged();
			return true;
		}
		return false;
	}

	bool Alert::enqueue(ActionAlert* message)
	{
		if (!message) {
			return false;
		} else if (Alert::Instance()->isModal(message)) {
			return true;
		}
		const int offset(_state_queue.size() + _dialog_queue.size());
		beginInsertRows({}, offset, offset);
		qWarning("Dialog: +%s(%p)", qUtf8Printable(message->objectName()), message);
		_dialog_queue.enqueue(message);
		endInsertRows();
		if (_dialog_queue.head() == message) {
			emit actioned(message, true);
			if (!_headless || message->getFlagSet().testFlag(ActionAlert::Flag::HeadFull)) {
				head(message);
			}
			emit message->modalChanged();
			return true;
		}
		return false;
	}

	bool Alert::dequeue(StateAlert* state)
	{
		if (!state) {
			return false;
		}
		const int ss1(_state_queue.count());
		if (ss1 < 1) {
			return false;
		}
		for (int s = 0, ss2 = _state_queue.count(); s < ss2; ++s) {
			if (_state_queue[s] == state) {
				beginRemoveRows({}, s, s);
				_state_queue.removeAt((--ss2, s--));
				endRemoveRows();
			}
		} if (_state_queue.count() == ss1) {
			return false;
		}
		emit stated(state, false);
		qWarning("State: -%s(%p)", qUtf8Printable(state->objectName()), state);
		if (state->parent() == this) {
			state->deleteLater();
		}
		return true;
	}

	bool Alert::dequeue(InputAlert* input)
	{
		if (!input) {
			return false;
		} else if (input->getText().isEmpty()) {
			emit input->canceled();
		} else {
			emit input->completed();
		}
		queue(input);
		emit inputed(input, false);
		return true;
	}

	bool Alert::dequeue(ActionAlert* action)
	{
		if (!action) {
			return false;
		}
		ActionAlert::SigBack signal_back
		(
			ActionAlert::Signal
			(
				_dialog_queue.head() == action
				? action->getResolution()
				: ActionAlert::Resolution::Invalid
			)
		);
		queue(action);
		emit actioned(action, false);
		std::invoke(signal_back, *action);
		return true;
	}

	void Alert::queue(DialogAlert* dialog)
	{
		const int dd1(_dialog_queue.count());
		if (dd1 < 1) {
			return ;
		} else if (!dialog) {
			return ;
		}

		for (int d = 0, dd2 = dd1; d < dd2; ++d) {
			if (_dialog_queue.at(d) == dialog) {
				beginRemoveRows({}, d, d);
				_dialog_queue.removeAt((--dd2, d--));
				qWarning("Dialog: -%s(%p)", qUtf8Printable(dialog->objectName()), dialog);
				endRemoveRows();
				/// @note Нет break, потому что этот же указатель может быть еще где-то в этой очереди
			}
		}
		const int dd3(_dialog_queue.count());
		if (dd3 == dd1) {
			/// @note Диалог был вне очереди
			return ;
		} else if (dialog->parent() == this) {
			dialog->deleteLater();
		}
		if (dd3 < 1) {
			return ;
		}

		InputAlert* input(qobject_cast<InputAlert*>(_dialog_queue.head()));
		if (input) {
			emit inputed(input, true);
			if (!_headless || input->getFlagSet().testFlag(InputAlert::Flag::HeadFull)) {
				head(input);
			}
			emit input->modalChanged();
			return;
		}

		ActionAlert* message(qobject_cast<ActionAlert*>(_dialog_queue.head()));
		if (message) {
			emit actioned(message, true);
			if (!_headless || message->getFlagSet().testFlag(ActionAlert::Flag::HeadFull)) {
				head(message);
			}
			emit message->modalChanged();
			return;
		}
	}

	int Alert::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() ? 0 : _state_queue.count() + _dialog_queue.count();
	}

	const QQueue<StateAlert*>& Alert::getStateQueue() const
	{
		return _state_queue;
	}

	const QQueue<DialogAlert*>& Alert::getDialogQueue() const
	{
		return _dialog_queue;
	}

	QVariant Alert::data(const QModelIndex& index, int key) const
	{
		const Role role(static_cast<Role>(key));
		const AbstractAlert* target(nullptr);
		do {
			int offset(_state_queue.size());
			if (index.row() < offset) {
				if (Role::Type == role) {
					return Type::State;
				}
				target = _state_queue.at(index.row());
				break;
			}
			offset = index.row() - offset;
			if (offset < _dialog_queue.size()) {
				target = _dialog_queue.at(offset);
				break;
			}
			return {};
		} while (false);
		switch (role) {
/*
			case Qt::UserRole:
				return QVariant::fromValue(target);
*/
			case Role::Type:
				if (!qobject_cast<const InputAlert*>(target)) {
					return Type::Input;
				} else if (!qobject_cast<const ActionAlert*>(target)) {
					return Type::Action;
				}
				return {};

			case Role::Level:
				return target->getLevel();

			case Role::Name:
				return target->objectName();

			case Role::Title:
				return target->getTitle();
		}
		return {};
	}

	QHash<int, QByteArray> Alert::roleNames() const
	{
		QHash<int, QByteArray> retval(QAbstractListModel::roleNames());
		retval[static_cast<int>(Role::Type)] = "type";
		retval[static_cast<int>(Role::Level)] = "level";
		retval[static_cast<int>(Role::Name)] = "name";
		retval[static_cast<int>(Role::Title)] = "title";
		return retval;
	}

	void Alert::setHeadless(bool onoff)
	{
		if (onoff != _headless) {
			_headless = onoff;
			emit headlessChanged();
		}
	}

	bool Alert::isHeadless() const
	{
		return _headless;
	}

	bool Alert::isModal(const StateAlert* state) const
	{
		return std::find(_state_queue.constBegin(), _state_queue.constEnd(), state) != _state_queue.constEnd();
	}

	bool Alert::isModal(const DialogAlert* dialog) const
	{
		return !_dialog_queue.isEmpty() && _dialog_queue.head() == dialog;
	}

	AbstractAlert::AbstractAlert(QObject* parent, const QString& name, const QString& title, Level level)
	:
		QObject(parent),

		_title(title),
		_level(level)
	{
		QObject::setObjectName(name);
	}

	AbstractAlert::AbstractAlert(const AbstractAlert& origin)
	:
		QObject(origin.parent()),

		_title(origin.getTitle()),
		_level(origin.getLevel())
	{

	}

	AbstractAlert::~AbstractAlert()
	{

	}

	bool AbstractAlert::warn()
	{
		return false;
	}

	const QString& AbstractAlert::getTitle() const
	{
		return _title;
	}

	AbstractAlert::Level AbstractAlert::getLevel() const
	{
		return _level;
	}

	void AbstractAlert::setTitle(const QString& value)
	{
		if (value != _title) {
			_title = value;
			emit titleChanged();
		}
	}

	void AbstractAlert::setLevel(Level value)
	{
		if (value != _level) {
			_level = value;
			emit levelChanged();
		}
	}
} } } }
