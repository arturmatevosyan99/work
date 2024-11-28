#include "CubeFile.hpp"

#include "../../../System/Native.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	CubeFile::CubeFile(const QString& license_key)
	:
		FileCall("XXXXXX.tmp")
	{
		FileCall::setCachePath(
								QString("%1/%2.cctube").arg(Core::System::Native::Instance()->getDataDirectory().absolutePath(),
								QString::number(qHash(license_key), 16)),
								true
		);
		setXeader(
					"Cadabra-Cloud-License",
					license_key
		);
		setXeader(
					"Cadabra-Cloud-Agent",
					QString("crn:cc:agent:")
											.prepend(Core::System::Native::Instance()->getBundleIdentifier())
											.append(Core::System::Native::Instance()->getNativeIdentifier())
		);
	}

	CubeFile::State CubeFile::load(const QUrl& url)
	{
		return State::Idle;
	}

	CubeFile::State CubeFile::load(const QString& channel, const QString& collection, const QString& file, const QString& extension)
	{
		setFilenameTemplate(QString("%1-%2-%3.%4").arg(channel, collection, file, extension));
		return FileCall::load(QString("https://cts.cadabra.cloud/sign/f/%1/%2/%3.%4").arg(channel, collection, file, extension));
	}

	CubeFile::State CubeFile::load(const QUuid& uid, const QString& extension)
	{
		setFilenameTemplate(QString("%1.%2").arg(uid.toString(QUuid::WithoutBraces), extension));
		return FileCall::load("https://cts.cadabra.cloud/sign/f/" + getFilenameTemplate());
	}
} } } }
