#ifndef Cadabra_Queen_Core_Data_Factory_hpp
#define Cadabra_Queen_Core_Data_Factory_hpp

#include "Provider.hpp"
#include "AbstractEntity.hpp"
#include "QueryStream.hpp"
#include "Predicate/CompoundPredicate.hpp"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSharedPointer>
#include <QJsonArray>
#include <QList>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class Factory : public QObject
	{
	/** @name Classes */

	public:
		enum class InsertionPolicy : quint8
		{
			Regular = 0,
			Replace = 1,
			Ignore = 2
		};

		enum class TransactionPolicy
		{
			None,
			AutoCommit,
			AutoRollback
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Factory(Provider* provider);
		Factory(Factory&& factory);
		virtual ~Factory();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Provider* _provider;
		TransactionPolicy _transaction;
		int _affected_rows_count;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool create(AbstractEntity* entity, bool safe_mode = false);
		bool drop(AbstractEntity* entity, bool safe_mode = false);
		bool truncate(AbstractEntity* entity);

		bool add(AbstractEntity* entity, const Field& field);

		bool insert(AbstractEntity* entity, Factory::InsertionPolicy policy = Factory::InsertionPolicy::Regular);
		bool insert(AbstractEntity* entity, const QJsonArray& rowset, Factory::InsertionPolicy policy = Factory::InsertionPolicy::Regular);
		bool insert(AbstractEntity* entity, const QList<QSqlRecord>& record_list, Factory::InsertionPolicy policy = Factory::InsertionPolicy::Regular);
		bool insert(AbstractEntity* entity, const QSqlRecord& record, Factory::InsertionPolicy policy = Factory::InsertionPolicy::Regular);
		bool update(AbstractEntity* entity);
		bool update(AbstractEntity* entity, WherePredicate* predicate);
		bool update(AbstractEntity* entity, const QSqlRecord& record, WherePredicate* predicate);
		bool remove(AbstractEntity* entity, WherePredicate* predicate);

		bool transact(TransactionPolicy policy = TransactionPolicy::AutoCommit);
		bool commit();
		bool rollback();

		qlonglong fetchAutoIncrement(Field& field);

	private:
		QSqlQuery prepare(const QueryStream& query);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const int& getAffectedRowsCount() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:		
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
