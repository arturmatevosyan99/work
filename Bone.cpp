#include "Bone.hpp"

namespace Cadabra { namespace Queen {
	Bone::Bone(const QString& name)
	:
		QObject(nullptr),

		_name(name),
		_cupboard(nullptr)
	{

	}

	Bone::Bone(const Bone& bone)
	{

	}

	Bone::~Bone()
	{

	}

	QVariant& Bone::optCommandLine(const QString& option_name, const QStringList& name_list, const QString& description, const QString& value_name, const QVariant& default_value)
	{
		OptionMap::Iterator o
		(
			_option_map.insert
			(
				option_name,
				qMakePair
				(
					QCommandLineOption
					(
						name_list,
						description,
						value_name,
						default_value.toString()
					),
					default_value
				)
			)
		);
		return o.value().second;
	}

	bool Bone::registerMetaObject()
	{
		return true;
	}

	bool Bone::registerMetaType()
	{
		return true;
	}

	bool Bone::registerMetaTypeConverter()
	{
		return true;
	}

	bool Bone::registerMetaTypeStreamOperator()
	{
		return true;
	}

	bool Bone::registerQMLType()
	{
		return true;
	}

	bool Bone::registerAceAction()
	{
		return true;
	}

	bool Bone::registerAceScheme()
	{
		return true;
	}

	bool Bone::registerQMLSingletonType()
	{
		return true;
	}

	bool Bone::registerTestCase()
	{
		return true;
	}

	bool Bone::tweakApplicationEngine(QQmlApplicationEngine& engine)
	{
		return true;
	}

	bool Bone::tweakRootContext(QQmlContext& context)
	{
		return true;
	}

	bool Bone::tweakRootObject(QObject& object)
	{
		return true;
	}

	const QString& Bone::getName() const
	{
		return _name;
	}

	Bone::OptionMap& Bone::getCommandLineOptionMap()
	{
		return _option_map;
	}

	const Bone::OptionMap& Bone::getCommandLineOptionMap() const
	{
		return _option_map;
	}

	Cupboard& Bone::getCupboard()
	{
		return *_cupboard;
	}

	const Cupboard& Bone::getCupboard() const
	{
		return *_cupboard;
	}

	QString Bone::makeTitle() const
	{
		QString retval(_name.toLower());
		retval[0] = retval.at(0).toTitleCase();
		return retval;
	}

	void Bone::onAttach(Skeleton& skeleton)
	{

	}

	void Bone::onDetach(Skeleton& skeleton)
	{

	}

	void Bone::onPreDisclose(Cupboard& cupboard)
	{

	}

	void Bone::onPostDisclose(Cupboard& cupboard)
	{

	}
} }
