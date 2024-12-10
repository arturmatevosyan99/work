#include "DateCase.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Test {
	bool DateCase::run()
	{
		qDebug() << "DateCase!";

		// operator==
		// Сравнение двух дат, содержащих хотя бы одно поле any или undefined, всегда возвращает false:
		equalCrutch(Qaos::Date("**-01-2020"), Qaos::Date("##-01-2020"), false);
		equalCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-##-2020"), false);
		equalCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-01-####"), false);

		equalCrutch(Qaos::Date("01-**-2020"), Qaos::Date("##-01-2020"), false);
		equalCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-##-2020"), false);
		equalCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-01-####"), false);

		equalCrutch(Qaos::Date("01-01-****"), Qaos::Date("##-01-2020"), false);
		equalCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-##-2020"), false);
		equalCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-####"), false);

		// Для valid-полей все стандартно:
		equalCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2020"), true);

		equalCrutch(Qaos::Date("01-01-2020"), Qaos::Date("02-01-2020"), false);
		equalCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-02-2020"), false);
		equalCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2022"), false);

		// Any-поле всегда равно valid-полю:
		equalCrutch(Qaos::Date("01-01-2020"), Qaos::Date("**-01-2020"), true);
		equalCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-**-2020"), true);
		equalCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-****"), true);

		// Undefined-поле всегда не равно valid-полю:
		equalCrutch(Qaos::Date("01-01-2020"), Qaos::Date("##-01-2020"), false);
		equalCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-##-2020"), false);
		equalCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-####"), false);

		// Any-поле всегда равно any-полю:
		equalCrutch(Qaos::Date("**-01-2020"), Qaos::Date("**-01-2020"), true);
		equalCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-**-2020"), true);
		equalCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-****"), true);

		// Undefined-поле всегда равно undefined-полю:
		equalCrutch(Qaos::Date("##-01-2020"), Qaos::Date("##-01-2020"), true);
		equalCrutch(Qaos::Date("01-##-2020"), Qaos::Date("01-##-2020"), true);
		equalCrutch(Qaos::Date("01-01-####"), Qaos::Date("01-01-####"), true);

		// operator!=
		// Сравнение двух дат, содержащих хотя бы одно поле any или undefined, всегда возвращает false:
		nonEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("##-01-2020"), false);
		nonEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-##-2020"), false);
		nonEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-01-####"), false);

		nonEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("##-01-2020"), false);
		nonEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-##-2020"), false);
		nonEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-01-####"), false);

		nonEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("##-01-2020"), false);
		nonEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-##-2020"), false);
		nonEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-####"), false);

		// Для valid-полей все стандартно:
		nonEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2020"), false);

		nonEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("02-01-2020"), true);
		nonEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-02-2020"), true);
		nonEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2022"), true);

		// Any-поле всегда равно valid-полю:
		nonEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("**-01-2020"), false);
		nonEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-**-2020"), false);
		nonEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-****"), false);

		// Undefined-поле всегда не равно valid-полю:
		nonEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("##-01-2020"), true);
		nonEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-##-2020"), true);
		nonEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-####"), true);

		// Any-поле всегда равно any-полю:
		nonEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("**-01-2020"), false);
		nonEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-**-2020"), false);
		nonEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-****"), false);

		// Undefined-поле всегда равно undefined-полю:
		nonEqualCrutch(Qaos::Date("##-01-2020"), Qaos::Date("##-01-2020"), false);
		nonEqualCrutch(Qaos::Date("01-##-2020"), Qaos::Date("01-##-2020"), false);
		nonEqualCrutch(Qaos::Date("01-01-####"), Qaos::Date("01-01-####"), false);

		// operator>
		// Сравнение двух дат, содержащих хотя бы одно поле any или undefined, всегда возвращает false:
		greaterCrutch(Qaos::Date("**-01-2020"), Qaos::Date("##-01-2020"), false);
		greaterCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-##-2020"), false);
		greaterCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-01-####"), false);

		greaterCrutch(Qaos::Date("01-**-2020"), Qaos::Date("##-01-2020"), false);
		greaterCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-##-2020"), false);
		greaterCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-01-####"), false);

		greaterCrutch(Qaos::Date("01-01-****"), Qaos::Date("##-01-2020"), false);
		greaterCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-##-2020"), false);
		greaterCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-####"), false);

		// Для valid-полей все стандартно:
		greaterCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2020"), false);

		greaterCrutch(Qaos::Date("02-01-2020"), Qaos::Date("01-01-2020"), true);
		greaterCrutch(Qaos::Date("01-02-2020"), Qaos::Date("01-01-2020"), true);
		greaterCrutch(Qaos::Date("01-01-2022"), Qaos::Date("01-01-2020"), true);

		greaterCrutch(Qaos::Date("01-01-2020"), Qaos::Date("02-01-2020"), false);
		greaterCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-02-2020"), false);
		greaterCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2022"), false);

		// Any-поле всегда равно valid-полю:
		greaterCrutch(Qaos::Date("01-01-2020"), Qaos::Date("**-01-2020"), false);
		greaterCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-**-2020"), false);
		greaterCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-****"), false);

		// Undefined-поле всегда не равно valid-полю:
		greaterCrutch(Qaos::Date("01-01-2020"), Qaos::Date("##-01-2020"), false);
		greaterCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-##-2020"), false);
		greaterCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-####"), false);

		// Any-поле всегда равно any-полю:
		greaterCrutch(Qaos::Date("**-01-2020"), Qaos::Date("**-01-2020"), false);
		greaterCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-**-2020"), false);
		greaterCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-****"), false);

		// Undefined-поле всегда равно undefined-полю:
		greaterCrutch(Qaos::Date("##-01-2020"), Qaos::Date("##-01-2020"), false);
		greaterCrutch(Qaos::Date("01-##-2020"), Qaos::Date("01-##-2020"), false);
		greaterCrutch(Qaos::Date("01-01-####"), Qaos::Date("01-01-####"), false);

		// operator>=
		// Сравнение двух дат, содержащих хотя бы одно поле any или undefined, всегда возвращает false:
		greaterOrEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("##-01-2020"), false);
		greaterOrEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-##-2020"), false);
		greaterOrEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-01-####"), false);

		greaterOrEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("##-01-2020"), false);
		greaterOrEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-##-2020"), false);
		greaterOrEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-01-####"), false);

		greaterOrEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("##-01-2020"), false);
		greaterOrEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-##-2020"), false);
		greaterOrEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-####"), false);

		// Для valid-полей все стандартно:
		greaterOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2020"), true);

		greaterOrEqualCrutch(Qaos::Date("02-01-2020"), Qaos::Date("01-01-2020"), true);
		greaterOrEqualCrutch(Qaos::Date("01-02-2020"), Qaos::Date("01-01-2020"), true);
		greaterOrEqualCrutch(Qaos::Date("01-01-2022"), Qaos::Date("01-01-2020"), true);

		greaterOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("02-01-2020"), false);
		greaterOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-02-2020"), false);
		greaterOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2022"), false);

		// Any-поле всегда равно valid-полю:
		greaterOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("**-01-2020"), true);
		greaterOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-**-2020"), true);
		greaterOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-****"), true);

		// Undefined-поле всегда не равно valid-полю:
		greaterOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("##-01-2020"), false);
		greaterOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-##-2020"), false);
		greaterOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-####"), false);

		// Any-поле всегда равно any-полю:
		greaterOrEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("**-01-2020"), true);
		greaterOrEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-**-2020"), true);
		greaterOrEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-****"), true);

		// Undefined-поле всегда равно undefined-полю:
		greaterOrEqualCrutch(Qaos::Date("##-01-2020"), Qaos::Date("##-01-2020"), true);
		greaterOrEqualCrutch(Qaos::Date("01-##-2020"), Qaos::Date("01-##-2020"), true);
		greaterOrEqualCrutch(Qaos::Date("01-01-####"), Qaos::Date("01-01-####"), true);

		// operator<
		// Сравнение двух дат, содержащих хотя бы одно поле any или undefined, всегда возвращает false:
		lessCrutch(Qaos::Date("**-01-2020"), Qaos::Date("##-01-2020"), false);
		lessCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-##-2020"), false);
		lessCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-01-####"), false);

		lessCrutch(Qaos::Date("01-**-2020"), Qaos::Date("##-01-2020"), false);
		lessCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-##-2020"), false);
		lessCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-01-####"), false);

		lessCrutch(Qaos::Date("01-01-****"), Qaos::Date("##-01-2020"), false);
		lessCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-##-2020"), false);
		lessCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-####"), false);

		// Для valid-полей все стандартно:
		lessCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2020"), false);

		lessCrutch(Qaos::Date("02-01-2020"), Qaos::Date("01-01-2020"), false);
		lessCrutch(Qaos::Date("01-02-2020"), Qaos::Date("01-01-2020"), false);
		lessCrutch(Qaos::Date("01-01-2022"), Qaos::Date("01-01-2020"), false);

		lessCrutch(Qaos::Date("01-01-2020"), Qaos::Date("02-01-2020"), true);
		lessCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-02-2020"), true);
		lessCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2022"), true);

		// Any-поле всегда равно valid-полю:
		lessCrutch(Qaos::Date("01-01-2020"), Qaos::Date("**-01-2020"), false);
		lessCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-**-2020"), false);
		lessCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-****"), false);

		// Undefined-поле всегда не равно valid-полю:
		lessCrutch(Qaos::Date("01-01-2020"), Qaos::Date("##-01-2020"), false);
		lessCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-##-2020"), false);
		lessCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-####"), false);

		// Any-поле всегда равно any-полю:
		lessCrutch(Qaos::Date("**-01-2020"), Qaos::Date("**-01-2020"), false);
		lessCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-**-2020"), false);
		lessCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-****"), false);

		// Undefined-поле всегда равно undefined-полю:
		lessCrutch(Qaos::Date("##-01-2020"), Qaos::Date("##-01-2020"), false);
		lessCrutch(Qaos::Date("01-##-2020"), Qaos::Date("01-##-2020"), false);
		lessCrutch(Qaos::Date("01-01-####"), Qaos::Date("01-01-####"), false);

		// operator<=
		// Сравнение двух дат, содержащих хотя бы одно поле any или undefined, всегда возвращает false:
		lessOrEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("##-01-2020"), false);
		lessOrEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-##-2020"), false);
		lessOrEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("01-01-####"), false);

		lessOrEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("##-01-2020"), false);
		lessOrEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-##-2020"), false);
		lessOrEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-01-####"), false);

		lessOrEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("##-01-2020"), false);
		lessOrEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-##-2020"), false);
		lessOrEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-####"), false);

		// Для valid-полей все стандартно:
		lessOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2020"), true);

		lessOrEqualCrutch(Qaos::Date("02-01-2020"), Qaos::Date("01-01-2020"), false);
		lessOrEqualCrutch(Qaos::Date("01-02-2020"), Qaos::Date("01-01-2020"), false);
		lessOrEqualCrutch(Qaos::Date("01-01-2022"), Qaos::Date("01-01-2020"), false);

		lessOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("02-01-2020"), true);
		lessOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-02-2020"), true);
		lessOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-2022"), true);

		// Any-поле всегда равно valid-полю:
		lessOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("**-01-2020"), true);
		lessOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-**-2020"), true);
		lessOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-****"), true);

		// Undefined-поле всегда не равно valid-полю:
		lessOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("##-01-2020"), false);
		lessOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-##-2020"), false);
		lessOrEqualCrutch(Qaos::Date("01-01-2020"), Qaos::Date("01-01-####"), false);

		// Any-поле всегда равно any-полю:
		lessOrEqualCrutch(Qaos::Date("**-01-2020"), Qaos::Date("**-01-2020"), true);
		lessOrEqualCrutch(Qaos::Date("01-**-2020"), Qaos::Date("01-**-2020"), true);
		lessOrEqualCrutch(Qaos::Date("01-01-****"), Qaos::Date("01-01-****"), true);

		// Undefined-поле всегда равно undefined-полю:
		lessOrEqualCrutch(Qaos::Date("##-01-2020"), Qaos::Date("##-01-2020"), true);
		lessOrEqualCrutch(Qaos::Date("01-##-2020"), Qaos::Date("01-##-2020"), true);
		lessOrEqualCrutch(Qaos::Date("01-01-####"), Qaos::Date("01-01-####"), true);

		return true;
	}

	void DateCase::equalCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result)
	{
		qDebug() << first << " == " << second << " - " << result << " : " << ((first == second) == result ? "PASS" : "FAIL");
	}

	void DateCase::nonEqualCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result)
	{
		qDebug() << first << " != " << second << " - " << result << " : " << ((first != second) == result ? "PASS" : "FAIL");
	}

	void DateCase::greaterCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result)
	{
		qDebug() << first << " > " << second << " - " << result << " : " << ((first > second) == result ? "PASS" : "FAIL");
	}

	void DateCase::greaterOrEqualCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result)
	{
		qDebug() << first << " >= " << second << " - " << result << " : " << ((first >= second) == result ? "PASS" : "FAIL");
	}

	void DateCase::lessCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result)
	{
		qDebug() << first << " < " << second << " - " << result << " : " << ((first < second) == result ? "PASS" : "FAIL");
	}

	void DateCase::lessOrEqualCrutch(const Qaos::Date& first, const Qaos::Date& second, bool result)
	{
		qDebug() << first << " <= " << second << " - " << result << " : " << ((first <= second) == result ? "PASS" : "FAIL");
	}

	void DateCase::initTestCase()
	{

	}

	void DateCase::initTestCase_data()
	{

	}

	void DateCase::cleanupTestCase()
	{

	}

	void DateCase::init()
	{

	}

	void DateCase::cleanup()
	{

	}
} } } }
