#ifndef Cadabra_Queen_Afrodita_hpp
#define Cadabra_Queen_Afrodita_hpp

#include "Skeleton.hpp"

#include "Core/System/Native.hpp"
#include "Core/System/AudioRole.hpp"

namespace Cadabra { namespace Queen {
	class Afrodita : public Skeleton
	{
	/** @name Constructors */
	/** @{ */
	public:
		Afrodita(int& argc, char** argv, int flags = ApplicationFlags);
		virtual ~Afrodita() override = default;
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
