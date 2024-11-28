#include "CAP-653.hpp"

#include "../../../Core/Data/Provider.hpp"
#include "../../../Core/Data/Factory.hpp"
#include "../../../Core/Data/Field.hpp"
#include "../../../Core/Data/Predicate/LikePredicate.hpp"

#include "../../../Qaos/MakeUnique.hpp"

namespace Cadabra { namespace Queen { namespace Stress { namespace Test {
	CAP653::CAP653(QObject* parent)
	:
		Core::Test::AbstractCase(parent)
	{
		setObjectName("SQLite case-insensitive search");
	}

	bool CAP653::run()
	{
		const QScopedPointer<Core::Data::Provider> data_provider(new Core::Data::Provider());
		data_provider->connect(":memory:");

		Core::Data::Factory data_factory(data_provider.get());
		Animal entity;

		if (!data_factory.create(&entity, true)) {
			return false;
		}

		entity.name.setValue("ёж");
		data_factory.insert(&entity);

		Core::Data::Field field("name", Core::Data::Field::Type::Text, false);
		Core::Data::LikePredicate like(field, "ЁЖ");

		return data_provider->count(&entity, Qaos::MakeUnique<Core::Data::LikePredicate>(like).get()) == 1;
	}

	void CAP653::initTestCase()
	{

	}

	void CAP653::initTestCase_data()
	{

	}

	void CAP653::cleanupTestCase()
	{

	}

	void CAP653::init()
	{

	}

	void CAP653::cleanup()
	{

	}

	Animal::Animal()
	:
		Core::Data::AbstractEntity("Core_Animal"),

		name("name", Field::Type::Text, false)
	{
		addField(name);
		setPrimaryKey({name});
	}

	void Animal::onSelect()
	{

	}

	void Animal::onCreate()
	{

	}

	void Animal::onForeign()
	{

	}
} } } }
