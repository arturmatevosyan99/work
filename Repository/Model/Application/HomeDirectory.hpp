#ifndef Cadabra_Queen_Repository_Model_Application_HomeDirectory_hpp
#define Cadabra_Queen_Repository_Model_Application_HomeDirectory_hpp

#include "Directory.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class HomeDirectory : public Directory
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		HomeDirectory(QObject* parent = nullptr);
		virtual ~HomeDirectory() override;
	/** @} */
	};
} } } }

#endif
