#ifndef Cadabra_Queen_Core_Data_Command_EntityClose_hpp
#define Cadabra_Queen_Core_Data_Command_EntityClose_hpp

#include "EntityCommand.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class EntityClose : public EntityCommand
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
		EntityClose();
		EntityClose(QSharedPointer<AbstractEntity> entity);
		EntityClose(const EntityClose& origin);
		EntityClose(EntityClose&& origin);
		virtual ~EntityClose() override;
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
