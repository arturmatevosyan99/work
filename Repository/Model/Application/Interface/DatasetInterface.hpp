#ifndef Cadabra_Queen_Repository_Model_Application_Interface_DatasetInterface_hpp
#define Cadabra_Queen_Repository_Model_Application_Interface_DatasetInterface_hpp

#include "../AbstractInterface.hpp"

#include <QtPlugin>

#include "../../../Net/Call/ObjectDataset.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class DatasetInterface : public virtual Repository::Model::AbstractInterface
	{
	/** @name Qroperties */
	/** @{ */
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		DatasetInterface() = default;
		virtual ~DatasetInterface() = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
//		const QScopedPointer<Net::ObjectDataset> _object_dataset;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */
	};
} } } }

Q_DECLARE_INTERFACE(Cadabra::Queen::Repository::Model::DatasetInterface, "cadabra.queen.repository.model.dataset/1.0");

#endif
