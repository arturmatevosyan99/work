#ifndef Cadabra_Queen_Repository_Data_AbstractSetup_hpp
#define Cadabra_Queen_Repository_Data_AbstractSetup_hpp

#include <QObject>

#include "../../Core/Data/Provider.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class AbstractSetup : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Static */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractSetup();
		virtual ~AbstractSetup();
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool go(Core::Data::Provider& data_provider) const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public slots:
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	public slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif