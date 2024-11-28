/**
 @file SoundAlert.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "SoundAlert.hpp"

#include <QThread>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	SoundAlert::SoundAlert(QObject* parent)
	:
		AbstractAlert(parent)
	{
		QObject::connect(this, &QObject::objectNameChanged, this, &SoundAlert::onSourced);
		QObject::connect(&_effect, &QSoundEffect::playingChanged, this, &SoundAlert::onPlayed);
	}

	SoundAlert::SoundAlert(QObject* parent, const QString& name, AbstractAlert::Level level)
	:
		AbstractAlert(parent, name, "Sound", level)
	{
		QObject::connect(this, &QObject::objectNameChanged, this, &SoundAlert::onSourced);
		QObject::connect(&_effect, &QSoundEffect::playingChanged, this, &SoundAlert::onPlayed);
	}

	bool SoundAlert::warn()
	{
		if (QObject::objectName().isEmpty()) {
			return false;
		} else if (QThread::currentThread() != QObject::thread()) {
			/// @ugh
			QObject::setObjectName(QObject::objectName());
		} else {
			_effect.play();
		}
		return true;
	}

	void SoundAlert::onSourced(const QString& url)
	{
		_effect.setSource(url);
		_effect.setLoopCount(1);
		_effect.play();
	}

	void SoundAlert::onPlayed()
	{
		if (!_effect.isPlaying() && _effect.loopsRemaining() == 0) {
			deleteLater();
		}
	}
} } } }
