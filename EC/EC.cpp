#include "EC.hpp"

#include "../Repository/Meta/ApplicationRender.hpp"
#include "../Conversation/Meta/MessageRender.hpp"
#include "../EC/Meta/InappReceipt.hpp"

#include "Model/Application/OnlineShop.hpp"

#include "Model/Message/PaymentMessage.hpp"
#include "Model/Message/OfferMessage.hpp"

#include "Data/Showcase.hpp"
#include "Data/Stock.hpp"
#include "Data/Basket.hpp"

#include "Meta/CustomerAccount.hpp"

namespace Cadabra { namespace Queen { namespace EC {
	Extension::Extension()
	:
		Queen::Bone("ec")
	{

	}

	Extension::~Extension()
	{

	}

	bool Extension::registerMetaObject()
	{
		qmlRegisterUncreatableMetaObject(EC::staticMetaObject, "EC.Global", 0, 1, "EC", "Access to enums & flags only");

		return true;
	}

	bool Extension::registerMetaType()
	{
		qRegisterMetaType<EC::PaymentFault>();
		qRegisterMetaType<EC::Meta::InappReceipt>("Cadabra::Queen::EC::Meta::InappReceipt");
		qRegisterMetaType<QMap<Cadabra::Queen::Repository::Meta::Essence::Id, Cadabra::Queen::SD::Meta::Money>>("QMap<Cadabra::Queen::Repository::Meta::Essence::Id, Cadabra::Queen::SD::Meta::Money>");
		qRegisterMetaType<QSet<Cadabra::Queen::Repository::Meta::Essence::Id>>("QSet<Cadabra::Queen::Repository::Meta::Essence::Id>");

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
		qmlRegisterType<EC::Model::OnlineShop>("EC.Model", 0, 1, "OnlineShop");
		qmlRegisterType<EC::Model::PaymentMessage>("EC.Model", 0, 1, "PaymentMessage");
		qmlRegisterType<EC::Model::OfferMessage>("EC.Model", 0, 1, "OfferMessage");
		qmlRegisterType<EC::Data::Showcase>("EC.Data", 0, 1, "Showcase");
		qmlRegisterAnonymousType<EC::Data::Stock>("EC.Data", 0);
		qmlRegisterAnonymousType<EC::Data::Basket>("EC.Data", 0);
		qmlRegisterAnonymousType<EC::Meta::CustomerAccount>("EC.Meta", 0);

		return true;
	}

	bool Extension::registerAceAction()
	{
		return true;
	}

	bool Extension::registerAceScheme()
	{
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<EC::Model::OnlineShop>(1, {"EC", "OnlineShop", 0, 1, "ShopShowcase"}); /// < deafult
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<EC::Model::OnlineShop>(11, {"EC", "ShopShowcase", 0, 1, "ShopShowcase"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<EC::Model::OnlineShop>(12, {"EC", "MerchantCentral", 0, 1, "MerchantCentral"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<EC::Model::OnlineShop>(13, {"EC", "CustomerAccount", 0, 1, "CustomerAccount"});

		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<EC::Model::PaymentMessage>(1, {"EC", 0, 1, "PaymentMessage"});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<EC::Model::OfferMessage>(2, {"EC", 0, 1, "OfferMessage"});

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
