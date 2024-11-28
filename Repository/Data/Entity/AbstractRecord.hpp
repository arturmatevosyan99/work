#ifndef Cadabra_Queen_Repository_Data_Entity_AbstractRecord_hpp
#define Cadabra_Queen_Repository_Data_Entity_AbstractRecord_hpp

#include "../../../Core/Data/AbstractEntity.hpp"

#include "../../../Core/Data/Predicate/ConditionPredicate.hpp"

#include "../../Repository.hpp"
#include "../../Meta/Essence.hpp"
#include "../../Meta/EssenceToken.hpp"
#include "../../Meta/FilterDelegate.hpp"

#include <QMap>
#include <QSharedPointer>
#include <QEvent>

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class Tag;
	class AbstractTag;
	/**
	 * @brief The AbstractRecord class
	 * ActiveRecord implementation
	 */
	class AbstractRecord : public Core::Data::AbstractEntity
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Static */
	/** @{ */
	public:
		static const QMap<Meta::EssenceExpression::OperatorType, Core::Data::ConditionPredicate::Condition> ConditionOperatorMap;
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractRecord(const QString& name);
		virtual ~AbstractRecord() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field id;

	private:
		bool _live_mode;

	protected:
		Meta::FilterDelegate _filter_delegate;
		const QSharedPointer<Core::Data::CompoundPredicate> _id_compound;

	private:
		QSharedPointer<AbstractTag> _bind_tag;
		QSharedPointer<Tag> _roaster_tag;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool filter(const Meta::EssenceExpression& expression);
		virtual bool filter(const QVector<Meta::EssenceToken>& token_vector);
		virtual bool order(const QVector<int>& sort_vector, bool reverse = false);
		virtual bool order(int role, Qt::SortOrder order = Qt::SortOrder::AscendingOrder);
		virtual void clear();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public slots:
		void setLive(bool onoff);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QModelIndex makeIndex(int row, int column) const;

	protected:		
		virtual QSharedPointer<AbstractTag> makeTag() const = 0;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	protected slots:
		bool addIdPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);
		bool addTagAliasPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);
		bool addTagIdPredicate(const QVariant& predicate, Meta::EssenceExpression::OperatorType operand);

	public slots:
		void onMutated(Meta::EssenceAlterMap map);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual bool event(QEvent* event) override;

		virtual void onSelect() override;
		virtual void onCreate() override;
		virtual void onForeign() override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isLiveMode() const;
	/** @} */
	};
} } } }

#endif
