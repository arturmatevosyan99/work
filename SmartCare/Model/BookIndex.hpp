#ifndef Cadabra_Queen_SmartCare_Model_BookIndex_hpp
#define Cadabra_Queen_SmartCare_Model_BookIndex_hpp

#include <QAbstractItemModel>
#include <QUrl>

#include "MarkdownDocument.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {

class BookIndex : public QAbstractItemModel
{
	/** @name Classes */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(MarkdownDocument* rootDocument READ getRootDocument NOTIFY opened)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit BookIndex(QObject* parent = nullptr);
		~BookIndex() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		MarkdownDocument* _root_document;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void open(const QUrl& url);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		MarkdownDocument* getRootDocument() const;
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex& index) const override;
		QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void opened();
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		enum Roles {
			NameRole = Qt::UserRole + 1,
			FilePathRole,
			DocumentRole
		};
	/** @} */
};

}}}} // namespace Cadabra::Queen::SmartCare::Model

#endif // Cadabra_Queen_SmartCare_Model_BookIndex_hpp
