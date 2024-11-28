#ifndef Cadabra_Queen_Core_Data_Command_EntityFetch_hpp
#define Cadabra_Queen_Core_Data_Command_EntityFetch_hpp

#include "EntityCommand.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class EntityFetch : public EntityCommand
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
		EntityFetch();
		EntityFetch(QSharedPointer<AbstractEntity> entity, int row_index);
		EntityFetch(const EntityFetch& origin);
		EntityFetch(EntityFetch&& origin);
		virtual ~EntityFetch() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const int _row_index;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const int& getRowIndex() const;
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
