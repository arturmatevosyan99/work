/**
 @file InappReceipt.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "InappReceipt.hpp"

#include "../../Skeleton.hpp"
#include "../../Core/Meta/Qd.hpp"
#include "../../Core/System/Native.hpp"

namespace Cadabra { namespace Queen { namespace EC { namespace Meta {
	InappReceipt::InappReceipt()
	:
		_distributor(InappReceipt::Distributor::Unknown),
		_sku(Core::System::Native::Instance()->getBundleIdentifier()),
		_sandbox(Cadabra::Queen::Skeleton::DebugMode),
		_container("")
	{

	}

	InappReceipt::InappReceipt(InappReceipt::Distributor distributor, const QString& sku, const QString& container, bool sandbox)
	:
		_distributor(distributor),
		_sku(sku),
		_sandbox(sandbox),
		_container(container)
	{

	}

	InappReceipt::InappReceipt(const InappReceipt& origin)
	:
		_distributor(origin._distributor),
		_sku(origin._sku),
		_sandbox(origin._sandbox),
		_container(origin._container)
	{

	}

	InappReceipt::InappReceipt(InappReceipt&& origin)
	:
		_distributor(std::move(origin._distributor)),
		_sku(std::move(origin._sku)),
		_sandbox(std::move(origin._sandbox)),
		_container(std::move(origin._container))
	{

	}

	InappReceipt::~InappReceipt()
	{

	}

	QJsonObject InappReceipt::makeJSON() const
	{
		return QJsonObject {
							{"receipt", _container},
							{"distributor", static_cast<char>(_distributor)},
							{"sku", _sku},
							{"environment", _sandbox ? "sandbox" : "production"}
		};
	}

	InappReceipt& InappReceipt::operator=(const InappReceipt& rhs)
	{
		_distributor = rhs._distributor;
		_sku = rhs._sku;
		_sandbox = rhs._sandbox;
		_container = rhs._container;

		return *this;
	}

	InappReceipt& InappReceipt::operator=(InappReceipt&& rhs)
	{
		_distributor = std::move(rhs._distributor);
		_sku = std::move(rhs._sku);
		_sandbox = std::move(rhs._sandbox);
		_container = std::move(rhs._container);

		return *this;
	}

	const InappReceipt::Distributor& InappReceipt::getDistributor() const
	{
		return _distributor;
	}

	const QString& InappReceipt::getSKU() const
	{
		return _sku;
	}

	const QString& InappReceipt::getContainer() const
	{
		return _container;
	}

	bool InappReceipt::isSandbox() const
	{
		return _sandbox;
	}

	bool InappReceipt::isInvalid() const
	{
		return Distributor::Unknown == _distributor || _sku.isEmpty() || _container.isEmpty();
	}
} } } }
