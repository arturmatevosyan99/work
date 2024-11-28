#ifndef Cadabra_Queen_Core_Data_Command_EntityOpen_hpp
#define Cadabra_Queen_Core_Data_Command_EntityOpen_hpp

#include "EntityCommand.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class EntityOpen : public EntityCommand
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
		EntityOpen();
		EntityOpen(QSharedPointer<AbstractEntity> entity, Provider* provider, QFlags<Provider::OpenMode> mode_set, Provider::RowsetType rowset_type);
		EntityOpen(const EntityOpen& origin);
		EntityOpen(EntityOpen&& origin);
		virtual ~EntityOpen() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Provider* const _provider;
		const QFlags<Provider::OpenMode> _mode_set;
		const Provider::RowsetType _rowset_type;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QFlags<Provider::OpenMode>& getModeSet() const;
		const Provider::RowsetType& getRowsetType() const;
		Provider* getProvider() const;
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
