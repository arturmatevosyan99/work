#ifndef Cadabra_Queen_Repository_Model_Application_Folder_hpp
#define Cadabra_Queen_Repository_Model_Application_Folder_hpp

#include "../AbstractApplication.hpp"

#include "Interface/FolderInterface.hpp"

#include "../../Meta/FolderIndex.hpp"

#include <QSharedPointer>
#include <QIODevice>
#include <QMimeType>
#include <QSet>
#include <QQueue>
#include <QHash>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Directory : public AbstractApplication, public FolderInterface
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_INTERFACES(Cadabra::Queen::Repository::Model::FolderInterface)
		Q_PROPERTY(QAbstractListModel* indexModel READ getIndexModel NOTIFY opened)
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Directory(QObject* parent = nullptr);
		virtual ~Directory() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QSharedPointer<Meta::FolderIndex> _index_model;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE Cadabra::Queen::Core::Net::CRPPromise index();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QAbstractListModel* getIndexModel() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void indexed();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onIndexed(const Model::FolderInterface::Map& map) override;

		virtual bool onHandle() override;
		virtual bool onOpen() override;
		virtual bool onSetup() override;
		virtual void customEvent(QEvent* event) override;
	/** @} */
	};
} } } }

#endif
