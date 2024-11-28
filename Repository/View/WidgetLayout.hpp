#ifndef Cadabra_Queen_Repository_View_WidgetLayout_hpp
#define Cadabra_Queen_Repository_View_WidgetLayout_hpp

#include "WidgetView.hpp"

#include "../Model/Avatar.hpp"

#include <QQuickItem>
#include <QQmlComponent>

namespace Cadabra { namespace Queen { namespace Repository { namespace View {
	class WidgetLayout;
	class WidgetView;

	class WidgetLayoutAttachment : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::View::WidgetLayout* layout READ getLayout NOTIFY layoutChanged)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Avatar* userAvatar READ getUserAvatar NOTIFY layoutChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		WidgetLayoutAttachment(QObject* widget_view = nullptr);
		virtual ~WidgetLayoutAttachment() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		WidgetLayout* _layout;
		WidgetView* _view;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
	/** @} */

	/** @name Getters */
	/** @{ */
	private:
		WidgetLayout* getLayout();
		Model::Avatar* getUserAvatar() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void layoutChanged();
	/** @} */

	/** @name Hooks*/
	/** @{ */
	protected:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};

	class WidgetLayout : public QQuickItem
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QRectF rect READ getRect NOTIFY rectChanged)
		Q_PROPERTY(qreal spacing READ getSpacing WRITE setSpacing NOTIFY spacingChanged)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Repository::View::WidgetView> views READ makeViewPropertyList CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Avatar* userAvatar READ getUserAvatar WRITE setUserAvatar NOTIFY userAvatarChanged)
		Q_CLASSINFO("DefaultProperty", "views");
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static WidgetLayoutAttachment* qmlAttachedProperties(QObject* object);

		static WidgetView* Item(QQmlListProperty<WidgetView>* list, int index);
		static int Size(QQmlListProperty<WidgetView>* list);
		static void Clear(QQmlListProperty<WidgetView>* list);
		static void Append(QQmlListProperty<WidgetView>* list, WidgetView* item);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit WidgetLayout(QQuickItem* parent = nullptr);
		virtual ~WidgetLayout() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		qreal _spacing;
		QList<WidgetView*> _view_list;
		Model::Avatar* _user_avatar;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		void arrange(WidgetView* widget_view = nullptr);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QQmlListProperty<WidgetView> makeViewPropertyList();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QRectF getRect() const;
		qreal getSpacing() const;
		Model::Avatar* getUserAvatar() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setSpacing(qreal value);
		void setUserAvatar(Model::Avatar* avatar);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void rectChanged(const QRectF& rect);
		void spacingChanged();
		void userAvatarChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onLayoutChanged(WidgetView* widget_view);
		void onVisibilityChanged(WidgetView* widget_view);
		void onPositionChanged(WidgetView* widget_view);
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void geometryChanged(const QRectF& new_geometry, const QRectF& old_geometry) override;
	/** @} */
	};
} } } }

/// @note Makes the type known to QMetaType (for using with QVariant)
QML_DECLARE_TYPE(Cadabra::Queen::Repository::View::WidgetLayoutAttachment)

/// @note Declares attached properties
QML_DECLARE_TYPEINFO(Cadabra::Queen::Repository::View::WidgetLayout, QML_HAS_ATTACHED_PROPERTIES)

#endif
