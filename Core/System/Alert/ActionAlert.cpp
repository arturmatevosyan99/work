/**
 @file ActionAlert.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "ActionAlert.hpp"

#include <QMetaEnum>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	ActionAlert::SigBack ActionAlert::Signal(ActionAlert::Resolution resolution)
	{
		switch (resolution) {
			case Resolution::Accept:
				return &ActionAlert::accepted;

			case Resolution::Action:
				return &ActionAlert::actioned;

			case Resolution::Apply:
				return &ActionAlert::applied;

			case Resolution::Reject:
				return &ActionAlert::rejected;

			case Resolution::Reset:
				return &ActionAlert::reset;

			case Resolution::Discard:
				return &ActionAlert::discarded;

			case Resolution::Yes:
				return &ActionAlert::yes;

			case Resolution::No:
				return &ActionAlert::no;

			case Resolution::Help:
				return &ActionAlert::helped;

			case Resolution::Invalid:
				return &ActionAlert::invalidated;

			case Resolution::Custom:
				return &ActionAlert::customized;
		}

		return &ActionAlert::invalidated;
	}

	ActionAlert::Resolution ActionAlert::OptionResolution(AlertOption::Flag option)
	{
		switch (option) {
			case AlertOption::Flag::Ok:
				return Resolution::Accept;

			case AlertOption::Flag::Open:
				return Resolution::Accept;

			case AlertOption::Flag::Save:
				return Resolution::Accept;

			case AlertOption::Flag::Cancel:
				return Resolution::Reject;

			case AlertOption::Flag::Close:
				return Resolution::Reject;

			case AlertOption::Flag::Discard:
				return Resolution::Discard;

			case AlertOption::Flag::Apply:
				return Resolution::Apply;

			case AlertOption::Flag::Reset:
				return Resolution::Reset;

			case AlertOption::Flag::Restore:
				return Resolution::Reset;

			case AlertOption::Flag::Help:
				return Resolution::Help;

			case AlertOption::Flag::SaveAll:
				return Resolution::Accept;

			case AlertOption::Flag::Yes:
				return Resolution::Yes;

			case AlertOption::Flag::YesToAll:
				return Resolution::Yes;

			case AlertOption::Flag::No:
				return Resolution::No;

			case AlertOption::Flag::NoToAll:
				return Resolution::No;

			case AlertOption::Flag::Abort:
				return Resolution::Reject;

			case AlertOption::Flag::Retry:
				return Resolution::Accept;

			case AlertOption::Flag::Ignore:
				return Resolution::Accept; /// @???

			case AlertOption::Flag::Custom:
				return Resolution::Custom;
		}
	}

	ActionAlert::ActionAlert(QObject* parent)
	:
		DialogAlert(parent),

		_resolution(Resolution::Invalid)
	{

	}

	ActionAlert::ActionAlert(QObject* parent, const QString& name, const QString& title, const QString& text,AlertOption::Flags option_flagset, ActionAlert::Flags message_flagset, DialogAlert::Level level)
	:
		DialogAlert(parent, name, title, level),

		_text(text),
		_flagset(message_flagset),
		_resolution(Resolution::Invalid)
	{
		const QMetaEnum meta_enum(QMetaEnum::fromType<AlertOption::Flag>());
		for (int e = 0; e < meta_enum.keyCount(); ++e) {
			const AlertOption::Flag option_flag(static_cast<AlertOption::Flag>(meta_enum.value(e)));
			if (option_flagset.testFlag(option_flag)) {
				addButton(option_flag);
			}
		}
	}

	QQmlListProperty<AlertOption> ActionAlert::makeOptionPropertyList()
	{
		return _option_property_list;
	}

	bool ActionAlert::warn()
	{
		_resolution = Resolution::Invalid;
		return Alert::Instance()->enqueue(this);
	}

	bool ActionAlert::opt(AlertOption* option)
	{
		_resolution = Resolution::Invalid;
		if (option) {
			for (const AlertOption* o : _option_property_list) {
				if (o == option) {
					_resolution = ActionAlert::OptionResolution(option->getFlag());
					emit option->opted();
					option = nullptr;
					break;
				}
			}
		}
		Alert::Instance()->dequeue(this);
		return true;
	}

	bool ActionAlert::resolve(Cadabra::Queen::Core::System::ActionAlert::Resolution resolution)
	{
		_resolution = resolution;
		Alert::Instance()->dequeue(this);
		return true;
	}

	AlertOption::Flags ActionAlert::mask() const
	{
		AlertOption::Flags retval(AlertOption::Flag::Custom);
		for (const AlertOption* alert_option : _option_property_list) {
			retval |= alert_option->getFlag();
		}
		return retval;
	}

	const QString& ActionAlert::getText() const
	{
		return _text;
	}

	const ActionAlert::Flags& ActionAlert::getFlagSet() const
	{
		return _flagset;
	}

	ActionAlert::Resolution ActionAlert::getResolution() const
	{
		return _resolution;
	}

	const Core::Meta::TemplatedPropertyList<AlertOption>& ActionAlert::getOptionPropertyList() const
	{
		return _option_property_list;
	}

	void ActionAlert::setText(const QString& value)
	{
		if (value != _text) {
			_text = value;
			emit textChanged();
		}
	}

	void ActionAlert::setFlagSet(Flags value)
	{
		if (value != _flagset) {
			_flagset = value;
			emit flagsetChanged();
		}
	}

	void ActionAlert::addButton(AlertOption::Flag flag)
	{
		_option_property_list.push_back(new AlertOption(this, flag));
	}

	void ActionAlert::addButton(const QString& caption, AlertOption::Flag flag)
	{
		_option_property_list.push_back(new AlertOption(this, flag, caption));
	}
} } } }
