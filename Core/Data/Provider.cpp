#include "Provider.hpp"

#include "../../Skeleton.hpp"
#include "AbstractEntity.hpp"
#include "Factory.hpp"
#include "Event.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QSqlError>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	Provider::Provider()
	:
		QObject(nullptr)
	{
		QObject::connect(this, &Provider::connectRequested, this, qOverload<const QString&, const QString&>(&Provider::connect));
		QObject::connect(this, &Provider::disconnectRequested, this, &Provider::disconnect);
	}

	Provider::~Provider()
	{
		disconnect();
		for (QHash<quintptr, QPointer<Provider>>::ConstIterator p = _thread_cache.constBegin(); p != _thread_cache.constEnd(); ++p) {
			if (!p->isNull()) {
				QObject::disconnect(p.value()->thread(), &QThread::finished, p.value(), nullptr);
				p.value()->deleteLater();
			}
		}
		_thread_cache.clear();
	}

	bool Provider::connect(const QString& file_path, const QString& connection_name)
	{
		if (!QSqlDatabase::contains(connection_name)) {
			_database = QSqlDatabase::addDatabase("QSQLITE", connection_name);
			_database.setDatabaseName(file_path);
			_database.setConnectOptions("QSQLITE_ENABLE_UNICODE");
		} else {
			_database = QSqlDatabase::database(connection_name);
		}
		if (!_database.isValid()) {
			qFatal("Cannot add database: %s", qPrintable(_database.lastError().text()));
		}
		/// @note When using the SQLite driver, open() will create the SQLite database if it doesn't exist.
		if (!_database.open()) {
			/// @wtf
			QFile::remove(file_path);
			qDebug("Cannot open database file(%s): %s", qPrintable(file_path), qPrintable(_database.lastError().text()));
			return false;
		} else {
			qDebug() << "Connection features > QuerySize =" << getDriver()->hasFeature(QSqlDriver::QuerySize);
		}
		emit connected();
		for (QHash<quintptr, QPointer<Provider>>::ConstIterator p = _thread_cache.constBegin(); p != _thread_cache.constEnd(); ++p) {
			emit p.value()->connectRequested(file_path, QString("%1_%2").arg(_database.connectionName()).arg(p.key()));
		}
		return true;
	}

	bool Provider::connect(ProviderConnect* command)
	{
		return connect(command->getFilePath(), command->getConnectionName());
	}

	bool Provider::connect(ProviderClone* command)
	{
		_database = QSqlDatabase::cloneDatabase(command->getOriginName(), command->getDestinationName());
		return _database.open();
	}

	bool Provider::disconnect()
	{
		if (!_database.isOpen()) {
			return false;
		} else {
			_database.close();
			emit disconnected();
		}
		for (QHash<quintptr, QPointer<Provider>>::ConstIterator p = _thread_cache.constBegin(); p != _thread_cache.constEnd(); ++p) {
			emit p.value()->disconnectRequested();
		}
		return true;
	}

	Provider* Provider::fork(QThread* thread)
	{
		if (!thread || Cadabra::Queen::Skeleton::instance()->thread() == thread) {
			return this;
		}
		const quintptr thread_id(reinterpret_cast<quintptr>(thread));
		QHash<quintptr, QPointer<Provider>>::ConstIterator p(_thread_cache.find(thread_id));
		if (_thread_cache.constEnd() != p) {
			return p->data();
		}
		QScopedPointer<Provider> retval(new Provider());
		retval->moveToThread(thread);
		QObject::connect(thread, &QThread::finished, std::bind(&Provider::finish, this, thread));
		QCoreApplication::postEvent(retval.get(), new Data::Event(new ProviderClone(_database.connectionName(), QString("%1_%2").arg(_database.connectionName()).arg(thread_id))));
		return _thread_cache.insert(thread_id, QPointer<Provider>(retval.take())).value().data();
	}

	void Provider::finish(QThread* thread)
	{
		if (!thread) {
			return ;
		}
		QPointer<Provider> provider(_thread_cache.take(reinterpret_cast<quintptr>(thread)));
		if (!provider.isNull()) {
			provider->deleteLater();
		}
	}

	bool Provider::vacuum()
	{
		QSqlQuery query(_database.exec("PRAGMA journal_mode"));
		if (query.lastError().isValid() || !query.next()) {
			qCritical() << "Could not determine journal mode of the source database: " << query.lastError();
//			emit faulted("Could not switch source database to WAL mode");
			return false;
		}
		const bool in_wal(query.value(0).toString().compare("WAL", Qt::CaseInsensitive) == 0);
		query.finish();
		if (!in_wal) {
			if (!query.exec("PRAGMA journal_mode=WAL")) {
				qCritical() << "Could not switch journal mode of the source database to WAL: " << query.lastError();
//				emit faulted("Could not switch source database to WAL mode");
				return false;
			}
		} else if (!query.exec("PRAGMA wal_checkpoint(TRUNCATE)") || !query.exec("VACUUM")) {
			qCritical() << "Could not vacuumize datasource: " << query.lastError();
//			emit faulted("Could not vacuumize datasource which merely means that database is probably broken");
			return false;
		}
		return true;
	}

	void Provider::flush()
	{
		return;
	}

	qsizetype Provider::count(AbstractEntity* entity, bool use_predicate)
	{
		const QString predicate(use_predicate ? static_cast<QString>(entity->getCompoundPredicate()) : "");
		QSqlQuery query(_database);
		if (!query.prepare("SELECT COUNT(*) FROM " + entity->getAlias() + (!predicate.isEmpty() ? " WHERE " + predicate : "")) || !query.exec()) {
			qWarning() << "Count error:" << query.lastError();
			return 0;
		}
		qDebug() << "Count query:" << query.lastQuery();
		return query.first() ? query.value(0).toULongLong() : 0;
	}

	qsizetype Provider::count(AbstractEntity* entity, AbstractPredicate* predicate)
	{
		QSqlQuery query(_database);
		if (!query.prepare("SELECT COUNT(*) FROM " + entity->getAlias() + (!predicate || static_cast<QString>(*predicate).isEmpty() ? "" : " WHERE " + predicate->makeSQL())) || !query.exec()) {
			qWarning() << "Count error:" << query.lastError();
			return 0;
		}
		qDebug() << "Count query:" << query.lastQuery();
		return query.first() ? query.value(0).toULongLong() : 0;
	}

	bool Provider::open(AbstractEntity* entity, QFlags<Provider::OpenMode> mode_set, Provider::RowsetType rowset_type)
	{
		return entity->open(entity->thread() == thread() ? this : fork(entity->thread()), mode_set, rowset_type);
	}

	bool Provider::fetch(AbstractEntity* entity, int row_index)
	{
		if (entity->thread() == thread()) {
			return entity->locate(row_index);
		}
		entity->fetchMore(entity->index(row_index, 0));
		return true;
	}

	QSqlDriver* Provider::getDriver() const
	{
		return _database.driver();
	}

	QString Provider::getName() const
	{
		return _database.databaseName();
	}

	Factory Provider::makeFactory()
	{
		return Factory(this);
	}

	QSqlDatabase Provider::makeDatabase() const
	{
		return _database;
	}

	void Provider::customEvent(QEvent* event)
	{
		if (event->type() == Core::Data::Event::Action<ProviderConnect>::RegisterEventType()) {
			connect(Core::Data::Event::TypeCast<ProviderConnect>(event));
		} else if (event->type() == Core::Data::Event::Action<ProviderDisconnect>::RegisterEventType()) {
			disconnect();
		} else if (event->type() == Core::Data::Event::Action<ProviderClone>::RegisterEventType()) {
			connect(Core::Data::Event::TypeCast<ProviderClone>(event));
		} else {
			return QObject::customEvent(event);
		}
	}

	bool Provider::isExist(AbstractEntity* entity) const
	{
		return isEntityExist(entity->getAlias());
	}

	bool Provider::isEntityExist(const QString& name) const
	{
		QSqlQuery query(_database);
		if (!query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='" + name + "'") || !query.exec()) {
			return 0;
		}
		return query.lastError().isValid() || !query.first() ? false : true;
	}

	bool Provider::hasEntity(AbstractEntity* entity)
	{
		return hasEntity(entity->getAlias());
	}

	bool Provider::hasEntity(const QString& name)
	{
		return !_database.record(name).isEmpty();
	}
} } } }
