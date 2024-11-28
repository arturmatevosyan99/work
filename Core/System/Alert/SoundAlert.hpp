#ifndef Cadabra_Queen_Core_System_Alert_SoundAlert_hpp
#define Cadabra_Queen_Core_System_Alert_SoundAlert_hpp

#include "../Alert.hpp"

#include <QSoundEffect>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class SoundAlert : public AbstractAlert
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit SoundAlert(QObject* parent = nullptr);
		explicit SoundAlert(QObject* parent, const QString& name, AbstractAlert::Level level = AbstractAlert::Level::Info);
		virtual ~SoundAlert() override = default;
	/** @} */

	/** @name Proeprties */
	/** @{ */
	private:
		QSoundEffect _effect;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual Q_INVOKABLE bool warn() override;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
		void onSourced(const QString& url);
		void onPlayed();
	/** @} */
	};
} } } }

#endif
