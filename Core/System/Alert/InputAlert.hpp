#ifndef Cadabra_Queen_Core_System_Alert_InputAlert_hpp
#define Cadabra_Queen_Core_System_Alert_InputAlert_hpp

#include "DialogAlert.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class InputAlert : public DialogAlert
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Flag
		{
			Default = 0b000000,
			HeadFull = 0b000001,
			CancelOption = 0b000010
		};
		Q_DECLARE_FLAGS(Flags, Flag)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_FLAG(Flags)
		Q_PROPERTY(QString placeholder READ getPlaceholder WRITE setPlaceholder NOTIFY placeholderChanged)
		Q_PROPERTY(QString text READ getText NOTIFY textChanged)
		Q_PROPERTY(Cadabra::Queen::Core::System::InputAlert::Flags flags READ getFlagSet WRITE setFlagSet NOTIFY flagsetChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit InputAlert(QObject* parent = nullptr);
		explicit InputAlert(QObject* parent, const QString& name, const QString& title, const QString& placeholder, Flags flagset = {}, DialogAlert::Level level = DialogAlert::Level::Info);
		virtual ~InputAlert() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _placeholder;
		QString _text;
		Flags _flagset;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual Q_INVOKABLE bool warn() override;
		Q_INVOKABLE bool complete(const QString& value);
		Q_INVOKABLE bool cancel();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getPlaceholder() const;
		const QString& getText() const;
		const Flags& getFlagSet() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setPlaceholder(const QString& value);
		void setFlagSet(Flags value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void placeholderChanged();
		void textChanged();
		void flagsetChanged();
		void completed();
		void canceled();
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Core::System::InputAlert::Flags)
Q_DECLARE_METATYPE(Cadabra::Queen::Core::System::InputAlert::Flags)

#endif
