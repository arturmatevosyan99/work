#include "CalorieBalanceQuiz.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	CalorieBalanceQuiz::CalorieBalanceQuiz(QObject* parent)
	:
		GenericQuiz(parent)
	{

	}

	CalorieBalanceQuiz::~CalorieBalanceQuiz()
	{

	}

	bool CalorieBalanceQuiz::onHandle()
	{
		return GenericQuiz::onHandle();
	}

	bool CalorieBalanceQuiz::onOpen()
	{

		return GenericQuiz::onOpen();
	}
} } } }
