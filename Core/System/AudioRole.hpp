#ifndef Cadabra_Queen_Core_System_AudioRole_hpp
#define Cadabra_Queen_Core_System_AudioRole_hpp

#include "Native.hpp"

#include <QObject>
#include <QQmlEngine>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class AudioRole : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Mode {
			Unknown,
			Playback,
			PlayAndRecord,
			Record,
			Ambient,
			SoloAmbient,
			Processing
		};

		enum Option {
			None = 0b000000,
			Mix = 0b000001,
			Stream = 0b000010,
			Background = 0b000100
		};
		Q_DECLARE_FLAGS(OptionFlags, Option)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static AudioRole* Instance();
		static QObject* QInstance(QQmlEngine* engine, QJSEngine* scriptEngine);
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT		
		Q_ENUM(Mode)
		Q_FLAG(OptionFlags)
		Q_PROPERTY(Mode defaultMode READ getDefaultMode WRITE setDefaultMode NOTIFY defaulted)
		Q_PROPERTY(OptionFlags defaultOptions READ getDefaultOptionFlagSet WRITE setDefaultOptionFlagSet NOTIFY defaulted)
		Q_PROPERTY(Mode mode READ getMode WRITE setMode NOTIFY changed)
		Q_PROPERTY(OptionFlags options READ getOptionFlagSet WRITE setOptionFlagSet NOTIFY changed)
		Q_PROPERTY(bool hasMonitor READ hasMonitor NOTIFY monitored)
	/** @} */

	/** @name Constructors */
	/** @{ */
	private:
		explicit AudioRole(QObject* parent = nullptr);
		virtual ~AudioRole() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	public:		
		Mode _default_mode;
		OptionFlags _default_option_flagset;

		Mode _mode;
		OptionFlags _option_flagset;

		QMetaObject::Connection _monitor;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool store(const Mode& mode, const OptionFlags& option_flagset);
		Q_INVOKABLE bool restore();
		Q_INVOKABLE bool monitor(bool onoff);

	private:
		void activate();
		void deactivate();
		Mode detectMode() const;
		OptionFlags detectOptionFlagSet() const;
		bool apply(const Mode& mode, const OptionFlags& option_flagset);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Mode getDefaultMode() const;
		OptionFlags getDefaultOptionFlagSet() const;
		Mode getMode() const;
		OptionFlags getOptionFlagSet() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setDefaultMode(Mode mode);
		void setDefaultOptionFlagSet(OptionFlags option_flagset);
		void setMode(Mode mode);
		void setOptionFlagSet(OptionFlags option_flagset);
	/** @} */

	/** @name Signals*/
	/** @{ */
	signals:
		void defaulted();
		void changed();
		void monitored();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onApplicationStateChanged(Mode mode, OptionFlags option_flagset, Core::ApplicationState previous, Core::ApplicationState current);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasMonitor() const;
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Core::System::AudioRole::OptionFlags)
Q_DECLARE_METATYPE(Cadabra::Queen::Core::System::AudioRole::OptionFlags)

#endif
