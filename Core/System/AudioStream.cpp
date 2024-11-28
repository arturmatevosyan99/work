#include "AudioStream.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	QStringList AudioStream::CodecList()
	{
		QStringList retval;
		void* i(nullptr);
		const AVCodec *c(nullptr);
		while ((c = av_codec_iterate(&i))) {
			retval << QString("%1(%2)").arg(c->long_name).arg(av_codec_is_encoder(c) ? "encoder" : "decoder");
		}
		return retval;
	}

	int AudioStream::DeviceWrite(void* opaque, uint8_t* buffer, int size)
	{
		QIODevice* io_device(reinterpret_cast<QIODevice*>(opaque));
		return !io_device ? 0 : io_device->write(reinterpret_cast<char*>(buffer), size);
	}

	int AudioStream::DeviceRead(void* opaque, uint8_t* buffer, int size)
	{
		QIODevice* io_device(reinterpret_cast<QIODevice*>(opaque));
		return !io_device ? 0 : io_device->read(reinterpret_cast<char*>(buffer), size);
	}

	int64_t AudioStream::DeviceSeek(void* opaque, int64_t offset, int whence)
	{
		if (whence == AVSEEK_SIZE) {			
			return -1;
		}
		QIODevice* io_device(reinterpret_cast<QIODevice*>(opaque));
		if (!io_device) {
			return -1;
		}
		if (io_device->isSequential()) {
			return -1;
		}
		if (!io_device->seek(offset)) {
			return -1;
		}
		return io_device->pos();
	}

	AudioStream::AudioStream(AudioStream::Codec codec, AudioStream::ChannelLayout channel_layout, std::int64_t bit_rate, int sample_rate, AudioStream::SampleFormat sample_format)
	:		
		QBuffer(),

		_codec(codec),
		_sample_format(sample_format),
		_sample_rate(sample_rate),
		_bit_rate(bit_rate),
		_channel_layout(channel_layout)
	{

	}

	AudioStream::~AudioStream()
	{

	}

	AudioStream::Codec AudioStream::getCodec() const
	{
		return _codec;
	}

	AudioStream::SampleFormat AudioStream::getSampleFormat() const
	{
		return _sample_format;
	}

	int AudioStream::getSampleRate() const
	{
		return _sample_rate;
	}

	std::int64_t AudioStream::getBitRate()
	{
		return _bit_rate;
	}

	AudioStream::ChannelLayout AudioStream::getChannelLayout() const
	{
		return _channel_layout;
	}

	bool AudioStream::isPCM() const
	{
		return (_codec <= Codec::PCMLXF && _codec >= Codec::PCMSigned16LittleEndian);
	}
} } } }
