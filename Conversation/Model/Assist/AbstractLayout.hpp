#ifndef Cadabra_Queen_Conversation_Model_Assist_AbstractLayout_hpp
#define Cadabra_Queen_Conversation_Model_Assist_AbstractLayout_hpp

#include "../AbstractAssist.hpp"

#include <QVector>
#include <QQmlListProperty>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class AbstractLayout : public AbstractAssist
	{
	/** @name Statics */
	/** @{ */
	private:
		static Cadabra::Queen::Conversation::Meta::Assist* Item(QQmlListProperty<Cadabra::Queen::Conversation::Meta::Assist>* list, int index);
		static int ItemListSize(QQmlListProperty<Cadabra::Queen::Conversation::Meta::Assist>* list);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Conversation::Meta::Assist> items READ makeItemList NOTIFY imported)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		AbstractLayout(Meta::Assist::Scheme scheme, QObject* parent = nullptr);
		virtual ~AbstractLayout() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QVector<Meta::Assist> _item_vector;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual Meta::Assist::TargetLayout getDefaultTargetLayout() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		QQmlListProperty<Cadabra::Queen::Conversation::Meta::Assist> makeItemList();
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
