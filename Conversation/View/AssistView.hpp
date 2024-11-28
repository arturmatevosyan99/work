#ifndef Cadabra_Queen_Conversation_View_AssistView_hpp
#define Cadabra_Queen_Conversation_View_AssistView_hpp

#include "../Model/AbstractAssist.hpp"
#include "../Model/AbstractMessage.hpp"

#include <QQuickPaintedItem>
#include <QPainter>

namespace Cadabra { namespace Queen { namespace Conversation { namespace View {
	class AssistLayout;

	class AssistView : public QQuickPaintedItem
	{
	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Conversation::View::AssistLayout* layout READ getLayout WRITE setLayout NOTIFY layoutChanged)
		Q_PROPERTY(Cadabra::Queen::Conversation::Model::AbstractAssist* model READ getModel WRITE setModel NOTIFY modelChanged)
		Q_PROPERTY(qreal layoutHeight READ getLayoutHeight NOTIFY layoutHeightChanged)
		Q_PROPERTY(qreal layoutWidth READ getLayoutWidth NOTIFY layoutWidthChanged)
		Q_PROPERTY(bool laydown READ hasLayout NOTIFY layoutChanged)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit AssistView(QQuickItem* parent = nullptr);
		virtual ~AssistView() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Cadabra::Queen::Conversation::View::AssistLayout* _layout;
		Cadabra::Queen::Conversation::Model::AbstractAssist* _model;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void paint(QPainter *painter) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Conversation::View::AssistLayout* getLayout() const;
		Cadabra::Queen::Conversation::Model::AbstractAssist* getModel() const;
		qreal getLayoutHeight() const;
		qreal getLayoutWidth() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setLayout(Cadabra::Queen::Conversation::View::AssistLayout* layout);
		void setModel(Cadabra::Queen::Conversation::Model::AbstractAssist* model);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void layoutChanged();
		void layoutHeightChanged();
		void layoutWidthChanged();
		void modelChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasLayout() const;
		bool hasModel() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onAppearanceChanged();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */
	};
} } } }

#endif
