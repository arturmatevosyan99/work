/**
 @file AlertOption.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "AlertOption.hpp"

#include "ActionAlert.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	AlertOption::AlertOption(QObject* parent)
	:
		QObject(parent)
	{

	}

	AlertOption::AlertOption(QObject* parent, Flag flag)
	:
		QObject(parent),

		_flag(flag)
	{
		setCaption(flag);
	}

	AlertOption::AlertOption(QObject* parent, Flag Flag, const QString& caption)
	:
		QObject(parent),

		_flag(Flag),
		_caption(caption)
	{

	}

	void AlertOption::opt()
	{
		emit opted();
		ActionAlert* alert(qobject_cast<ActionAlert*>(parent()));
		if (alert) {
			alert->opt(this);
		}
	}

	const QString& AlertOption::getCaption() const
	{
		return _caption;
	}

	AlertOption::Flag AlertOption::getFlag() const
	{
		return _flag;
	}

	void AlertOption::setCaption(const QString& value)
	{
		if (_caption != value) {
			_caption = value;
			emit captionChanged();
		}
	}

	void AlertOption::setCaption(AlertOption::Flag flag)
	{
		switch (flag) {
			case Flag::Ok:
				return setCaption(tr("Ok"));

			case Flag::Open:
				return  setCaption(tr("Open"));

			case Flag::Save:
				return setCaption(tr("Save"));

			case Flag::Cancel:
				return setCaption(tr("Cancel"));

			case Flag::Close:
				return setCaption(tr("Close"));

			case Flag::Discard:
				return setCaption(tr("Discard"));

			case Flag::Apply:
				return setCaption(tr("Apply"));

			case Flag::Reset:
				return setCaption(tr("Reset"));

			case Flag::Restore:
				return setCaption(tr("Restore Defaults"));

			case Flag::Help:
				return setCaption(tr("Help"));

			case Flag::SaveAll:
				return setCaption(tr("Save All"));

			case Flag::Yes:
				return setCaption(tr("Yes"));

			case Flag::YesToAll:
				return setCaption(tr("All Yes"));

			case Flag::No:
				return setCaption(tr("No"));

			case Flag::NoToAll:
				return setCaption(tr("All No"));

			case Flag::Abort:
				return setCaption(tr("Abort"));

			case Flag::Retry:
				return setCaption(tr("Retry"));

			case Flag::Ignore:
				return setCaption(tr("Ignore"));

			default:
				return setCaption(tr("Button"));
		}
	}

	void AlertOption::setFlag(AlertOption::Flag value)
	{
		if (_flag != value) {
			_flag = value;
			emit flagChanged();
			if (_caption.isEmpty()) {
				setCaption(_flag);
			}
		}
	}
} } } }
