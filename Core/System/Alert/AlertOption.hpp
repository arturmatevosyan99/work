#ifndef Cadabra_Queen_Core_System_Alert_AlertOption_hpp
#define Cadabra_Queen_Core_System_Alert_AlertOption_hpp

#include <QObject>

#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class AlertOption : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Flag
		{
			Ok = 0x00000400, /// > An "OK" button defined with the AcceptRole.
			Open = 0x00002000, /// > An "Open" button defined with the AcceptRole.
			Save = 0x00000800, /// > A "Save" button defined with the AcceptRole.
			Cancel = 0x00400000, /// > A "Cancel" button defined with the RejectRole.
			Close = 0x00200000, /// > A "Close" button defined with the RejectRole.
			Discard = 0x00800000, /// > A "Discard" or "Don't Save" button, depending on the platform, defined with the DestructiveRole.
			Apply = 0x02000000, // > An "Apply" button defined with the ApplyRole.
			Reset = 0x04000000, /// > A "Reset" button defined with the ResetRole.
			Restore = 0x08000000, // > A "Restore Defaults" button defined with the ResetRole.
			Help = 0x01000000, /// > A "Help" button defined with the HelpRole.
			SaveAll = 0x00001000, /// > A "Save All" button defined with the AcceptRole.
			Yes = 0x00004000, /// > A "Yes" button defined with the YesRole.
			YesToAll = 0x00008000, /// > A "Yes to All" button defined with the YesRole.
			No = 0x00010000, /// > A "No" button defined with the NoRole.
			NoToAll = 0x00020000, /// A "No to All" button defined with the NoRole.
			Abort = 0x00040000, /// > An "Abort" button defined with the RejectRole.
			Retry = 0x00080000, /// > A "Retry" button defined with the AcceptRole.
			Ignore = 0x00100000, /// > An "Ignore" button defined with the AcceptRole.
			Custom = 0x00000000 /// > An invalid button.
		};
		Q_DECLARE_FLAGS(Flags, Flag)
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Flag)
		Q_PROPERTY(QString caption READ getCaption WRITE setCaption NOTIFY captionChanged)
		Q_PROPERTY(Flag flag READ getFlag WRITE setFlag NOTIFY flagChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AlertOption(QObject* parent = nullptr);
		AlertOption(QObject* parent, Flag Flag = Flag::Ok);
		AlertOption(QObject* parent, Flag Flag, const QString& caption);
		virtual ~AlertOption() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Flag _flag;
		QString _caption;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void opt();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getCaption() const;
		Flag getFlag() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setCaption(const QString& value);
		void setCaption(Flag flag);
		void setFlag(Flag flag);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void opted();
		void captionChanged();
		void flagChanged();
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Core::System::AlertOption::Flags)
Q_DECLARE_METATYPE(Cadabra::Queen::Core::System::AlertOption::Flags)

#endif
