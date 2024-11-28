#ifndef Cadabra_Queen_Core_System_NotificationService_APNService_h
#define Cadabra_Queen_Core_System_NotificationService_APNService_h

#include "../NotificationService.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class APNService : public NotificationService
	{
	friend class NotificationService;

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	private:
		APNService();
		virtual ~APNService() override;
	/** @} */

	/** @{ Properties */
	private:
		bool _active;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void requestPermission() override;
		virtual void requestToken() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		Q_INVOKABLE virtual QString makePrefixedToken() const override;
		Q_INVOKABLE virtual QString makeCNSToken() const override;
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool hasPermission() const override;
	/** @} */
	};
} } } }

#endif
