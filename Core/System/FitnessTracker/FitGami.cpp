#include "../FitnessTracker.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class FitGami : public FitnessTracker
	{
	public:
		FitGami()
		:
			FitnessTracker()
		{
		}

		virtual ~FitGami() override = default;

	public:
		virtual bool requestAccess() override
		{
			return true;
		}

		virtual bool revokeAccess() override
		{
			return true;
		}

		virtual FitGami::Family getFamily() const override
		{
			return FitGami::Family::Cadabra;
		}

		virtual bool hasAccess() const override
		{
			return false;
		}
	};

	FitnessTracker* FitnessTracker::Instance()
	{
		static Cadabra::Queen::Core::System::FitGami retval;
		return &retval;
	}
} } } }
