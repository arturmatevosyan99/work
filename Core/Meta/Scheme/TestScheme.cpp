/**
 @file TestScheme.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "TestScheme.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	TestScheme::TestScheme()
	:
		Core::Meta::AbstractScheme(),

		_variation(TestVariation::Unit),
		_dialect(TestDialect::None),
		_factory(TestFactory::General)
	{

	}

	TestScheme::TestScheme(const QString& extname, const QString& modelname, TestVariation variation, TestDialect dialect, TestFactory factory)
	:
		Core::Meta::AbstractScheme(extname, modelname),

		_variation(variation),
		_dialect(dialect),
		_factory(factory)
	{

	}

	TestScheme::TestScheme(const QString& vendorname, const QString& modulename, const QString& modelname, TestVariation variation, TestDialect dialect, TestFactory factory)
	:
		Core::Meta::AbstractScheme(vendorname, modulename, modelname),

		_variation(variation),
		_dialect(dialect),
		_factory(factory)
	{

	}

	TestScheme::TestScheme(const TestScheme& origin)
	:
		Core::Meta::AbstractScheme(origin),

		_variation(origin._variation),
		_dialect(origin._dialect),
		_factory(origin._factory)
	{

	}

	TestScheme::TestScheme(TestScheme&& origin)
	:
		Core::Meta::AbstractScheme(origin.getVendorName(), origin.getModuleName(), origin.getModelName()),

		_variation(origin._variation),
		_dialect(origin._dialect),
		_factory(origin._factory)
	{

	}

	TestScheme& TestScheme::operator=(const TestScheme& origin)
	{
		_variation = origin._variation;
		_dialect = origin._dialect;
		_factory = origin._factory;

		AbstractScheme::operator=(origin);

		return *this;
	}

	TestScheme& TestScheme::operator=(TestScheme&& origin)
	{
		_variation = std::move(origin._variation);
		_dialect = std::move(origin._dialect);
		_factory = std::move(origin._factory);

		AbstractScheme::operator=(origin);

		return *this;
	}

	bool TestScheme::load(const Cadabra::Queen::Core::Meta::Qd& qd)
	{
		Core::Meta::Scheme<TestScheme>::Iterator i(Core::Meta::Scheme<TestScheme>::Iterate(qd));
		if (!Core::Meta::Scheme<TestScheme>::Exists(i)) {
			return false;
		}
		operator=(i.value());
		return true;
	}

	QString TestScheme::makeModuleDisplayName() const
	{
		return TestModule::Extension == getModule() ? getModuleName() : getVendorName();
	}

	QString TestScheme::makeModuleAbsolutePath() const
	{
		if (getModule() != TestModule::Extension) {
			return getModuleName();
		}
		switch (_variation) {
			case TestVariation::Unit:
				return "qrc:/" + getModuleName() + "/Test/Unit";
			case TestVariation::Development:
				return "qrc:/" + getModuleName() + "/Test/Development";
			case TestVariation::Integration:
				return "qrc:/" + getModuleName() + "/Test/Integration";
			case TestVariation::System:
				return "qrc:/" + getModuleName() + "/Test/System";
			case TestVariation::Death:
				return "qrc:/" + getModuleName() + "/Test/Death";
			case TestVariation::Proof:
				return "qrc:/" + getModuleName() + "/Test/Proof";
			default:
				return "qrc:/" + getModuleName() + "/Test/Custom";
		}
		return "";
	}

	QString TestScheme::makeModelFileName() const
	{
		if (getModule() != TestModule::Extension) {
			return getModelName();
		}
		switch (_dialect) {
			case TestDialect::Gherkin:
				return getModelName() + ".ghk";
			case TestDialect::GraphML:
				return getModelName() + ".gml";
			case TestDialect::QML:
				return getModelName() + ".qml";
			case TestDialect::Python:
				return getModelName() + ".py";
			case TestDialect::None:
			default:
				return "";
		}
	}

	QString TestScheme::makeModelAbsolutePath() const
	{
		return makeModuleAbsolutePath() + "/" + makeModelFileName();
	}

	QHash<int, QByteArray> TestScheme::roleNames() const
	{
		QHash<int, QByteArray> retval(AbstractScheme::roleNames());
		retval[Role::Variation] = "variation";
		retval[Role::Factory] = "factory";
		retval[Role::Dialect] = "dialect";
		retval[Role::Module] = "module";
		retval[Role::ModuleDisplayName] = "module_display_name";
		retval[Role::ModuleAbsolutePath] = "module_absoulte_path";
		retval[Role::ModelFileName] = "model_file_name";

		return retval;
	}

	QVariant TestScheme::data(int role) const
	{
		switch (role) {
			case Role::Variation: return static_cast<int>(_variation);
			case Role::Factory: return static_cast<int>(_factory);
			case Role::Dialect: return static_cast<int>(_dialect);
			case Role::Module: return static_cast<int>(getModule());
			case Role::ModuleDisplayName: return makeModuleDisplayName();
			case Role::ModuleAbsolutePath: return makeModuleAbsolutePath();
			case Role::ModelFileName: return makeModelFileName();

			default: return AbstractScheme::data(role);
		}
	}

	TestVariation TestScheme::getVariation() const
	{
		return _variation;
	}

	TestDialect TestScheme::getDialect() const
	{
		return _dialect;
	}

	TestModule TestScheme::getModule() const
	{
		if (getModuleName().startsWith("http:/") || getModuleName().startsWith("https:/")) {
			return TestModule::Web;
		} else if (getModuleName().startsWith("ftp:/")) {
			return TestModule::FTP;
		} else if (getModuleName().startsWith("git:/")) {
			return TestModule::Git;
		} else if (getModuleName().startsWith("file:/")) {
			return TestModule::FS;
		} else if (getModuleName().startsWith("qrc:/")) {
			return TestModule::QRC;
		}
		return TestModule::Extension;
	}

	TestFactory TestScheme::getFactory() const
	{
		return _factory;
	}

} } } }
