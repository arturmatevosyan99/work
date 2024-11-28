#include "MemoryShocker.hpp"

#include <algorithm>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Stress { namespace Test {
	MemoryShocker::MemoryShocker(QObject* parent)
	:
		AbstractShocker(parent),

		_span(0),
		_pointer(nullptr)
	{

	}

	MemoryShocker::~MemoryShocker()
	{
		if (nullptr != _pointer) {
			qWarning() << "Destructing" << _span << "bytes...";
			delete [] _pointer;
		}
	}

	int MemoryShocker::getSpan() const
	{
		return _span;
	}

	void MemoryShocker::setSpan(int span)
	{
		if (span > 0 && _span != span && nullptr == _pointer) {
			_span = span;
			emit spanChanged();
		}
	}


	bool MemoryShocker::turnOn()
	{
		if (_span <= 0 || nullptr != _pointer) {
			return false;
		}
		qWarning() << "Allocating" << _span << "bytes...";
		_pointer = new char[_span];
		std::fill(_pointer, _pointer + _span, 1);
		return true;
	}

	bool MemoryShocker::turnOff()
	{
		if (nullptr == _pointer) {
			return false;
		}
		qWarning() << "Freeing" << _span << "bytes...";
		delete [] _pointer;
		_pointer = nullptr;
		return true;
	}
} } } }
