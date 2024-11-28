#ifndef Cadabra_Queen_Core_Data_Provider_hpp
#define Cadabra_Queen_Core_Data_Provider_hpp

#include "Command/ProviderConnect.hpp"
#include "Command/ProviderDisconnect.hpp"
#include "Command/ProviderClone.hpp"
#include "AbstractPredicate.hpp"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QString>
#include <QHash>
#include <QThread>
#include <QPointer>
#include <QScopedPointer>
#include <QEvent>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class AbstractEntity;
	class Factory;

	class Provider : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString name READ getName NOTIFY connected)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class RowsetType : int
		{
			Regular = 0,
			Custom = 1,
			Version = 2,
			Shade = 3,
			Tele = 4,
		};
		enum class OpenMode : int
		{
			WithoutReset = 0x01,
			FetchAll = 0x02,
			FetchThrow = 0x04,
			Reverse = 0x08,
			AutoReopen = 0x10
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Provider();
		virtual ~Provider();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QSqlDatabase _database;
		QHash<quintptr, QPointer<Provider>> _thread_cache;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool connect(const QString& file_path, const QString& connection_name = QLatin1String(QSqlDatabase::defaultConnection));
		bool connect(ProviderConnect* command);
		bool connect(ProviderClone* command);
		bool disconnect();
		bool vacuum();
		void flush();

		Provider* fork(QThread* thread = nullptr);
		void finish(QThread* thread = nullptr);

		qsizetype count(AbstractEntity* entity, bool use_predicate = true);
		qsizetype count(AbstractEntity* entity, AbstractPredicate* predicate);

	public slots:
		bool open(AbstractEntity* entity, QFlags<Provider::OpenMode> mode, Provider::RowsetType rowset_type);
		bool fetch(AbstractEntity* entity, int row_index);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QSqlDriver* getDriver() const;
		QString getName() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		Factory makeFactory();
		QSqlDatabase makeDatabase() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void connected();
		void disconnected();
		void connectRequested(QString file_path, QString connection_name);
		void disconnectRequested();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual void customEvent(QEvent* event) override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isExist(AbstractEntity* entity) const;
		bool isEntityExist(const QString& name) const;
		bool hasEntity(AbstractEntity* entity);
		bool hasEntity(const QString& name);
	/** @} */

	/** @name Friends */
	/** @{ */
	friend class Factory;
	/** @} */
	};
} } } }

#endif
