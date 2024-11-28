#include "Field.hpp"

#include "AbstractEntity.hpp"
#include "Expression/FieldExpression.hpp"

#include "../../Qaos/MakeDateTime.hpp"
#include <QUuid>
#include <QDateTime>
#include <QJsonDocument>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	const QMap<Field::Condition, QString> Field::ConditionMap = {
		{Field::Condition::Unknown, "?"},
		{Field::Condition::Is, "IS"},
		{Field::Condition::IsNot, "IS NOT"},
		{Field::Condition::Equal, "="},
		{Field::Condition::NotEqual, "<>"},
		{Field::Condition::Less, "<"},
		{Field::Condition::LessOrEqual, "<="},
		{Field::Condition::Greater, ">"},
		{Field::Condition::GreaterOrEqual, ">="}
	};

	QMetaType::Type Field::ConvertToMetaType(Field::Type type)
	{
		switch (type) {
			case Field::Type::Blob:
				return QMetaType::QByteArray;

			case Field::Type::Int:
				return QMetaType::Int;

			case Field::Type::Integer:
				return QMetaType::LongLong;

			case Field::Type::AutoIncrement:
				return QMetaType::LongLong;

			case Field::Type::Real:
				return QMetaType::Double;

			case Field::Type::Text:
				return QMetaType::QString;

			case Field::Type::Boolean:
				return QMetaType::Bool;

			case Field::Type::Document:
				return QMetaType::QJsonDocument;

			case Field::Type::UUID:
				return QMetaType::QUuid;

			case Field::Type::DateTime:
			case Field::Type::Timestamp:
				return QMetaType::QDateTime;

			case Field::Type::Duration:
				return QMetaType::User;

			default:
				return QMetaType::UnknownType;
		}
	}

	Field::ValueHolder Field::MakeValueHolder(const Field& field, const QVariant& value)
	{
		return qMakePair(std::cref(field), value);
	}

	Field::Field(const QString& field_name, Field::Type type, QVariant default_value)
	:
		QSqlField(field_name, static_cast<QVariant::Type>(Field::ConvertToMetaType(type))),

		_entity(nullptr),
		_type(type),
		_expression(nullptr)
	{
		if (_type == Field::Type::AutoIncrement) {
			setAutoValue(true);
			// setRequired(true);
		} else {
			if (default_value.isNull()) {
				setRequired(false);
			}
			setDefaultValue(default_value);
		}
	}

	Field::Field(const QString& field_name, Field::Type type, bool is_nullable)
	:
		QSqlField(field_name, static_cast<QVariant::Type>(Field::ConvertToMetaType(type))),

		_entity(nullptr),
		_type(type),
		_expression(nullptr)
	{
		if (_type == Field::Type::AutoIncrement) {
			setAutoValue(true);
		} else {
			setRequired(!is_nullable);
		}
	}

	Field::Field(Field::Type type, QVariant value)
	:
		QSqlField("", static_cast<QVariant::Type>(Field::ConvertToMetaType(type))),

		_entity(nullptr),
		_type(type),
		_expression(nullptr)
	{
		setValue(value);
	}

	Field::Field(const Field& origin)
	:
		QSqlField(origin),

		_entity(origin._entity),
		_type(origin._type),
		_expression(nullptr)
	{

	}

	Field::~Field()
	{
		if (_expression) {
			delete _expression;
		}
	}

	Field& Field::operator=(const Field& origin)
	{
		QSqlField::operator=(origin);

		_entity = origin._entity;
		_type = origin._type;

		return *this;
	}

	const Field::Type& Field::getValueType() const
	{
		return _type;
	}

	QString Field::getAlias() const
	{
		return name();
	}

	QString Field::getFullAlias() const
	{
		return (_entity) ? _entity->getAlias() + "." + name() : name();
	}

	AbstractEntity* Field::getEntity() const
	{
		return _entity;
	}

	QString Field::getEntityAlias() const
	{
		return _entity ? _entity->getAlias() : "";
	}

	FieldExpression* Field::getExpression() const
	{
		return _expression;
	}

	QString Field::getPrint() const
	{
		return value().toString();
	}

	void Field::setEntity(AbstractEntity* entity)
	{
		_entity = entity;
	}

	void Field::setExpression(FieldExpression* expression)
	{
		if (_expression) {
			delete _expression;
		}
		_expression = expression;
		if (_expression) {
			_expression->setTarget(this);
		}
	}

	QString Field::describe() const
	{
		/** @brief
		 * Each value stored in an SQLite database (or manipulated by the database engine) has one of the following storage classes:
		 * NULL. The value is a NULL value.
		 * INTEGER. The value is a signed integer, stored in 1, 2, 3, 4, 6, or 8 bytes depending on the magnitude of the value.
		 * REAL. The value is a floating point value, stored as an 8-byte IEEE floating point number.
		 * TEXT. The value is a text string, stored using the database encoding (UTF-8, UTF-16BE or UTF-16LE).
		 * BLOB. The value is a blob of data, stored exactly as it was input.
		*/
		switch (_type) {
			case Field::Type::Blob:
				return "BLOB";

			case Field::Type::Int:
				return "INTEGER";

			case Field::Type::Integer:
				return "INTEGER";

			case Field::Type::AutoIncrement:
				return "INTEGER AUTOINCREMENT";

			case Field::Type::Real:
				return "REAL";

			case Field::Type::Text:
				return "TEXT";

			case Field::Type::Boolean:
				return "INTEGER";

			case Field::Type::Document:
				return "TEXT";

			case Field::Type::UUID:
				return "TEXT";

			case Field::Type::DateTime:
				return "TEXT";

			case Field::Type::Timestamp:
				return "INTEGER";

			default:
				return "";
		}
	}

	QString Field::express() const
	{
		if (isStandalone()) {
			if (isNull()) {
				return "NULL";
			}
			QString retval;

			const QMetaType::Type meta_type(static_cast<QMetaType::Type>(type()));
			const Field::Type value_type(getValueType());

			switch (meta_type) {
				case QMetaType::QJsonDocument:
					retval = "'" + QString::fromUtf8(value().toJsonDocument().toJson(QJsonDocument::Compact)) + "'";
					break;

				case QMetaType::QUuid:
					retval = "'" + QUuid(value().toString()).toString() + "'";
					break;

				case QMetaType::QDateTime:
					retval = (Field::Type::Timestamp == value_type) ? QString::number(value().toDateTime().toSecsSinceEpoch()) : Qaos::MakeDateTime(value()).toString("''yyyy-MM-dd hh:mm:ss.zzz''");
					break;

				case QMetaType::LongLong: /// < Это такой <int32
					retval = QString::number(value().toLongLong());
					break;

				case QMetaType::User:
					switch (value_type) {
						case Field::Type::Duration:
							retval = value().toString();
							retval = retval.isEmpty() ? "date('now')" : "date('now', " + retval + ")";
							break;

						default:
							retval = value().toString();
					}
					break;

				default:
					retval = isQuoted() ? "\'" + getPrint() + "\'" : getPrint();
			}

			return retval;
		} else if (hasExpression()) {
			return static_cast<QString>(*_expression);
		}
		return getFullAlias();
	}

	bool Field::isStandalone() const
	{
		return nullptr == _entity;
	}

	bool Field::isNullable() const
	{
		return requiredStatus() == RequiredStatus::Optional;
	}

	bool Field::isNull() const
	{
		return QSqlField::isNull();
	}

	bool Field::isQuoted() const
	{
		switch (_type) {
			case Field::Type::Blob:
			case Field::Type::Int:
			case Field::Type::Integer:
			case Field::Type::AutoIncrement:
			case Field::Type::Real:
			case Field::Type::Boolean:
			case Field::Type::Timestamp:
				return false;

			case Field::Type::UUID:
			case Field::Type::Text:
			case Field::Type::Document:
			case Field::Type::DateTime:
				return true;

			default:
				return false;
		}
	}

	bool Field::hasEntity(const AbstractEntity* entity) const
	{
		return entity == _entity;
	}

	bool Field::hasExpression() const
	{
		return nullptr != _expression;
	}
} } } }
