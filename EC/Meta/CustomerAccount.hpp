/**
 @file CustomerAccount.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_EC_Meta_CustomerAccount_hpp
#define Cadabra_Queen_EC_Meta_CustomerAccount_hpp

#include "../../Core/Meta/Qd.hpp"
#include "../../Repository/Meta/Subject.hpp"
#include "../../SD/Meta/Counterparty.hpp"

#include <QObject>
#include <QString>
#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace EC { namespace Meta {
	class CustomerAccount
	{
	/** @name Statics */
	/** @{ */
	public:
		static const QString EmptyString;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd subjectQd READ getSubjectId CONSTANT)
		Q_PROPERTY(QString subjectName READ getSubjectName CONSTANT)
		Q_PROPERTY(QString subjectDescription READ getSubjectDescription CONSTANT)
		Q_PROPERTY(QString subjectPicture READ getSubjectPicture CONSTANT)
		Q_PROPERTY(QString subjectLocale READ getSubjectLocale CONSTANT)
		Q_PROPERTY(QString subjectTimezone READ getSubjectTimezone CONSTANT)

		Q_PROPERTY(Cadabra::Queen::SD::LegalType legalType READ getLegalType CONSTANT)
		Q_PROPERTY(QString legalName READ getLegalName CONSTANT)
		Q_PROPERTY(QString legalComment READ getLegalComment CONSTANT)
		Q_PROPERTY(QString legalCountryCode READ getLegalCountryCode CONSTANT)
		Q_PROPERTY(QString legalPostCode READ getLegalPostCode CONSTANT)
		Q_PROPERTY(QString legalPostAddress1 READ getLegalPostAddress1 CONSTANT)
		Q_PROPERTY(QString legalPostAddress2 READ getLegalPostAddress2 CONSTANT)
		Q_PROPERTY(QString legalEmailAddress READ getLegalEmailAddress CONSTANT)
		Q_PROPERTY(QString legalPhoneNumber READ getLegalPhoneNumber CONSTANT)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CustomerAccount();
		CustomerAccount(QSharedPointer<const Repository::Meta::Subject> subject, QSharedPointer<const SD::Meta::Counterparty> counteryparty);
		CustomerAccount(const CustomerAccount& origin);
		CustomerAccount(CustomerAccount&& origin);
		~CustomerAccount();
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		QSharedPointer<const Repository::Meta::Subject> _subject;
		QSharedPointer<const SD::Meta::Counterparty> _counteryparty;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		CustomerAccount& operator=(const CustomerAccount& rhs);
		CustomerAccount& operator=(CustomerAccount&& rhs);
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Repository::Meta::Subject::Id& getSubjectId() const;
		const QString& getSubjectName() const;
		const QString& getSubjectDescription() const;
		const QString& getSubjectPicture() const;
		const QString& getSubjectLocale() const;
		const QString& getSubjectTimezone() const;

		SD::LegalType getLegalType() const;
		const QString& getLegalName() const;
		const QString& getLegalComment() const;
		const QString& getLegalCountryCode() const;
		const QString& getLegalPostCode() const;
		const QString& getLegalPostAddress1() const;
		const QString& getLegalPostAddress2() const;
		const QString& getLegalEmailAddress() const;
		QString getLegalPhoneNumber() const;
	/** @} */

	/** @name Formatters */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name Templates */
	/** @{ */
	public:
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::EC::Meta::CustomerAccount)

#endif
