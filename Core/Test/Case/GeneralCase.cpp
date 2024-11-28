#include "GeneralCase.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	GeneralCase::GeneralCase(QObject* parent)
	:
		AbstractCase(parent)
	{

	}

	GeneralCase::~GeneralCase()
	{
		if (!_unit_o.isNull()) {
			_unit_o->deleteLater();
		}
	}

	bool GeneralCase::run()
	{
		QVariant retval(false);
		QMetaObject::invokeMethod(_unit_o, "test", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retval));
		return static_cast<QMetaType::Type>(retval.type()) == QMetaType::Type::Bool ? retval.toBool() : false;
	}

	bool GeneralCase::dial(const QQmlIncubator& incubator)
	{
		if (!_unit_o.isNull()) {
			_unit_o->deleteLater();
		}
		return !(_unit_o = incubator.object()).isNull();
	}

	const QPointer<QObject>& GeneralCase::getObject() const
	{
		return _unit_o;
	}

	void GeneralCase::initTestCase()
	{

	}

	void GeneralCase::initTestCase_data()
	{

	}

	void GeneralCase::cleanupTestCase()
	{

	}

	void GeneralCase::init()
	{

	}

	void GeneralCase::cleanup()
	{

	}
} } } }
