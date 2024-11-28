#include "AudioRecorder.hpp"

#include <QAudioEncoderSettings>
#include <chrono>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	AudioRecorder::AudioRecorder(QObject* parent)
	:
		QObject(parent),

		_time_limit(0),
		_actual_duration(0),
		_role_mode(AudioRole::Instance()->getMode())
	{
	}

	bool AudioRecorder::start(qint64 limit)
	{
		if (!_input) {
			QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
			qDebug() << "Audio device name:" << info.deviceName();
			qDebug() << "Audio device supported byte orders:" << info.supportedByteOrders();
			qDebug() << "Audio device supported channel counts:" << info.supportedChannelCounts();
			qDebug() << "Audio device supported codecs:" << info.supportedCodecs();
			qDebug() << "Audio device supported sample rates:" << info.supportedSampleRates();
			qDebug() << "Audio device supported sample sizes:" << info.supportedSampleSizes();
			qDebug() << "Audio device supported sample types:" << info.supportedSampleTypes();
			qDebug() << "Available audio muxers:" << Core::System::AudioFile::MuxerList();
			qDebug() << "Available audio demuxers:" << Core::System::AudioFile::DemuxerList();
			qDebug() << "Available audio codecs:" << Core::System::AudioStream::CodecList();

			QAudioFormat format(info.preferredFormat());
			format.setSampleRate(48000);
			format.setChannelCount(1);
			format.setSampleSize(16);
			format.setCodec("audio/pcm");
			format.setByteOrder(QAudioFormat::LittleEndian);
			format.setSampleType(QAudioFormat::SignedInt);

			if (!info.isFormatSupported(format)) {
				qDebug() << "Format is not supported";
				qDebug() << "Codec:" << format.codec();
				qDebug() << "Sample rate:" << format.sampleRate();
				qDebug() << "Sample size:" << format.sampleSize();
				qDebug() << "Sample type:" << format.sampleType();
				qDebug() << "Byte order:" << format.byteOrder();
				qDebug() << "Channel count:" << format.channelCount();
				return false;
			}
			_input.reset(new QAudioInput(info, format));
			QObject::connect(_input.get(), &QAudioInput::stateChanged, this, &AudioRecorder::onStateChanged);
			QObject::connect(_input.get(), &QAudioInput::notify, this, &AudioRecorder::onNotified);
			_role_mode = AudioRole::Instance()->getMode();
			AudioRole::Instance()->setMode(AudioRole::Mode::PlayAndRecord);
		} else if (_input->state() != QAudio::State::StoppedState) { /// @xxx Могут быть и другие кейсы
			return false;
		}
		emit flushed();
		_actual_duration = 0;
		_stream.reset(new Core::System::AudioStream(Core::System::AudioStream::Codec::PCMSigned16LittleEndian, Core::System::AudioStream::ChannelLayout::Mono, 16000, 48000, Core::System::AudioStream::SampleFormat::Signed16));
		if (!_stream->open(QBuffer::ReadWrite)) {
			return false;
		}
		_time_limit = limit;
		_input->start(_stream.get());
		const QAudio::Error error(_input->error());
		if (error == QAudio::Error::NoError) {
			emit stateChanged(State::On);
			return true;
		}
		return false;
	}

	bool AudioRecorder::pause()
	{
		if (_input->state() == QAudio::State::SuspendedState) {
			_input->resume();
			emit stateChanged(State::On);
			return true;
		} else if (_input->state() == QAudio::State::ActiveState) {
			_input->suspend();
			emit stateChanged(State::Suspend);
			return true;
		}
		return false;
	}

	bool AudioRecorder::stop()
	{
		if (!_input || _input->state() == QAudio::StoppedState || !_stream) {
			return false;
		}
		_input->stop();
		QObject::disconnect(_input.get(), &QAudioInput::stateChanged, this, &AudioRecorder::onStateChanged);
		QObject::disconnect(_input.get(), &QAudioInput::notify, this, &AudioRecorder::onNotified);
		_input->reset();
		_input.reset();
		emit stateChanged(State::Off);

		AudioRole::Instance()->setMode(_role_mode);
		return true;
	}

	bool AudioRecorder::flush()
	{
		if (!_stream || _actual_duration <= 0) {
			return true;
		}
		_file.reset(new AudioFile());
		if (_file->initialize(Core::System::AudioFile::Container::OGG, 1, 16)) {
			if (_file->encode(_stream, Core::System::AudioStream::Codec::Opus, Core::System::AudioStream::ChannelLayout::Mono, 16000, 48000, Core::System::AudioStream::SampleFormat::None)) {
				if (_file->flush()) {
					_stream.reset();
					emit flushed();
					return true;
				}
			}
		}
		return false;
	}

	qint64 AudioRecorder::getActualDuration() const
	{
		return _actual_duration;
	}

	QBuffer* AudioRecorder::getBuffer() const
	{
		return _file ? _file.get() : nullptr;
	}

	QTime AudioRecorder::makeDurationTime() const
	{
		return QTime::fromMSecsSinceStartOfDay(_actual_duration);
	}

	void AudioRecorder::onStateChanged(QAudio::State state)
	{
		qDebug() << "State changed:" << state;
		switch (state) {
			case QAudio::StoppedState:
				if (_input->error() != QAudio::NoError) {
					qDebug() << "Audio input error";
					/// @todo Error handling
				} else {
					/// @todo Finished recording
				}
				break;

			case QAudio::ActiveState: /// < Started recording - read from IO device
				break;

			default: /// < ... other cases as appropriate
				break;
		}
	}

	void AudioRecorder::onNotified()
	{
		qDebug() << "Seconds progressed:" << _input->processedUSecs();
		const std::chrono::microseconds micro_duration(_input->processedUSecs());
		const qint64 milli_duration(std::chrono::duration_cast<std::chrono::milliseconds>(micro_duration).count());

		_actual_duration = milli_duration;
		emit durationChanged(milli_duration);
		if (_time_limit > 0 && _time_limit <= milli_duration) {
			stop();
		}
	}

	bool AudioRecorder::isBusy() const
	{
		return nullptr != _input;
	}

	bool AudioRecorder::isEmpty() const
	{
		return _actual_duration == 0;
	}
} } } }
