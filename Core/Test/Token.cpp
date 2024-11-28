#include "Token.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	Token::Token(const QString& description, const QVector<Assertion*>& position_vector)
	:
		QObject(nullptr),

		_description(description),
		_position_vector(position_vector),
		_next_action(nullptr)
	{

	}

	Token::~Token()
	{

	}

	bool Token::passBy(Cadabra::Queen::Core::Test::Action* action)
	{
		return _next_action != nullptr ? false : !!(_next_action = action);
	}

	bool Token::stopAt(Cadabra::Queen::Core::Test::Action* action)
	{
		return _next_action != action ? false : !(_next_action = nullptr);
	}

	const QString& Token::getDescription() const
	{
		return _description;
	}

	const QVector<Assertion*>& Token::getPositionVector() const
	{
		return _position_vector;
	}

	QVariant Token::getProperty(const QString& name, const QVariant& default_value)
	{
		QVariant return_value(QObject::property(name.toLocal8Bit().data()));
		return return_value.isValid() ? return_value : default_value;
	}

	void Token::setDescription(const QString& description)
	{
		if (description != _description) {
			_description = description;
			emit descriptionChanged();
		}
	}

	void Token::setPositionVector(const QVector<Assertion*>& position_vector)
	{
		_position_vector = position_vector;
		emit positionVectorChanged();
	}

	void Token::addPosition(Assertion* position)
	{
		_position_vector.append(position);
		emit positionVectorChanged();
	}

	void Token::setProperty(const QString& name, const QVariant& value)
	{
		QObject::setProperty(name.toLocal8Bit().data(), value);
	}

	bool Token::hasProperty(const QString& name) const
	{
		return QObject::property(name.toLocal8Bit().data()).isValid();
	}
} } } }
