#ifndef Cadabra_Queen_Core_Data_ForeignKey_hpp
#define Cadabra_Queen_Core_Data_ForeignKey_hpp

#include <QString>
#include <QPair>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class ForeignKey
	{
	/** @name Constructors */
	/** @{ */
	public:
		ForeignKey() = default;
		virtual ~ForeignKey() = default;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual const QString& getAlias() const = 0;
		virtual QString getLocalEntityName() const = 0;
		virtual QString getForeignEntityName() const = 0;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QList<QPair<QString, QString>> makePairList() const = 0;
	/** @} */
	};
} } } }

#endif
