#include "ERP.hpp"

#include "../Conversation/Meta/MessageRender.hpp"

#include "Model/AbstractDocument.hpp"
#include "Model/Message/DocumentMessage.hpp"

namespace Cadabra { namespace Queen { namespace ERP {
	Extension::Extension()
	:
		Queen::Bone("erp")
	{

	}

	Extension::~Extension()
	{

	}

	bool Extension::registerMetaObject()
	{
		qmlRegisterUncreatableMetaObject(ERP::staticMetaObject, "ERP.Global", 0, 1, "ERP", "Access to enums & flags only");

		return true;
	}

	bool Extension::registerMetaType()
	{
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
		qmlRegisterType<ERP::Model::AbstractDocument>("ERP.Model", 0, 1, "AbstractDocuemnt");
		qmlRegisterType<ERP::Model::DocumentMessage>("ERP.Model", 0, 1, "DocumentMessage");

		return true;
	}

	bool Extension::registerAceAction()
	{
		return true;
	}

	bool Extension::registerAceScheme()
	{
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<ERP::Model::DocumentMessage>(1, {"ERP", 0, 1, "DocumentMessage"});

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
