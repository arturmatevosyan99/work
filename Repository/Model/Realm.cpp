#include "Realm.hpp"

#include "Descriptor/SessionDescriptor.hpp"

#include "../Meta/SetupDependency.hpp"

#include "../Data/Entity/Extension.hpp"

#include "../../Core/System/Alert.hpp"
#include "../../Core/System/Alert/StateAlert.hpp"
#include "../../Core/Data/Factory.hpp"

#include "../../Core/System/Native.hpp"

#include <QDebug>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	QString Realm::PreferenceFilename()
	{
		return Core::System::Native::Instance()->getDataDirectory().absoluteFilePath("preference").append(Core::System::Preference::FileExtension);
	}

	Realm::Realm(QObject* parent)
	:
		QObject(parent),

		_is_initialized(false),
		_synchronization_mode(SynchronizationMode::Lazy),
		_preference(new Core::System::Preference(Realm::PreferenceFilename(), "default")),
		_endpoint(new Repository::Net::AceEndpoint()),
		_hierarchy(new Hierarchy(this)),
		_data_provider(new Core::Data::Provider()),
		_session_descriptor(nullptr)
	{
		const QVariant ace_endpoint_option(Skeleton::ActiveInstance()->getEnvironmentVariable("core.net.ace.endpoint"));
		if (ace_endpoint_option.isValid()) {
			const QUrl ace_endpoint_url(ace_endpoint_option.toUrl());
			_endpoint->setHost(ace_endpoint_url.host());
			_endpoint->setSecure(ace_endpoint_url.scheme() == "https");
			_endpoint->setPort(ace_endpoint_url.port(_endpoint->isSecured() ? 443 : 80));
		}

		QObject::connect(this, &Realm::initialized, this, &Realm::initChanged);
		QObject::connect(this, &Realm::deinitialized, this, &Realm::initChanged);
		QObject::connect(this, &Realm::faulted, this, &Realm::onFaulted);
	}

	Realm::~Realm()
	{
		qDebug() << "Realm is destructing...";
		delete _data_provider;
		delete _hierarchy;
		delete _endpoint;
		delete _preference;
	}

	bool Realm::initialize(SessionDescriptor* session_descriptor)
	{
		if (_is_initialized || !session_descriptor) {
			return false;
		}

		const QString stamp(
							QString::number
							(
								qHash
								(
									QString("%1://%2@%3:%4")
										.arg(_endpoint->isSecured() ? "aces" : "ace")
										.arg(session_descriptor->getPoolId()).arg(_endpoint->getHost())
										.arg(_endpoint->getPort()),
									qHash(objectName())
								), 16
							)
		);
		_storage_path.setPath(Core::System::Native::Instance()->getDataDirectory().absolutePath().append("/").append(stamp).append(".caprepository"));

		if (!_storage_path.mkpath(".")) {
			qCritical("Failed to create writable directory for Realm storage at %s", qPrintable(_storage_path.absolutePath()));
			return false;
		} else {
			qDebug() << "Realm root has been created at: " << _storage_path.absolutePath();
		}

		if (!_data_provider->connect(_storage_path.absolutePath() + "/realm.sqlite3", stamp)) {
			emit faulted("Could not connect to data source");
			return false;
		} else if (!_data_provider->vacuum()) {
			emit faulted("Could not switch database to WAL mode or database is broken and could not be vacuumize");
			return false;
		}

		if (!_preference->setGroup(stamp)) {
			_preference->beginGroup(stamp);
		}

		if (!setup()) {
			emit faulted("Could not upgrade the Realm");
			return false;
		} else if (!_hierarchy->assemble()) {
			emit faulted("Could not build the hierarchy");
			return false;
		}

		_preference->setValue("_", stamp);
		_session_descriptor = session_descriptor;
		_is_initialized = true;
		emit initialized();
		return true;
	}

	bool Realm::deinitialize(bool clean)
	{
		if (!_is_initialized) {
			return false;
		}

		_is_initialized = false;
		_session_descriptor = nullptr;
		_data_provider->disconnect();

		if (clean) {
			_storage_path.removeRecursively();
			_storage_path = QDir();
			_preference->clearGroup();
			_preference->setValue("_", "");
		}
		emit deinitialized();
		return true;
	}

	bool Realm::import(const QString& file_name)
	{
		QFile file(file_name);
		if (file_name.isEmpty() || !file.exists()) {
			emit faulted("Dump is broken or not applicable");
			return false;
		}

		/// @note We need force Hierarchy to teardown(), let's emit faked deinitialized;
		emit deinitialized();
		_data_provider->disconnect();

		QFileInfoList file_list(_storage_path.entryInfoList(QStringList("realm.sqlite*")));
		for (int f = 0; f < file_list.count(); ++f) {
			if (!QFile::remove(file_list.at(f).absoluteFilePath())) {
				qDebug() << "Could not remove one of the database realm files: " << file_list.at(f).absoluteFilePath();
//				emit faulted("Could not remove current realm");
//				return ;
			}
		}
		const bool ok(file.rename(_storage_path.absolutePath() + "/realm.sqlite3"));
		file.close();
		if (ok) {
			if (!_data_provider->connect(file.fileName(), _preference->group())) {
				emit faulted("Could not connect to data source");
				return false;
			} else if (_data_provider->makeDatabase().exec("PRAGMA journal_mode=WAL").lastError().isValid()) {
				emit faulted("Could not switch source database to WAL mode");
				return false;
			} else if (!setup()) {
				emit faulted("Could not upgrade the Realm");
			} else if (!_hierarchy->assemble()) {
				emit faulted("Could not build the hierarchy");
				return false;
			}
			emit restored();
			return true;
		}
		emit faulted("Could not restore realm dump");
		return false;
	}

	bool Realm::setup()
	{
		/// @note We need at least an empty Data::Extension to proceed
		QMap<QString, Core::Meta::Scheme<Meta::SetupDependency>::Salt> extension_map;
		Data::Extension extension_data;
		Core::Data::Factory data_factory(_data_provider->makeFactory());
		if (!_data_provider->isExist(&extension_data)) {
			extension_data.name.setValue("repository");
			extension_data.revision.setValue(0);
			if (!data_factory.create(&extension_data, true)) {
				qCritical("Extension data structure could not be created");
				return false;
			} else if (!data_factory.insert(&extension_data, Core::Data::Factory::InsertionPolicy::Regular)) {
				qCritical("Extension data structure could not be initialized");
				return false;
			}
		} else if (!extension_data.open(_data_provider)) {
			qCritical("Extension data structure could not be opened");
			return false;
		} else if (!extension_data.isEmpty()) { /// < Получаем текущие ревизии
			do {
				extension_map[extension_data.name.value().toString().toLower()] = extension_data.revision.value().toLongLong();
			} while (extension_data.next());
		}

		Core::Meta::SchemeModel setup_scheme_model;
		if (!setup_scheme_model.setFactoryAlias("Repository.Meta.SetupDependency")) {
			qCritical() << "Setup factory not found and Realm structure cannot be guaranteed";
			return false;
		}
		/// @todo filter already created
		setup_scheme_model.addFilter(new Core::Meta::Filter(&setup_scheme_model, Core::Meta::Filter::Equal, "model_name", "Extension"));

		const int set_role_hash(setup_scheme_model.getRoleHashKey("set"));

		struct SetupSpecification
		{
			QString name;
			quint32 hash;
			Core::Meta::Scheme<Meta::SetupDependency>::Salt revision;
			QSet<Core::Meta::Scheme<Meta::SetupDependency>::Id> set;
		};

		/// @brief Собираем зависимости
		QMap<Core::Meta::Scheme<Meta::SetupDependency>::Id, SetupSpecification> setup_map, setdown_map;
		for (int count = setup_scheme_model.rowCount(), s = 0; s < count; ++s) {
			const Core::Meta::Scheme<Meta::SetupDependency>::Id id(setup_scheme_model.data(setup_scheme_model.index(s, 0), Qt::UserRole).value<Core::Meta::Qd>().toInt64());
			SetupSpecification specification
			{
				setup_scheme_model.data(setup_scheme_model.index(s, 0), Core::Meta::AbstractScheme::Role::ModuleName).toString().toLower(),
				setup_scheme_model.data(setup_scheme_model.index(s, 0), Core::Meta::AbstractScheme::Role::ExtensionHash).value<quint32>(),
				static_cast<Core::Meta::Scheme<Meta::SetupDependency>::Salt>(id & 0xFFFFFFFF),
				setup_scheme_model.data(setup_scheme_model.index(s, 0), set_role_hash).value<QSet<Core::Meta::Scheme<Meta::SetupDependency>::Id>>()
			};
			if (extension_map.value(specification.name, 0) >= specification.revision) {
				setdown_map[id] = specification;
			} else {
				setup_map[id] = specification;
			}
		}

		/// @brief Установка
		QMap<Core::Meta::Scheme<Meta::SetupDependency>::Id, SetupSpecification>::ConstIterator s(setup_map.constBegin());
		while (setup_map.constEnd() != s) {
			const QString& name(s->name);
			const quint32& hash(s->hash);
			const Core::Meta::Scheme<Meta::SetupDependency>::Salt& revision(s->revision);
			for (const Core::Meta::Scheme<Meta::SetupDependency>::Id& id : s->set) {
				if (!setdown_map.contains(id)) {
					s = std::as_const(setup_map).find(id);
					if (setup_map.constEnd() == s) {
						qCritical() << "Dependency" << id << "for" << name << "cannot be resolved";
						return false;
					} else {
						break;
					}
				}
			}
			if (hash != s->hash || revision != s->revision) {
				continue;
			}
			const QSharedPointer<Repository::Data::AbstractSetup> data_setup(Core::Meta::Scheme<Meta::SetupDependency>::Produce(s.key()));
			if (!data_setup) {
				qCritical() << "Dependency" << s.key() << "for" << name << "cannot be resolved";
				return false;
			} else if (!data_setup->go(*(_data_provider))) {
				qCritical() << s->name << "was not upgraded up to" << s->revision << " because of an unexpected fault";
				return false;
			}
			extension_data.name.setValue(s->name);
			extension_data.revision.setValue(s->revision);
			if (!data_factory.insert(&extension_data, Core::Data::Factory::InsertionPolicy::Replace)) {
				qCritical() << s->name << "was upgraded up to" << s->revision << " but revision could not be incremented";
				return false;
			}
			extension_map[s->name] = s->revision;
			setdown_map.insert(s.key(), s.value());
			setup_map.remove(s.key());
			s = setup_map.constBegin();
		}

		return true;
	}

	Realm::SynchronizationMode Realm::getSynchronizationMode() const
	{
		return _synchronization_mode;
	}

	QDir Realm::getStoragePath() const
	{
		return isInitialized() ? _storage_path : QDir();
	}

	Cadabra::Queen::Core::System::Preference* Realm::getPreference() const
	{
		return _preference;
	}

	Cadabra::Queen::Core::Net::Endpoint* Realm::getEndpoint() const
	{
		return _endpoint;
	}

	Cadabra::Queen::Repository::Model::Hierarchy* Realm::getHierarchy() const
	{
		return _hierarchy;
	}

	Cadabra::Queen::Repository::Model::SessionDescriptor* Realm::getSessionDescriptor() const
	{
		return _session_descriptor;
	}

	Cadabra::Queen::Core::Data::Provider* Realm::getDataProvider(QThread* thread) const
	{
		return nullptr == thread ? _data_provider : _data_provider->fork(thread);
	}

	void Realm::setSynchronizationMode(SynchronizationMode mode)
	{
		if (mode != _synchronization_mode && !_is_initialized) {
			_synchronization_mode = mode;
			emit synchronizationModeChanged();
		}
	}

	void Realm::onInitialized()
	{

	}

	void Realm::onDeinitialized()
	{
	}

	void Realm::onFaulted(const QString& message)
	{
		qCritical() << "Realm fault:" << message;
		Core::System::StateAlert* state(findChild<Core::System::StateAlert*>("CORRUPTED_REALM"));
		if (state) {
			state->setDescription(state->getDescription() + QChar(QChar::SpecialCharacter::LineFeed) + message);
		} else {
			/// @note Это game over и нужно вообще все закончить, потому что Realm.Lite сломан
			Core::System::Alert::Instance()->enqueue
			(
				new Core::System::StateAlert
				(
					this,
					"CORRUPTED_REALM",
					tr("Corrupted database"),
					message,
					this,
					Core::System::StateAlert::Level::Fault
				)
			);
		}
	}

	bool Realm::isInitialized() const
	{
		return _is_initialized;
	}
} } } }
