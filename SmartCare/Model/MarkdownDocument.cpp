#include "MarkdownDocument.hpp"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDir>
#include <QFileInfo>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {

MarkdownDocument::MarkdownDocument(QObject* parent)
	: QAbstractListModel(parent)
{
}

int MarkdownDocument::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return _elements.count();
}

QVariant MarkdownDocument::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || index.row() >= _elements.count())
		return QVariant();

	const auto& element = _elements[index.row()];
	switch (role) {
	case TypeRole:
		return element.type;
	case ContentRole:
		return element.content;
	case AttributesRole:
		return element.attributes;
	default:
		return QVariant();
	}
}

QHash<int, QByteArray> MarkdownDocument::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[TypeRole] = "type";
	roles[ContentRole] = "content";
	roles[AttributesRole] = "attributes";
	return roles;
}

QUrl MarkdownDocument::getPath() const
{
	return _path;
}

QString MarkdownDocument::getName() const
{
	return _name;
}

bool MarkdownDocument::isDirectory() const
{
	return _is_directory;
}

void MarkdownDocument::setPath(const QUrl& url)
{
	if (_path == url)
		return;

	_path = url;

	QFileInfo fileInfo(_path.toLocalFile());
	_name = fileInfo.fileName();
	if (_name.isEmpty())
		_name = _path.toString();

	loadContent();
	emit pathChanged();
}

void MarkdownDocument::setParentDocument(MarkdownDocument* parentDoc)
{
	_parent_document = parentDoc;
}

MarkdownDocument* MarkdownDocument::parentDocument() const
{
	return _parent_document;
}

void MarkdownDocument::addChildDocument(MarkdownDocument* child)
{
	if (child) {
		child->setParentDocument(this);
		child->setParent(this);
		_children_documents.append(child);
	}
}

const QList<MarkdownDocument*>& MarkdownDocument::childrenDocuments() const
{
	return _children_documents;
}

void MarkdownDocument::loadContent()
{
	beginResetModel();
	_elements.clear();
	qDeleteAll(_children_documents);
	_children_documents.clear();
	endResetModel();

	QFileInfo fileInfo(_path.toLocalFile());
	if (!fileInfo.exists())
		return;

	_is_directory = fileInfo.isDir();

	if (_is_directory) {
		// Directory: Show sub-directories and files as links (like in the first version)
		QDir dir(_path.toLocalFile());
		QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries, QDir::Name);

		beginResetModel();
		for (const QFileInfo& entry : entries) {
			// Create a link element for the directory listing
			MarkdownElement element;
			element.type = "link";
			element.content = entry.fileName();
			element.attributes["url"] = QUrl::fromLocalFile(entry.absoluteFilePath());
			_elements.append(element);

			// Also create a child document for the hierarchical structure
			MarkdownDocument* childDoc = new MarkdownDocument(this);
			childDoc->setPath(QUrl::fromLocalFile(entry.absoluteFilePath()));
			addChildDocument(childDoc);
		}
		endResetModel();

	} else {
		// File: Parse markdown content
		QFile file(_path.toLocalFile());
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream stream(&file);
			QString content = stream.readAll();
			file.close();

			beginResetModel();
			_elements = processMarkdown(content);
			endResetModel();
		}
	}
}

QList<MarkdownDocument::MarkdownElement> MarkdownDocument::processMarkdown(const QString& content)
{
	QList<MarkdownElement> elements;
	const auto lines = content.split('\n');
	for (int i = 0; i < lines.size(); ++i) {
		QString line = lines[i];

		if (line.trimmed().startsWith('|')) {
			elements.append(parseTable(lines, i));
		} else if (line.startsWith('#')) {
			elements.append(parseHeader(line));
		} else if (line.startsWith('![')) {
			elements.append(parseImage(line));
		} else if (line.startsWith('[')) {
			elements.append(parseLink(line));
		} else if (!line.trimmed().isEmpty()) {
			elements.append(parseParagraph(line));
		}
	}
	return elements;
}

MarkdownDocument::MarkdownElement MarkdownDocument::parseHeader(const QString& line)
{
	MarkdownElement element;
	int headerLevel = line.count('#');
	element.type = "header";
	element.content = line.mid(headerLevel).trimmed();
	element.attributes["level"] = headerLevel;
	return element;
}

MarkdownDocument::MarkdownElement MarkdownDocument::parseParagraph(const QString& line)
{
	MarkdownElement element;
	element.type = "paragraph";

	QRegularExpression boldRegex(R"(\*\*(.+?)\*\*)");
	QString formattedContent = line;
	formattedContent.replace(boldRegex, "<b>\\1</b>");

	element.content = formattedContent.trimmed();
	return element;
}

MarkdownDocument::MarkdownElement MarkdownDocument::parseLink(const QString& line)
{
	MarkdownElement element;
	element.type = "link";
	QRegularExpression linkRegex(R"(\[(.+)\]\((.+)\))");
	auto match = linkRegex.match(line);
	if (match.hasMatch()) {
		element.content = match.captured(1);
		element.attributes["url"] = QUrl::fromUserInput(match.captured(2));
	}
	return element;
}

MarkdownDocument::MarkdownElement MarkdownDocument::parseImage(const QString& line)
{
	MarkdownElement element;
	element.type = "image";

	QRegularExpression imageRegex(R"(!\[(.*)\]\((.+)\))");
	auto match = imageRegex.match(line);
	if (match.hasMatch()) {
		element.content = match.captured(1);
		element.attributes["src"] = QUrl::fromUserInput(match.captured(2));
	}
	return element;
}

MarkdownDocument::MarkdownElement MarkdownDocument::parseTable(const QStringList& lines, int& currentLineIndex)
{
	MarkdownElement element;
	element.type = "table";

	QList<QVariantMap> rows;
	int columnCount = 0;
	bool isHeaderParsed = false;

	while (currentLineIndex < lines.size() && lines[currentLineIndex].trimmed().startsWith('|')) {
		QString line = lines[currentLineIndex];
		QStringList columns = line.split('|', Qt::SkipEmptyParts);
		QList<QVariantMap> cells;

		if (columnCount == 0) {
			columnCount = columns.size();
		}

		for (const QString& column : columns) {
			QVariantMap cell;
			cell["content"] = column.trimmed();
			cell["isHeader"] = !isHeaderParsed;
			cells.append(cell);
		}

		QVariantMap row;
		row["cells"] = QVariant::fromValue(cells);
		row["isHeader"] = !isHeaderParsed;

		rows.append(row);
		isHeaderParsed = true;
		currentLineIndex++;
	}

	element.attributes["rows"] = QVariant::fromValue(rows);
	element.attributes["columnCount"] = columnCount;

	// Adjust current line index back by one
	currentLineIndex--;

	return element;
}

}}}} // namespace
