#include "LogList.hpp"

#include "../Net/TCPLogClient.hpp"

namespace Cadabra { namespace Queen { namespace Stress { namespace Model {
	LogList::LogList(QObject* parent)
	:
		QAbstractListModel(parent)
	{

	}

	LogList::~LogList()
	{
	}

	void LogList::flush(QString message)
	{
		_cache += message + "\n";
		emit flushed();
		for (const QPair<QUrl, System::LogClient*>& pair : _list) {
			if (!pair.second->send(message)) {
				qWarning("Flush error");
			}
		}
	}

	void LogList::clear()
	{
		_cache.clear();
		emit flushed();
	}

	bool LogList::append(const QUrl& url)
	{
		const QUrlQuery query(url.query());
		LogFormat format(LogFormat::Text);
		if (query.hasQueryItem("format")) {
			const QString raw_format(query.queryItemValue("format"));
			if (raw_format.compare("json", Qt::CaseInsensitive) == 0) {
				format = LogFormat::JSON;
			} else if (raw_format.compare("xml", Qt::CaseInsensitive) == 0) {
				format = LogFormat::XML;
			}
		}

		if (url.scheme().startsWith("tcp")) {
			QScopedPointer<Net::TCPLogClient> log_client(new Net::TCPLogClient(format));
			if (!log_client->connect(url.host(), url.port())) {
				return false;
			}
			beginInsertRows(QModelIndex(), _list.count(), _list.count());
			_list.append(qMakePair(url, log_client.take()));
			_list.back().second->setParent(this);
			endInsertRows();
		} else {
			return false;
		}
		return true;
	}

	bool LogList::remove(const QUrl& url)
	{
		for (int p = 0; p < _list.count(); ++p) {
			const QPair<QUrl, System::LogClient*>& pair(_list.at(p));
			if (pair.first == url) {
				beginRemoveRows(QModelIndex(), p, p);
				pair.second->deleteLater();
				_list.removeAt(p);
				endRemoveRows();
				return true;
			}
		}
		return false;
	}

	const QString& LogList::getCache() const
	{
		return _cache;
	}

	System::LogClient* LogList::getLast() const
	{
		return _list.isEmpty() ? nullptr : _list.last().second;
	}

	int LogList::rowCount(const QModelIndex& parent) const
	{
		return parent.isValid() ? 0 : _list.count();
	}

	QVariant LogList::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid() || index.row() >= _list.count()) {
			return QVariant();
		} else if (Role::Client == role) {
			return QVariant::fromValue(_list.at(index.row()).second);
		} else if (Role::Endpoint == role) {
			return _list.at(index.row()).first;
		} else if (Role::Alias == role) {
			const QUrlQuery query(_list.at(index.row()).first.query());
			return query.hasQueryItem("alias") ? query.queryItemValue("alias") : _list.at(index.row()).first;
		}
		return QVariant();
	}

	QHash<int, QByteArray> LogList::roleNames() const
	{
		return {
				{Role::Client, "client"},
				{Role::Endpoint, "endpoint"},
				{Role::Alias, "alias"},
		};
	}
} } } }
