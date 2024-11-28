#include "FilterDelegate.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	FilterDelegate::FilterDelegate()
	{

	}

	FilterDelegate::~FilterDelegate()
	{
	}

	bool FilterDelegate::operator()(const Meta::EssenceExpression& expression)
	{
		if (expression.getLeftOperandType() != Meta::EssenceExpression::OperandType::Variable) {
			return false;
		}
		CallbackMap::ConstIterator c(_callback_map.find(qMakePair(expression.getLeftOperand().toByteArray(), expression.getRightOperandType())));
		return (_callback_map.constEnd() != c) ? (*c)(expression.getRightOperand(), expression.getOperatorType()) : false;
	}

	void FilterDelegate::handle(const QByteArray& variable_name, Meta::EssenceExpression::OperandType operand, Callback&& callback)
	{
		_callback_map[qMakePair(variable_name, operand)] = callback;
	}

	void FilterDelegate::clear()
	{
		_callback_map.clear();
	}
} } } }
