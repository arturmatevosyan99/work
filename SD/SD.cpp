#include "SD.hpp"

#include "Meta/PaymentPreference.hpp"
#include "Meta/PurchaseItem.hpp"
#include "Meta/Money.hpp"
#include "Meta/Counterparty.hpp"
#include "Meta/ContactCounterparty.hpp"
#include "Meta/LegalCounterparty.hpp"

namespace Cadabra { namespace Queen { namespace SD {
	Extension::Extension()
	:
		Queen::Bone("sd")
	{

	}

	Extension::~Extension()
	{

	}

	bool Extension::registerMetaObject()
	{
		qmlRegisterUncreatableMetaObject(SD::staticMetaObject, "SD.Global", 0, 1, "SD", "Access to enums & flags only");

		return true;
	}

	bool Extension::registerMetaType()
	{
		qRegisterMetaType<SD::LegalType>();
		qRegisterMetaType<SD::PersonSex>();

		return true;
	}

	bool Extension::registerMetaTypeConverter()
	{
		return true;
	}

	bool Extension::registerMetaTypeStreamOperator()
	{
		return true;
	}

	bool Extension::registerQMLType()
	{
		qmlRegisterAnonymousType<SD::Meta::PurchaseItem>("SD.Meta", 0);
		qRegisterMetaType<SD::Meta::PaymentPreference>();
		qRegisterMetaType<SD::Meta::Money>();
		qmlRegisterAnonymousType<SD::Meta::Counterparty>("SD.Meta", 0);
		qmlRegisterAnonymousType<SD::Meta::ContactCounterparty>("SD.Meta", 0);
		qmlRegisterAnonymousType<SD::Meta::LegalCounterparty>("SD.Meta", 0);

		return true;
	}

	bool Extension::registerAceAction()
	{
		return true;
	}

	bool Extension::registerAceScheme()
	{
		return true;
	}

	bool Extension::registerQMLSingletonType()
	{
		return true;
	}

	bool Extension::tweakRootContext(QQmlContext& context)
	{
		return true;
	}
} } }
