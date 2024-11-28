#ifndef Cadabra_Queen_Core_Data_Command_EntityMutate_hpp
#define Cadabra_Queen_Core_Data_Command_EntityMutate_hpp

#include "EntityCommand.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class EntityMutate : public EntityCommand
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
		EntityMutate();
		EntityMutate(QSharedPointer<AbstractEntity> entity, const Rowset::MutationList& list);
		EntityMutate(QSharedPointer<AbstractEntity> entity, const QSqlRecord& record, Rowset::Mutation operation);
		EntityMutate(const EntityMutate& origin);
		EntityMutate(EntityMutate&& origin);
		virtual ~EntityMutate() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Rowset::MutationList _list;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Rowset::MutationList& getList() const;
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
