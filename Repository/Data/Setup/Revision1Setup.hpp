#ifndef Cadabra_Queen_Repository_Data_Setup_Revision1Setup_hpp
#define Cadabra_Queen_Repository_Data_Setup_Revision1Setup_hpp

#include "../AbstractSetup.hpp"

#include <QObject>

namespace Cadabra { namespace Queen { namespace Repository { namespace Data {
	class Revision1Setup : public AbstractSetup
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Revision1Setup();
		virtual ~Revision1Setup() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool go(Core::Data::Provider& data_provider) const override;
	/** @} */

	/** @name Factories */
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
