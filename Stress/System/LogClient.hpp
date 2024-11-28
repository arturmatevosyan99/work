#ifndef Cadabra_Queen_Stress_System_LogClient_hpp
#define Cadabra_Queen_Stress_System_LogClient_hpp

#include "../Stress.hpp"

#include <QObject>
#include <QByteArray>

namespace Cadabra { namespace Queen { namespace Stress { namespace System {
	class LogClient : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		LogClient(LogFormat format, QObject* parent = nullptr);
		virtual ~LogClient() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		const LogFormat _format;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool send(const QString& message);

	protected:
		virtual bool write(const QByteArray& message) = 0;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	public slots:
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
