#include "SmartBookModel.hpp"

#include <QFileInfo>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {

BookIndex::BookIndex(QObject* parent)
:
    QAbstractItemModel(parent),
    _root_node(std::make_unique<Node>("root", ""))
{
}

BookIndex::~BookIndex() = default;

QVariant BookIndex::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const Node* node = static_cast<Node*>(index.internalPointer());

    switch (role) {
        case Qt::DisplayRole:
            return node->name;
        case Qt::ToolTipRole:
            return node->path;
        default:
            return QVariant();
    }
}

Qt::ItemFlags BookIndex::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex BookIndex::index(int row, int column, const QModelIndex& parent) const
{
    if (column != 0 || row < 0) {
        return QModelIndex();
    }

    Node* parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : _root_node.get();

    if (row >= parentNode->children.size()) {
        return QModelIndex();
    }

    return createIndex(row, column, parentNode->children[row].get());
}

QModelIndex BookIndex::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    Node* childNode = static_cast<Node*>(index.internalPointer());
    Node* parentNode = childNode->parent;

    if (!parentNode || parentNode == _root_node.get()) {
        return QModelIndex();
    }

    Node* grandparentNode = parentNode->parent;
    int row = grandparentNode ? grandparentNode->children.indexOf(parentNode) : 0;

    return createIndex(row, 0, parentNode);
}

int BookIndex::rowCount(const QModelIndex& parent) const
{
    const Node* parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : _root_node.get();
    return parentNode->children.size();
}

int BookIndex::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}

void BookIndex::loadDirectory(const QString& path)
{
    beginResetModel();
    _root_node->children.clear();
    populateNode(_root_node.get(), QDir(path));
    endResetModel();
}

void BookIndex::populateNode(Node* node, const QDir& dir)
{
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

    for (const QFileInfo& entry : entries) {
        auto childNode = std::make_unique<Node>(entry.fileName(), entry.absoluteFilePath(), node);

        if (entry.isDir()) {
            populateNode(childNode.get(), QDir(entry.absoluteFilePath()));
        }

        node->children.append(std::move(childNode));
    }
}

} } } }
