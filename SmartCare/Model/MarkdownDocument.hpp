#ifndef Cadabra_Queen_SmartCare_Model_MarkdownDocument_hpp
#define Cadabra_Queen_SmartCare_Model_MarkdownDocument_hpp

#include <QAbstractListModel>
#include <QList>
#include <QVariantMap>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {

class MarkdownDocument : public QAbstractListModel
{
	/** @name Classes */
	/** @{ */
	private:
		struct MarkdownElement {
			QString type;				// "header", "paragraph", "link", "image", "table"
			QString content;			// Content
			QVariantMap attributes;		// Additional attributes
		};
	/** @} */

	/** @name Properties */
	/** @{ */
	Q_OBJECT
	Q_PROPERTY(QUrl path READ getPath WRITE setPath NOTIFY pathChanged)
	Q_PROPERTY(QString name READ getName NOTIFY pathChanged)
	Q_PROPERTY(bool isDirectory READ isDirectory NOTIFY pathChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit MarkdownDocument(QObject* parent = nullptr);
		virtual ~MarkdownDocument() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QUrl _path;
		QString _name;
		bool _is_directory = false;
		QList<MarkdownElement> _elements;
		MarkdownDocument* _parent_document = nullptr;
		QList<MarkdownDocument*> _children_documents;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QHash<int, QByteArray> roleNames() const override;

		QUrl getPath() const;
		QString getName() const;
		bool isDirectory() const;

		Q_INVOKABLE void setPath(const QUrl& url);

		void setParentDocument(MarkdownDocument* parentDoc);
		MarkdownDocument* parentDocument() const;
		void addChildDocument(MarkdownDocument* child);
		const QList<MarkdownDocument*>& childrenDocuments() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void pathChanged();
	/** @} */

	/** @name Hooks */
	/** @{ */
	private:
		void loadContent();
		QList<MarkdownElement> processMarkdown(const QString& content);
		MarkdownElement parseHeader(const QString& line);
		MarkdownElement parseParagraph(const QString& line);
		MarkdownElement parseLink(const QString& line);
		MarkdownElement parseImage(const QString& line);
		MarkdownElement parseTable(const QStringList& lines, int& currentLineIndex);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		enum Roles {
			TypeRole = Qt::UserRole + 1,
			ContentRole,
			AttributesRole
		};
	/** @} */
};

}}}} // namespace

#endif
