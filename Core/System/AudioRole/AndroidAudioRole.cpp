#include "../AudioRole.hpp"

#include <QtAndroid>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	void AudioRole::activate()
	{

	}

	void AudioRole::deactivate()
	{

	}

	AudioRole::Mode AudioRole::detectMode() const
	{
		return AudioRole::Unknown;
	}

	AudioRole::OptionFlags AudioRole::detectOptionFlagSet() const
	{
		return {};
	}

	bool AudioRole::apply(const Mode& mode, const OptionFlags& option_flagset)
	{
		return false;
	}
} } } }
