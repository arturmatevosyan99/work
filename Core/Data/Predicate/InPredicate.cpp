/**
 @file InPredicate.cpp
 @date 20.04.16
 @copyright Cadabra / Queen
 @author Daniil A Megrabyan
*/

#include "InPredicate.hpp"

#include <QTextStream>

namespace Cadabra { namespace Queen { namespace Core  { namespace Data {
	InPredicate::InPredicate(const Field& source_field)
	:
		_source_field(retain(source_field))
	{
		
		
	}

	InPredicate::InPredicate(const Field& source_field, const std::initializer_list<QVariant>& value_list)
	:
		_source_field(retain(source_field)),
		_value_list(value_list)
	{

	}

	InPredicate::InPredicate(const Field& source_field, const QVariantList& value_list)
	:
		_source_field(retain(source_field)),
		_value_list(value_list)
	{

	}

	void InPredicate::emplace(const Field& field)
	{
		_field_list.push_back(&(retain(field)));
	}

	void InPredicate::emplace(QVariant&& value)
	{
		_value_list.push_back(value);
	}
	void InPredicate::emplace(const QVariant& value)
	{
		_value_list.push_back(value);
	}

	QString InPredicate::makeSQL() const
	{
		QString string;
		QTextStream stream(&string);
		stream << _source_field.getFullAlias() << " IN (";

		QListIterator<const Field*> f(_field_list);
		while (f.hasNext()) {
			const Field* field(f.next());
			if (field->isStandalone()) {
				if (field->isNull()) {
					stream << "NULL";
				} else {
					stream << (field->isQuoted() ? ("\'" + field->getPrint() + "\'") : field->getPrint());
				}
			} else {
				stream << field->getFullAlias();
			}

			if (f.hasNext()) {
				stream << ", ";
			} else {
				if (!_value_list.isEmpty()) {
					stream << ", ";
				}
				break;
			}
		}

		QListIterator<QVariant> v(_value_list);
		while (v.hasNext()) {
			if (_source_field.isQuoted()) {
				stream << "'" << v.next().toString() << "'";
			} else {
				stream << v.next().toString();
			}
			if (v.hasNext()) {
				stream  << ", ";
			} else {
				break;
			}
		}

		stream << ")";
		return stream.readAll();
	}
} } } }
