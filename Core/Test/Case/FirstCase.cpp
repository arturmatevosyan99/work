#include "FirstCase.hpp"

#include <thread>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	bool FirstCase::run()
	{		
		/// @note error: no member named 'sleep_for' in namespace 'std::__ndk1::this_thread'
//		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		qDebug() << "FirstCase!";
		return false;
	}

	void FirstCase::initTestCase()
	{

	}

	void FirstCase::initTestCase_data()
	{

	}

	void FirstCase::cleanupTestCase()
	{

	}

	void FirstCase::init()
	{

	}

	void FirstCase::cleanup()
	{

	}
} } } }
