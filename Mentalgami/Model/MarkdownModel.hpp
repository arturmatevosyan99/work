#include <QAbstractListModel>
#include <QStringList>
#include <QVariantMap>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {

class MarkdownProcessor
{
public:
    struct MarkdownElement {
        QString type;               // "header", "paragraph", "table", "link", "image"
        QString content;            // Основное содержимое элемента
        QVariantMap attributes;     // Дополнительные атрибуты (например, строки таблицы, src изображения и т.д.)
    };

    // Обрабатывает содержимое Markdown и возвращает список MarkdownElement
    static QList<MarkdownElement> processMarkdown(const QString &content);

private:
    // Парсинг заголовка
    static MarkdownElement parseHeader(const QString &line);

    // Парсинг абзаца
    static MarkdownElement parseParagraph(const QString &line);

    // Парсинг ссылки
    static MarkdownElement parseLink(const QString &line);

    // Парсинг изображения
    static MarkdownElement parseImage(const QString &line);

    // Парсинг таблицы
    static MarkdownElement parseTable(const QStringList &lines, int &currentLineIndex);
};

// ------------------------------

class MarkdownModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)

public:
    explicit MarkdownModel(QObject *parent = nullptr);

    enum MarkdownRoles {
        TypeRole = Qt::UserRole + 1,
        ContentRole,
        AttributesRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString filePath() const;
    void setFilePath(const QString &filePath);

signals:
    void filePathChanged();

private:
    QString m_filePath;
    QList<MarkdownProcessor::MarkdownElement> m_elements;

    void loadMarkdownFile();
};

}}}}
