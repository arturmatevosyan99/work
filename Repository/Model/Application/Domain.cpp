#include "Domain.hpp"

#include "../AbstractPatch.hpp"
#include "../Descriptor/SessionDescriptor.hpp"
#include "../Descriptor/StatefullDescriptor.hpp"

#include "../../Data/Entity/Extension.hpp"
#include "../../Data/Entity/Record/Group.hpp"
#include "../../Data/Entity/Record/Subject.hpp"
#include "../../Data/Entity/Record/Object.hpp"
#include "../../Data/Setup/Revision1Setup.hpp"

#include "../../../Core/Data/Predicate/ConditionPredicate.hpp"

#include "../../../Qaos/MakeUnique.hpp"
#include <QMap>
#include <QJsonValue>
#include <QJsonArray>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Domain::Domain(QObject* parent)
	:
		Repository::Model::AbstractApplication(parent),

		_authentication_state(Domain::AuthenticationState::Unknown),
		_assembly_method(AssemblyMethod::Lazy),
		_assembly_file({nullptr}),
		_live_mode(false),
		_auto_prolongation_mode(false),
		_auto_prolongation_timerid(0),
		_session(nullptr),
		_sync_error(0)
	{
	}

	Domain::~Domain()
	{
	}

	bool Domain::open(Cadabra::Queen::Repository::Model::AbstractDescriptor* descriptor)
	{
		return AbstractApplication::open(descriptor);
	}

	bool Domain::open(Cadabra::Queen::Repository::Model::Session* session, Cadabra::Queen::Repository::Model::Realm* realm)
	{
		if (!session || !realm || _session) {
			return false;
		}

		_session = session;
		QObject::connect(_session, &Session::activated, this, &Domain::onSessionActivated);
		QObject::connect(_session, &Session::declined, this, std::bind(&Domain::onSessionActivated, this, false));
		QObject::connect(_session, &Session::faulted, this, &Domain::onSessionFaulted);
		QObject::connect(_session, &Session::upstreamed, this, &Domain::onSessionAlived);
		QObject::connect(realm, &Realm::restored, this, &Domain::onRealmRestored);
		if (_session->prolongate(realm)) {
			_assembly_method = static_cast<AssemblyMethod>(_session->getRealm()->getPreference()->getOption("realm_assembly_method", static_cast<int>(_assembly_method)).toInt());
			_live_mode = _session->getRealm()->getPreference()->getOption("realm_live_mode", _live_mode).toBool();
			setAuthenticationState(AuthenticationState::Reactive);
		}
		emit endpointChanged();
		return true;
	}

	bool Domain::signIn(const QString& phonemail, const QString& password, Cadabra::Queen::Repository::Model::Domain::AssemblyMethod assembly_method, bool live_mode)
	{
		if (!_session || _session->isActive()) {
			return false;
		}
		_assembly_method = assembly_method;
		_live_mode = live_mode;
		setAuthenticationState(AuthenticationState::Progress);
		if (phonemail.contains('@')) {
			_session->signInEmail(phonemail, password);
		} else {
			_session->signInPhone(phonemail, password);
		}
		return true;
	}

	bool Domain::signOut(bool force)
	{
		bool retval(false);
		if (!_session || !_session->isActive()) {
			retval = false;
		} else if (AuthenticationState::Active == _authentication_state) {
			retval = _session->hasDescriptor() ? (_session->signOut(), true) : false;
		} else if (AuthenticationState::Reactive == _authentication_state) {
			retval = _session->invalidate();
		}

		if (retval) {
			/// @note Чтобы перейти в Unknown от Deactive, а не Active
			setAuthenticationState(AuthenticationState::Deactive);
		} else if (force) {
			_assembly_method = AssemblyMethod::Lazy;
			_live_mode = false;
			setAutoProlongationMode(false);
			setAuthenticationState(AuthenticationState::Unknown);
			return true;
		}
		return retval;
	}

	void Domain::log(Meta::Essence::Target target)
	{
		if (!hasDescriptor()) {
			return ;
		}
		/// @todo Use one from HierarchyInteface
		/// @todo check witch active logs
		const QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect(
						call.get(),
						qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),
						std::bind
						(
							(void(Domain::*)(Meta::Essence::Target, QJsonValue))&Domain::onRealmLogged,
							this,
							target, std::placeholders::_1
						)
		);
		QObject::connect(
						call.get(),
						qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),
						std::bind
						(
							(void(Domain::*)(Meta::Essence::Target, quint8, quint32, QString))&Domain::onRealmLogged,
							this,
							target, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
						)
		);
		const LogStamp log_stamp(getLastLogStamp(target));
		call->act(
					"Cadabra::Ace::Repository::Model::HierarchyInterface",
					"Cadabra::Ace::Repository::Model::RealmLog",
					{
						{"target", static_cast<char>(target)},
						{"acestamp", log_stamp.first},
						{"timestamp", log_stamp.second},
						{"limit", QString::number(Domain::LogLimit)},
					}
		);
		setSyncState(target, true);
	}

	bool Domain::patch()
	{
		if (!_group_patch.hasMap() || !_subject_patch.hasMap() || !_object_patch.hasMap()) {
			return false;
		}

		return (
				1
				&
				patch(Meta::Essence::Target::Group)
				&
				patch(Meta::Essence::Target::Subject)
				&
				patch(Meta::Essence::Target::Object)
		);
	}

	bool Domain::patch(Meta::Essence::Target target)
	{
		AbstractPatch& target_patch(getPatch(target));
		if (!target_patch.hasStash()) {
			return true;
		}
		qDebug() << "Applying patch(" << static_cast<char>(target) << ") with log from" << target_patch.getLogStampRange().first << "up to" << target_patch.getLogStampRange().second;

		const bool retval(target_patch.apply());
		QStringList error_list(target_patch.getErrorList());

		if (!retval) { /// < Не удалось применить вообще ничего
			error_list << tr("Hierarchy has not been updated due to unexpected error");
		} else {
			const QSharedPointer<Data::Sync> sync_data(new Data::Sync());
			sync_data->target.setValue(static_cast<int>(target));
			if (target_patch.hasStash()) { /// < Удалось применить не все - берем первую отметку
				sync_data->acestamp.setValue(target_patch.getLogStampRange().first.first);
				sync_data->timestamp.setValue(target_patch.getLogStampRange().first.second);
				error_list << tr("Hierarchy has been just partially synchronized");
			} else { /// < Применены все изменения
				sync_data->acestamp.setValue(target_patch.getLogStampRange().second.first);
				sync_data->timestamp.setValue(target_patch.getLogStampRange().second.second);
			}

			if (getDescriptor()->getRealm()->getDataProvider()->makeFactory().insert(sync_data.get())) {
				getDescriptor()->getRealm()->getHierarchy()->getLastEssenceId(target, true);
			} else {
				error_list << tr("Could not move forward the log checkpoint, but hierarchy has been succesfully synced upto date");
			}
		}

		target_patch.clear();
		getLastLogStamp(target, true);

		if (!error_list.isEmpty()) {
			emit exceptionThrew(MessageLevel::CriticalMessage, tr("Patch error, realm is probably broken and unusable"));
		}

		return retval;
	}

	bool Domain::load()
	{
		if (!hasDescriptor() || _assembly_file._data_file || _assembly_file._net_file) {
			return false;
		} else if (Domain::AssemblyMethod::Dump == _assembly_method) {
			_assembly_file._net_file = getDescriptor()->getRealm()->getHierarchy()->loadObjectFile(1, "realm-dump.sqlite3", Hierarchy::LoadMode::TimeLess).get();
			QObject::connect(_assembly_file._net_file, &Net::ObjectFile::faulted, this, qOverload<QNetworkReply::NetworkError, QString>(&Domain::onRealmDumped));
			QObject::connect(_assembly_file._net_file, &Net::ObjectFile::saved, this, qOverload<QString>(&Domain::onRealmDumped));
		} else if (Domain::AssemblyMethod::Slip == _assembly_method) {
			QScopedPointer<Core::Data::Provider> data_file(new Core::Data::Provider());
			Core::Data::Factory data_factory(data_file->makeFactory());
			if (!data_file->connect(
										QString("%1/realm-dump.sqlite3-%2").arg(getRealm()->getStoragePath().absolutePath()).arg(QDateTime::currentSecsSinceEpoch()),
										"candidate-" + getRealm()->getPreference()->group()
										)
			) {
				return false;
			}
			Data::Extension extension_data;
			if (!data_factory.create(&extension_data, false)) {
				return false;
			}

			Data::Revision1Setup data_setup;
			if (!data_setup.go(*(data_file.get()))) {
				return false;
			}

			extension_data.name.setValue("repository");
			extension_data.revision.setValue(1);
			if (!data_factory.insert(&extension_data, Core::Data::Factory::InsertionPolicy::Regular)) {
				return false;
			}

			for (const Meta::Essence::Target& target : {Meta::Essence::Target::Group, Meta::Essence::Target::Subject, Meta::Essence::Target::Object}) {
				const QSharedPointer<Core::Net::CRPCall> call(makeCall());
				QObject::connect(
								call.get(),
								qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),
								std::bind
								(
									(void(Domain::*)(Meta::Essence::Target, QJsonValue))&Domain::onRealmFlushed,
									this,
									target, std::placeholders::_1
								)
				);
				QObject::connect(
								call.get(),
								qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),
								std::bind
								(
									(void(Domain::*)(Meta::Essence::Target, quint8, quint32, QString))&Domain::onRealmFlushed,
									this,
									target, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
								)
				);
				call->act(
							"Cadabra::Ace::Repository::Model::HierarchyInterface",
							"Cadabra::Ace::Repository::Model::RealmFlush",
							{
								{"target", static_cast<char>(target)}
							}
				);
				getPatch(target).clear();
			}
			_assembly_file._data_file = data_file.take();
		} else {
			return false;
		}
		return true;
	}

	bool Domain::pull(Meta::Essence::Target target, Meta::Essence::Id offset_id)
	{
		const QSharedPointer<Core::Net::CRPCall> call(makeCall());
		QObject::connect(
						call.get(),
						qOverload<QJsonValue>(&Core::Net::CRPCall::reacted),
						std::bind
						(
							(void(Domain::*)(Meta::Essence::Target, QJsonValue))&Domain::onRealmPulled,
							this,
							target, std::placeholders::_1
						)
		);
		QObject::connect(
						call.get(),
						qOverload<quint8, quint32, QString>(&Core::Net::CRPCall::excepted),
						std::bind
						(
							(void(Domain::*)(Meta::Essence::Target, quint8, quint32, QString))&Domain::onRealmPulled,
							this,
							target,
							std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
						)
		);
		call->act(
					"Cadabra::Ace::Repository::Model::HierarchyInterface",
					"Cadabra::Ace::Repository::Model::RealmPull",
					{
						{"target", static_cast<char>(target)},
						{"offset", QString::number(offset_id)},
						{"limit", QString::number(Domain::PullLimit)},
					}
		);
		return true;
	}

	Realm* Domain::getRealm() const
	{
		return hasDescriptor() ? getDescriptor()->getRealm() : nullptr;
	}

	Domain::AuthenticationState Domain::getAuthenticationState() const
	{
		return _authentication_state;
	}

	Domain::AssemblyMethod Domain::getAssemblyMethod() const
	{
		return _assembly_method;
	}

	QString Domain::getHostEndpoint() const
	{
		if (!_session || !_session->isRealized()) {
			return QString("%1:%2").arg(Net::AceEndpoint::DefaultHost).arg(Net::AceEndpoint::DefaultPort);
		}
		return QString("%1:%2").arg(_session->getRealm()->getEndpoint()->getHost()).arg(_session->getRealm()->getEndpoint()->getPort());
	}

	bool Domain::getSSLEndpoint() const
	{
		if (!_session || !_session->isRealized()) {
			return Net::AceEndpoint::DefaultSecure;
		}
		return _session->getRealm()->getEndpoint()->isSecured();
	}

	const Domain::LogStamp& Domain::getLastLogStamp(Meta::Essence::Target target, bool force)
	{
		if (force || !getPatch(target).hasLogStampOffset()) {
			Cadabra::Queen::Core::Data::Provider* data_provider(getDescriptor()->getRealm()->getDataProvider());
			QSharedPointer<Data::Sync> sync_data(new Data::Sync());
			const Core::Data::Field target_field(Core::Data::Field::Type::Integer, static_cast<int>(target));
			sync_data->addPredicate(QSharedPointer<Core::Data::ConditionPredicate>::create(sync_data->target, target_field, Core::Data::ConditionPredicate::Condition::Equal));
			sync_data->addOrder(sync_data->timestamp, Qt::SortOrder::DescendingOrder);
			sync_data->setRowLimit(1);
			getPatch(target).setLogStampOffset
			(
				!sync_data->open(data_provider) || sync_data->isEmpty()
				? Domain::NilStamp
				: qMakePair(sync_data->acestamp.value().toString(), sync_data->timestamp.value().toString())
			);
		}
		return getPatch(target).getLogStampOffset();
	}

	AbstractPatch& Domain::getPatch(Meta::Essence::Target target)
	{
		static ZilchPatch zilch_patch;
		switch (target) {
			case Meta::Essence::Target::Group: return _group_patch;
			case Meta::Essence::Target::Subject: return _subject_patch;
			case Meta::Essence::Target::Object: return _object_patch;

			default: return zilch_patch;
		}
		return zilch_patch;
	}

	void Domain::setAuthenticationState(AuthenticationState state)
	{
		if (_authentication_state != state) {
			const AuthenticationState backup(_authentication_state);
			_authentication_state = state;
			emit authenticationStateChanged(backup);

			if (AuthenticationState::Active == state) {
				if (!hasDescriptor()) {
					/// @xxx "8660964310169681921" -> ApplicationScheme::Ident()
					open(_session->getRealm()->getSessionDescriptor()->handle(1, 1, "8660964310169681921", tr("Domain")));
				}
			} else if (hasDescriptor()) {
				close();
			}
		}
	}

	bool Domain::setSyncState(Meta::Essence::Target target, bool inout)
	{
		if (!inout) {
			if (--_sync_map[target] <= 0) {
				_sync_map.remove(target);
				emit syncStateChanged(target);
				return true;
			}
		} else {
			if (++_sync_map[target] == 1) {
				emit syncStateChanged(target);
			}
		}
		return false;
	}

	void Domain::setHostEndpoint(const QString& value)
	{
		QUrl url(value.contains("://") ? value : (_session->getRealm()->getEndpoint()->isSecured() ? "aces://" : "ace://") + value);
		_session->getRealm()->getEndpoint()->setURL(url);
	}

	void Domain::setSSLEndpoint(bool onoff)
	{
		_session->getRealm()->getEndpoint()->setSecure(onoff);
	}

	void Domain::setAutoProlongationMode(bool onoff)
	{
		if (_auto_prolongation_mode != onoff) {
			if (!(_auto_prolongation_mode = onoff)) {
				QObject::killTimer(_auto_prolongation_timerid);
				_auto_prolongation_timerid = 0;
			} else {
				if (_auto_prolongation_timerid > 0) {
					QObject::killTimer(_auto_prolongation_timerid);
					_auto_prolongation_timerid = 0;
				}
				if (AuthenticationState::Active != _authentication_state) {
					if (_session->prolongate(nullptr)) {
						setAuthenticationState(AuthenticationState::Reactive);
						_auto_prolongation_timerid = QObject::startTimer(5000);
					}
				}
			}
			emit autoProlongationModeChanged();
		}
	}

	void Domain::customEvent(QEvent* event)
	{
		if (event->type() == Repository::Model::AceEvent::Action<ObjectAction>::RegisterEventType(0)) {
			return onAction(Repository::Model::AceEvent::TypeCast<ObjectAction>(static_cast<Repository::Model::AceEvent*>(event)));
		} else if (event->type() == Repository::Model::AceEvent::Action<SubjectAction>::RegisterEventType(0)) {
			return onAction(Repository::Model::AceEvent::TypeCast<SubjectAction>(static_cast<Repository::Model::AceEvent*>(event)));
		} else if (event->type() == Repository::Model::AceEvent::Action<GroupAction>::RegisterEventType(0)) {
			return onAction(Repository::Model::AceEvent::TypeCast<GroupAction>(static_cast<Repository::Model::AceEvent*>(event)));
		}

		return AbstractApplication::customEvent(event);
	}

	void Domain::timerEvent(QTimerEvent* event)
	{
		if (_auto_prolongation_timerid == event->timerId()) {
			if (AuthenticationState::Active == _authentication_state) {
				setAutoProlongationMode(false);
			} else if (_session->prolongate(nullptr)) {
				setAuthenticationState(AuthenticationState::Reactive);
			} else {
				setAuthenticationState(AuthenticationState::Fault);
			}
		}
	}

	void Domain::onSessionActivated(bool onoff)
	{
		if (onoff) {
			setAutoProlongationMode(false);
		}
		setAuthenticationState(onoff ? AuthenticationState::Active : AuthenticationState::Unknown);
	}

	void Domain::onSessionFaulted(Session::AuthorizationPhase phase, Session::AuthorizationReject reject, QString message)
	{
		if (AuthenticationState::Unknown == _authentication_state) {
			/// @note Just ignore becauase probably was a hard reset before
			return ;
		} else if (Session::AuthorizationPhase::Prolongation == phase && _auto_prolongation_mode) {
			/// @brief Не считаем ошибкой, когда пролонгируем сессию и включен автоматический режим
			setAuthenticationState(AuthenticationState::Reactive);
		} else if (Session::AuthorizationPhase::Verification == phase && _session->isActive()) {
			/// @brief Не считаем ошибкой, когда верифицируем соединение(с последующим пролонгированием)
			setAuthenticationState(AuthenticationState::Reactive);
		} else {
			setAuthenticationState(AuthenticationState::Fault);
		}
	}

	void Domain::onSessionAlived(bool onoff)
	{
		if (AuthenticationState::Reactive == _authentication_state && _session->isActive() && onoff) {
			setAuthenticationState(AuthenticationState::Active);
		}
	}

	void Domain::onRealmRestored()
	{
		setSyncState(Meta::Essence::Target::Zilch, false);
		if (!_live_mode) {
			return ;
		}
		_session->subscribe("realm", true);
		log(Meta::Essence::Target::Group);
		log(Meta::Essence::Target::Subject);
		log(Meta::Essence::Target::Object);
	}

	void Domain::onRealmDumped(QNetworkReply::NetworkError error_code, QString error_message)
	{
		emit exceptionThrew(MessageLevel::CriticalMessage, "Realm could not be restored because of the remote or network error: " + error_message);
	}

	void Domain::onRealmDumped(QString file_name)
	{
		qDebug() << "Realm dump loaded" << file_name;
		if (getDescriptor()->getRealm()->import(file_name)) {
			onRealmRestored();
		}
		_assembly_file._net_file = nullptr;
	}

	void Domain::onRealmFlushed(Meta::Essence::Target target, quint8 domain, quint32 code, QString message)
	{
		emit exceptionThrew(++_sync_error > 10 ? MessageLevel::CriticalMessage : MessageLevel::WarningMessage, "Realm could not be restored because of a remote or network error during flush: " + message);
		pull(target, 0);
	}

	void Domain::onRealmFlushed(Meta::Essence::Target target, QJsonValue payload)
	{
		if (!payload.isObject()) {
			emit exceptionThrew(MessageLevel::CriticalMessage, "Realm could not be restored because of the unexpected or a malformed `flush`");
			return ;
		}

		const QJsonObject object(payload.toObject());
		qDebug("Flushing a limit(%c)...", static_cast<char>(target));

		if (!getPatch(target).flush(
									object.value("id").toString("0").toULongLong(),
									{
										object.value("acestamp").toString(Domain::NilStamp.first),
										object.value("timestamp").toString(Domain::NilStamp.second)
									}
			)
		) {
			return ;
		}

		Data::Sync sync_data;
		sync_data.target.setValue(static_cast<char>(target));
		sync_data.acestamp.setValue(object.value("acestamp").toString(Domain::NilStamp.first));
		sync_data.timestamp.setValue(object.value("timestamp").toString(Domain::NilStamp.second));

		Core::Data::Factory data_factory(_assembly_file._data_file->makeFactory());
		if (!data_factory.insert(&sync_data)) {
			emit exceptionThrew(MessageLevel::CriticalMessage, "Realm could not be restored because of the data factory error");
		} else {
			pull(target, 0);
		}
	}

	void Domain::onRealmPulled(Meta::Essence::Target target, quint8 domain, quint32 code, QString message)
	{
		qCritical() << "[" << domain << code << "]" << "Pull error: " << message;

		QScopedPointer<Data::AbstractRecord> record_data;
		switch (target) {
			case Meta::Essence::Target::Object:
				record_data.reset(new Data::Object());
				break;
			case Meta::Essence::Target::Subject:
				record_data.reset(new Data::Subject());
				break;
			case Meta::Essence::Target::Group:
				record_data.reset(new Data::Group());
				break;

			default:
				emit exceptionThrew(MessageLevel::CriticalMessage, "Realm could not be restored because of a remote or network error during pull: " + message);
		}

		record_data->addOrder(record_data->id, Qt::SortOrder::DescendingOrder);
		record_data->setRowLimit(1);
		if (!record_data->open(_assembly_file._data_file, false)) {
			emit exceptionThrew(MessageLevel::CriticalMessage, "Realm could not be restored because of a remote or network error during pull");
		} else {
			emit exceptionThrew(++_sync_error > 10 ? MessageLevel::CriticalMessage : MessageLevel::WarningMessage, "Realm could not be restored because of a remote or network error during pull");
			pull(target, record_data->id.value().toLongLong());
		}
	}

	void Domain::onRealmPulled(Meta::Essence::Target target, QJsonValue payload)
	{
		if (!payload.isObject()) {
			emit exceptionThrew(MessageLevel::CriticalMessage, "Realm could not be restored because of the unexpected or malformed `pull`");
			return ;
		}

		const QJsonArray array(payload.toObject().value("array").toArray());
		qDebug("Inserting a bulk(%c)...", static_cast<char>(target));

		QScopedPointer<Data::AbstractRecord> record_data;
		switch (target) {
			case Meta::Essence::Target::Object:
				record_data.reset(new Data::Object());
				break;
			case Meta::Essence::Target::Subject:
				record_data.reset(new Data::Subject());
				break;

			case Meta::Essence::Target::Group:
				record_data.reset(new Data::Group());
				break;

			default:
				return ;
		}

		Core::Data::Factory data_factory(_assembly_file._data_file->makeFactory());
		if (!data_factory.insert(record_data.get(), array)) {
			emit exceptionThrew(MessageLevel::CriticalMessage, "Realm could not be restored because of a data factory error");
			return ;
		} else if (data_factory.getAffectedRowsCount() == 0 && array.count() > 0) {
			emit exceptionThrew(MessageLevel::CriticalMessage, "Realm slip was not inserted because of a data factory error");
			return ;
		}

		const Meta::Essence::Id limit_id(getPatch(target).getLastId());
		if (array.count() < Domain::PullLimit) { /// < Больше на бэкенде ничего нет
			getPatch(target).clear();
		} else if (limit_id > 0) { ///< Тянем только до отсчечки
			if (!data_factory.remove(
									record_data.get(),
									Qaos::MakeUnique<Core::Data::ConditionPredicate>
									(
										record_data->id,
										Core::Data::Field(Core::Data::Field::Type::Integer, limit_id),
										Core::Data::ConditionPredicate::Condition::Greater
									).get()
						)
			) {
				emit exceptionThrew(MessageLevel::CriticalMessage, "Further pulling is stopped because could not remove limited data");
				return ;
			} else if (data_factory.getAffectedRowsCount() > 0) { /// < Стянули даже лишнее, поэтому больше ничего не нужно
				getPatch(target).clear();
			}
		}

		if (getPatch(target).hasStash()) { /// < Просим еще
			record_data->addOrder(record_data->id, Qt::SortOrder::DescendingOrder);
			record_data->setRowLimit(1);
			if (!record_data->open(_assembly_file._data_file, false) || record_data->isEmpty()) {
				emit exceptionThrew(MessageLevel::CriticalMessage, "Realm could not be pulled because of the data provider error");
				return ;
			}
			/// @note Запрашиваем еще пачку данных
			pull(target, record_data->id.value().toLongLong());
			return ;
		}

		std::set<Meta::Essence::Target> target_set({Meta::Essence::Target::Group, Meta::Essence::Target::Subject, Meta::Essence::Target::Object});
		target_set.erase(target);
		if (std::any_of(target_set.cbegin(), target_set.cend(), [this](const Meta::Essence::Target& t) { return getPatch(t).hasStash(); })) {
			return ;
		}

		const QString file_name(_assembly_file._data_file->makeDatabase().databaseName());
		_assembly_file._data_file->vacuum();
		_assembly_file._data_file->disconnect();
		_assembly_file._data_file->deleteLater();
		getDescriptor()->getRealm()->import(file_name);
		onRealmRestored();
	}

	void Domain::onRealmLogged(Meta::Essence::Target target, quint8 domain, quint32 code, QString message)
	{
		qCritical() << "[" << domain << code << "]" << "Log error:" << message;
		setSyncState(target, false);
	}

	void Domain::onRealmLogged(Meta::Essence::Target target, QJsonValue payload)
	{
		if (!payload.isObject()) {
			emit exceptionThrew(MessageLevel::CriticalMessage, QString("Malfromed log(%c) cannot be stashed").arg(static_cast<char>(target)));
			setSyncState(target, false);
			return ;
		}

		AbstractPatch& target_patch(getPatch(target));
		const QJsonArray array(payload.toObject().value("array").toArray());
		qDebug("Stashing(%c) %d incidents...", static_cast<char>(target), array.count());
		if (array.count() == 0) {
			setSyncState(target, false);
			return ;
		}
		using ResolutionType = std::underlying_type<AbstractPatch::Resolution>::type;
		const ResolutionType resolution(static_cast<ResolutionType>(target_patch.stash(array)));
		if ((static_cast<ResolutionType>(AbstractPatch::Resolution::Malformed) & resolution) != 0) {
			emit exceptionThrew(MessageLevel::CriticalMessage, QString("Stash(%1) critical resolution: %2").arg(static_cast<char>(target)).arg(resolution));
		} else if (static_cast<ResolutionType>(AbstractPatch::Resolution::Accept) != resolution) {
			emit exceptionThrew(MessageLevel::WarningMessage, QString("Stash(%1) warning resolution: %2").arg(static_cast<char>(target)).arg(resolution));
		}

		///@brief Получить еще пачку логов
		if (setSyncState(target, false) && array.count() >= Domain::LogLimit) {
			log(target);
		} else if (!isSyncState()) { /// < Нет активных запросов лога - можно патчить!
			if (!patch()) {
				emit exceptionThrew(MessageLevel::CriticalMessage, "Hierarhcy was not patched completely");
			} else {
				qDebug("Hierarhcy was sucessfully patched");
			}
		}
	}

	bool Domain::onHandle()
	{
		_sync_data.reset(new Data::Sync());
		return true;
	}

	bool Domain::onOpen()
	{
		_group_patch.map(getDescriptor()->getRealm()->getHierarchy()->getGroupModel());
		_subject_patch.map(getDescriptor()->getRealm()->getHierarchy()->getSubjectModel());
		_object_patch.map(getDescriptor()->getRealm()->getHierarchy()->getObjectModel());
		_sync_error = 0;

		if (AssemblyMethod::Lazy == _assembly_method) {
			/// @todo
			if (_session->getRealm()->getHierarchy()->assemble()) {

			}
		} else if (
					getLastLogStamp(Meta::Essence::Target::Group, true) == Domain::NilStamp
					||
					getLastLogStamp(Meta::Essence::Target::Subject, true) == Domain::NilStamp
					||
					getLastLogStamp(Meta::Essence::Target::Object, true) == Domain::NilStamp
		) {
			/// @??? Пересадить на другой поток
			/// @??? Добавить таймер
			if (load()) {
				setSyncState(Meta::Essence::Target::Zilch, true);
			} else {
				qCritical() << "Unexpected assembly error";
				return false;
			}
		}

		if (_sync_map.value(Meta::Essence::Target::Zilch, 0) == 0) {
			onRealmRestored();
		} else {
			_session->getRealm()->getPreference()->setOption("realm_assembly_method", static_cast<int>(_assembly_method));
			_session->getRealm()->getPreference()->setOption("realm_live_mode", _live_mode);
		}

		return true;
	}

	bool Domain::onClose()
	{
		_group_patch.unmap();
		_subject_patch.unmap();
		_object_patch.unmap();
		_sync_error = 0;

		return true;
	}

	bool Domain::onSetup()
	{
		return true;
	}

	void Domain::onAction(EssenceAction* action)
	{
		qDebug() << "EssenceAction(" << static_cast<char>(action->getTarget()) << ") is fired with logstamp = {" << action->getAceStamp() << ";" << action->getTimeStamp() << "}";
		const LogStamp& logstamp(getLastLogStamp(action->getTarget()));
		const int stamp_compare(logstamp.second.compare(action->getTimeStamp()));
		if (stamp_compare < 0 || (stamp_compare == 0 && logstamp.first.compare(action->getAceStamp()) != 0)) {
			qDebug() << "... requesting the log with timestamp up to" << action->getTimeStamp();
			log(action->getTarget());
		} else {
			qDebug() << "... nothing to request, already synced this log" << action->getTimeStamp();
		}
	}

	void Domain::onAction(SubjectAction* action)
	{
		onAction(static_cast<EssenceAction*>(action));
	}

	void Domain::onAction(GroupAction* action)
	{
		onAction(static_cast<EssenceAction*>(action));
	}

	void Domain::onAction(ObjectAction* action)
	{	
		onAction(static_cast<EssenceAction*>(action));
	}

	bool Domain::isActive() const
	{
		return !(!_session || !_session->isActive());
	}

	bool Domain::isSyncState() const
	{
		return !_sync_map.isEmpty();
	}

	bool Domain::isLiveMode() const
	{
		return _live_mode;
	}

	bool Domain::isAutoProlongationMode() const
	{
		return _auto_prolongation_mode;
	}
} } } }
