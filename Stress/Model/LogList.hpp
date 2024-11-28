#ifndef Cadabra_Queen_Stress_Model_LogList_hpp
#define Cadabra_Queen_Stress_Model_LogList_hpp

#include <QAbstractListModel>

#include <QList>
#include <QPair>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Stress { namespace System {
	class LogClient;
} } } }

namespace Cadabra { namespace Queen { namespace Stress { namespace Model {
	class LogList : public QAbstractListModel
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			Client = Qt::UserRole + 1,
			Endpoint,
			Alias
		};
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString cache READ getCache NOTIFY flushed)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		LogList(QObject* parent = nullptr);
		virtual ~LogList() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _cache;
		QList<QPair<QUrl, System::LogClient*>> _list;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void flush(QString message);
		Q_INVOKABLE void clear();
		bool append(const QUrl& url);
		bool remove(const QUrl& url);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getCache() const;
		System::LogClient* getLast() const;
		virtual int rowCount(const QModelIndex& parent) const override;
		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	signals:
		void flushed();
	/** @} */
	};
} } } }

#endif
