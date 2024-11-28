#include "../AudioRole.hpp"

#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	void AudioRole::activate()
	{
//		[[AVAudioSession sharedInstance] setActive:YES error:nil];
	}

	void AudioRole::deactivate()
	{
//		[[AVAudioSession sharedInstance] setActive:NO error:nil];
	}

	AudioRole::Mode AudioRole::detectMode() const
	{
		return Mode::Unknown;
	}

	AudioRole::OptionFlags AudioRole::detectOptionFlagSet() const
	{
		return AudioRole::Option::Mix;
	}

	bool AudioRole::apply(const Mode& mode, const OptionFlags& option_flagset)
	{
		return false;
	}
} } } }
