#include "BookIndex.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {

BookIndex::BookIndex(QObject* parent)
	: QAbstractItemModel(parent),
	  _root_document(nullptr)
{
}

BookIndex::~BookIndex()
{
	if (_root_document)
		delete _root_document;
}

void BookIndex::open(const QUrl& url)
{
	beginResetModel();

	if (_root_document)
		delete _root_document;

	_root_document = new MarkdownDocument(this);
	_root_document->setPath(url);

	endResetModel();
	emit opened();
}

MarkdownDocument* BookIndex::getRootDocument() const
{
	return _root_document;
}

int BookIndex::rowCount(const QModelIndex& parent) const
{
	MarkdownDocument* parentDoc;

	if (!parent.isValid())
		parentDoc = _root_document;
	else
		parentDoc = static_cast<MarkdownDocument*>(parent.internalPointer());

	return parentDoc ? parentDoc->childrenDocuments().count() : 0;
}

int BookIndex::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return 1;
}

QVariant BookIndex::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	auto* doc = static_cast<MarkdownDocument*>(index.internalPointer());
	if (!doc)
		return QVariant();

	switch (role) {
	case Qt::DisplayRole:
	case NameRole:
		return doc->getName();
	case FilePathRole:
		return doc->getPath().toString();
	case DocumentRole:
		return QVariant::fromValue(doc);
	default:
		return QVariant();
	}
}

QModelIndex BookIndex::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	MarkdownDocument* parentDoc;

	if (!parent.isValid())
		parentDoc = _root_document;
	else
		parentDoc = static_cast<MarkdownDocument*>(parent.internalPointer());

	if (!parentDoc || row < 0 || row >= parentDoc->childrenDocuments().size())
		return QModelIndex();

	MarkdownDocument* childDoc = parentDoc->childrenDocuments().at(row);
	return createIndex(row, column, childDoc);
}

QModelIndex BookIndex::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	MarkdownDocument* childDoc = static_cast<MarkdownDocument*>(index.internalPointer());
	if (!childDoc)
		return QModelIndex();

	MarkdownDocument* parentDoc = childDoc->parentDocument();
	if (!parentDoc || parentDoc == _root_document)
		return QModelIndex();

	MarkdownDocument* grandParentDoc = parentDoc->parentDocument();
	int row = 0;
	if (grandParentDoc)
		row = grandParentDoc->childrenDocuments().indexOf(parentDoc);

	return createIndex(row, 0, parentDoc);
}

QHash<int, QByteArray> BookIndex::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[NameRole] = "name";
	roles[FilePathRole] = "filePath";
	roles[DocumentRole] = "document";
	return roles;
}

}}}} // namespace
