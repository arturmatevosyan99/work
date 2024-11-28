/**
 @file CustomerAccount.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#include "CustomerAccount.hpp"

namespace Cadabra { namespace Queen { namespace EC { namespace Meta {
	const QString CustomerAccount::EmptyString = {};

	CustomerAccount::CustomerAccount()
	{

	}

	CustomerAccount::CustomerAccount(QSharedPointer<const Repository::Meta::Subject> subject, QSharedPointer<const SD::Meta::Counterparty> counteryparty)
	:
		_subject(subject),
		_counteryparty(counteryparty)
	{

	}

	CustomerAccount::CustomerAccount(const CustomerAccount& origin)
	:
		_subject(origin._subject),
		_counteryparty(origin._counteryparty)
	{

	}

	CustomerAccount::CustomerAccount(CustomerAccount&& origin)
	:
		_subject(std::move(origin._subject)),
		_counteryparty(std::move(origin._counteryparty))
	{

	}

	CustomerAccount::~CustomerAccount()
	{

	}

	CustomerAccount& CustomerAccount::operator=(const CustomerAccount& rhs)
	{
		_subject = rhs._subject;
		_counteryparty = rhs._counteryparty;

		return *this;
	}

	CustomerAccount& CustomerAccount::operator=(CustomerAccount&& rhs)
	{
		_subject = std::move(rhs._subject);
		_counteryparty = std::move(rhs._counteryparty);

		return *this;
	}

	const Repository::Meta::Subject::Id& CustomerAccount::getSubjectId() const
	{
		return !_subject ? Repository::Meta::Subject::DummyId : _subject->getId();
	}

	const QString& CustomerAccount::getSubjectName() const
	{
		return !_subject ? CustomerAccount::EmptyString : _subject->getName();
	}

	const QString& CustomerAccount::getSubjectDescription() const
	{
		return !_subject ? CustomerAccount::EmptyString : _subject->getDescription();
	}

	const QString& CustomerAccount::getSubjectPicture() const
	{
		return !_subject ? CustomerAccount::EmptyString : _subject->getPicture();
	}

	const QString& CustomerAccount::getSubjectLocale() const
	{
		return !_subject ? CustomerAccount::EmptyString : _subject->getLocale();
	}

	const QString& CustomerAccount::getSubjectTimezone() const
	{
		return !_subject ? CustomerAccount::EmptyString : _subject->getTimezone();
	}

	SD::LegalType CustomerAccount::getLegalType() const
	{
		return !_counteryparty ? SD::LegalType::UndefinedPerson : _counteryparty->getLegal().getType();
	}

	const QString& CustomerAccount::getLegalName() const
	{
		return !_counteryparty ? CustomerAccount::EmptyString : _counteryparty->getLegal().getName();
	}

	const QString& CustomerAccount::getLegalComment() const
	{
		return !_counteryparty ? CustomerAccount::EmptyString : _counteryparty->getLegal().getComment();
	}

	const QString& CustomerAccount::getLegalCountryCode() const
	{
		return !_counteryparty ? CustomerAccount::EmptyString : _counteryparty->getLegal().getCountryCode();
	}

	const QString& CustomerAccount::getLegalPostCode() const
	{
		return !_counteryparty ? CustomerAccount::EmptyString : _counteryparty->getLegal().getPostCode();
	}

	const QString& CustomerAccount::getLegalPostAddress1() const
	{
		return !_counteryparty ? CustomerAccount::EmptyString : _counteryparty->getLegal().getAddress1();
	}

	const QString& CustomerAccount::getLegalPostAddress2() const
	{
		return !_counteryparty ? CustomerAccount::EmptyString : _counteryparty->getLegal().getAddress2();
	}

	const QString& CustomerAccount::getLegalEmailAddress() const
	{
		return !_counteryparty || !_counteryparty->getLegal().hasEmail() ? CustomerAccount::EmptyString : _counteryparty->getLegal().getEmail();
	}

	QString CustomerAccount::getLegalPhoneNumber() const
	{
		return !_counteryparty || !_counteryparty->getLegal().hasPhone() ? "" : _counteryparty->getLegal().getPhone();
	}
} } } }
