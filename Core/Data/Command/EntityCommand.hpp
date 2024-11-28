#ifndef Cadabra_Queen_Core_Data_Command_EntityCommand_hpp
#define Cadabra_Queen_Core_Data_Command_EntityCommand_hpp

#include "../AbstractCommand.hpp"

#include "../AbstractEntity.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class EntityCommand : public AbstractCommand
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
		EntityCommand(QSharedPointer<AbstractEntity> entity);
		EntityCommand(const EntityCommand& origin);
		EntityCommand(EntityCommand&& origin);
		virtual ~EntityCommand() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QSharedPointer<AbstractEntity> _entity;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QSharedPointer<AbstractEntity>& getEntity() const;
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
