#ifndef Cadabra_Queen_Repository_Meta_FilterDelegate_hpp
#define Cadabra_Queen_Repository_Meta_FilterDelegate_hpp

#include "EssenceToken.hpp"

#include <QObject>
#include <QMap>
#include <QPair>
#include <QByteArray>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class FilterDelegate
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using Callback = std::function<bool(const QVariant&, Meta::EssenceExpression::OperatorType)>;
		using CallbackMap = QMap<QPair<QByteArray, Meta::EssenceExpression::OperandType>, Callback>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit FilterDelegate();
		virtual ~FilterDelegate();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		CallbackMap _callback_map;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		bool operator()(const Meta::EssenceExpression& expression);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		template <typename R, typename F>
//		typename std::enable_if<std::is_convertible<R*, QObject*>::value, void>::type
		void handle(const QByteArray& variable_name, Meta::EssenceExpression::OperandType operand, R* receiver, F&& callback)
		{
			_callback_map[qMakePair(variable_name, operand)] = std::bind(std::forward<F>(callback), receiver, std::placeholders::_1, std::placeholders::_2);
		}
		void handle(const QByteArray& variable_name, Meta::EssenceExpression::OperandType operand, Callback&& callback);
		void clear();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
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
