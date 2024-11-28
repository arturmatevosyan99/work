#ifndef Cadabra_Queen_Mentalgami_Controller_QuizFlow_hpp
#define Cadabra_Queen_Mentalgami_Controller_QuizFlow_hpp

#include "../../../Repository/Controller/Application.hpp"

#include "../../View/RiddleView.hpp"

#include <QAbstractItemModel>
#include <QQmlEngine>
#include <QQmlIncubator>
#include <QQueue>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Controller {
	class QuizFlow;
	class QuizFlowAttachment : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		QuizFlowAttachment(QObject* riddle = nullptr);
		virtual ~QuizFlowAttachment() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		View::RiddleView* _riddle_view;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		void connect();
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void activeChanged();
		void activationRequested();
		void previousRequested();
		void nextRequested();
	/** @} */

	/** @name Hooks*/
	/** @{ */
	protected:
		virtual bool eventFilter(QObject* object, QEvent* event) override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isActive() const;
	/** @} */
	};

	class QuizFlow : public Repository::Controller::Application
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QAbstractItemModel* riddleList READ getRiddleList WRITE setRiddleList NOTIFY riddleListChanged)
		Q_PROPERTY(QQuickItem* delegate READ getDelegate WRITE setDelegate NOTIFY delegateChanged)
		Q_PROPERTY(Cadabra::Queen::Mentalgami::View::RiddleView* active READ getActive NOTIFY activeChanged)
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
		class Incubator : public QQmlIncubator
		{
		/** @name `classes*/
		/** @{ */
		public:
			enum class Method
			{
				Queue,
				Skip,
				Cull
			};
		/** @} */

		/** @name Constructors */
		/** @{ */
		public:
			Incubator(QuizFlow& parent);
			virtual ~Incubator() override;
		/** @} */

		/** @name Constructors */
		/** @{ */
		private:
			QuizFlow& _flow;
		/** @} */

		/** @name Hooks */
		/** @{ */
		protected:
			virtual void statusChanged(QQmlIncubator::Status status) override;
		/** @} */
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static QuizFlowAttachment* qmlAttachedProperties(QObject* object);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		QuizFlow(QQuickItem* parent = nullptr);
		virtual ~QuizFlow() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QAbstractItemModel* _list;
		QQuickItem* _delegate;
		View::RiddleView* _active_riddle;
		QMap<View::RiddleView*, int> _riddle_map;
		QQmlComponent* _component;
		Incubator _incubator;
		QQueue<int> _incubation_queue;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool activate(QQuickItem* item);
		bool incubate(QuizFlow::Incubator::Method method = QuizFlow::Incubator::Method::Queue);

	private:
		bool dequeue(View::RiddleView* view);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QAbstractItemModel* getRiddleList();
		QQuickItem* getDelegate();
		View::RiddleView* getActive();
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setRiddleList(QAbstractItemModel* model);
		void setDelegate(QQuickItem* delegate);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void riddleListChanged();
		void delegateChanged();

		void activeChanged();
		void previousRequested();
		void nextRequested();

		void stacked(Cadabra::Queen::Mentalgami::View::RiddleView* view, int index);
	/** @} */

	/** @name Signals */
	/** @{ */
	private:
		bool onIncubated(QQmlComponent::Status status);

		void onReset();
		void onRiddleInserted(const QModelIndex& parent, int first, int last);
		void onRiddleMoved(const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row);
		void onRiddleRemoved(const QModelIndex& parent, int first, int last);
	/** @} */
	};
} } } }

/// @note Makes the type known to QMetaType (for using with QVariant)
QML_DECLARE_TYPE(Cadabra::Queen::Mentalgami::Controller::QuizFlowAttachment)

/// @note Declares attached properties
QML_DECLARE_TYPEINFO(Cadabra::Queen::Mentalgami::Controller::QuizFlow, QML_HAS_ATTACHED_PROPERTIES)

#endif
