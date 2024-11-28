#include "Stress.hpp"

#include "../Skeleton.hpp"

#include "System/TestThread.hpp"

#include "Test/Shocker/MemoryShocker.hpp"
#include "Test/Shocker/AceShocker.hpp"
#include "Test/Shocker/NetShocker.hpp"

#include "../Core/Meta/Scheme/TestScheme.hpp"
#include "../Core/Test/Case/GeneralCase.hpp"
#include "../Core/Test/Case/PetriCase.hpp"
#include "../Core/Meta/Order.hpp"
#include "../Core/Test/PetrI.hpp"
#include "../Core/Test/Token.hpp"
#include "../Core/Test/Assert.hpp"
#include "../Core/Test/Node/Action.hpp"
#include "../Core/Test/Node/Assertion.hpp"
#include "../Core/Test/Case/GeneralCase.hpp"
#include "../Core/Test/Case/PetriCase.hpp"

#include "Test/Unit/CAP-653.hpp"

#include "../Core/System/Native.hpp"

#include <QFile>
#include <QVector>
#include <QtConcurrent>

namespace Cadabra { namespace Queen { namespace Stress {
	const QMap<QString, Core::TestVariation> Extension::VariationMap = {
		{"Unit", Core::TestVariation::Unit}, /// < 🔬
		{"Integration", Core::TestVariation::Integration}, /// < 🔌
		{"System", Core::TestVariation::System},/// < 🚲
		{"Death", Core::TestVariation::Death}, /// < 💀
		{"Proof", Core::TestVariation::Proof}, /// < 👮
		{"Development", Core::TestVariation::Development}, /// < 🛠
		{"Custom", Core::TestVariation::Custom}/// < 🚲
	};

	QByteArray Extension::DebuggerOption()
	{
		QFile debugger_file(Body::BaseDirectory.absoluteFilePath("systems.cadabra.queen.strees.qmljsdebugger"));
		if (!debugger_file.open(QFile::ReadOnly)) {
			return "";
		}
		/// @note -qmljsdebugger=port:<port_from>[,port_to][,host:<ip address>][,block][,file:<local socket>][,services:<comma-separated list of services to enable>]
		QByteArray retval(debugger_file.readLine());
		if (retval.isEmpty()) {
			retval = "port:12150";
		}
		return retval;
	}

	Extension::Extension()
	:
		Queen::Bone("stress")
	{

	}

	Extension::~Extension()
	{

	}

	bool Extension::registerMetaObject()
	{
		qmlRegisterUncreatableMetaObject(Stress::staticMetaObject, "Stress.Global", 0, 1, "Stress", "Access to enums & flags only");

		return true;
	}

	bool Extension::registerMetaType()
	{
		return true;
	}

	bool Extension::registerQMLType()
	{
		qmlRegisterType<Stress::System::TestThread>("Stress.System", 0, 1, "TestThread");

		qmlRegisterType<Stress::Test::MemoryShocker>("Stress.Test", 0, 1, "MemoryShocker");
		qmlRegisterType<Stress::Test::AceShocker>("Stress.Test", 0, 1, "AceShocker");
		qmlRegisterType<Stress::Test::NetShocker>("Stress.Test", 0, 1, "NetShocker");

		qmlRegisterAnonymousType<Stress::Model::CheckList>("Stress.Model", 0);

		return true;
	}

	bool Extension::registerMetaTypeStreamOperator()
	{
		qRegisterMetaTypeStreamOperators<QSet<QUrl>>("QSet<QUrl>");

		return true;
	}

	bool Extension::registerTestCase()
	{
		Core::Meta::Scheme<Core::Meta::TestScheme>::Salt salt(0);
/*
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "PetriExample", Core::TestVariation::Integration, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "PetriConditionExample", Core::TestVariation::Integration, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "PetriPortExample", Core::TestVariation::Integration, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "PetriForPort1", Core::TestVariation::Integration, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "PetriForPort2", Core::TestVariation::Integration, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "PetriForPort3", Core::TestVariation::Integration, Core::TestDialect::QML});
*/
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "0", Core::TestVariation::Death, Core::TestDialect::QML});

		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Stress::Test::CAP653>(salt++, {"Stress", "CAP-653", Core::TestVariation::Unit});

		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::GeneralCase>(salt++, {"Stress", "DeviceTokenGet", Core::TestVariation::Development, Core::TestDialect::QML, Core::TestFactory::General});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::GeneralCase>(salt++, {"Stress", "Emojis.qml", Core::TestVariation::Development, Core::TestDialect::QML, Core::TestFactory::General});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::GeneralCase>(salt++, {"Stress", "WebView", Core::TestVariation::Development, Core::TestDialect::QML, Core::TestFactory::General});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::GeneralCase>(salt++, {"Stress", "FontTheme", Core::TestVariation::Development, Core::TestDialect::QML, Core::TestFactory::General});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::GeneralCase>(salt++, {"Stress", "DisplayCutout", Core::TestVariation::Development, Core::TestDialect::QML, Core::TestFactory::General});

		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::GeneralCase>(salt++, {"Stress", "CAP-652", Core::TestVariation::Integration, Core::TestDialect::QML, Core::TestFactory::General});

//		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "1", Core::TestVariation::System, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "CAP-622", Core::TestVariation::System, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "CAP-50", Core::TestVariation::System, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "CAP-643", Core::TestVariation::System, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "CAP-636", Core::TestVariation::System, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "CAP-639", Core::TestVariation::System, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "CAP-664", Core::TestVariation::System, Core::TestDialect::QML});

		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::PetriCase>(salt++, {"Stress", "CAP-449", Core::TestVariation::Proof, Core::TestDialect::QML});
		Core::Meta::Scheme<Core::Meta::TestScheme>::Validate<Core::Test::GeneralCase>(salt++, {"Stress", "CAP-654", Core::TestVariation::Proof, Core::TestDialect::QML, Core::TestFactory::General});

		return true;
	}

	void Extension::run()
	{
		System::TestThread* test_thread(new System::TestThread(this));

		QObject::connect(test_thread, &System::TestThread::finished, test_thread, &QObject::deleteLater); /// < чтобы не потерять память
		QObject::connect(test_thread, &System::TestThread::started, this, &Stress::Extension::onTestStarted); /// < для того, чтобы ловить в QML и дизаблить баттон
		QObject::connect(test_thread, &System::TestThread::finished, this, &Stress::Extension::onTestFinished); /// < для того, чтобы ловить в QML и инаблить баттон
		QObject::connect(test_thread, &System::TestThread::ran, this, &Stress::Extension::onTestRan); /// < для того, чтобы поставить busy
		QObject::connect(test_thread, &System::TestThread::completed, this, &Stress::Extension::onTestCompleted); /// < для того, чтобы поставить fail | pass

		test_thread->start(&_check_list, false);
	}

	bool Extension::run(Core::Meta::Qd& scheme_id)
	{
		const QSharedPointer<Core::Test::AbstractCase> test(Core::Meta::Scheme<Core::Meta::TestScheme>::Produce(scheme_id));
		return test ? test->run() : false;
	}

	bool Extension::attach(QUrl url, bool persistent, bool onoff)
	{
		if (!onoff) {
			if (_check_list.remove(url) && persistent) {
				QSet<QUrl> repository_set;
				repository_set = Skeleton::ActiveInstance()->getPreferenceOption("repository_set", QVariant::fromValue(repository_set)).value<QSet<QUrl>>();
				if (repository_set.remove(url) > 0) {
					Skeleton::ActiveInstance()->setPreferenceOption("repository_set", QVariant::fromValue(repository_set));
				}
				return true;
			}
			return false;
		} else if (!_check_list.append(url)) {
			return false;
		} else if (persistent) {
			QSet<QUrl> repository_set;
			repository_set = Skeleton::ActiveInstance()->getPreferenceOption("repository_set", QVariant::fromValue(repository_set)).value<QSet<QUrl>>();
			repository_set.insert(_check_list.getLast()->getFactoryAlias());
			Skeleton::ActiveInstance()->setPreferenceOption("repository_set", QVariant::fromValue(repository_set));
		}
		return true;
	}

	bool Extension::replicate(QUrl url, bool persistent, bool onoff)
	{
		if (!onoff) {
			if (_log_list.remove(url) && persistent) {
				QSet<QUrl> logclient_set;
				logclient_set = Skeleton::ActiveInstance()->getPreferenceOption("logclient_set", QVariant::fromValue(logclient_set)).value<QSet<QUrl>>();
				if (logclient_set.remove(url) > 0) {
					Skeleton::ActiveInstance()->setPreferenceOption("logclient_set", QVariant::fromValue(logclient_set));
				}
				return true;
			}
			return false;
		} else if (!_log_list.append(url)) {
			return false;
		} else if (persistent) {
			QSet<QUrl> logclient_set;
			logclient_set = Skeleton::ActiveInstance()->getPreferenceOption("logclient_set", QVariant::fromValue(logclient_set)).value<QSet<QUrl>>();
			logclient_set.insert(url);
			Skeleton::ActiveInstance()->setPreferenceOption("logclient_set", QVariant::fromValue(logclient_set));
		}
		return true;
	}

	bool Extension::debug(const QString& argument)
	{
		QFile file(Body::BaseDirectory.absoluteFilePath("systems.cadabra.queen.strees.qmljsdebugger"));
		if (argument.isEmpty()) {
			if (file.exists()) {
				file.remove();
			}
			return true;
		} else if (!file.open(QFile::WriteOnly)) {
			return false;
		}
		return file.write(argument.toLatin1());
	}

	bool Extension::vary(const QString& key, const QVariant& value)
	{
		if (key.isEmpty()) {
			return false;
		} else if (!Skeleton::ActiveInstance()->setEnvironmentVariable(key, value)) {
			return false;
		}
		QVariantMap environment_map;
		environment_map = Skeleton::ActiveInstance()->getPreferenceOption("environment_map", environment_map).value<QVariantMap>();
		if (value.isValid()) {
			environment_map[key] = value;
		} else {
			environment_map.remove(key);
		}
		return Skeleton::ActiveInstance()->setPreferenceOption("environment_map", environment_map);
	}

	Model::CheckList* Extension::getCheckList()
	{
		return &_check_list;
	}

	Model::LogList* Extension::getLogList()
	{
		return &_log_list;
	}

	QString Extension::getDebuggerOption()
	{
		return DebuggerOption();
	}

	void Extension::onAttach(Skeleton& skeleton)
	{

	}

	void Extension::onDetach(Skeleton& skeleton)
	{

	}

	void Extension::onTestStarted()
	{

	}

	void Extension::onTestFinished()
	{

	}

	void Extension::onTestRan(QModelIndex index)
	{
		Q_UNUSED(index)
	}

	void Extension::onTestCompleted()
	{

	}

	void Extension::onTestSkipped()
	{

	}
} } }

QDataStream& operator<<(QDataStream& out, const QSet<QUrl>& set)
{
	out << set.count();
	for (const QUrl& url : set) {
		out << url.toString();
	}
	return out;
}

QDataStream& operator>>(QDataStream& in, QSet<QUrl>& set)
{
	int count(0);
	in >> count;
	while (count-- > 0) {
		QString url;
		in >> url;
		set.insert(url);
	}
	return in;
}
