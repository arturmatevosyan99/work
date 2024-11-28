/**
 @file InappReceipt.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_EC_Meta_InappReceipt_hpp
#define Cadabra_Queen_EC_Meta_InappReceipt_hpp

#include "../../Repository/Meta/Subject.hpp"

#include <QObject>
#include <QSharedPointer>
#include <QJsonObject>
#include <QDateTime>
#include <QUuid>

namespace Cadabra { namespace Queen { namespace EC { namespace Meta {
	class InappReceipt
	{
	/** @name Aliases */
	/** @{ */
	public:
		enum class Distributor : char
		{
			Unknown = '0',
			Cadabraza = 'C',
			AppStore = 'A',
			MarketPlace = 'G',
			MicrosoftStore = 'M'
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_ENUM(Distributor)
		Q_PROPERTY(Distributor distributor READ getDistributor)
		Q_PROPERTY(QString container READ getContainer)
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
		InappReceipt();
		InappReceipt(InappReceipt::Distributor distributor, const QString& sku, const QString& container, bool sandbox);
		InappReceipt(const InappReceipt& origin);
		InappReceipt(InappReceipt&& origin);
		~InappReceipt();
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		Distributor _distributor;
		QString _sku;
		bool _sandbox;
		QString _container;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QJsonObject makeJSON() const;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		InappReceipt& operator=(const InappReceipt& rhs);
		InappReceipt& operator=(InappReceipt&& rhs);
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void setContainer(const QString& container);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const InappReceipt::Distributor& getDistributor() const;		
		const QString& getSKU() const;
		const QString& getContainer() const;
	/** @} */

	/** @name Formatters */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isSandbox() const;
		bool isInvalid() const;
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

Q_DECLARE_METATYPE(Cadabra::Queen::EC::Meta::InappReceipt)

#endif
