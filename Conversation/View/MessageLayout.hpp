#ifndef Cadabra_Queen_Conversation_View_MessageLayout_hpp
#define Cadabra_Queen_Conversation_View_MessageLayout_hpp

#include "MessageView.hpp"

#include "../Meta/MessageRender.hpp"
#include "../Model/AbstractMessage.hpp"
#include "../Model/Application/GroupChannel.hpp"
#include "../../Core/Meta/Qd.hpp"

#include <QFlag>
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QQmlComponent>
#include <QQmlIncubator>

namespace Cadabra { namespace Queen { namespace Conversation { namespace View {
	/**
	 * @brief The MessageLayout class
	 * @todo Добавить qml-свойства для определения шрифтов
	 */
	class MessageLayout : public QQuickPaintedItem
	{
	/** @name Classes */
	/** @{ */
	private:
		class Incubator : public QQmlIncubator
		{
		/** @name Constructors */
		/** @{ */
		public:
			Incubator(MessageLayout* parent);
			virtual ~Incubator() override;
		/** @} */

		/** @name Constructors */
		/** @{ */
		private:
			QPointer<MessageLayout> _layout;
		/** @} */

		/** @name Hooks */
		/** @{ */
		protected:
			virtual void statusChanged(QQmlIncubator::Status status) override;
		/** @} */
		};

	public:
		enum class Side {
			Left,
			Right,
			Center
		};

		enum class Flag {
			None				= 0b0000000000,
			AsyncRender			= 0b0000000001,
			RightSideImage		= 0b0000000010,
			LeftSideImage		= 0b0000000100,
			DateTimeLabel		= 0b0000001000,
			BlurEffect			= 0b0000010000,
			OddIndex			= 0b0000100000,
			StateLabel			= 0b0001000000,
			EditModifier		= 0b0010000000,
			ReceiveModifier		= 0b0100000000,
			ReadModifier		= 0b1000000000
		};
		Q_DECLARE_FLAGS(Flags, Flag)
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Side)
		Q_FLAG(Flags)
		Q_PROPERTY(Cadabra::Queen::Conversation::Model::AbstractMessage* model READ getModel NOTIFY rendered)
		Q_PROPERTY(Cadabra::Queen::Conversation::View::MessageView* view READ getView NOTIFY rendered)
		Q_PROPERTY(Side side READ getSide WRITE setSide NOTIFY sideChanged)
		Q_PROPERTY(Cadabra::Queen::Conversation::View::MessageLayout::Flags flags READ getFlagSet WRITE setFlagSet NOTIFY flagsetChanged)
		Q_PROPERTY(qreal viewMinimumWidth READ getViewMinimumWidth WRITE setViewMinimumWidth NOTIFY viewWidthMinimized)
		Q_PROPERTY(qreal viewMinimumHeight READ getViewMinimumHeight WRITE setViewMinimumHeight NOTIFY viewHeightMinimized)
		Q_PROPERTY(qreal imageWidth READ getImageWidth WRITE setImageWidth NOTIFY imageWidthChanged)
		Q_PROPERTY(qreal statusHeight READ getStatusHeight WRITE setStatusHeight NOTIFY statusHeightChanged)
		Q_PROPERTY(bool hasModel READ hasModel NOTIFY imported)
		Q_PROPERTY(bool hasView READ hasView NOTIFY rendered)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit MessageLayout(QQuickItem* parent = nullptr);
		virtual ~MessageLayout() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QQmlComponent* _component;
		Incubator _incubator;
		MessageView* _view;
		RenderLayout _type;

		Side _side;
		QFlags<Flag> _flag_set;
		QImage _image;

		qreal _view_minimum_width;
		qreal _view_minimum_height;
		qreal _image_width;
		qreal _status_height;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void paint(QPainter* painter) override;
		Q_INVOKABLE bool render(Cadabra::Queen::Core::Meta::Qd scheme, Cadabra::Queen::Conversation::RenderLayout layout = Cadabra::Queen::Conversation::RenderLayout::FeedLayout);
		Q_INVOKABLE bool load(Cadabra::Queen::Conversation::Model::GroupChannel* channel_application, Cadabra::Queen::Conversation::Meta::Message::Id id, QUuid uid, Cadabra::Queen::Conversation::Meta::Message::Id reply_id, Cadabra::Queen::Repository::Meta::Subject::Id subject_id, QJsonObject body, QDateTime post_time, QDateTime modification_time);

	protected:
		virtual QSGNode* updatePaintNode(QSGNode* oldNode, QQuickItem::UpdatePaintNodeData* data) override;

	private:
		bool attach(MessageView* view);
		bool incubate(QQmlComponent::Status status);
		void align();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Conversation::Model::AbstractMessage* getModel() const;
		Cadabra::Queen::Conversation::View::MessageView* getView() const;
		QDateTime getActualDateTime() const;
		const Side& getSide() const;
		const QFlags<MessageLayout::Flag>& getFlagSet() const;
		qreal getViewMinimumWidth() const;
		qreal getViewMinimumHeight() const;
		qreal getImageWidth() const;
		qreal getStatusHeight() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setSide(Side value);
		void setFlagSet(Flags set);
		void setViewMinimumWidth(qreal value);
		void setViewMinimumHeight(qreal value);
		void setImageWidth(qreal value);
		void setStatusHeight(qreal value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void viewCompleted();
		void viewHeld(QPoint position, bool canceled);
		void rendered();
		void modified();
		void imported();
		void popup(QQmlComponent* component, QVariantMap properties);
		void viewWidthMinimized();
		void viewHeightMinimized();
		void imageWidthChanged();
		void statusHeightChanged();
		void stateChanged();
		void sideChanged();
		void flagsetChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasModel() const;
		bool hasView() const;
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		void onViewHeightChanged();
		void onViewWidthChanged();
		void onModelProgressed();
		void onModelDelivered();
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Conversation::View::MessageLayout::Flags)
Q_DECLARE_METATYPE(Cadabra::Queen::Conversation::View::MessageLayout::Flags)

#endif
