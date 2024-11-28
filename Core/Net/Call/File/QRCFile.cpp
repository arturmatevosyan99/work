#include "QRCFile.hpp"

#include "../../../System/Native.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	QRCFile::QRCFile()
	:
		FileCall("XXXXXX.tmp")
	{
		FileCall::setCachePath(Core::System::Native::Instance()->getDataDirectory().absolutePath() + "/queen.qtrcollection", true);
	}

	QRCFile::State QRCFile::load(const QUrl& url)
	{
		return State::Idle;
	}

	QRCFile::State QRCFile::load(const QString& extension, const QString& filename)
	{
		setFilenameTemplate(filename);
		QString bone(extension.toLower()); bone[0] = bone.at(0).toUpper();
		QFile file(QString(":/%1/Asset/File/%2").arg(bone, filename));
		return file.exists() && file.open(QFile::ReadOnly) ? FileCall::load(file.readAll()) : State::Fault;
	}
} } } }
