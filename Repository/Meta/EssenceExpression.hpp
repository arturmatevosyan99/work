/**
 @file EssenceExpression.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_EssenceExpression_hpp
#define Cadabra_Queen_Repository_Meta_EssenceExpression_hpp

#include <QVariant>
#include <QMetaType>
#include <QDataStream>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class EssenceExpression
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class LogicType : char
		{
			Or = '|',
			And = '&'
		};

		enum class OperatorType : char
		{
			None = ' ',
			Equal = '=',
			NotEqual = '!',
			Less = '<',
			LessOrEqual = 'L',
			Greater = '>',
			GreaterOrEqual = 'G',
			Addition = '+',
			Subtraction = '-',
			Division = '/',
			Multiplication = '*'
		};

		enum class OperandType {
			Invalid,
			Variable,
			Expression,
			Null,
			Boolean,
			String,
			Int,
			LongLong,
			Double
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static QPair<QStringRef, QStringRef> Split(const QStringRef& input, EssenceExpression::OperatorType& op);
		static QVariant Parse(const QStringRef& input);
		static EssenceExpression::OperandType DetectOperandType(const QVariant& operand);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		EssenceExpression();
		EssenceExpression(const QString& left, const QVariant& right, EssenceExpression::OperatorType op = EssenceExpression::OperatorType::Equal);
		EssenceExpression(const QStringRef& left, const QStringRef& right, EssenceExpression::OperatorType op = EssenceExpression::OperatorType::Equal);
		EssenceExpression(const QString& input);
		EssenceExpression(const EssenceExpression& origin);
		EssenceExpression(EssenceExpression&& origin);
		~EssenceExpression() = default;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_ENUM(LogicType)
		Q_ENUM(OperatorType)
		Q_ENUM(OperandType)
		Q_PROPERTY(OperatorType operatorType READ getOperatorType CONSTANT)
		Q_PROPERTY(OperandType leftType READ getLeftOperandType CONSTANT)
		Q_PROPERTY(OperandType rightType READ getRightOperandType CONSTANT)
		Q_PROPERTY(QVariant left READ getLeftOperand CONSTANT)
		Q_PROPERTY(QVariant right READ getRightOperand CONSTANT)
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator QVariant() const;
		EssenceExpression& operator=(const EssenceExpression& rhs);
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		OperatorType _operator;
		QVariant _left;
		QVariant _right;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const OperatorType& getOperatorType() const;
		const QVariant& getLeftOperand() const;
		const QVariant& getRightOperand() const;
		OperandType getLeftOperandType() const;
		OperandType getRightOperandType() const;
	/** @} */

	/** @name Signals*/
	/** @{ */
	signals:
	/** @} */

	/** @name Converters */
	/** @{ */
	public:
		Q_INVOKABLE QVariant toVariant() const noexcept;
	/** @} */

	/** @name Friends */
	/** @{ */
	friend QDataStream& operator<<(QDataStream& out, const EssenceExpression& handle);
	friend QDataStream& operator>>(QDataStream& in, EssenceExpression& handle);
	friend QDebug operator<<(QDebug debug, const EssenceExpression& handle);
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};

	inline uint qHash(const EssenceExpression::OperatorType& operator_type)
	{
		return static_cast<char>(operator_type);
	}
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Meta::EssenceExpression)

#endif
