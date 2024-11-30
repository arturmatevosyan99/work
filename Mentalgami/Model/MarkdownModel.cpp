#include "MarkdownModel.hpp"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {

QList<MarkdownProcessor::MarkdownElement> MarkdownProcessor::processMarkdown(const QString &content) {
    QList<MarkdownElement> elements;
    const auto lines = content.split('\n');
    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines[i];

        // Проверка на таблицу
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

MarkdownProcessor::MarkdownElement MarkdownProcessor::parseHeader(const QString &line) {
    MarkdownElement element;
    int headerLevel = line.count('#');
    element.type = "header";
    element.content = line.mid(headerLevel).trimmed();
    element.attributes["level"] = headerLevel;
    return element;
}

MarkdownProcessor::MarkdownElement MarkdownProcessor::parseParagraph(const QString &line) {
    MarkdownElement element;
    element.type = "paragraph";

    // Обработка жирного текста
    QRegularExpression boldRegex(R"(\*\*(.+?)\*\*)");
    QString formattedContent = line;
    formattedContent.replace(boldRegex, "<b>\\1</b>"); // Преобразование **текст** в жирный HTML

    element.content = formattedContent.trimmed();
    return element;
}

MarkdownProcessor::MarkdownElement MarkdownProcessor::parseLink(const QString &line) {
    MarkdownElement element;
    element.type = "link";
    QRegularExpression linkRegex(R"(\[(.+)\]\((.+)\))");
    auto match = linkRegex.match(line);
    if (match.hasMatch()) {
        element.content = match.captured(1);
        element.attributes["url"] = match.captured(2);
    }
    return element;
}

MarkdownProcessor::MarkdownElement MarkdownProcessor::parseImage(const QString &line) {
    MarkdownElement element;
    element.type = "image";

    QRegularExpression imageRegex(R"(!\[(.*)\]\((.+)\))");
    auto match = imageRegex.match(line);
    if (match.hasMatch()) {
        element.content = match.captured(1); // Alt текст
        element.attributes["src"] = match.captured(2); // Источник изображения
    }
    return element;
}

MarkdownProcessor::MarkdownElement MarkdownProcessor::parseTable(const QStringList &lines, int &currentLineIndex) {
    MarkdownElement element;
    element.type = "table";

    QList<QVariantMap> cells;
    int columnCount = 0;

    // Получаем заголовки таблицы
    QString headerLine = lines[currentLineIndex];
    QStringList headers = headerLine.split('|', Qt::SkipEmptyParts);
    columnCount = headers.size();

    for (const QString &header : headers) {
        QVariantMap cell;
        cell["content"] = header.trimmed();
        cell["isHeader"] = true;
        cells.append(cell);
    }

    // Проверяем наличие разделителя
    currentLineIndex++;
    if (currentLineIndex < lines.size() && lines[currentLineIndex].contains("---")) {
        // Пропускаем разделитель
        currentLineIndex++;
    }

    // Обрабатываем строки таблицы
    while (currentLineIndex < lines.size() && lines[currentLineIndex].trimmed().startsWith('|')) {
        QString line = lines[currentLineIndex];
        QStringList columns = line.split('|', Qt::SkipEmptyParts);
        for (const QString &column : columns) {
            QVariantMap cell;
            cell["content"] = column.trimmed();
            cell["isHeader"] = false;
            cells.append(cell);
        }
        currentLineIndex++;
    }

    element.attributes["cells"] = QVariant::fromValue(cells);
    element.attributes["columnCount"] = columnCount;

    // Отступаем на один индекс для корректной работы внешнего цикла
    currentLineIndex--;

    return element;
}


// ---------------------------------------------------------------------------

MarkdownModel::MarkdownModel(QObject *parent)
    : QAbstractListModel(parent)
{
    setFilePath("/home/arturvm/workspace/MDF/test1.md");
}

int MarkdownModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_elements.count();
}

QVariant MarkdownModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_elements.count())
        return QVariant();

    const auto &element = m_elements[index.row()];
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

QHash<int, QByteArray> MarkdownModel::roleNames() const
{
    return {
        {TypeRole, "type"},
        {ContentRole, "content"},
        {AttributesRole, "attributes"}
    };
}

QString MarkdownModel::filePath() const
{
    return m_filePath;
}

void MarkdownModel::setFilePath(const QString &filePath)
{
    if (m_filePath == filePath)
        return;

    m_filePath = filePath;
    emit filePathChanged();
    loadMarkdownFile();
}

void MarkdownModel::loadMarkdownFile()
{
    beginResetModel();
    m_elements.clear();
    QFile file(m_filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString content = stream.readAll();
        m_elements = MarkdownProcessor::processMarkdown(content);
        file.close();

        qDebug() << "Loaded elements:" << m_elements.count();
    } else {
        qDebug() << "Failed to open file:" << m_filePath;
    }
    endResetModel();
}

}}}}
