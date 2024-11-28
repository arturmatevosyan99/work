/**
 @file Counterparty.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_SD_Meta_Counterparty_hpp
#define Cadabra_Queen_SD_Meta_Counterparty_hpp

#include "LegalCounterparty.hpp"
#include "ContactCounterparty.hpp"

#include <QObject>
#include <QMetaType>
#include <QDataStream>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace SD { namespace Meta {
	class Counterparty : public QObject
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Legal = LegalCounterparty;
		using Contact = ContactCounterparty;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::SD::Meta::LegalCounterparty legal READ getLegal NOTIFY parsed)
		Q_PROPERTY(QVector<Cadabra::Queen::SD::Meta::ContactCounterparty> contactList READ getContactList NOTIFY parsed)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static QSharedPointer<Counterparty> Parse(const QJsonObject& object);
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Counterparty();
		Counterparty(const Counterparty& origin);
		Counterparty(Counterparty&& origin);
		virtual ~Counterparty() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		LegalCounterparty _legal;
		QVector<ContactCounterparty> _contact_vector;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator QJsonObject() const;
		Counterparty& operator=(const Counterparty& rhs);
		Counterparty& operator=(Counterparty&& rhs);
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
		void parse(const QJsonObject& object);
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const LegalCounterparty& getLegal() const;
		const QVector<ContactCounterparty>& getContactList() const;
	/** @} */

	/** @name Formatters */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void parsed();
	/** @} */

	/** @name Templates */
	/** @{ */
	public:
	/** @} */

	/** @name Friends */
	/** @{ */
	friend QDataStream& operator<<(QDataStream& out, const Counterparty& counterparty);
	friend QDataStream& operator>>(QDataStream& in, Counterparty& counterparty);
	friend QDebug operator<<(QDebug debug, const Counterparty& counterparty);
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::SD::Meta::Counterparty)

#endif
