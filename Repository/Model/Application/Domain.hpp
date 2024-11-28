#ifndef Cadabra_Queen_Repository_Model_Application_Domain_hpp
#define Cadabra_Queen_Repository_Model_Application_Domain_hpp

#include "../AbstractApplication.hpp"

#include "Session.hpp"
#include "Interface/DatasetInterface.hpp"
#include "Interface/HierarchyInterface.hpp"
#include "../Action/SubjectAction.hpp"
#include "../Action/GroupAction.hpp"
#include "../Action/ObjectAction.hpp"
#include "../Patch/GroupPatch.hpp"
#include "../Patch/SubjectPatch.hpp"
#include "../Patch/ObjectPatch.hpp"
#include "../../Data/Entity/Sync.hpp"

#include <QString>
#include <QTimerEvent>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Domain : public AbstractApplication, public HierarchyInterface, public DatasetInterface
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class AuthenticationState
		{
			Unknown,
			Progress,
			Active,
			Reactive,
			Deactive,
			Fault
		};

		enum class AssemblyMethod : int
		{
			Lazy = 0,
			Slip = 1,
			Dump = 2
		};

		union AssemblyFile {
			Net::ObjectFile* _net_file;
			Core::Data::Provider* _data_file;
		};
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_INTERFACES(Cadabra::Queen::Repository::Model::DatasetInterface)
		Q_ENUM(AuthenticationState)
		Q_ENUM(AssemblyMethod)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Realm* realm READ getRealm NOTIFY endpointChanged)
		Q_PROPERTY(QString hostEndpoint READ getHostEndpoint WRITE setHostEndpoint NOTIFY endpointChanged)
		Q_PROPERTY(bool sslEndpoint READ getSSLEndpoint WRITE setSSLEndpoint NOTIFY endpointChanged)
		Q_PROPERTY(AuthenticationState authenticationState READ getAuthenticationState NOTIFY authenticationStateChanged)
		Q_PROPERTY(AssemblyMethod assemblyMethod READ getAssemblyMethod NOTIFY authenticationStateChanged)
		Q_PROPERTY(bool syncState READ isSyncState NOTIFY syncStateChanged)
		Q_PROPERTY(bool liveMode READ isLiveMode NOTIFY authenticationStateChanged)
		Q_PROPERTY(bool autoProlongationMode READ isAutoProlongationMode WRITE setAutoProlongationMode NOTIFY autoProlongationModeChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Domain(QObject* parent = nullptr);
		virtual ~Domain() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		AuthenticationState _authentication_state;

		AssemblyMethod _assembly_method;
		AssemblyFile _assembly_file;

		bool _live_mode;
		bool _auto_prolongation_mode;
		int _auto_prolongation_timerid;
		Session* _session;

		quint16 _sync_error;
		QSharedPointer<Data::Sync> _sync_data;
		QMap<Meta::Essence::Target, qint16> _sync_map;

		GroupPatch _group_patch;
		SubjectPatch _subject_patch;
		ObjectPatch _object_patch;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual Q_INVOKABLE bool open(Cadabra::Queen::Repository::Model::AbstractDescriptor* descriptor) override;

	public:
		Q_INVOKABLE bool open(Cadabra::Queen::Repository::Model::Session* session, Cadabra::Queen::Repository::Model::Realm* realm);
		Q_INVOKABLE bool signIn(const QString& phonemail, const QString& password, Cadabra::Queen::Repository::Model::Domain::AssemblyMethod assembly_method = AssemblyMethod::Lazy, bool live_mode = false);
		Q_INVOKABLE bool signOut(bool force = false);

	private:
		void log(Meta::Essence::Target target);
		bool patch(Meta::Essence::Target target);
		bool patch();
		bool load();
		bool pull(Meta::Essence::Target target, Meta::Essence::Id offset_id);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Realm* getRealm() const;
		AuthenticationState getAuthenticationState() const;
		AssemblyMethod getAssemblyMethod() const;

		QString getHostEndpoint() const;
		bool getSSLEndpoint() const;

		const Domain::LogStamp& getLastLogStamp(Meta::Essence::Target target, bool force = false);
		AbstractPatch& getPatch(Meta::Essence::Target target);
	/** @} */

	/** @name Mutators */
	/** @{ */
	private:
		void setAuthenticationState(AuthenticationState state);
		bool setSyncState(Meta::Essence::Target target, bool inout);

	public:
		void setHostEndpoint(const QString& value);
		void setSSLEndpoint(bool onoff);
		void setAutoProlongationMode(bool onoff);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void endpointChanged();
		void authenticationStateChanged(Cadabra::Queen::Repository::Model::Domain::AuthenticationState state);
		void autoProlongationModeChanged();
		void syncStateChanged(Meta::Essence::Target target);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onSessionActivated(bool onoff);
		void onSessionAlived(bool onoff);
		void onSessionFaulted(Session::AuthorizationPhase phase, Session::AuthorizationReject reject, QString message);

		void onRealmRestored();

		void onRealmDumped(QNetworkReply::NetworkError error_code, QString error_message);
		void onRealmDumped(QString file_name);

		void onRealmFlushed(Meta::Essence::Target target, quint8 domain, quint32 code, QString message);
		void onRealmFlushed(Meta::Essence::Target target, QJsonValue payload);

		void onRealmPulled(Meta::Essence::Target target, quint8 domain, quint32 code, QString message);
		void onRealmPulled(Meta::Essence::Target target, QJsonValue payload);

		void onRealmLogged(Meta::Essence::Target target, quint8 domain, quint32 code, QString message);
		void onRealmLogged(Meta::Essence::Target target, QJsonValue payload);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void customEvent(QEvent* event) override;
		virtual void timerEvent(QTimerEvent* event) override;
		virtual bool onHandle() override;
		virtual bool onOpen() override;
		virtual bool onClose() override;
		virtual bool onSetup() override;
//		virtual bool onTeardown() override;

		void onAction(EssenceAction* action);
		void onAction(SubjectAction* action);
		void onAction(GroupAction* action);
		void onAction(ObjectAction* action);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isActive() const;
		bool isSyncState() const;
		bool isLiveMode() const;
		bool isAutoProlongationMode() const;
	/** @} */
	};
} } } }

#endif
