#include "MenuCallback.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Model {
	MenuCallback::MenuCallback(QObject* parent)
	:	
		QObject(parent),

		_is_activated(true),
		_is_highlighted(false)
	{

	}

	MenuCallback::~MenuCallback()
	{

	}

	QString MenuCallback::getTitle() const
	{
		return _title;
	}

	QString MenuCallback::getSection() const
	{
		return _section;
	}

	QUrl MenuCallback::getIcon() const
	{
		return _icon;
	}

	void MenuCallback::setTitle(QString value)
	{
		if (value != _title) {
			_title = value;
			emit titleChanged();
		}
	}

	void MenuCallback::setSection(QString value)
	{
		if (value != _section) {
			_section = value;
			emit sectionChanged();
		}
	}

	void MenuCallback::setIcon(QUrl value)
	{
		if (value != _icon) {
			_icon = value;
			emit iconChanged();
		}
	}

	void MenuCallback::setActive(bool value)
	{
		if (value != _is_activated) {
			_is_activated = value;
			emit activeChanged();
		}
	}

	void MenuCallback::setHighlight(bool value)
	{
		if (value != _is_highlighted) {
			_is_highlighted = value;
			emit highlightChanged();
		}
	}

	bool MenuCallback::isActivated() const
	{
		return _is_activated;
	}

	bool MenuCallback::isHighlighted() const
	{
		return _is_highlighted;
	}
} } } }
