#ifndef Cadabra_Queen_Repository_Net_AbstractCursor_hpp
#define Cadabra_Queen_Repository_Net_AbstractCursor_hpp

#include "../../Core/Net/AbstractResource.hpp"

#include "../../Core/Net/Endpoint.hpp"
#include "../Meta/FilterDelegate.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Net {
	class AbstractCursor : public Core::Net::AbstractResource
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
		AbstractCursor(QObject* parent);
		virtual ~AbstractCursor();
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		Meta::FilterDelegate _filter_delegate;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool page(bool fetch_all = false) override;
		virtual bool filter(const Meta::EssenceExpression& expression);
		virtual bool filter(const QVector<Meta::EssenceToken>& token_vector);
		virtual bool order(const QVector<int>& role_vector, bool reverse = false) override;
		virtual bool order(int role, Qt::SortOrder order) override;
		virtual void clear() override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		virtual QModelIndex parent(const QModelIndex& child) const override;
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
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
