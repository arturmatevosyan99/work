#ifndef Cadabra_Queen_Repository_Model_Application_Infrastructure_hpp
#define Cadabra_Queen_Repository_Model_Application_Infrastructure_hpp

#include "../AbstractApplication.hpp"

#include "../Proxy/ObjectProxy.hpp"
#include "../../Meta/Object.hpp"

#include <QAbstractListModel>
#include <QMap>
#include <QList>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Infrastructure : public AbstractApplication
	{
	/** @name Classes */
	/** @{ */
	public:
		using CacheMap = QMap<Repository::Meta::Object::Id, QSharedPointer<AbstractApplication>>;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::Model::ObjectProxy* objectProxy READ getObjectProxy NOTIFY opened)
		Q_PROPERTY(QAbstractItemModel* objectModel READ getObjectModel NOTIFY opened)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::ObjectPath* objectPath READ getObjectPath CONSTANT)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Infrastructure(QObject* parent = nullptr);
		virtual ~Infrastructure() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Repository::Model::ObjectProxy _model;
		CacheMap _cache;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Repository::Model::ObjectProxy* getObjectProxy();
		QAbstractItemModel* getObjectModel();
		ObjectPath* getObjectPath();

		Q_INVOKABLE Cadabra::Queen::Repository::Model::AbstractApplication* getApplication(Cadabra::Queen::Core::Meta::Qd object_id);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void customEvent(QEvent* event) override;
		virtual bool onHandle() override;
		virtual bool onOpen() override;
		virtual bool onSetup() override;
		virtual bool onClose() override;
		virtual bool onTeardown() override;

	private slots:
		void onModelTokenChanged(int index, bool onoff);
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
