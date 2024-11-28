#ifndef Cadabra_Queen_Conversation_Model_Application_Air_hpp
#define Cadabra_Queen_Conversation_Model_Application_Air_hpp

#include "GroupChannel.hpp"
#include "../../../Repository/Meta/Object.hpp"
#include "../../../Repository/Model/AbstractApplication.hpp"
#include "../../../Repository/Model/Proxy/ObjectProxy.hpp"

#include <QAbstractListModel>
#include <QMap>
#include <QList>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class Air : public Repository::Model::AbstractApplication
	{
	/** @name Classes */
	/** @{ */
	public:
		using CacheMap = QMap<Repository::Meta::Object::Id, QSharedPointer<GroupChannel>>;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::Model::ObjectProxy* objectProxy READ getObjectProxy NOTIFY opened)
		Q_PROPERTY(QAbstractItemModel* objectModel READ getObjectModel NOTIFY opened)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Air(QObject* parent = nullptr);
		virtual ~Air() override;
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
		Q_INVOKABLE Cadabra::Queen::Repository::Model::AbstractJob* spread(const QString& text_message);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Repository::Model::ObjectProxy* getObjectProxy();
		QAbstractItemModel* getObjectModel();

		Q_INVOKABLE Cadabra::Queen::Conversation::Model::GroupChannel* getChannel(Cadabra::Queen::Core::Meta::Qd channel_id, bool auto_build = true);
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
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

	private:
		void onAction(BroadcastAction* action);

	private slots:
		void onModelLiveChanged();
		void onModelReverseChanged();
		void onModelTokenChanged(int index, bool onoff);
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
