#ifndef Cadabra_Queen_Core_Net_AbstractResource_hpp
#define Cadabra_Queen_Core_Net_AbstractResource_hpp

#include <QAbstractItemModel>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class AbstractResource : public QAbstractItemModel
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractResource(QObject* parent);
		virtual ~AbstractResource();
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool page(bool fetch_all = false);
		virtual bool order(const QVector<int>& role_vector, bool reverse = false);
		virtual bool order(int role, Qt::SortOrder order);
		virtual void clear();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
		virtual QModelIndex parent(const QModelIndex &child) const override;
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	protected:
	/** @} */

	/** @name Factories */
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
