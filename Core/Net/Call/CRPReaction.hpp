/**
 @file CRPReaction.hpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Core_Net_Call_CRPReaction_hpp
#define Cadabra_Queen_Core_Net_Call_CRPReaction_hpp

#include <QString>
#include <QJsonValue>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class CRPReaction
	{
	/** @name Constructors */
	/** @{ */
	public:
		CRPReaction();
		CRPReaction(CRPReaction&& orig);
		CRPReaction(const CRPReaction& orig);

		CRPReaction(quint8 domain, quint32 code, const QString& message);
		CRPReaction(QJsonValue&& value);
		CRPReaction(const QJsonValue& value);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		quint8 _domain;
		quint32 _code;
		QString _message;
		QJsonValue _value;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator bool() const;
		bool operator!() const;
		QJsonValue& operator*();
		const QJsonValue& operator*() const;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void log(const QString& prefix = "") const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const quint8& getDomain() const;
		const quint32& getCode() const;
		const QString& getMessage() const;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isFaulty() const;
		bool isUndefined() const;
	/** @} */
	};
} } } }

#endif
