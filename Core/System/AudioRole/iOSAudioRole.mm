#include "../AudioRole.hpp"

#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAudioSession.h>

#include <QDebug>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	void AudioRole::activate()
	{
		/// @???
		[[AVAudioSession sharedInstance] setActive:YES error:nil];
	}

	void AudioRole::deactivate()
	{
		/// @???
		[[AVAudioSession sharedInstance] setActive:NO error:nil];
	}

	AudioRole::Mode AudioRole::detectMode() const
	{
		NSString* category = [[AVAudioSession sharedInstance] category];
		if ([category isEqual:AVAudioSessionCategoryAmbient]) {
			return Mode::Ambient;
		} else if ([category isEqual:AVAudioSessionCategorySoloAmbient]) {
			return Mode::SoloAmbient;
		} else if ([category isEqual:AVAudioSessionCategoryPlayback]) {
			return Mode::Playback;
		} else if ([category isEqual:AVAudioSessionCategoryRecord]) {
			return Mode::Record;
		} else if ([category isEqual:AVAudioSessionCategoryPlayAndRecord]) {
			return Mode::PlayAndRecord;
		} else if ([category isEqual:AVAudioSessionCategoryAudioProcessing]) {
			return Mode::Processing;
		}
		return Mode::Unknown;
	}

	AudioRole::OptionFlags AudioRole::detectOptionFlagSet() const
	{
		AVAudioSessionCategoryOptions options = [[AVAudioSession sharedInstance] categoryOptions];
	}

	bool AudioRole::apply(const Mode& mode, const OptionFlags& option_flagset)
	{
		NSString* category(nil);
		switch (mode) {
			case Mode::Ambient:
				category = AVAudioSessionCategoryAmbient;
				break;
			case Mode::SoloAmbient:
				category = AVAudioSessionCategorySoloAmbient;
				break;
			case Mode::Playback:
				category = AVAudioSessionCategoryPlayback;
				break;
			case Mode::Record:
				category = AVAudioSessionCategoryRecord;
				break;
			case Mode::PlayAndRecord:
				category = AVAudioSessionCategoryPlayAndRecord;
				break;
			case Mode::Processing:
				category = AVAudioSessionCategoryAudioProcessing;
				break;

			default:
				return false;
		}

		NSUInteger options(0);
		if (option_flagset.testFlag(Option::Background)) {
//			options |= AVAudioSessionCategoryOptionMixWithOthers;
		}
		if (option_flagset.testFlag(Option::Mix)) {
			options |= AVAudioSessionCategoryOptionMixWithOthers;
		}
		if (option_flagset.testFlag(Option::Stream)) {
			options |= AVAudioSessionCategoryOptionAllowAirPlay;
		}

		NSError* error(nil);
		if (0 == options) {
			[[AVAudioSession sharedInstance] setCategory: category error:&error];
		} else {
			[[AVAudioSession sharedInstance] setCategory: category withOptions: options error:&error];
		}

		if (error) {
			qCritical() << "AudioRoile::apply() Error: " << QString::fromNSString(error.localizedDescription);
		}

		return !error;
	}
} } } }
