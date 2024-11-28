#ifndef Cadabra_Queen_Regina_hpp
#define Cadabra_Queen_Regina_hpp

#include "Skeleton.hpp"

#include "Core/System/Native.hpp"
#include "Core/System/AudioRole.hpp"

namespace Cadabra { namespace Queen {
	class Regina : public Skeleton
	{
	/** @name Constructors */
	/** @{ */
	public:
		Regina(int& argc, char** argv, int flags = ApplicationFlags);
		virtual ~Regina() override = default;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual bool tune() override;
		virtual bool extend() override;
		virtual int boot() override;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onApplicationStateChanged(Core::ApplicationState previous, Core::ApplicationState current);
	/** @} */
	};
} }

#endif
