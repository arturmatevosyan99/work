#ifndef Cadabra_Queen_Conversation_View_AssistLayout_hpp
#define Cadabra_Queen_Conversation_View_AssistLayout_hpp

#include "AssistView.hpp"

#include "../Model/AbstractAssist.hpp"
#include "../../Core/Meta/Qd.hpp"

#include <QQuickItem>
#include <QQmlComponent>

namespace Cadabra { namespace Queen { namespace Conversation { namespace View {
	class AssistLayout : public QQuickItem
	{
	/** @name Enum */
	/** @{ */
	public:
		using TargetLayout = Meta::Assist::TargetLayout;
	/** @} */

	/** @name Qroperties */
	/** @{ */
    Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Conversation::Model::AbstractAssist* model READ getModel NOTIFY rendered)
		Q_PROPERTY(Cadabra::Queen::Conversation::View::AssistView* view READ getView NOTIFY viewChanged)
		Q_PROPERTY(bool hasModel READ hasModel NOTIFY rendered)
		Q_PROPERTY(bool hasView READ hasView NOTIFY viewChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit AssistLayout(QQuickItem* parent = nullptr);
		virtual ~AssistLayout() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Cadabra::Queen::Core::Meta::Qd _scheme;
		AssistView* _view;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void render(Cadabra::Queen::Core::Meta::Qd scheme);
		Q_INVOKABLE void reset();
	/** @} */

	/** @name Getters */
	/** @{ */
    public:
		Cadabra::Queen::Conversation::Model::AbstractAssist* getModel() const;
		Cadabra::Queen::Conversation::View::AssistView* getView() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void rendered();
		void modelImported();
		void viewChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onViewWidthChanged();
		void onViewHeightChanged();
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
	/** @} */
	};
} } } }

#endif
