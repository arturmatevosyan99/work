#include "Cupboard.hpp"

#include <QDir>

namespace Cadabra { namespace Queen {
	Cupboard::Cupboard(QObject* parent)
	:
		QObject(parent),

		_file(new QTemporaryFile(QDir::tempPath().append("/XXXXXX.cdb"))),
		_archiver(_file)
	{

	}

	Cupboard::~Cupboard()
	{
		_archiver.close();
	}

	QIODevice& Cupboard::pack()
	{
		if (!_archiver.save()) {
			_file->open();
		}
		return *(_file.data());
	}

	bool Cupboard::file(const QString& name, QIODevice* io_device)
	{
		return _archiver.addFile(name, io_device);
	}

	bool Cupboard::file(const QString& name, const QByteArray& byte_array)
	{
		return _archiver.addFile(name, byte_array);
	}
} }
