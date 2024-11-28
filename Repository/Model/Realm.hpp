#ifndef Cadabra_Queen_Repository_Model_Realm_hpp
#define Cadabra_Queen_Repository_Model_Realm_hpp

#include "Hierarchy.hpp"
#include "../Net/AceEndpoint.hpp"

#include "../../Core/System/Preference.hpp"
#include "../../Core/Data/Provider.hpp"

#include <QObject>
#include <QDir>
#include <QStandardPaths>
#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class SessionDescriptor;
	class Realm : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	public:
		enum class SynchronizationMode
		{
			Lazy = 0,
			Lite = 1
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(SynchronizationMode)
		/**
		 * Preference configuration
		 */
		Q_PROPERTY(Cadabra::Queen::Core::System::Preference* preference READ getPreference CONSTANT)
		/**
		 * Backend network manager
		 */
		Q_PROPERTY(Cadabra::Queen::Core::Net::Endpoint* endpoint READ getEndpoint CONSTANT)
		/**
		 * Repository hierarchy (groups, subjects, objects)
		 */
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Hierarchy* hierearchy READ getHierarchy CONSTANT)
		/**
		 * Database access provider
		 */
		Q_PROPERTY(Cadabra::Queen::Core::Data::Provider* dataProvider READ getDataProvider CONSTANT)
		/**
		 * Synchronization mode
		 */
		Q_PROPERTY(SynchronizationMode synchronizationMode READ getSynchronizationMode WRITE setSynchronizationMode NOTIFY synchronizationModeChanged)
		/**
		 * @brief Is initialized and ready to use
		 */
		Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY initChanged)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static QString PreferenceFilename();
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Realm(QObject* parent = nullptr);
		virtual ~Realm();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		bool _is_initialized;
		SynchronizationMode _synchronization_mode;
		QDir _storage_path;

		Core::System::Preference* const _preference;
		Repository::Net::AceEndpoint* const _endpoint;
		Repository::Model::Hierarchy* const _hierarchy;
		Core::Data::Provider* const _data_provider;
		SessionDescriptor* _session_descriptor;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		/**
		 * @brief Mandatory preparation for futher use
		 * Database is preparing and connection establishing, configuration preference is loading
		 * @param session
		 * @return
		 */
		bool initialize(SessionDescriptor* session);
		/**
		 * @brief Reset to defaults for futher reuse
		 * @param clean
		 * @return
		 */
		bool deinitialize(bool clean = false);
		/**
		 * @brief Import remotly assembled Realm
		 * @param file_name
		 * @return
		 */
		bool import(const QString& file_name);

	private:
		bool setup();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		SynchronizationMode getSynchronizationMode() const;
		QDir getStoragePath() const;
		Cadabra::Queen::Core::System::Preference* getPreference() const;
		Cadabra::Queen::Core::Net::Endpoint* getEndpoint() const;
		Cadabra::Queen::Repository::Model::Hierarchy* getHierarchy() const;
		Cadabra::Queen::Repository::Model::SessionDescriptor* getSessionDescriptor() const;
		Cadabra::Queen::Core::Data::Provider* getDataProvider(QThread* thread = nullptr) const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setSynchronizationMode(SynchronizationMode mode);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		/**
		 * @brief Signal "Basic initialization was completed and instance is ready to use"
		 */
		void initialized();
		/**
		 * @brief Signal "Instance was cleaned up and reset to reuse"
		 */
		void deinitialized();
		/**
		 * @brief Signal "Error is occured"
		 * @param message
		 */
		void faulted(const QString& message);
		/**
		 * @brief Signal "General state was changed"
		 * (initialized or deinitialized)
		 */
		void initChanged();
		/**
		 * @brief Signal "Synchonization mode was changed"
		 */
		void synchronizationModeChanged();
		/**
		 * @brief Signal "Local hierarchy was restored from a slip"
		 */
		void restored();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onInitialized();
		void onDeinitialized();
		void onFaulted(const QString& message);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isInitialized() const;
	/** @} */
	};
} } } }

#endif
