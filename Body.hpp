#ifndef Cadabra_Queen_Body_hpp
#define Cadabra_Queen_Body_hpp

#include <QDir>
#include <QFile>
#include <QUuid>
#include <QString>
#include <QLoggingCategory>

namespace Cadabra { namespace Queen {
	class Body : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Platform : quint8
		{
			Unknown = 0,

			iOS = 11,
			Android = 12,

			iPadOS = 21,

			tvOS = 31,

			watchOS = 41,
			wearOS = 42,

			Darwin = 50,
			macOS = 51,

			Linux = 60,
			Ubuntu = 61,
			Debian = 62,
			Alpine = 63,

			Berkley = 70,
			FreeBSD = 71,
			OpenBSD = 72,
			DragonFly = 73,

			Windows = 80
		};

		enum class Device : quint8
		{
			Unknown = 0,

			Smartphone = 10,
			Tablet = 20,
			TV = 30,
			Watchface = 40,
			Desktop = 50,
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QtMessageHandler SystemMessageHandler;
		static const QDir BaseDirectory;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Platform)
		Q_ENUM(Device)
		Q_PROPERTY(Platform platformType READ getPlatformType CONSTANT)
		Q_PROPERTY(Device deviceType READ getDeviceType CONSTANT)
		Q_PROPERTY(QString skeletonName READ getSkeletonName CONSTANT)
		Q_PROPERTY(QString productName READ getProductName CONSTANT)
		Q_PROPERTY(QString platformName READ getPlatformName CONSTANT)
		Q_PROPERTY(QString hostName READ getHostName CONSTANT)
		Q_PROPERTY(QString bundleIdentifier READ getBundleIdentifier CONSTANT)
		Q_PROPERTY(QString machineIdentifier READ getMachineIdentifier CONSTANT)
		Q_PROPERTY(QString bootIdentifier READ getBootIdentifier CONSTANT)
		Q_PROPERTY(QString nativeIdentifier READ getNativeIdentifier CONSTANT)
		Q_PROPERTY(QString frameworkVersion READ getFrameworkVersion CONSTANT)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Body(Platform platform = Platform::Unknown, const QString& bundle_identifier = "systems.cadabra.queen", const QString& native_identifier = QUuid::createUuid().toString());
		virtual ~Body();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QString _product_name;
		const QString _host_name;
		const Platform _platform_type;
		const Device _device_type;
		const QString _bundle_id;
		const QString _machine_id;
		const QString _boot_id;
		const QString _native_id;

		QDir _data_directory;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getApplicationName() const;
		const QString& getSkeletonName() const;
		const QString& getProductName() const;
		const QString& getHostName() const;
		const QString& getPlatformName() const;
		const Platform& getPlatformType() const;
		const Device& getDeviceType() const;
		const QString& getBundleIdentifier() const;
		const QString& getMachineIdentifier() const;
		const QString& getBootIdentifier() const;
		const QString& getNativeIdentifier() const;
		const QString& getFrameworkVersion() const;
		const quint32& getFrameworkSerial() const;
		const QDir& getDataDirectory() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	private:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	public slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasDataDirectory() const;
	/** @} */
	};
} }

#endif
