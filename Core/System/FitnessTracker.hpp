#ifndef Cadabra_Queen_Core_System_FitnessTracker_hpp
#define Cadabra_Queen_Core_System_FitnessTracker_hpp

#include <QObject>
#include <QQmlEngine>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class FitnessTracker : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(bool hasAccess READ hasAccess NOTIFY registered)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class Family : char
		{
			Apple = 'A',
			Google = 'G',
			Cadabra = 'C'
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static FitnessTracker* Instance();
		static QObject* QInstance(QQmlEngine* engine, QJSEngine* scriptEngine);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit FitnessTracker() = default;
		virtual ~FitnessTracker() = default;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool requestAccess() = 0;
		virtual bool revokeAccess() = 0;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual Family getFamily() const = 0;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void registered();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool hasAccess() const = 0;
	/** @} */
	};
} } } }

#endif
