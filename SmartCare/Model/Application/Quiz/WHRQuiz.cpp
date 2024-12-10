#include "WHRQuiz.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	WHRQuiz::WHRQuiz(QObject* parent)
	:
		GenericQuiz(parent)
	{

	}

	WHRQuiz::~WHRQuiz()
	{

	}

	bool WHRQuiz::onHandle()
	{
		return GenericQuiz::onHandle();
	}

	bool WHRQuiz::onOpen()
	{

		return GenericQuiz::onOpen();
	}
} } } }
