/**
 @file AudioRole.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "AudioRole.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	AudioRole* AudioRole::Instance()
	{
		static AudioRole retval;
		return &retval;
	}

	QObject* AudioRole::QInstance(QQmlEngine* engine, QJSEngine* scriptEngine)
	{
		Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)

		AudioRole* retval(AudioRole::Instance());

		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}

	AudioRole::AudioRole(QObject *parent)
	:
		QObject(parent),

		_default_mode(Mode::Unknown),
		_default_option_flagset(Option::None),
		_mode(Mode::Unknown),
		_option_flagset(Option::None)
	{
		activate();

		_default_mode = detectMode();
		_default_option_flagset = detectOptionFlagSet();
		_mode = _default_mode;
		_option_flagset = _default_option_flagset;
	}

	AudioRole::~AudioRole()
	{
		deactivate();
	}

	bool AudioRole::store(const Mode& mode, const OptionFlags& option_flagset)
	{
		if (!apply(mode, option_flagset)) {
			return false;
		}
		_default_mode = mode;
		_default_option_flagset = option_flagset;
		emit defaulted();

		return true;
	}

	bool AudioRole::restore()
	{
		if (!apply(_default_mode, _default_option_flagset)) {
			return false;
		}
		_mode = _default_mode;
		_option_flagset = _default_option_flagset;
		return true;
	}

	bool AudioRole::monitor(bool onoff)
	{
		if (onoff) {
			if (_monitor) {
				return false;
			}
			_monitor = QObject::connect(Native::Instance(), &Native::applicationStateChanged, std::bind(&AudioRole::onApplicationStateChanged, this, _mode, _option_flagset, std::placeholders::_1, std::placeholders::_2));
		} else {
			if (!_monitor) {
				return false;
			}
			QObject::disconnect(_monitor);
			_monitor = QMetaObject::Connection();
		}
		emit monitored();
		return true;
	}

	AudioRole::Mode AudioRole::getDefaultMode() const
	{
		return _default_mode;
	}

	AudioRole::OptionFlags AudioRole::getDefaultOptionFlagSet() const
	{
		return _default_option_flagset;
	}

	AudioRole::Mode AudioRole::getMode() const
	{
		return _mode;
	}

	AudioRole::OptionFlags AudioRole::getOptionFlagSet() const
	{
		return _option_flagset;
	}

	void AudioRole::setDefaultMode(Mode mode)
	{
		if (mode != _default_mode) {
			_default_mode = mode;
			emit defaulted();
		}
	}

	void AudioRole::setDefaultOptionFlagSet(OptionFlags option_flagset)
	{
		if (option_flagset != _default_option_flagset) {
			_default_option_flagset = option_flagset;
			emit defaulted();
		}
	}

	void AudioRole::setMode(Mode mode)
	{
		if (mode != _mode && apply(mode, _option_flagset)) {
			emit changed();
		}
	}

	void AudioRole::setOptionFlagSet(OptionFlags option_flagset)
	{
		if (option_flagset != _option_flagset && apply(_mode, option_flagset)) {
			emit changed();
		}
	}

	void AudioRole::onApplicationStateChanged(Mode mode, OptionFlags option_flagset, Core::ApplicationState previous, Core::ApplicationState current)
	{
		if (previous != current && Core::ApplicationState::ActiveApplication == current && apply(mode, option_flagset)) {
			_mode = mode;
			_option_flagset = option_flagset;
		}
	}

	bool AudioRole::hasMonitor() const
	{
		return _monitor;
	}
} } } }
