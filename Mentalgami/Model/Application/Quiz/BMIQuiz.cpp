#include "BMIQuiz.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	BMIQuiz::BMIQuiz(QObject* parent)
	:
		GenericQuiz(parent)
	{

	}

	BMIQuiz::~BMIQuiz()
	{

	}

	bool BMIQuiz::onHandle()
	{
		return GenericQuiz::onHandle();
	}

	bool BMIQuiz::onOpen()
	{

		return GenericQuiz::onOpen();
	}
} } } }
