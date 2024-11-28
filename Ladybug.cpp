#include "Ladybug.hpp"

#include "Stress/Stress.hpp"
#include "Core/Core.hpp"
#include "Repository/Repository.hpp"
#include "Conversation/Conversation.hpp"
#include "DrOblozhko/DrOblozhko.hpp"
#include "ERP/ERP.hpp"
#include "SD/SD.hpp"
#include "EC/EC.hpp"
#include "Mentalgami/Mentalgami.hpp"

#include "Core/System/Alert.hpp"
#include "Core/Meta/Scheme/TestScheme.hpp"
#include "Stress/System/TestThread.hpp"

#include <QByteArray>
#include <QVector>
#include <QMap>
#include <QDir>
#include <QFile>

int SkeletonMain(int& argc, char** argv)
{
	QByteArray debugger_option(Cadabra::Queen::Stress::Extension::DebuggerOption());
	if (debugger_option.isEmpty()) {
		return Cadabra::Queen::Ladybug(argc, argv).run();
	}
	int arg_length(argc + 1);
	QVector<char*> arg_vector(argv, argv + argc);
	arg_vector.push_back(debugger_option.prepend("-qmljsdebugger=").data());
	arg_vector.push_back(nullptr);
	return Cadabra::Queen::Ladybug(arg_length, arg_vector.data()).run();
}

namespace Cadabra { namespace Queen {
	Ladybug::Ladybug(int argc)
	:
		Skeleton("ladybug", argc, nullptr, ApplicationFlags),

		_stress_extension(new Stress::Extension()),

		_model_repository_option("model-repository", "Add test-repository", "URI", "queen:/"),
		_model_mask_option("model-mask", "Run tests", "Masked path", "*/Unit/*")
	{
	}

	Ladybug::Ladybug(int& argc, char** argv, int flags)
	:
		Skeleton("ladybug", argc, argv, flags),

		_stress_extension(new Stress::Extension()),

		_model_repository_option("model-repository", "Add test-repository", "URI", "queen:/"),
		_model_mask_option("model-mask", "Run tests", "Masked path", "*/Unit/*")
	{
		if (hasLogFile()) {
			QObject::connect(this, &Skeleton::infoMessaged, this, &Skeleton::fileLog, Qt::AutoConnection);
			QObject::connect(this, &Skeleton::debugMessaged, this, &Skeleton::fileLog, Qt::AutoConnection);
		}
	}

	bool Ladybug::config()
	{
		if (!Skeleton::config()) {
			return false;
		}
		if (!std::all_of(getBoneList().begin(), getBoneList().end(), [](Bone* bone) -> bool { return bone->registerTestCase(); })) {
			qCritical() << "Register error(QML test type)";
			return false;
		}

		QSet<QUrl> log_client_set;
		log_client_set = getPreferenceOption("logclient_set", QVariant::fromValue(log_client_set)).value<QSet<QUrl>>();
		for (const QUrl& url : qAsConst(log_client_set)) {
			_stress_extension->replicate(url, false);
		}

		QSet<QUrl> model_repository_set;
		model_repository_set = getPreferenceOption("repository_set", QVariant::fromValue(model_repository_set)).value<QSet<QUrl>>();
		if (_cli_parser.isSet(_model_repository_option)) {
			const QStringList model_repository_list(_cli_parser.values(_model_repository_option));
			model_repository_set += QSet<QUrl>(model_repository_list.begin(), model_repository_list.end());
		}
		for (const QUrl& url : qAsConst(model_repository_set)) {
			_stress_extension->attach(url, false);
		}
		if (_cli_parser.isSet(_model_mask_option)) {
			_stress_extension->getCheckList()->filter(_cli_parser.values(_model_mask_option));
		}

		QVariantMap environment_map;
		environment_map = getPreferenceOption("environment_map", QVariant::fromValue(environment_map)).value<QVariantMap>();
		for (QVariantMap::ConstIterator v = environment_map.constBegin(); v != environment_map.constEnd(); ++v) {
			setEnvironmentVariable(v.key(), v.value());
		}

		setEnvironmentVariable("QUEEN_VERSION", QCoreApplication::applicationVersion());

		return true;
	}

	bool Ladybug::tune()
	{
		if (!Skeleton::tune()) {
			return false;
		}

		Core::System::Alert::Instance()->setHeadless(true);

		Core::View::Theme::Instance()->load("Guiness", Core::View::Theme::Light);

		return true;
	}

	bool Ladybug::extend()
	{
		Skeleton::extend(new Core::Extension());
		Skeleton::extend(new Repository::Extension());
		Skeleton::extend(new Conversation::Extension());
		Skeleton::extend(new ERP::Extension());
		Skeleton::extend(new SD::Extension());
		Skeleton::extend(new EC::Extension());
		Skeleton::extend(new DrOblozhko::Extension());
		Skeleton::extend(new Mentalgami::Extension());
		Skeleton::extend(_stress_extension);

		return true;
	}

	int Ladybug::command()
	{
		return command(_cli_parser.positionalArguments());
	}

	int Ladybug::command(const QStringList& path_list)
	{
		Stress::Model::CheckList* check_list(_stress_extension->getCheckList());
		if (!check_list) {
			return 0;
		}
		Stress::System::TestThread test_thread;
		check_list->filter(path_list);
		return (test_thread.start(check_list, false) < 1) || !test_thread.wait() ? EXIT_FAILURE : EXIT_SUCCESS;
	}

	int Ladybug::boot()
	{
		_cli_parser.addOption(_model_repository_option);
		_cli_parser.addOption(_model_mask_option);

		return Skeleton::boot(QUrl(QStringLiteral("qrc:/Ladybug.qml")));
	}

	void Ladybug::testCoreUnit()
	{
		QVERIFY(command({"Core/Unit/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testCoreIntegration()
	{
		QVERIFY(command({"Core/Integration/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testCoreSystem()
	{
		QVERIFY(command({"Core/System/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testCoreDeath()
	{
		QVERIFY(command({"Core/Death/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testCoreProof()
	{
		QVERIFY(command({"Core/Proof/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testCoreDevelopment()
	{
		QVERIFY(command({"Core/Development/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testCoreCustom()
	{
		QVERIFY(command({"Core/Custom/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testStressUnit()
	{
		QVERIFY(command({"Stress/Unit/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testStressIntegration()
	{
		QVERIFY(command({"Stress/Integration/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testStressSystem()
	{
		QVERIFY(command({"Stress/System/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testStressDeath()
	{
		QVERIFY(command({"Stress/Death/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testStressProof()
	{
		QVERIFY(command({"Stress/Proof/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testStressDevelopment()
	{
		QVERIFY(command({"Stress/Development/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testStressCustom()
	{
		QVERIFY(command({"Stress/Custom/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testRepositoryUnit()
	{
		QVERIFY(command({"Repository/Unit/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testRepositoryIntegration()
	{
		QVERIFY(command({"Repository/Integration/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testRepositorySystem()
	{
		QVERIFY(command({"Repository/Stress/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testRepositoryDeath()
	{
		QVERIFY(command({"Repository/Death/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testRepositoryProof()
	{
		QVERIFY(command({"Repository/Proof/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testRepositoryDevelopment()
	{
		QVERIFY(command({"Repository/Development/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testRepositoryCustom()
	{
		QVERIFY(command({"Repository/Custom/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testERPUnit()
	{
		QVERIFY(command({"ERP/Unit/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testERPIntegration()
	{
		QVERIFY(command({"ERP/Integration/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testERPSystem()
	{
		QVERIFY(command({"ERP/System/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testERPDeath()
	{
		QVERIFY(command({"ERP/Death/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testERPProof()
	{
		QVERIFY(command({"ERP/Proof/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testERPDevelopment()
	{
		QVERIFY(command({"ERP/Development/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testERPCustom()
	{
		QVERIFY(command({"ERP/Custom/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testSDUnit()
	{
		QVERIFY(command({"SD/Unit/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testSDIntegration()
	{
		QVERIFY(command({"SD/Integration/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testSDSystem()
	{
		QVERIFY(command({"SD/System/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testSDDeath()
	{
		QVERIFY(command({"SD/Death/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testSDProof()
	{
		QVERIFY(command({"SD/Proof/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testSDDevelopment()
	{
		QVERIFY(command({"SD/Development/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testSDCustom()
	{
		QVERIFY(command({"SD/Custom/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testECUnit()
	{
		QVERIFY(command({"EC/Unit/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testECIntegration()
	{
		QVERIFY(command({"EC/Integration/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testECSystem()
	{
		QVERIFY(command({"EC/System/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testECDeath()
	{
		QVERIFY(command({"EC/Death/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testECProof()
	{
		QVERIFY(command({"EC/Proof/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testECDevelopment()
	{
		QVERIFY(command({"EC/Development/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testECCustom()
	{
		QVERIFY(command({"EC/Custom/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testConversationUnit()
	{
		QVERIFY(command({"Conversation/Unit/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testConversationIntegration()
	{
		QVERIFY(command({"Conversation/Integration/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testConversationSystem()
	{
		QVERIFY(command({"Conversation/System/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testConversationDeath()
	{
		QVERIFY(command({"Conversation/Death/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testConversationProof()
	{
		QVERIFY(command({"Conversation/Proof/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testConversationDevelopment()
	{
		QVERIFY(command({"Conversation/Development/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testConversationCustom()
	{
		QVERIFY(command({"Conversation/Custom/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testMentalgamiUnit()
	{
		QVERIFY(command({"Mentalgam/Unit/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testMentalgamiIntegration()
	{
		QVERIFY(command({"Mentalgam/Integration/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testMentalgamiSystem()
	{
		QVERIFY(command({"Mentalgam/System/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testMentalgamiDeath()
	{
		QVERIFY(command({"Mentalgami/Death/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testMentalgamiProof()
	{
		QVERIFY(command({"Mentalgami/Proof/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testMentalgamiDevelopment()
	{
		QVERIFY(command({"Mentalgami/Development/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testMentalgamiCustom()
	{
		QVERIFY(command({"Mentalgami/Custom/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testDrOblozhkoUnit()
	{
		QVERIFY(command({"DrOblozhko/Unit/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testDrOblozhkoIntegration()
	{
		QVERIFY(command({"DrOblozhko/Integration/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testDrOblozhkoSystem()
	{
		QVERIFY(command({"DrOblozhko/System/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testDrOblozhkoDeath()
	{
		QVERIFY(command({"DrOblozhko/Death/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testDrOblozhkoProof()
	{
		QVERIFY(command({"DrOblozhko/Proof/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testDrOblozhkoDevelopment()
	{
		QVERIFY(command({"DrOblozhko/Development/*"}) == EXIT_SUCCESS);
	}

	void Ladybug::testDrOblozhkoCustom()
	{
		QVERIFY(command({"DrOblozhko/Custom/*"}) == EXIT_SUCCESS);
	}
} }

namespace LadybugAutoTest {
	QTEST_MAIN(Cadabra::Queen::Ladybug)
}
