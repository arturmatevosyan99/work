#ifndef Cadabra_Queen_Mentalgami_Model_Application_EurekaDirectory_hpp
#define Cadabra_Queen_Mentalgami_Model_Application_EurekaDirectory_hpp

#include "../../../Repository/Model/Application/HomeDirectory.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class EurekaDirectory : public Repository::Model::HomeDirectory
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		EurekaDirectory(QObject* parent = nullptr);
		virtual ~EurekaDirectory() override;
	/** @} */
	};
} } } }

#endif