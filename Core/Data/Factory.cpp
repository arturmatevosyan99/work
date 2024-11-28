#include "Factory.hpp"

#include "Predicate/CompoundPredicate.hpp"
#include "Predicate/ConditionPredicate.hpp"

#include <QSqlError>
#include <QtDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QDateTime>
#include <QUuid>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	Factory::Factory(Provider* provider)
	:
		QObject(provider),

		_provider(provider),
		_transaction(TransactionPolicy::None),
		_affected_rows_count(0)
	{

	}

	Factory::Factory(Factory&& factory)
	:
		QObject(factory.parent()),

		_provider(factory._provider),
		_transaction(factory._transaction),
		_affected_rows_count(factory._affected_rows_count)
	{
		factory._provider = nullptr;
		factory._transaction = TransactionPolicy::None;
		factory._affected_rows_count = 0;
	}

	Factory::~Factory()
	{
		switch (_transaction) {
			case TransactionPolicy::AutoCommit:
				commit();
				break;
			case TransactionPolicy::AutoRollback:
				rollback();
				break;
			default:
				return;
		}
		_transaction = TransactionPolicy::None;
	}

	QSqlQuery Factory::prepare(const QueryStream& query)
	{
		QSqlQuery retval(_provider->_database);
		if (!retval.prepare(query)) {
			qCritical() << "Failed to prepare `" << retval.lastError() << "` the query: '" << query << "'";
		}

		return retval;
	}

	bool Factory::create(AbstractEntity* entity, bool safe_mode)
	{
		_affected_rows_count = 0;
		QueryStream q;
		q << "CREATE TABLE" << (safe_mode ? " IF NOT EXISTS " : " ") << entity->getAlias() << Qt::endl;

		/// @note Открывашка
		q << "(" << Qt::endl;

		/** @brief
			P_Id int NOT NULL PRIMARY KEY,
			LastName varchar(255) NOT NULL,
			FirstName varchar(255),
			Address varchar(255),
			City varchar(255)
		 */
		/// @note Список полей
		QMapIterator<AbstractEntity::FieldMap::key_type, AbstractEntity::FieldMap::mapped_type> f(entity->getFieldMap());
		while (f.hasNext()) {
			const Field& field(f.next().value().get());
			q << field.name() << " " << field.describe() << " " << (field.isNullable() ? "NULL" : "NOT NULL") << (f.hasNext() ? ", \n" : "");
		}

		/// @brief CONSTRAINT pk_PersonId PRIMARY KEY (P_Id, LastName)
		/// @note Primary Key
		if (entity->hasPrimaryKey()) {
			QSetIterator<AbstractEntity::FieldSet::value_type> pk(entity->getPrimaryKey().second);
			q << "," << Qt::endl << "CONSTRAINT " << entity->getPrimaryKey().first << " PRIMARY KEY(";
			while (pk.hasNext()) {
				const Field& field(pk.next().get());
				q << field.name() << (pk.hasNext() ? ", " : "");
			}
			q << ")";
		}

		/// @brief CONSTRAINT uc_PersonId UNIQUE (P_Id,LastName)
		/// @note Unique Constraint
		QListIterator<AbstractEntity::ConstraintList::value_type> uk(entity->getUniqueKeyList());
		while (uk.hasNext()) {
			const AbstractEntity::Constraint& constraint(uk.next());
			q << "," << Qt::endl << "CONSTRAINT " << constraint.first << " UNIQUE(";
			QSetIterator<AbstractEntity::FieldSet::value_type> u(constraint.second);
			while (u.hasNext()) {
				q << u.next().get().name() << (u.hasNext() ? ", " : "");
			}
			q << ")";
		}

		/// @note Foreign Key
		/// @brief CONSTRAINT fk_PerOrders FOREIGN KEY (P_Id) REFERENCES Persons(P_Id)
		QListIterator<AbstractEntity::ForeignList::value_type> fk(entity->getForeignKeyList());
		while (fk.hasNext()) {
			const ForeignKey& foreign_key(fk.next().get());
			QList<QPair<QString, QString>> pair_list(foreign_key.makePairList());

			QStringList local_list, foreign_list;
			QListIterator<QPair<QString, QString>> p(pair_list);
			while (p.hasNext()) {
				const QPair<QString, QString>& pair(p.next());
				local_list += pair.first;
				foreign_list += pair.second;
			}
			q << "," << Qt::endl << "CONSTRAINT " << foreign_key.getAlias() << " FOREIGN KEY(" << local_list.join(", ") << ") REFERENCES " << foreign_key.getForeignEntityName() << "(" << foreign_list.join(", ") << ")";
		}

		/// @todo Unique Constraint
		/// @brief CONSTRAINT chk_Person CHECK (P_Id>0 AND City='Sandnes')

		/// @note Закрывашка CREATE TABLE
		q << Qt::endl << ")";

		QSqlQuery query(prepare(q));
		if (!query.exec()) {
			qCritical() << "Failed to create table: " << query.lastError();
			return false;
		}
		return true;
	}

	bool Factory::drop(AbstractEntity* entity, bool safe_mode)
	{
		_affected_rows_count = 0;

		QueryStream q;
		q << "DROP TABLE" << (safe_mode ? " IF NOT EXISTS " : " ") << entity->getAlias() << Qt::endl;
		QSqlQuery query(prepare(q));
		if (!query.exec()) {
			qCritical() << "Failed to drop table: " << query.lastError();
			return false;
		}
		return true;
	}

	bool Factory::truncate(AbstractEntity* entity)
	{
		_affected_rows_count = 0;
		/*
		 * @brief Unfortunately, we do not have TRUNCATE TABLE command in SQLite but you can use SQLite DELETE command
		 * to delete complete data from an existing table, though it is recommended to use DROP TABLE command to drop
		 * the complete table and re-create it once again.
		 */
//		QSqlQuery query(prepare("TRUNCATE TABLE " + entity->getAlias()));
		QSqlQuery query(prepare("DELETE FROM " + entity->getAlias()));
		if (!query.exec()) {			
			qCritical() << "Failed to truncate table: " << query.lastError();
			return false;
		}
		_affected_rows_count = query.numRowsAffected();
		return true;
	}

	bool Factory::add(AbstractEntity* entity, const Field& field)
	{
		/// @brief ALTER TABLE Repository_Object ADD COLUMN FAVOURITE BOOLEAN DEFAULT false
		QString sql_type;
		switch (field.getValueType()) {
			case Field::Type::Boolean:
				sql_type = "BOOLEAN";
				break;

			default:
				return false;
		}

		QueryStream q;
		q << "ALTER TABLE " << entity->getAlias() << " ADD COLUMN " << field.name() << " " << sql_type;
		if (field.defaultValue().isValid()) {
			q << " DEFAULT " << field.value().toString();
		}

		QSqlQuery query(prepare(q));
		if (!query.exec()) {
			qCritical() << "Failed to add column to the table: " << query.lastError();
			return false;
		}
		return true;
	}

	bool Factory::insert(AbstractEntity* entity, Factory::InsertionPolicy policy)
	{
		_affected_rows_count = 0;
		QStringList name_list, bind_list;
		QMapIterator<AbstractEntity::FieldMap::key_type, AbstractEntity::FieldMap::mapped_type> f(entity->getFieldMap());
		while (f.hasNext()) {
			const Field& field(f.next().value().get());
			if (field.isGenerated()) {
				name_list += field.name();
				bind_list += "?";
			}
		}
		f.toFront();

		QueryStream q;
		switch (policy) {
			case Factory::InsertionPolicy::Replace:
				q << "INSERT OR REPLACE INTO ";
				break;

			case Factory::InsertionPolicy::Ignore:
				q << "INSERT OR IGNORE INTO ";
				break;

			default:
				q << "INSERT INTO ";
		}
		q << entity->getAlias() << Qt::endl << "(" << name_list.join(", ") << ")" << Qt::endl << "VALUES(" << bind_list.join(", ") << ")" << Qt::endl;
		QSqlQuery query(prepare(q));

		while (f.hasNext()) {
			const Field& field(f.next().value().get());
			if (field.isGenerated()) {
				const QMetaType::Type meta_type(static_cast<QMetaType::Type>(field.type()));
				const Field::Type value_type(field.getValueType());
				switch (meta_type) {
					case QMetaType::QJsonDocument:
						query.addBindValue(QString::fromUtf8(field.value().toJsonDocument().toJson(QJsonDocument::Compact)));
						break;
					case QMetaType::QUuid:
						query.addBindValue(QUuid(field.value().toString()).toString());
						break;
					case QMetaType::QDateTime:
						if (Field::Type::Timestamp == value_type) {
							query.addBindValue(field.value().toDateTime().toSecsSinceEpoch());
						} else {
							query.addBindValue(field.value().toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
						}
						break;
					case QMetaType::LongLong: /// < Это такой <int32
						query.addBindValue(field.value().toLongLong());
						break;
					default:
						query.addBindValue(field.value());
				}
			}
		}

		if (!query.exec()) {
			qCritical() << "Failed to insert: " << query.lastError();
			return false;
		}
		_affected_rows_count = query.numRowsAffected();
		return true;
	}

	bool Factory::insert(AbstractEntity* entity, const QJsonArray& rowset, Factory::InsertionPolicy policy)
	{
		_affected_rows_count = 0;
		QStringList name_list, bind_list;
		QMapIterator<AbstractEntity::FieldMap::key_type, AbstractEntity::FieldMap::mapped_type> f(entity->getFieldMap());
		QVector<QVariantList> batch_vector;
		while (f.hasNext()) {
			const Field& field(f.next().value().get());
			const QMetaType::Type meta_type(static_cast<QMetaType::Type>(field.type()));
			const Field::Type value_type(field.getValueType());
			name_list += field.name();
			bind_list += "?";

			batch_vector.push_back(QVariantList());
			QVariantList& column_list(batch_vector.back());

			QJsonArray::const_iterator r(rowset.constBegin());
			QJsonObject obj(r->toObject());
			if (obj.find(field.name()) == obj.constEnd()) {
				/// @note Берем дефолтное значение
				for (; r != rowset.constEnd(); obj = (++r)->toObject()) {
					column_list << field.defaultValue();
				}
			} else if (QMetaType::QJsonDocument == meta_type) {
				for (; r != rowset.constEnd(); obj = (++r)->toObject()) {
					column_list << QString::fromUtf8(QJsonDocument(obj[field.name()].toObject()).toJson(QJsonDocument::Compact));
				}
			} else if (QMetaType::QUuid == meta_type) {
				for (; r != rowset.constEnd(); obj = (++r)->toObject()) {
					column_list << QUuid(obj[field.name()].toString()).toString();
				}
			} else if (QMetaType::LongLong == meta_type && r->toObject()[field.name()].isString()) { /// < Это такой <int32
				for (; r != rowset.constEnd(); obj = (++r)->toObject()) {
					column_list << obj[field.name()].toString("0").toLongLong();
				}
			} else if (QMetaType::QDateTime == meta_type && Field::Type::Timestamp == value_type) {
				for (; r != rowset.constEnd(); obj = (++r)->toObject()) {
					column_list << QDateTime::fromSecsSinceEpoch(obj[field.name()].toInt(), Qt::UTC).toSecsSinceEpoch(); /// @ocd
				}
			} else if (QMetaType::QDateTime == meta_type) {
				for (; r != rowset.constEnd(); obj = (++r)->toObject()) {
					column_list << QDateTime::fromSecsSinceEpoch(obj[field.name()].toInt(), Qt::UTC).toString("yyyy-MM-dd hh:mm:ss.zzz");
				}
			} else {
				for (; r != rowset.constEnd(); obj = (++r)->toObject()) {
					column_list << obj[field.name()].toVariant();
				}
			}
		}

		QueryStream q;
		switch (policy) {
			case Factory::InsertionPolicy::Replace:
				q << "INSERT OR REPLACE INTO ";
				break;

			case Factory::InsertionPolicy::Ignore:
				q << "INSERT OR IGNORE INTO ";
				break;

			default:
				q << "INSERT INTO ";
		}
		q << entity->getAlias() << Qt::endl << "(" << name_list.join(", ") << ")" << Qt::endl << "VALUES(" << bind_list.join(", ") << ")" << Qt::endl;
		QSqlQuery query(prepare(q));

		QVectorIterator<QVariantList> r(batch_vector);
		while (r.hasNext()) {
			query.addBindValue(r.next());
		}

		if (!query.execBatch()) {
			qCritical() << "Failed to insert: " << query.lastError() << " with query: " << q;
			return false;
		}
		_affected_rows_count = query.numRowsAffected();
		return true;
	}

	bool Factory::insert(AbstractEntity* entity, const QList<QSqlRecord>& record_list, Factory::InsertionPolicy policy)
	{
		_affected_rows_count = 0;
		const int field_count(record_list.isEmpty() ? 0 : record_list.first().count());
		if (field_count == 0) {
			return false;
		}

		QList<QPair<int, QVariantList>> index_list;
		QList<AbstractEntity::FieldMap::ConstIterator> field_list;
		QStringList name_list, bind_list;
		for (int f = 0; f < field_count; ++f) {
			QSqlField field(record_list.first().field(f));
			if (field.isGenerated()) {
				field_list.push_back(entity->getFieldMap().find(field.name()));
				if (entity->getFieldMap().constEnd() == field_list.last()) {
					field_list.pop_back();
				} else {
					name_list << field.name();
					bind_list << "?";
					index_list << qMakePair(f, QVariantList{});
				}
			}
		}

		if (field_list.isEmpty()) {
			return false;
		}

		QueryStream q;
		switch (policy) {
			case Factory::InsertionPolicy::Replace:
				q << "INSERT OR REPLACE INTO ";
				break;

			case Factory::InsertionPolicy::Ignore:
				q << "INSERT OR IGNORE INTO ";
				break;

			default:
				q << "INSERT INTO ";
		}
		q << entity->getAlias() << Qt::endl << "(" << name_list.join(", ") << ")" << Qt::endl << "VALUES(" << bind_list.join(", ") << ")" << Qt::endl;
		QSqlQuery query(prepare(q));

		QListIterator<QSqlRecord> r(record_list);
		while (r.hasNext()) {
			const QSqlRecord& query_record(r.next());
			for (int i = 0; i < index_list.count(); i++) {
//			for (QList<QPair<int, QVariantList>>::value_type& i : index_list) {
				QPair<int, QVariantList>& p(index_list[i]);
				const QSqlField& query_field(query_record.field(p.first));
				const Field& entity_field(field_list.at(i).value());
				const QMetaType::Type meta_type(static_cast<QMetaType::Type>(entity_field.type()));
				const Field::Type value_type(entity_field.getValueType());
				switch (meta_type) {
					case QMetaType::QJsonDocument:
						p.second << QString::fromUtf8(query_field.value().toJsonDocument().toJson(QJsonDocument::Compact));
						break;

					case QMetaType::QUuid:
						p.second << QUuid(query_field.value().toString()).toString();
						break;

					case QMetaType::QDateTime:
						if (Field::Type::Timestamp == value_type) {
							p.second << query_field.value().toDateTime().toSecsSinceEpoch();
						} else {
							p.second << query_field.value().toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
						}
						break;

					case QMetaType::LongLong: /// < Это такой <int32
						p.second << query_field.value().toLongLong();
						break;

					default:
						p.second << query_field.value();
				}
			}
		}

		for (QList<QPair<int, QVariantList>>::value_type& i : index_list) {
			query.addBindValue(i.second);
		}

		if (!query.execBatch()) {
			qCritical() << "Failed to insert: " << query.lastError() << " with query: " << q;
			return false;
		}
		_affected_rows_count = query.numRowsAffected();
		return true;
	}

	bool Factory::insert(AbstractEntity* entity, const QSqlRecord& record, Factory::InsertionPolicy policy)
	{
		return insert(entity, QList{record}, policy);
	}

	bool Factory::update(AbstractEntity* entity)
	{
		_affected_rows_count = 0;
		CompoundPredicate predicate(CompoundPredicate::Logic::And);
		for (const std::reference_wrapper<Field>& field : entity->getPrimaryKey().second) {
			predicate.append(
								QSharedPointer<ConditionPredicate>::create(
																			field.get(),
																			Field(field.get().getValueType(), field.get().value())
								)
			);
		}
		return !predicate.isEmpty() ? update(entity, &predicate) : false;
	}

	bool Factory::update(AbstractEntity* entity, WherePredicate* predicate)
	{
		_affected_rows_count = 0;
		QMapIterator<AbstractEntity::FieldMap::key_type, AbstractEntity::FieldMap::mapped_type> f(entity->getFieldMap());
		QStringList field_list;
		while (f.hasNext()) {
			const Field& field(f.next().value().get());
			if (field.isGenerated()) {
				field_list << field.name() + "=?";
			}
		}
		f.toFront();

		QueryStream q;
		q << "UPDATE " << entity->getAlias() << Qt::endl << " SET " << field_list.join(", ") << (predicate ? " WHERE " + predicate->makeSQL() : "") << Qt::endl;

		QSqlQuery query(prepare(q));
		while (f.hasNext()) {
			const Field& field(f.next().value().get());
			const Field::Type value_type(field.getValueType());
			if (field.isGenerated()) {
				const QMetaType::Type meta_type(static_cast<QMetaType::Type>(field.type()));
				switch (meta_type) {
					case QMetaType::QJsonDocument:
						query.addBindValue(QString::fromUtf8(field.value().toJsonDocument().toJson(QJsonDocument::Compact)));
						break;

					case QMetaType::QUuid:
						query.addBindValue(QUuid(field.value().toString()).toString());
						break;

					case QMetaType::LongLong:
						/// @note Это такой <int32
						query.addBindValue(field.value().toLongLong());
						break;

					case QMetaType::QDateTime:
						if (Field::Type::Timestamp == value_type) {
							query.addBindValue(field.value().toDateTime().toSecsSinceEpoch());
						} else {
							query.addBindValue(field.value().toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
						}
						break;

					default:
						query.addBindValue(field.value());
				}
			}
		}
		if (!query.exec()) {
			qCritical() << "Failed to update: " << query.lastError() << " with query: " << q;
			return false;
		}
		_affected_rows_count = query.numRowsAffected();
		return true;
	}

	bool Factory::update(AbstractEntity* entity, const QSqlRecord& record, WherePredicate* predicate)
	{
		_affected_rows_count = 0;
		QStringList name_list;

		const int field_count(record.count());
		for (int f=0; f <field_count; ++f) {
			QSqlField field(record.field(f));
			if (field.isGenerated()) {
				name_list << field.name();
			}
		}

		QueryStream q;
		q << "UPDATE " << entity->getAlias() << Qt::endl << " SET " << name_list.join("=?, ") << "=?" << (predicate ? " WHERE " + predicate->makeSQL() : "") << Qt::endl;
		QSqlQuery query(prepare(q));

		QStringListIterator n(name_list);
		while (n.hasNext()) {
			const QSqlField query_field(record.field(n.next()));
			AbstractEntity::FieldMap::ConstIterator f(entity->getFieldMap().find(query_field.name()));
			if (entity->getFieldMap().constEnd() == f) {
				continue;
			}
			const Field& entity_field(f.value().get());
			const QMetaType::Type meta_type(static_cast<QMetaType::Type>(entity_field.type()));
			const Field::Type value_type(entity_field.getValueType());
			switch (meta_type) {
				case QMetaType::QJsonDocument:
					query.addBindValue(QString::fromUtf8(query_field.value().toJsonDocument().toJson(QJsonDocument::Compact)));
					break;

				case QMetaType::QUuid:
					query.addBindValue(QUuid(query_field.value().toString()).toString());
					break;
				case QMetaType::QDateTime:
					if (Field::Type::Timestamp == value_type) {
						query.addBindValue(query_field.value().toDateTime().toSecsSinceEpoch());
					} else {
						query.addBindValue(query_field.value().toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
					}
					break;
				case QMetaType::LongLong: /// < Это такой <int32
					query.addBindValue(query_field.value().toLongLong());
					break;
				default:
					query.addBindValue(query_field.value());
			}
		}

		if (!query.exec()) {
			qCritical() << "Failed to update: " << query.lastError() << " with query: " << q;
			return false;
		}
		_affected_rows_count = query.numRowsAffected();
		return true;
	}

	bool Factory::remove(AbstractEntity* entity, WherePredicate* predicate)
	{
		_affected_rows_count = 0;
		QueryStream q;
		q << "DELETE FROM " << entity->getAlias() << (predicate ? " WHERE " + predicate->makeSQL() : "") << Qt::endl;

		QSqlQuery query(prepare(q));
		if (!query.exec()) {
			qCritical() << "Failed to delete: " << query.lastError() << " with query: " << q;
			return false;
		}
		_affected_rows_count = query.numRowsAffected();
		return true;
	}

	bool Factory::transact(TransactionPolicy policy)
	{
		if (TransactionPolicy::None != _transaction || !_provider->_database.transaction()) {
			return false;
		}
		_transaction = policy;
		return true;
	}

	bool Factory::commit()
	{
		if (TransactionPolicy::None == _transaction) {
			return false;
		}
		_transaction = TransactionPolicy::None;
		return _provider->_database.commit();
	}

	bool Factory::rollback()
	{
		if (TransactionPolicy::None == _transaction) {
			return false;
		}
		_transaction = TransactionPolicy::None;
		return _provider->_database.rollback();
	}

	qlonglong Factory::fetchAutoIncrement(Field& field)
	{
		if (field.isStandalone()) {
			return 0;
		}

		/// @todo Other DBMS
		QueryStream q;
		q << "SELECT " << field.getAlias() << " FROM " << field.getEntityAlias() << "WHERE rowid=last_insert_rowid()" << Qt::endl;
		QSqlQuery query(prepare(q));
		while (query.next()) {
			return query.value(0).toLongLong();
		}
		return 0;
	}

	const int& Factory::getAffectedRowsCount() const
	{
		return _affected_rows_count;
	}
} } } }
