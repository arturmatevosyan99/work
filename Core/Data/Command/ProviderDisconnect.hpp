#ifndef Cadabra_Queen_Core_Data_Command_ProviderDisconnect_hpp
#define Cadabra_Queen_Core_Data_Command_ProviderDisconnect_hpp

#include "../AbstractCommand.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class ProviderDisconnect : public AbstractCommand
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ProviderDisconnect();
		ProviderDisconnect(const ProviderDisconnect& origin);
		ProviderDisconnect(ProviderDisconnect&& origin);
		virtual ~ProviderDisconnect() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
