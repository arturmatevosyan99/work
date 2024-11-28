/**
 @file CompoundPredicate.hpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Data_Predicate_CompoundPredicate_hpp
#define Cadabra_Queen_Core_Data_Predicate_CompoundPredicate_hpp

#include "WherePredicate.hpp"

#include <QSharedPointer>
#include <QList>
#include <QMap>
#include <QString>
#include <initializer_list>

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	class CompoundPredicate : public WherePredicate
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Logic : std::uint8_t
		{
			And = 0x00,
			Or = 0x01,
			Not = 0x02
		};
	/** @} */

	/** @name Constants */
	/** @{ */
	public:
		static const QMap<CompoundPredicate::Logic, QString> Map;
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		CompoundPredicate(CompoundPredicate::Logic logic);
		CompoundPredicate(const QSharedPointer<const WherePredicate>& predicate);
		CompoundPredicate(QList<QSharedPointer<const WherePredicate>> list, CompoundPredicate::Logic = CompoundPredicate::Logic::Or);
		CompoundPredicate(std::initializer_list<QSharedPointer<const WherePredicate>> list, CompoundPredicate::Logic = CompoundPredicate::Logic::Or);
		CompoundPredicate();
		virtual ~CompoundPredicate() override = default;
	/** @} */
		
	/** @name Properties */
	/** @{ */
	private:
		QList<QSharedPointer<const WherePredicate>> _list;
		CompoundPredicate::Logic _logic;
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
		virtual QString makeSQL() const override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */
	
	/** @name Mutators */
	/** @{ */
	public:
		void clear();
		void append(const QSharedPointer<const WherePredicate>& predicate);
		template<typename T, typename ...TArgs>
		void emplace(TArgs&& ...args)
		{
			_list.push_back(QSharedPointer<T>::create(std::forward<TArgs>(args)...));
		}
		void setLogic(CompoundPredicate::Logic logic);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isEmpty() const;
	/** @} */
	};
} } } }

#endif
