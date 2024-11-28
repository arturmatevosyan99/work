#ifndef Cadabra_Queen_Core_Data_Field_hpp
#define Cadabra_Queen_Core_Data_Field_hpp

#include <QSqlField>
#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class AbstractEntity;
	class FieldExpression;
	/**
	 * @brief The Field class
	 * @todo Expressions(AVG, SUM, MAX, MIN)
	 */
	class Field : public QSqlField
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Condition
		{
			Unknown,
			Equal,
			NotEqual,
			Is,
			IsNot,
			Less,
			LessOrEqual,
			Greater,
			GreaterOrEqual
		};

		enum class Type : quint8
		{
			Text = 't',
			Int = 'i',
			Integer = 'I',
			AutoIncrement = 'A',
			Real = 'r',
			Blob = 'b',
			Boolean = 'B',
			Document = 'd',
			DateTime = 'D',
			Timestamp = 'T',
			Duration = 'u',
			UUID = 'U',
			Common = 'c'
		};
		using ValueHolder = QPair<std::reference_wrapper<const Field>, QVariant>;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QMap<Field::Condition, QString> ConditionMap;
		static QMetaType::Type ConvertToMetaType(Field::Type type);
		static ValueHolder MakeValueHolder(const Field& field, const QVariant& value);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Field(const QString& field_name = QString(), Field::Type type = Field::Type::Common, QVariant default_value = QVariant());
		Field(const QString& field_name = QString(), Field::Type type = Field::Type::Common, bool is_nullable = true);
		Field(Field::Type type, QVariant value);
		Field(const Field& origin);
		virtual ~Field();
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		Field& operator=(const Field& origin);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		AbstractEntity* _entity;
		Field::Type _type;
		FieldExpression* _expression;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Field::Type& getValueType() const;
		QString getAlias() const;
		QString getFullAlias() const;
		AbstractEntity* getEntity() const;
		QString getEntityAlias() const;
		FieldExpression* getExpression() const;
		QString getPrint() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setEntity(AbstractEntity* entity);
		void setExpression(FieldExpression* expression);
	/** @} */

	/** @name Formatters */
	/** @{ */
	public:
		QString describe() const;
		QString express() const;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isStandalone() const;
		bool isNullable() const;
		bool isNull() const;
		bool isQuoted() const;
		bool hasEntity(const AbstractEntity* entity) const;
		bool hasExpression() const;
	/** @} */
	};

	inline bool operator==(const std::reference_wrapper<Field>& field1, const std::reference_wrapper<Field>& field2)
	{
		return field1.get().name() == field2.get().name();
	}

	inline uint qHash(const std::reference_wrapper<Field>& field)
	{
		return qHash(field.get().name());
	}
} } } }

#endif
