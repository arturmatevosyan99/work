#ifndef Cadabra_Queen_Repository_Data_Entity_Record_Object_hpp
#define Cadabra_Queen_Repository_Data_Entity_Record_Object_hpp

#include "../AbstractRecord.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class Object : public AbstractRecord
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Object();
		virtual ~Object() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field parent_id;
		Field application_scheme;
		Field name;
		Field favourite;
		Field localization;
		Field picture;
		Field permission;
		Field creation_timestamp;
		Field modification_timestamp;
		Field access_timestamp;
		Field meta; /// @note local meta, not the meta from ace

		const QSharedPointer<Core::Data::CompoundPredicate> _application_compound;
		const QSharedPointer<Core::Data::CompoundPredicate> _parent_id_compound;
		const QSharedPointer<Core::Data::CompoundPredicate> _timeframe_compound;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool order(int role, Qt::SortOrder order = Qt::SortOrder::AscendingOrder) override;
		virtual void clear() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	protected:
		virtual QSharedPointer<AbstractTag> makeTag() const override;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool addIdentPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);
		bool addPathPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);
		bool addParentIdPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);
		bool addApplicationPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);
		bool addTimeframePredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual void onSelect() override;
		virtual void onCreate() override;
		virtual void onForeign() override;
	/** @} */
	};
} } } }

#endif
