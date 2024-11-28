#include "MojiFile.hpp"

#include "../../../Core/System/Native.hpp"

#include "../../../Qaos/Zip.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Net {
	MojiFile::MojiFile(const QString& family)
	:
		Core::Net::QRCFile(),

		_storage_path(QString("%1/%2.cmpack").arg(Core::System::Native::Instance()->getDataDirectory().absolutePath(), family)),
		_family(family)
	{

	}

	MojiFile::State MojiFile::load(const QString& collection)
	{
		if (!_storage_path.mkpath(collection)) {
			return State::Fault;
		} else if (_storage_path.exists(collection + "/.ccsum")) {
			_collection = collection;
			return index() ? State::Cached : State::Fault;
		}
		_collection = collection;
//		load("mentalgami", _family, collection, "cmz"); /// < mentalgami/awardji/com.oblozhko.metabolicplate.cmz
		const State retval(Core::Net::QRCFile::load("Mentalgami", QString("%1.%2.cmz").arg(collection, _family)));
		return State::Cached != retval ? retval : unpack() ? State::Cached : State::Fault;
	}

	bool MojiFile::unpack()
	{
		Qaos::ZipReader zip;
		if (!zip.open(getFile().fileName())) {
			qWarning() << "File error";
			return false;
		} else if (!zip.extract(_storage_path.absoluteFilePath(_collection))) {
			qWarning() << "Extraction error";
			return false;
		}
		const CheckSum check_sum(makeCheckSum());
		QFile check_file(_storage_path.absoluteFilePath(_collection + "/.ccsum"));
		if (!check_file.open(QFile::WriteOnly)) {
			qWarning() << "Checksum file error";
			return false;
		} else if (check_file.write(QString("%1=%2").arg(check_sum.first).arg(check_sum.second).toLatin1()) < 0) {
			qWarning() << "Checksum write error";
			return false;
		} else if (!remove()) {
			qWarning() << "Cleanup failure";
		} else if (!index()) {
			qWarning() << "Index failure";
			return false;
		}
		emit loaded();
		return true;
	}

	bool MojiFile::index()
	{
		QString full_path("%1/%2/index.json"); full_path = full_path.arg(_storage_path.absolutePath(), _collection);
		QFile file(full_path);
		if (!file.open(QFile::ReadOnly)) {
			qWarning() << "File error";
			return false;
		}
		QJsonParseError error;
		const QJsonDocument document(QJsonDocument::fromJson(file.readAll(), &error));
		if (error.error != QJsonParseError::NoError || !document.isObject()) {
			qWarning() << "JSON parser error:" << error.errorString();
			return false;
		}
		_index = document.object();
		return true;
	}

	MojiFile::E MojiFile::locate(const QString& name) const
	{
		if (_collection.isEmpty()) {
			return {};
		}
		E retval{};
		QString full_path("%1/%2/%3.svg"); full_path = full_path.arg(_storage_path.absolutePath(), _collection, name);
		if (!QFile::exists(full_path)) {
			return retval;
		}
		retval.url = QUrl::fromLocalFile(full_path);
		QJsonObject::ConstIterator i(_index.find(name));
		if (_index.constEnd() != i && i->isObject()) {
			const QJsonObject e(i->toObject());
			retval.unicode = e.value("unicode").toInt(0);
			QJsonObject::ConstIterator l(e.find(Skeleton::ActiveInstance()->getTranslatorLanguage().prepend("@")));
			if (e.constEnd() == l) {
				l = e.find("@en");
			}
			if (e.constEnd() == l || !l->isString()) {
				retval.meaning = tr("Unknown award");
			} else {
				retval.meaning = l->toString("");
			}
		}
		return retval;
	}

	MojiFile::E MojiFile::locate(const QChar& code) const
	{
		return {};
	}
} } } }
