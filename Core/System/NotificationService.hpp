#ifndef Cadabra_Queen_Core_System_NotificationService_hpp
#define Cadabra_Queen_Core_System_NotificationService_hpp

#include <QObject>

#include "NotificationCenter.hpp"

#include <QString>

#include <type_traits>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class NotificationService : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString token READ getToken NOTIFY tokenAcquired)
		Q_PROPERTY(bool hasPermission READ hasPermission NOTIFY permissionGranted)
		Q_PROPERTY(bool hasToken READ hasToken NOTIFY tokenAcquired)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		template <typename T>
		static typename std::enable_if<std::is_base_of<NotificationService, T>::value, NotificationService*>::type Instance()
		{
			static T retval;
			return &retval;
		}
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		NotificationService();
		virtual ~NotificationService() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	public:
		QString _token;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE virtual void requestPermission() = 0;
		Q_INVOKABLE virtual void requestToken() = 0;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		Q_INVOKABLE virtual QString makePrefixedToken() const = 0;
		Q_INVOKABLE virtual QString makeCNSToken() const = 0;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getToken() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setToken(QString token);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void permissionGranted();
		void tokenAcquired(QString old_token);
		void messageReceived(QString title, QString icon, QString sound, QString body);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool hasPermission() const = 0;
		bool hasToken() const;
	/** @} */
	};
} } } }

#endif
