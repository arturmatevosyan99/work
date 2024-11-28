#ifndef Cadabra_Queen_Core_System_NotificationCenter_hpp
#define Cadabra_Queen_Core_System_NotificationCenter_hpp

#include <QObject>

#include <functional>

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QString>
#include <QMap>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class NotificationService;
	/**
	 * @brief The NotificationCenter class
	 */
	class NotificationCenter : public QObject
	{
	/** @name Aliases */
	/** @{ */
	public:
		using ServiceFactory = std::function<NotificationService*()>;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class Family : char
		{
			Unknown = 0,
			APN = 'A',
			GCM = 'G', /// @deprecated
			FCM = 'F',
			HCM = 'H',
			RSM = 'R'
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		template <Family F>
		static const ServiceFactory Singleton;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Family)
		Q_PROPERTY(Family family READ getFamily NOTIFY registered)
		Q_PROPERTY(QString token READ getToken NOTIFY registered)
		Q_PROPERTY(bool hasService READ hasService NOTIFY initialized)
		Q_PROPERTY(bool hasPermission READ hasPermission NOTIFY activated)
		Q_PROPERTY(bool hasToken READ hasToken NOTIFY registered)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static NotificationCenter* Instance();
		static QObject* QInstance(QQmlEngine* engine, QJSEngine* script_engine);
	/** @} */

	/** @name Constructors */
	/** @{ */
	protected:
		explicit NotificationCenter();
		virtual ~NotificationCenter();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Family _family;
		NotificationService* _service;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		bool setUp(NotificationCenter::Family family, NotificationService* service);

	public:
		bool setUp(Family family = Family::Unknown);
		bool setUp(const QString& token);
		bool setUp(Family family, const QString& token);

		Q_INVOKABLE void requestToken() const;
		Q_INVOKABLE void updateToken() const;
		Q_INVOKABLE void propagateMessage(const QString& title, const QString& icon, const QString& sound, const QString& body);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Q_INVOKABLE Cadabra::Queen::Core::System::NotificationService* getService(Cadabra::Queen::Core::System::NotificationCenter::Family family) const;
		Q_INVOKABLE NotificationService* getService() const;
		Q_INVOKABLE Family getFamily() const;
		Q_INVOKABLE QString getToken() const;
		Q_INVOKABLE QString getPrefixedToken() const;
		Q_INVOKABLE QString getCNSToken() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void activated();
		void initialized(Family family);
		void registered(QString old_token);
		void received(QString title, QString icon, QString sound, QString body);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasService() const;
		bool hasPermission() const;
		bool hasToken() const;
	/** @} */
	};

	template <NotificationCenter::Family F>
	const NotificationCenter::ServiceFactory NotificationCenter::Singleton = []() -> NotificationService* { return nullptr; };
} } } }

#endif
