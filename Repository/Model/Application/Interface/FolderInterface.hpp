#ifndef Cadabra_Queen_Repository_Model_Application_Interface_FolderInterface_hpp
#define Cadabra_Queen_Repository_Model_Application_Interface_FolderInterface_hpp

#include "../AbstractInterface.hpp"

#include "../../../Meta/Object.hpp"

#include <QtPlugin>
#include <QMultiMap>
#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class FolderInterface : public virtual Repository::Model::AbstractInterface
	{
	/** @name Aliases */
	/** @{ */
	protected:
		using Map = QMultiMap<Meta::Object::Id, QSharedPointer<const Meta::Object>>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		FolderInterface() = default;
		virtual ~FolderInterface() = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		/**
		 * @brief Load child objects (think like GroupChannel is a folder with files)
		 * @note Result index may vary depending on the access level
		 */
		Core::Net::CRPDelay index();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onIndexed(const Map& map);

	private:
		void onIndex(const Core::Net::CRPReaction& reaction);
	/** @} */
	};
} } } }

Q_DECLARE_INTERFACE(Cadabra::Queen::Repository::Model::FolderInterface, "cadabra.queen.repository.model.folder/1.0");

#endif
