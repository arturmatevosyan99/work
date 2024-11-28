#ifndef Cadabra_Queen_Core_View_AlertDialog_hpp
#define Cadabra_Queen_Core_View_AlertDialog_hpp

#include "../../Core/Meta/PropertyList.hpp"

#include <QObject>
#include <QMetaObject>

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	class AlertAction : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Style : quint8
		{
			Default = 0,
			Ok = 1,
			Cancel = 2,
			Discard = 3
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Style)
		Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
		Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY labelChanged)
		Q_PROPERTY(Style style READ getStyle WRITE setStyle NOTIFY styleChanged)
		Q_PROPERTY(bool enabled READ isEnabled WRITE enable NOTIFY enableChanged)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AlertAction(QObject* parent = nullptr);
		virtual ~AlertAction() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _text;
		QString _label;
		Style _style;
		bool _is_enabled;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void enable(bool onoff = true);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getText() const;
		const QString& getLabel() const;
		const Style& getStyle() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setText(const QString& value);
		void setLabel(const QString& value);
		void setStyle(Style value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void done();
		void textChanged();
		void labelChanged();
		void styleChanged();
		void enableChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isEnabled() const;
	/** @} */
	};

	class AlertState : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
		Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)

		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::View::AlertAction> actions READ makeActionPropertyList CONSTANT)
		Q_CLASSINFO("DefaultProperty", "actions")
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AlertState(QObject* parent = nullptr);
		virtual ~AlertState() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _name;
		QString _title;
		QString _text;
		Meta::TemplatedPropertyList<AlertAction> _action_property_list;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QQmlListProperty<AlertAction> makeActionPropertyList();
		QStringList makeActionTitleList() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getName() const;
		const QString& getTitle() const;
		const QString& getText() const;
		AlertAction* getAction(int index) const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setName(const QString& value);
		void setTitle(const QString& value);
		void setText(const QString& value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void nameChanged();
		void titleChanged();
		void textChanged();
	/** @} */
	};

	class AlertDialog : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString state READ getState WRITE setState NOTIFY stateChanged)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::View::AlertState> states READ makeStatePropertyList CONSTANT)
		Q_CLASSINFO("DefaultProperty", "states")
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AlertDialog(QObject* parent = nullptr);
		virtual ~AlertDialog() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		AlertState* _state;
		Meta::TemplatedPropertyList<AlertState> _state_property_list;
		QMetaObject::Connection _connection;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool show(const QString& target_state = "");
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QQmlListProperty<AlertState> makeStatePropertyList();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getState() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setState(const QString& value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void done(QString state, AlertAction::Style style, QString label);
		void stateChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onDone(int index, QString ident, Cadabra::Queen::Core::View::AlertState* state);
	/** @} */
	};
} } } }

#endif
