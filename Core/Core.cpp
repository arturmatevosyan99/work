#include "Core.hpp"

#include "Meta/Qd.hpp"
#include "Meta/Filter.hpp"
#include "Meta/Order.hpp"
#include "Meta/DynamicRole.hpp"
#include "Meta/ObjectList.hpp"
#include "Meta/MapModel.hpp"
#include "Meta/Duration.hpp"
#include "Meta/Scheme.hpp"

#include "Model/MediaInfo.hpp"
#include "Model/MenuCallback.hpp"
#include "Model/SortFilterProxyModel.hpp"

#include "Net/Endpoint.hpp"
#include "Net/Call/CRPPromise.hpp"

#include "Data/Event.hpp"
#include "Data/Command/ProviderConnect.hpp"
#include "Data/Command/ProviderDisconnect.hpp"
#include "Data/Command/ProviderClone.hpp"
#include "Data/Command/EntityOpen.hpp"
#include "Data/Command/EntityReopen.hpp"
#include "Data/Command/EntityClose.hpp"
#include "Data/Command/EntityFetch.hpp"
#include "Data/Command/EntityMutate.hpp"
#include "Data/AbstractEntity.hpp"
#include "Data/Rowset.hpp"

#include "Core/Test/AbstractCase.hpp"
#include "Core/Test/AbstractShocker.hpp"
#include "Core/Test/Token.hpp"
#include "Core/Test/Context.hpp"
#include "Core/Test/PetrI.hpp"
#include "Core/Test/Assert.hpp"
#include "Core/Test/Node.hpp"
#include "Core/Test/Node/Port.hpp"
#include "Core/Test/Node/Shock.hpp"
#include "Core/Test/Node/Action.hpp"
#include "Core/Test/Node/Assertion.hpp"
#include "Core/Test/Node/Waiter/WaiterCallback.hpp"
#include "Core/Test/Node/Waiter/TimeWaiter.hpp"
#include "Core/Test/Node/Waiter/GroupWaiter.hpp"
#include "Core/Test/Node/Waiter/SignalWaiter.hpp"
#include "Core/Test/Node/Waiter/ObjectWaiter.hpp"

#include "System/Preference.hpp"
#include "System/AudioRole.hpp"
#include "System/AudioRecorder.hpp"
#include "System/ImageThumbnail.hpp"
#include "System/NotificationCenter.hpp"
#include "System/NotificationService.hpp"
#include "System/FitnessTracker.hpp"
#include "System/Native.hpp"
#include "System/UserTracker.hpp"
#include "System/DiagnosticBundle.hpp"
#include "System/Alert.hpp"
#include "System/Alert/StateAlert.hpp"
#include "System/Alert/SoundAlert.hpp"
#include "System/Alert/InputAlert.hpp"
#include "System/Alert/ActionAlert.hpp"

#include "View/Theme.hpp"
#include "View/StatusBar.hpp"
#include "View/StateIsland.hpp"
#include "View/WindowManager.hpp"
#include "View/ProgressCircle.hpp"
#include "View/AlertDialog.hpp"

#include "Meta/Font.hpp"

#include "../Queen.hpp"

#include <QSet>
#include <QMap>
#include <QLocale>
#include <QDateTime>
#include <QQuickStyle>
#include <QQuickWindow>

#if defined (Q_OS_ANDROID)
#include <QtAndroid>
#endif

namespace Cadabra { namespace Queen { namespace Core {
	Extension::Extension()
	:
		Queen::Bone("core")
	{

	}

	Extension::~Extension()
	{

	}

	bool Extension::registerMetaObject()
	{
		qmlRegisterUncreatableMetaObject(Core::staticMetaObject, "Core.Global", 0, 1, "Core", "Access to enums & flags only");

		return true;
	}

	bool Extension::registerMetaType()
	{
		qRegisterMetaType<QSet<int>>();
		qRegisterMetaType<QMap<int, int>>();
		qRegisterMetaType<Core::ApplicationState>("Cadabra::Queen::Core::ApplicationState");
		qRegisterMetaType<Core::ApplicationMessage>("Cadabra::Queen::Core::ApplicationMessage");
		qRegisterMetaType<Core::Meta::Qd>("Cadabra::Queen::Core::Meta::Qd");
		qRegisterMetaType<Core::Net::CRPPromise>("Cadabra::Queen::Core::Net::CRPPromise");
		qRegisterMetaType<QSharedPointer<Core::Data::AbstractEntity>>("Cadabra::Queen::Core::Data::AbstractEntitySharedPointer");
		qRegisterMetaType<QSharedPointer<Core::Data::Rowset>>("Cadabra::Queen::Core::Data::RowsetSharedPointer");

		qRegisterMetaType<Core::Meta::Font>("Cadabra::Queen::Core::Meta::Font");

		qRegisterMetaType<Core::TestVariation>("Cadabra::Queen::Core::TestVariation");
		qRegisterMetaType<Core::TestDialect>("Cadabra::Queen::Core::TestDialect");
		qRegisterMetaType<Core::TestModule>("Cadabra::Queen::Core::TestModule");

		return true;
	}

	bool Extension::registerMetaTypeConverter()
	{
		QMetaType::registerConverter(&Core::Meta::Qd::toString);
		QMetaType::registerConverter(&Core::Meta::Qd::toInt64);
		QMetaType::registerConverter<QVariant, Core::Meta::Qd>(Core::Meta::Qd::Make);
		QMetaType::registerComparators<Core::Meta::Qd>();
		QMetaType::registerConverter(&Core::Meta::Duration::toString);
		QMetaType::registerComparators<Core::Meta::Duration>();

		QMetaType::registerConverter(&Core::Meta::Font::toFont);

		return true;
	}

	bool Extension::registerMetaTypeStreamOperator()
	{
		qRegisterMetaTypeStreamOperators<Core::Meta::Qd>("Cadabra::Queen::Core::Meta::Qd");

		return true;
	}

	bool Extension::registerQMLType()
	{
		qmlRegisterType<Core::Net::Endpoint>("Core.Net", 0, 1, "Endpoint");
		qmlRegisterAnonymousType<Core::Data::AbstractEntity>("Core.Data", 0);
		qmlRegisterAnonymousType<Core::Test::AbstractWaiter>("Core.Test", 0);
		qmlRegisterAnonymousType<Core::Test::AbstractCase>("Core.Test", 0);

		qmlRegisterType<Core::Meta::Filter>("Core.Meta", 0, 1, "Filter");
		qmlRegisterType<Core::Meta::Order>("Core.Meta", 0, 1, "Order");
		qmlRegisterType<Core::Meta::DynamicRole>("Core.Meta", 0, 1, "DynamicRole");
		qmlRegisterType<Core::Meta::SchemeModel>("Core.Meta", 0, 1, "SchemeModel");
		qmlRegisterType<Core::Meta::ObjectList>("Core.Meta", 0, 1, "ObjectList");
		qmlRegisterType<Core::Meta::MapModel>("Repository.Model", 0, 1, "MapModel");

		qmlRegisterType<Core::Test::PetrI>("Core.Test", 0, 1, "PetrI");
		qmlRegisterType<Core::Test::Assertion>("Core.Test", 0, 1, "Assertion");
		qmlRegisterType<Core::Test::Action>("Core.Test", 0, 1, "Action");
		qmlRegisterType<Core::Test::Shock>("Core.Test", 0, 1, "Shock");
		qmlRegisterType<Core::Test::Port>("Core.Test", 0, 1, "Port");
		qmlRegisterType<Core::Test::WaiterCallback>("Core.Test", 0, 1, "WaiterCallback");
		qmlRegisterType<Core::Test::TimeWaiter>("Core.Test", 0, 1, "TimeWaiter");
		qmlRegisterType<Core::Test::GroupWaiter>("Core.Test", 0, 1, "GroupWaiter");
		qmlRegisterType<Core::Test::SignalWaiter>("Core.Test", 0, 1, "SignalWaiter");
		qmlRegisterType<Core::Test::ObjectWaiter>("Core.Test", 0, 1, "ObjectWaiter");
		qmlRegisterType<Core::Test::Token>("Core.Test", 0, 1, "Token");
		qmlRegisterType<Core::Test::Context>("Core.Test", 0, 1, "Context");
		qmlRegisterType<Core::Test::Assert>("Core.Test", 0, 1, "Assert");
		qmlRegisterAnonymousType<Core::Test::Node>("Core.Test", 0);
		qmlRegisterAnonymousType<Core::Test::AbstractShocker>("Core.Test", 0);

		qmlRegisterType<Core::System::DiagnosticBundle>("Core.System", 0, 1, "DiagnosticBundle");

		qmlRegisterAnonymousType<Core::System::NotificationService>("Core.System", 0);

		qmlRegisterType<Core::System::AudioRecorder>("Core.System", 0, 1, "AudioRecorder");
		qmlRegisterType<Core::System::ImageThumbnail>("Core.System", 0, 1, "ImageThumbnail");

		qmlRegisterAnonymousType<Core::System::AbstractAlert>("Core.System", 0);
		qmlRegisterType<Core::System::DialogAlert>("Core.System", 0, 1, "DialogAlert");
		qmlRegisterType<Core::System::SoundAlert>("Core.System", 0, 1, "SoundAlert");
		qmlRegisterType<Core::System::InputAlert>("Core.System", 0, 1, "InputAlert");
		qmlRegisterType<Core::System::ActionAlert>("Core.System", 0, 1, "ActionAlert");
		qmlRegisterType<Core::System::AlertOption>("Core.System", 0, 1, "AlertOption");
		qmlRegisterType<Core::View::StateIsland>("Core.View", 0, 1, "StateIsland");
		qmlRegisterType<Core::View::AlertDialog>("Core.View", 0, 1, "AlertDialog");
		qmlRegisterType<Core::View::AlertState>("Core.View", 0, 1, "AlertState");
		qmlRegisterType<Core::View::AlertAction>("Core.View", 0, 1, "AlertAction");
		qmlRegisterType<Core::View::ProgressCircle>("Core.View", 0, 1, "ProgressCircle");

		qmlRegisterAnonymousType<Core::Meta::Font>("Core.Meta", 0);

		qmlRegisterType<Core::Model::SortFilterProxyModel>("Core.Model", 0, 1, "SortFilterProxyModel");
		qmlRegisterType<Core::Model::MediaInfo>("Core.Model", 0, 1, "MediaInfo");
		qmlRegisterType<Core::Model::MenuCallback>("Core.Model", 0, 1, "MenuCallback");

		return true;
	}

	bool Extension::registerAceAction()
	{
		Core::Data::Event::Action<Core::Data::ProviderConnect>::RegisterMetaType("Cadabra::Queen::Core::Data::ProviderConnect");
		Core::Data::Event::Action<Core::Data::ProviderDisconnect>::RegisterMetaType("Cadabra::Queen::Core::Data::ProviderDisconnect");
		Core::Data::Event::Action<Core::Data::ProviderClone>::RegisterMetaType("Cadabra::Queen::Core::Data::ProviderClone");
		Core::Data::Event::Action<Core::Data::EntityOpen>::RegisterMetaType("Cadabra::Queen::Core::Data::EntityOpen");
		Core::Data::Event::Action<Core::Data::EntityReopen>::RegisterMetaType("Cadabra::Queen::Core::Data::EntityReopen");
		Core::Data::Event::Action<Core::Data::EntityClose>::RegisterMetaType("Cadabra::Queen::Core::Data::EntityClose");
		Core::Data::Event::Action<Core::Data::EntityFetch>::RegisterMetaType("Cadabra::Queen::Core::Data::EntityFetch");
		Core::Data::Event::Action<Core::Data::EntityMutate>::RegisterMetaType("Cadabra::Queen::Core::Data::EntityMutate");

		return true;
	}

	bool Extension::registerAceScheme()
	{
		return true;
	}

	bool Extension::registerQMLSingletonType()
	{
		qmlRegisterSingletonType<Core::View::Theme>("Core.View", 0, 1, "Theme", Core::View::Theme::QInstance);
		qmlRegisterSingletonType<Core::View::StatusBar>("Core.View", 0, 1, "StatusBar", Core::View::StatusBar::QInstance);
		qmlRegisterSingletonType<Core::View::WindowManager>("Core.View", 0, 1, "WindowManager", Core::View::WindowManager::QInstance);

		qmlRegisterSingletonType<Core::System::Native>("Core.System", 0, 1, "Native", Core::System::Native::QInstance);
		qmlRegisterSingletonType<Core::System::Alert>("Core.System", 0, 1, "Alert", Core::System::Alert::QInstance);
		qmlRegisterSingletonType<Core::System::UserTracker>("Core.System", 0, 1, "UserTracker", Core::System::UserTracker::QInstance);
		qmlRegisterSingletonType<Core::System::AudioRole>("Core.System", 0, 1, "AudioRole", Core::System::AudioRole::QInstance);
		qmlRegisterSingletonType<Core::System::NotificationCenter>("Core.System", 0, 1, "NotificationCenter", Core::System::NotificationCenter::QInstance);
		qmlRegisterSingletonType<Core::System::FitnessTracker>("Core.System", 0, 1, "FitnessTracker", Core::System::FitnessTracker::QInstance);

		qmlRegisterSingletonType<Core::Meta::SchemeFactory>("Core.Meta", 0, 1, "SchemeFactory", Core::Meta::SchemeFactory::QInstance);

		return true;
	}

	bool Extension::tweakApplicationEngine(QQmlApplicationEngine& engine)
	{
		return true;
	}

	bool Extension::tweakRootContext(QQmlContext& context)
	{
		return true;
	}

	bool Extension::tweakRootObject(QObject& object)
	{
		QObject* root_object(&object);
		if (!root_object) {
			return false;
		}
		QQuickWindow* quick_window(qobject_cast<QQuickWindow*>(root_object));
		if (!quick_window) {
			return false;
		}
		return true;
	}

	Cadabra::Queen::Core::View::StatusBar* Extension::getStatusBar() const
	{
		return View::StatusBar::Instance();
	}

	Cadabra::Queen::Core::View::Theme* Extension::getTheme() const
	{
		return View::Theme::Instance();
	}

	void Extension::onAttach(Skeleton& skeleton)
	{
		Core::Meta::SchemeFactory::Close();
	}

	void Extension::onPreDisclose(Cupboard& cupboard)
	{
		System::Native* native(System::Native::Instance());
		QStringList version("BundleRevision=Q1");
		version.append("DateTime=" + QLocale().toString(QDateTime::currentDateTime(), QLocale::LongFormat));
		version.append("PlatformName=" + native->getPlatformName());
		version.append("HostName=" + native->getHostName());
		version.append("SkeletonName=" + native->getSkeletonName());
		version.append("FrameworkVersion=" + native->getFrameworkVersion());
		version.append("BundleIdentifier=" + native->getBundleIdentifier());
		version.append("MachineId=" + native->getMachineIdentifier());
		version.append("NativeId=" + native->getNativeIdentifier());
		version.append("BootId=" + native->getBootIdentifier());
		cupboard.file("version.txt", version.join("\n").toLatin1());
		QSharedPointer<QTemporaryFile> log_file(new QTemporaryFile(QDir::tempPath().append("/Queen.XXXXXX.log")));
		log_file->setAutoRemove(true);
		if (log_file->open()) {
			Skeleton::ActiveInstance()->rotateLog(*log_file.get());
			if (log_file->isOpen()) {
				cupboard.file("log.txt", log_file.data());
			}
		}
	}

	void Extension::onPostDisclose(Cupboard& cupboard)
	{

	}

	void Extension::onDetach(Skeleton& skeleton)
	{
	}
} } }
