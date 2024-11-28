#ifndef Cadabra_Queen_Core_System_Alert_ActionAlert_hpp
#define Cadabra_Queen_Core_System_Alert_ActionAlert_hpp

#include "DialogAlert.hpp"

#include "AlertOption.hpp"

#include "../../Meta/PropertyList.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class ActionAlert : public DialogAlert
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Flag
		{
			Default = 0b000000,
			HeadFull = 0b000001,
			SheetMode = 0b000010
		};
		Q_DECLARE_FLAGS(Flags, Flag)

		enum class Resolution
		{
			Invalid = -1, /// >	The button is invalid.
			Accept = 0, /// > Clicking the button causes the dialog to be accepted (e.g. OK).
			Reject = 1, /// > Clicking the button causes the dialog to be rejected (e.g. Cancel).
			Discard = 2, /// > Clicking the button causes a destructive change (e.g. for Discarding Changes) and closes the dialog.
			Action = 3, /// > Clicking the button causes changes to the elements within the dialog.
			Help = 4, /// > The button can be clicked to request help.
			Yes = 5, /// > The button is a "Yes"-like button.
			No = 6, /// > The button is a "No"-like button.
			Reset = 7, /// > The button resets the dialog's fields to default values.
			Apply = 8, /// > The button applies current changes.
			Custom = 9
		};
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using SigBack = void(ActionAlert::*)();
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static SigBack Signal(Resolution resolution);
		static Resolution OptionResolution(AlertOption::Flag option);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_FLAG(Flags)
		Q_ENUM(Resolution)
		Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
		Q_PROPERTY(Cadabra::Queen::Core::System::ActionAlert::Flags flags MEMBER _flagset)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::System::AlertOption> options READ makeOptionPropertyList CONSTANT)
		Q_CLASSINFO("DefaultProperty", "options");
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit ActionAlert(QObject* parent = nullptr);
		explicit ActionAlert(QObject* parent, const QString& name, const QString& title, const QString& text, AlertOption::Flags option_flagset = {}, Flags message_flagset = {}, DialogAlert::Level level = DialogAlert::Level::Info);
		virtual ~ActionAlert() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _text;
		Flags _flagset;
		Resolution _resolution;
		Core::Meta::TemplatedPropertyList<AlertOption> _option_property_list;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QQmlListProperty<AlertOption> makeOptionPropertyList();
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual Q_INVOKABLE bool warn() override;
		Q_INVOKABLE bool opt(Cadabra::Queen::Core::System::AlertOption* option);
		Q_INVOKABLE bool resolve(Cadabra::Queen::Core::System::ActionAlert::Resolution resolution);
		Q_INVOKABLE AlertOption::Flags mask() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getText() const;
		const Flags& getFlagSet() const;
		Resolution getResolution() const;
		const Core::Meta::TemplatedPropertyList<AlertOption>& getOptionPropertyList() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setText(const QString& value);
		void setFlagSet(Flags value);
		void addButton(AlertOption::Flag flag);
		void addButton(const QString& caption, AlertOption::Flag flag);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void textChanged();
		void flagsetChanged();
		void yes();
		void no();
		void accepted();
		void rejected();
		void discarded();
		void reset();
		void actioned();
		void applied();
		void helped();
		void invalidated();
		void customized();
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Core::System::ActionAlert::Flags)
Q_DECLARE_METATYPE(Cadabra::Queen::Core::System::ActionAlert::Flags)

#endif
