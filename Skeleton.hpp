#ifndef Cadabra_Queen_Skeleton_hpp
#define Cadabra_Queen_Skeleton_hpp

#include <QGuiApplication>

#include "Cupboard.hpp"
#include "Environment.hpp"
#include "Core/System/Preference.hpp"
#include "Core/Meta/PropertyList.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <QMap>
#include <QHash>
#include <QList>
#include <QString>
#include <QVariant>
#include <QByteArray>

#include <QDir>
#include <QFile>

#include <QSettings>
#include <QStandardPaths>

#include <QThread>
#include <QFuture>

#include <QTranslator>
#include <QMessageLogContext>

namespace Cadabra { namespace Queen {
	class Bone;

	class Skeleton : public ::QGuiApplication
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Bone> boneList READ readBoneList CONSTANT)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const bool DebugMode;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static Skeleton* ActiveInstance();
		static QThread* ThreadInstance(const QString& key);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Skeleton(const QString& name, int& argc, char** argv, int flags = ApplicationFlags);
		virtual ~Skeleton();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QString _application_name;
		const QString _target_name;
		const QString _platform_name;
		const QString _framework_version;
		const quint32 _framework_serial;

		QObject* _root_object;

		QQmlApplicationEngine _qml_engine;
		QTranslator _translator;

		Core::Meta::TemplatedPropertyList<Bone> _bone_list;
		QHash<QString, QPointer<QThread>> _thread_hashmap;

		QFile _log_file;

		const QCommandLineOption _log_level_option;
		const QCommandLineOption _boot_mode_option;
		const QCommandLineOption _string_envar_option;
		const QCommandLineOption _integer_envar_option;
		const QCommandLineOption _double_envar_option;

	protected:
		QCommandLineParser _cli_parser;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual bool notify(QObject* receiver, QEvent* event) override;
		virtual bool config();
		virtual bool service();
		virtual bool tune();
		virtual int boot();
		virtual int command();
		virtual bool extend();
		bool extend(Bone* bone);
		bool translate(const QString& language = "");
		int boot(const QUrl& url);

	public:
		virtual int run();
		virtual QFuture<void> disclose(Cupboard& cupboard);
		QQmlListProperty<Bone> readBoneList();
		void rotateLog(QFile& rotation_file);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getApplicationName() const;
		const QString& getTargetName() const;
		const QString& getPlatformName() const;
		const QString& getFrameworkVersion() const;
		const quint32& getFrameworkSerial() const;

		const QQmlEngine& getQMLEngine() const;

		QString getTranslatorLanguage() const;

		Q_INVOKABLE QString getBootMode() const;

		Q_INVOKABLE QStringList getArgumentList() const;
		Q_INVOKABLE QString getCommandArgument(const QString& key, const QString& default_value = QString{}) const;

		Q_INVOKABLE Cadabra::Queen::Core::System::Preference* getPreference() const;
		Q_INVOKABLE QVariant getPreferenceOption(const QString& key, const QVariant& default_value = QVariant{}) const;

		Q_INVOKABLE Cadabra::Queen::Environment* getEnvironment() const;
		Q_INVOKABLE QVariant getEnvironmentVariable(const QString& key, const QVariant& default_value = QVariant{}) const;

	protected:
		const QList<Bone*>& getBoneList() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	protected:
		void addCommandLineOption(const QCommandLineOption& option);

	public:
		Q_INVOKABLE bool setPreferenceOption(const QString& key, const QVariant& value);
		Q_INVOKABLE bool setEnvironmentVariable(const QString& key, const QVariant& value);
	/** @} */

	/** @name Hooks */
	/** @{ */
	private:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void debugMessaged(QString file_name, QString function_name, int code_line, QString message);
		void infoMessaged(QString file_name, QString function_name, int code_line, QString message);
		void warningMessaged(QString file_name, QString function_name, int code_line, QString message);
		void criticalMessaged(QString file_name, QString function_name, int code_line, QString message);
		void fatalMessaged(QString file_name, QString function_name, int code_line, QString message);
	/** @} */

	/** @name Slots */
	/** @{ */
	public slots:
		void requestClose();
		void fileLog(QString file_name, QString function_name, int code_line, QString message);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasDataDirectory() const;
		bool hasLogFile() const;
	/** @} */
	};
} }

#endif
