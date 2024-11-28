#ifndef Cadabra_Queen_Core_Data_ForeignEntity_hpp
#define Cadabra_Queen_Core_Data_ForeignEntity_hpp

#include "ForeignKey.hpp"
#include "Field.hpp"
#include "AbstractEntity.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	/// @todo LocalEntity<> : public AbstractEntity::ForeignKey
	template <typename E>
	class ForeignEntity : public ForeignKey
	{
	/** @name Aliases */
	/** @{ */
	public:
		using ForeignPointer = const Field E::*;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ForeignEntity(std::initializer_list<QPair<std::reference_wrapper<Field>, ForeignPointer>> list, const AbstractEntity * const entity)
		:
			_entity(entity),
			_foreign_entity(new E()),
			_list(list),
			_alias(_foreign_entity->getAlias() + "_FK")
		{

		}

		template<typename ...TArgs>
		ForeignEntity(std::initializer_list<QPair<std::reference_wrapper<Field>, ForeignPointer>> list, const AbstractEntity * const entity, TArgs&& ...foreign_args)
		:
			_entity(entity),
			_foreign_entity(new E(std::forward<TArgs>(foreign_args)...)),
			_list(list),
			_alias(_foreign_entity->getAlias() + "_FK")
		{

		}

		virtual ~ForeignEntity() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const AbstractEntity * const _entity;
		QScopedPointer<E> _foreign_entity;
		QList<QPair<std::reference_wrapper<Field>, ForeignPointer>> _list;
		QString _alias;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		template<typename ...TArgs>
		void initialize(TArgs&& ...foreign_args)
		{
			_foreign_entity.reset(new E(std::forward<TArgs>(foreign_args)...));
		}
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		void setAlias(const QString& alias)
		{
			_alias = alias;
		}
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual const QString& getAlias() const override
		{
			return _alias;
		}

		virtual QString getLocalEntityName() const override
		{
			return _entity ? _entity->getAlias() : "";
		}

		virtual QString getForeignEntityName() const override
		{
			return _foreign_entity ? _foreign_entity->getAlias() : "";
		}
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QList<QPair<QString, QString>> makePairList() const override
		{
			if (!_entity || !_foreign_entity) {
				return {};
			}

			QList<QPair<QString, QString>> retval;
			QListIterator<QPair<std::reference_wrapper<Field>, ForeignPointer>> f(_list);
			while (f.hasNext()) {
				const QPair<std::reference_wrapper<Field>, ForeignPointer>& link(f.next());
				const Field& local_field(link.first.get());
				const Field& foregin_field(*(_foreign_entity.get()).*(link.second));
				retval.push_back(qMakePair(local_field.name(), foregin_field.name()));
			}
			return retval;
		}
	/** @} */
	};
} } } }

#endif
