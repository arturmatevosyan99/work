#ifndef Cadabra_Queen_Core_System_DiagnosticBundle_hpp
#define Cadabra_Queen_Core_System_DiagnosticBundle_hpp

#include "../../Cupboard.hpp"
#include "../../Skeleton.hpp"
#include "../Net/Call/DiagnosticCall.hpp"

#include <QObject>
#include <QFutureWatcher>
#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class DiagnosticBundle : public Cadabra::Queen::Cupboard
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		DiagnosticBundle(QObject* parent = nullptr);
		virtual ~DiagnosticBundle() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Queen::Skeleton* _skeleton;
		QFutureWatcher<void> _discloser;
		Net::DiagnosticCall _uploader;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool collect();
		Q_INVOKABLE bool upload();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void collected();
		void loaded(double progress);
		void faulted(const QString& error);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onCollected();
	/** @} */
	};
} } } }

#endif
