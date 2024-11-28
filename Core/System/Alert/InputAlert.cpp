/**
 @file InputAlert.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "InputAlert.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	InputAlert::InputAlert(QObject* parent)
	:
		DialogAlert(parent)
	{

	}

	InputAlert::InputAlert(QObject* parent, const QString& name, const QString& title, const QString& placeholder, InputAlert::Flags flagset, DialogAlert::Level level)
	:
		DialogAlert(parent, name, title, level),

		_placeholder(placeholder),
		_flagset(flagset)
	{

	}

	bool InputAlert::warn()
	{
		_text.clear();
		return Alert::Instance()->enqueue(this);
	}

	bool InputAlert::complete(const QString& value)
	{
		if (value != _text) {
			_text = value;
			emit textChanged();
		}
		Alert::Instance()->dequeue(this);
		return true;
	}

	bool InputAlert::cancel()
	{
		if (!_text.isEmpty()) {
			_text.clear();
			emit textChanged();
		}
		Alert::Instance()->dequeue(this);
		return true;
	}

	const QString& InputAlert::getPlaceholder() const
	{
		return _placeholder;
	}

	const QString& InputAlert::getText() const
	{
		return _text;
	}

	const InputAlert::Flags& InputAlert::getFlagSet() const
	{
		return _flagset;
	}

	void InputAlert::setPlaceholder(const QString& value)
	{
		if (value != _placeholder) {
			_placeholder = value;
			emit placeholderChanged();
		}
	}

	void InputAlert::setFlagSet(Flags value)
	{
		if (value != _flagset) {
			_flagset = value;
			emit flagsetChanged();
		}
	}
} } } }
