#ifndef Cadabra_Queen_Core_Data_Command_EntityReopen_hpp
#define Cadabra_Queen_Core_Data_Command_EntityReopen_hpp

#include "EntityCommand.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class EntityReopen : public EntityCommand
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
		EntityReopen();
		EntityReopen(QSharedPointer<AbstractEntity> entity, const Rowset::HintTable& hint_table);
		EntityReopen(const EntityReopen& origin);
		EntityReopen(EntityReopen&& origin);
		virtual ~EntityReopen() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const Rowset::HintTable _hint_table;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Rowset::HintTable& getHintTable() const;
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
