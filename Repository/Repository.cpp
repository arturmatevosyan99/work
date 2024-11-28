#include "Repository.hpp"

#include "View/Widgetboard.hpp"
#include "View/Widget.hpp"

#include "View/WidgetLayout.hpp"
#include "View/WidgetView.hpp"

#include "Meta/ApplicationRender.hpp"
#include "Meta/JobSpecification.hpp"
#include "Meta/Tag.hpp"
#include "Meta/EssenceToken.hpp"
#include "Meta/SetupDependency.hpp"
#include "Meta/FolderIndex.hpp"

#include "Model/TagSet.hpp"
#include "Model/Realm.hpp"
#include "Model/Hierarchy.hpp"
#include "Model/Avatar.hpp"
#include "Model/ObjectPath.hpp"
#include "Model/AceEvent.hpp"

#include "Model/Action/HandleAction.hpp"
#include "Model/Action/SubjectAction.hpp"
#include "Model/Action/GroupAction.hpp"
#include "Model/Action/ObjectAction.hpp"

#include "Controller/Application.hpp"
#include "Controller/Application/BusyBoard.hpp"

#include "Model/AbstractDescriptor.hpp"

#include "Model/AbstractApplication.hpp"
#include "Model/Application/Session.hpp"
#include "Model/Application/Domain.hpp"
#include "Model/Application/Infrastructure.hpp"
#include "Model/Application/WebApplication.hpp"
#include "Model/Application/Directory.hpp"

#include "Data/Setup/Revision1Setup.hpp"
#include "Data/Setup/Revision2Setup.hpp"

#include "Model/AbstractProxy.hpp"
#include "Model/Proxy/ObjectProxy.hpp"
//#include "Model/Proxy/SubjectProxy.hpp"
//#include "Model/Proxy/GroupProxy.hpp"

#include "Model/Job/Vacuum.hpp"
#include "Model/Job/Oblivion.hpp"

#include "../Core/System/Native.hpp"

#include <QScopedPointer>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTemporaryFile>

namespace Cadabra { namespace Queen { namespace Repository {
	Extension::Extension()
	:
		Queen::Bone("repository")
	{

	}

	Extension::~Extension()
	{

	}

	bool Extension::registerMetaObject()
	{
		qmlRegisterUncreatableMetaObject(Repository::staticMetaObject, "Repository.Global", 0, 1, "Repository", "Access to enums & flags only");

		return true;
	}

	bool Extension::registerMetaType()
	{
		qRegisterMetaType<Repository::JobState>("Cadabra::Queen::Repository::JobState");
		qRegisterMetaType<Repository::EssenceIncidentFlags>("Cadabra::Queen::Repository::EssenceIncidentFlags");
		qRegisterMetaType<Repository::EssenceAlterFlags>("Cadabra::Queen::Repository::EssenceAlterFlags");
		qRegisterMetaType<Repository::Meta::EssenceAlterMap>("Cadabra::Queen::Repository::Meta::EssenceAlterMap");

		qRegisterMetaType<Repository::Meta::Essence::Id>("Cadabra::Queen::Repository::Meta::Essence::Id");
		qRegisterMetaType<Repository::Meta::SubjectId>("Cadabra::Queen::Repository::Meta::SubjectId");
		qRegisterMetaType<Repository::Meta::ObjectId>("Cadabra::Queen::Repository::Meta::ObjectId");
		qRegisterMetaType<Repository::Meta::Subject::Id>("Cadabra::Queen::Repository::Meta::Subject::Id");
		qRegisterMetaType<Repository::Meta::Object::Id>("Cadabra::Queen::Repository::Meta::Object::Id");
		qRegisterMetaType<Repository::Meta::Session::Id>("Cadabra::Queen::Repository::Meta::Session::Id");

		qRegisterMetaType<Repository::Meta::Essence::Handle>("Cadabra::Queen::Repository::Meta::Essence::Handle");
		qRegisterMetaType<Repository::Meta::Session::Handle>("Cadabra::Queen::Repository::Meta::Session::Handle");
		qRegisterMetaType<Repository::Meta::Object::Handle>("Cadabra::Queen::Repository::Meta::Object::Handle");

		qRegisterMetaType<Repository::Meta::Essence::Scheme>("Cadabra::Queen::Repository::Meta::Essence::Scheme");
		qRegisterMetaType<Repository::Meta::Object::ApplicationScheme>("Cadabra::Queen::Repository::Meta::Object::ApplicationScheme");
		qRegisterMetaType<Repository::Meta::Subject::AvatarScheme>("Cadabra::Queen::Repository::Meta::Subject::AvatarScheme");

		qRegisterMetaType<Repository::Meta::Essence::Handle>();

		qRegisterMetaType<Repository::Meta::Cache>();
		qRegisterMetaType<Repository::Meta::Object::Cache>();
		qRegisterMetaType<Repository::Meta::Session::Pool>();

		qRegisterMetaType<Repository::Model::TagSet>("Cadabra::Queen::Repository::Model::TagSet");
		qRegisterMetaType<Repository::Model::Session::AttachFact>("Cadabra::Queen::Repository::Model::Session::AttachFact");

		qRegisterMetaType<Repository::Model::Session::AuthorizationPhase>("Cadabra::Queen::Repository::Model::Session::AuthorizationPhase");
		qRegisterMetaType<Repository::Model::Session::AuthorizationReject>("Cadabra::Queen::Repository::Model::Session::AuthorizationReject");

		return true;
	}

	bool Extension::registerMetaTypeConverter()
	{
		QMetaType::registerConverter(&Repository::Model::TagSet::toString);

		return true;
	}

	bool Extension::registerMetaTypeStreamOperator()
	{
		qRegisterMetaTypeStreamOperators<Repository::Meta::Essence::Handle>("Cadabra::Queen::Repository::Meta::Essence::Handle");
		qRegisterMetaTypeStreamOperators<Repository::Meta::Cache>("Cadabra::Queen::Repository::Meta::Cache");
		qRegisterMetaTypeStreamOperators<Repository::Meta::Profile>("Cadabra::Queen::Repository::Meta::Profile");

		return true;
	}

	bool Extension::registerQMLType()
	{
//		qmlRegisterType<Repository::Model::Realm>("Core.View", 0, 1, "StatusBar");
		qmlRegisterType<Repository::View::Widget>("Repository.View", 0, 1, "Widget");
		qmlRegisterType<Repository::View::Widgetboard>("Repository.View", 0, 1, "Widgetboard");

		qmlRegisterType<Repository::View::WidgetView>("Repository.View", 0, 1, "WidgetView");
		qmlRegisterType<Repository::View::WidgetLayout>("Repository.View", 0, 1, "WidgetLayout");
		qmlRegisterUncreatableType<Repository::View::WidgetLayoutAttachment>("Repository.View", 0, 1, "WidgetLayoutAttachment", "WidgetLayoutAttachment is only available as an attached property");

//		qmlRegisterAnonymousType<Repository::Controller::Application>("Repository.Controller", 0);
		qmlRegisterType<Repository::Controller::BusyBoard>("Repository.Controller", 0, 1, "BusyBoard");

		qmlRegisterAnonymousType<Repository::Model::AbstractDescriptor>("Repository.Model", 0);
//		qmlRegisterAnonymousType<Repository::Model::AbstractApplication>("Repository.Model", 0);
		qmlRegisterType<Repository::Model::Realm>("Repository.Model", 0, 1, "Realm");
		qmlRegisterType<Repository::Model::Session>("Repository.Model", 0, 1, "Session");
		qmlRegisterType<Repository::Model::Avatar>("Repository.Model", 0, 1, "Avatar");
		qmlRegisterType<Repository::Model::Domain>("Repository.Model", 0, 1, "Domain");
		qmlRegisterType<Repository::Model::WebApplication>("Repository.Model", 0, 1, "WebApplication");
		qmlRegisterType<Repository::Model::Infrastructure>("Repository.Model", 0, 1, "Infrastructure");
		qmlRegisterType<Repository::Model::ObjectPath>("Repository.Model", 0, 1, "ObjectPath");
		qmlRegisterType<Repository::Model::Directory>("Repository.Model", 0, 1, "Directory");

		qmlRegisterAnonymousType<Repository::Model::Hierarchy>("Repository.Model", 0);
		qmlRegisterAnonymousType<Repository::Model::AbstractJob>("Repository.Model", 0);
		qmlRegisterAnonymousType<Repository::Model::AbstractProxy>("Repository.Model", 0);
		qmlRegisterAnonymousType<Repository::Model::ObjectProxy>("Repository.Model", 0);
//		qmlRegisterAnonymousType<Repository::Model::SubjectProxy>("Repository.Model", 0);
//		qmlRegisterAnonymousType<Repository::Model::GroupProxy>("Repository.Model", 0);
		qmlRegisterAnonymousType<Repository::Model::SubjectKey>("Repository.Model", 0);

		qmlRegisterAnonymousType<Repository::Meta::Tag>("Repository.Meta", 0);
		qmlRegisterType<Repository::Meta::EssenceToken>("Repository.Meta", 0, 1, "EssenceToken");
		qmlRegisterType<Repository::Meta::FolderIndex>("Repository.Meta", 0, 1, "FolderIndex");

		return true;
	}

	bool Extension::registerAceAction()
	{
		/// @note Ace Actions (Queen Events)
		Repository::Model::AceEvent::Action<Repository::Model::HandleAction>::RegisterMetaType("Cadabra::Queen::Repository::Model::Handle");
		Repository::Model::AceEvent::Action<Repository::Model::SubjectAction>::RegisterMetaType("Cadabra::Ace::Repository::Model::Subject");
		Repository::Model::AceEvent::Action<Repository::Model::GroupAction>::RegisterMetaType("Cadabra::Ace::Repository::Model::Group");
		Repository::Model::AceEvent::Action<Repository::Model::ObjectAction>::RegisterMetaType("Cadabra::Ace::Repository::Model::Object");

		return true;
	}

	bool Extension::registerAceScheme()
	{
		/// @note Data Setups
		qRegisterMetaType<Repository::Meta::SetupDependency>("Cadabra::Queen::Repository::Meta::SetupDependency");
		Core::Meta::SchemeFactory::Bind<Repository::Meta::SetupDependency>("Repository.Meta.SetupDependency");
		Core::Meta::Scheme<Repository::Meta::SetupDependency>::Validate<Repository::Data::Revision1Setup>(1, {"Repository", "Extension", {}});
		Core::Meta::Scheme<Repository::Meta::SetupDependency>::Validate<Repository::Data::Revision2Setup>(2, {"Repository", "Extension", {}});

		/// @note Application Renderers
		qRegisterMetaType<Repository::Meta::ApplicationRender>("Cadabra::Queen::Repository::Meta::ApplicationRender");
		Core::Meta::SchemeFactory::Bind<Repository::Meta::ApplicationRender>("Repository.Meta.ApplicationRender");
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Repository::Model::WebApplication>(20, {"Repository", "WebApplication", 0, 1, "WebApplication"});

		/// @note Job Specifications
		qRegisterMetaType<Repository::Meta::JobSpecification>("Cadabra::Queen::Repository::Meta::JobSpecification");
		Core::Meta::SchemeFactory::Bind<Repository::Meta::JobSpecification>("Repository.Meta.JobSpecification");
		Core::Meta::Scheme<Repository::Meta::JobSpecification>::Validate<Repository::Model::Vacuum>(1, {"Repository", "Vacuum"});
		Core::Meta::Scheme<Repository::Meta::JobSpecification>::Validate<Repository::Model::Oblivion>(2, {"Repository", "Oblivion"});

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

	void Extension::onAttach(Skeleton& skeleton)
	{
		Core::System::Preference preference(Model::Realm::PreferenceFilename(), "default");
		QString current_token(preference.value("device_token", "").toString());
		if (!current_token.isEmpty()) {
			Core::System::NotificationCenter::Instance()->setUp(current_token);
		} else {
			Core::System::NotificationCenter::Instance()->setUp();
		}

		QObject::connect
		(
			Core::System::NotificationCenter::Instance(),
			&Core::System::NotificationCenter::registered,

			this,
			&Extension::onTokenRegistered
		);
	}

	void Extension::onDetach(Skeleton& skeleton)
	{
		QObject::disconnect
		(
			Core::System::NotificationCenter::Instance(),
			&Core::System::NotificationCenter::registered,

			this,
			&Extension::onTokenRegistered
		);
	}

	void Extension::onPreDisclose(Cupboard& cupboard)
	{
		/// @brief Дампим конфигурационный файл
		QSharedPointer<QFile> preference_file(new QFile(Model::Realm::PreferenceFilename()));
		if (preference_file->open(QIODevice::ReadOnly)) {
			cupboard.file("preference" + Core::System::Preference::FileExtension, preference_file.data());
		}

		/// @brief Дампим Realm Lite
		const QStringList connection_list(QSqlDatabase::connectionNames());
		for (const QString& connection_name : connection_list) {
			if (!connection_name.contains("_")) {
				QSqlDatabase database_connection(QSqlDatabase::database(connection_name, true));
				if (!database_connection.isValid()) {
					qWarning() << "Failed to open `" << connection_name << "` connection";
				} else {
					QSqlQuery vacuum_query(database_connection);
					QSharedPointer<QTemporaryFile> dump_file(new QTemporaryFile("XXXXXX.realm"));
					dump_file->setAutoRemove(true);
					if (!dump_file->open()) {
						qWarning() << "Failed to create temp file for Realm dump at " << dump_file->fileName();
						continue;
					}
					dump_file->close();
					if (!vacuum_query.prepare("VACUUM INTO '" + dump_file->fileName().append("'")) || !vacuum_query.exec()) {
						qWarning() << "Failed vacuum Realm into" << dump_file->fileName() << "because of a error:" << vacuum_query.lastError();
					} else if (!dump_file->open()) {
						qWarning() << "Failed to reopen Realm dump at " << dump_file->fileName();
					} else {
						qWarning() << "Realm is dumped and bundled";
						cupboard.file(connection_name + ".sqlite3", dump_file.data());
					}
				}
			}
		}
	}

	void Extension::onPostDisclose(Cupboard& cupboard)
	{

	}

	void Extension::onTokenRegistered(QString token)
	{
		if (Core::System::NotificationCenter::Instance()->getToken() != token) {
			Core::System::Preference preference(Model::Realm::PreferenceFilename(), "default");
			preference.setValue
			(
				"device_token",
				Core::System::NotificationCenter::Instance()->getCNSToken()
			);
		}
	}
} } }
