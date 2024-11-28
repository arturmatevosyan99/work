#include "Body.hpp"

#include "Skeleton.hpp"

#include <QDir>
#include <QFile>
#include <QtGlobal>
#include <QSysInfo>
#include <QStandardPaths>
#include <QGuiApplication>
#include <QCoreApplication>

#include <QDebug>

namespace Cadabra { namespace Queen {
	struct NativeBack
	{
		static void MessageLog(QtMsgType type, const QMessageLogContext& context, const QString& message)
		{
			Body::SystemMessageHandler(type, context, message);
			const char* file(context.file ? context.file : "");
			const char* function(context.function ? context.function : "");
			switch (type)
			{
				case QtDebugMsg:
//					fprintf(stdout, "[D] %s> %s (%s:%u)\n", function, message.toLocal8Bit().constData(), file, context.line);
//					fflush(stdout);
					QMetaObject::invokeMethod(Skeleton::ActiveInstance(), "debugMessaged", Qt::DirectConnection, Q_ARG(QString, context.file), Q_ARG(QString, context.function), Q_ARG(int, context.line), Q_ARG(QString, message));
					break;

				case QtInfoMsg:
//					fprintf(stdout, "[I] %s> %s (%s:%u)\n", function, message.toLocal8Bit().constData(), file, context.line);
//					fflush(stdout);
					QMetaObject::invokeMethod(Skeleton::ActiveInstance(), "infoMessaged", Qt::DirectConnection, Q_ARG(QString, context.file), Q_ARG(QString, context.function), Q_ARG(int, context.line), Q_ARG(QString, message));
					break;

				case QtWarningMsg:
//					fprintf(stdout, "[W] %s> %s (%s:%u)\n", function, message.toLocal8Bit().constData(), file, context.line);
//					fflush(stdout);
					QMetaObject::invokeMethod(Skeleton::ActiveInstance(), "warningMessaged", Qt::DirectConnection, Q_ARG(QString, context.file), Q_ARG(QString, context.function), Q_ARG(int, context.line), Q_ARG(QString, message));
					break;

				case QtCriticalMsg:
//					fprintf(stderr, "[C] %s> %s (%s:%u)\n", function, message.toLocal8Bit().constData(), file, context.line);
//					fflush(stderr);
					QMetaObject::invokeMethod(Skeleton::ActiveInstance(), "criticalMessaged", Qt::DirectConnection, Q_ARG(QString, context.file), Q_ARG(QString, context.function), Q_ARG(int, context.line), Q_ARG(QString, message));
					break;

				case QtFatalMsg:
//					fprintf(stderr, "[F] %s> %s (%s:%u)\n", function, message.toLocal8Bit().constData(), file, context.line);
//					fflush(stderr);
					QMetaObject::invokeMethod(Skeleton::ActiveInstance(), "fatalMessaged", Qt::DirectConnection, Q_ARG(QString, context.file), Q_ARG(QString, context.function), Q_ARG(int, context.line), Q_ARG(QString, message));
					break;
			}
		}
	};
	const QtMessageHandler Body::SystemMessageHandler = qInstallMessageHandler(&NativeBack::MessageLog);

	const QDir Body::BaseDirectory = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).at(0);

	Body::Device DeviceType(Body::Platform platform)
	{
		switch (platform) {
			case Body::Platform::macOS:
			case Body::Platform::Linux:
			case Body::Platform::Ubuntu:
			case Body::Platform::Debian:
			case Body::Platform::Alpine:
			case Body::Platform::Berkley:
			case Body::Platform::FreeBSD:
			case Body::Platform::OpenBSD:
			case Body::Platform::DragonFly:
			case Body::Platform::Windows:
				return Body::Device::Desktop;

			case Body::Platform::iOS:
			case Body::Platform::Android:
				return Body::Device::Smartphone;

			case Body::Platform::watchOS:
			case Body::Platform::wearOS:
				return Body::Device::Watchface;

			case Body::Platform::tvOS:
				return Body::Device::Desktop;

			default:
				return Body::Device::Unknown;
		}
	}

	Body::Body(Platform platform, const QString& bundle_identifier, const QString& native_identifier)
	:
		QObject(nullptr),

		_product_name(QSysInfo::prettyProductName().toLatin1().data()),
		_host_name(QSysInfo::machineHostName()),
		_platform_type(platform),
		_device_type(DeviceType(platform)),
		_bundle_id(bundle_identifier),
		_machine_id(QSysInfo::machineUniqueId()),
		_boot_id(QSysInfo::bootUniqueId().data()),
		_native_id(native_identifier),
		_data_directory(Body::BaseDirectory)
	{
		if (platform == Body::Platform::macOS) {
			_data_directory.setPath(_data_directory.absoluteFilePath(_bundle_id));
		}
	}

	Body::~Body()
	{

	}

	const QString& Body::getApplicationName() const
	{
		return Skeleton::ActiveInstance()->getApplicationName();
	}

	const QString& Body::getSkeletonName() const
	{
		return Skeleton::ActiveInstance()->getTargetName();
	}

	const QString& Body::getProductName() const
	{
		return _product_name;
	}

	const QString& Body::getHostName() const
	{
		return _host_name;
	}

	const QString& Body::getPlatformName() const
	{
		return Skeleton::ActiveInstance()->getPlatformName();
	}

	const Body::Platform& Body::getPlatformType() const
	{
		return _platform_type;
	}

	const Body::Device& Body::getDeviceType() const
	{
		return _device_type;
	}

	const QString& Body::getBundleIdentifier() const
	{
		return _bundle_id;
	}

	const QString& Body::getMachineIdentifier() const
	{
		return _machine_id;
	}

	const QString& Body::getBootIdentifier() const
	{
		return _boot_id;
	}

	const QString& Body::getNativeIdentifier() const
	{
		return _native_id;
	}

	const QString& Body::getFrameworkVersion() const
	{
		return Skeleton::ActiveInstance()->getFrameworkVersion();
	}

	const quint32& Body::getFrameworkSerial() const
	{
		return Skeleton::ActiveInstance()->getFrameworkSerial();
	}

	const QDir& Body::getDataDirectory() const
	{
		return _data_directory;
	}

	bool Body::hasDataDirectory() const
	{
		return _data_directory.exists() && _data_directory.entryInfoList({"."}).at(0).isWritable();
	}
} }
