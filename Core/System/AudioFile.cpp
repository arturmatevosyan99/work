#include "AudioFile.hpp"

extern "C" {
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	QStringList AudioFile::MuxerList()
	{
		QStringList retval;
		void* i(nullptr);
		const AVOutputFormat *f(nullptr);
		while ((f = av_muxer_iterate(&i))) {
			retval << f->long_name;
		}
		return retval;
	}

	QStringList AudioFile::DemuxerList()
	{
		QStringList retval;
		void* i(nullptr);
		const AVInputFormat *f(nullptr);
		while ((f = av_demuxer_iterate(&i))) {
			retval << f->long_name;
		}
		return retval;
	}

	AudioFile::AudioFile()
	:		
		QBuffer(&_buffer),
		_format_context(nullptr),
		_container(Container::WAV),
		_channel_count(1),
		_bit_rate(16)
	{

	}

	AudioFile::~AudioFile()
	{

	}

	bool AudioFile::initialize(Container container, int channel_count, int bit_rate)
	{
		if (_format_context || !open(QIODevice::ReadWrite)) {
			return false;
		}

		decltype(av_guess_format(nullptr, nullptr, nullptr)) format;
		switch (container) {
			case Container::OGG:
				format = av_guess_format(nullptr, "file.ogg", nullptr);
				break;
			default:
				format = nullptr;
		}
		if (!format) {
			return false;
		}

		QScopedPointer<unsigned char, AVIOBufferDeleter> io_buffer(reinterpret_cast<unsigned char*>(av_malloc(AudioFile::BufferSize + AV_INPUT_BUFFER_PADDING_SIZE)));
		_io_context.reset(avio_alloc_context(io_buffer.get(), AudioFile::BufferSize, 1, reinterpret_cast<void*>(this), &AudioStream::DeviceRead, &AudioStream::DeviceWrite, &AudioStream::DeviceSeek));
		if (!_io_context) {
			return false;
		}
		io_buffer.take();
		AVFormatContext* format_context(nullptr);
		if (avformat_alloc_output_context2(&format_context, format, nullptr, nullptr) < 0 || !format_context) {
			return false;
		}
		_format_context.reset(format_context);
		_format_context->pb = _io_context.get();

		_container = container;
		_channel_count = channel_count;
		_bit_rate = bit_rate;

		return true;
	}

	bool AudioFile::tuneFrame(AVFrame* frame, AVCodecContext* codec_context)
	{
		if (!frame || !codec_context) {
			return false;
		}

//		const int frame_size = FFMIN(av_audio_fifo_size(fifo), codec_context->frame_size);
		frame->nb_samples = codec_context->frame_size;
		frame->format = codec_context->sample_fmt;
		frame->channel_layout = codec_context->channel_layout;
		frame->channels = codec_context->channels;
		frame->sample_rate = codec_context->sample_rate;

		return av_frame_get_buffer(frame, 0) == 0;
	}

	bool AudioFile::flush()
	{
		if (!_format_context || _encode_vector.isEmpty()) {
			return false;
		}

		if (avformat_write_header(_format_context.get(), nullptr) < 0) {
			_format_context.reset();
			return false;
		}

		for (int e=0; e < _encode_vector.length(); ++e) {
			EncodeOperation& encode_operation(_encode_vector[e]);
			encode_operation.source_stream->seek(0);
			if (!encode_operation.source_stream->isReadable() || encode_operation.source_stream->buffer().count() <= 0) {
				return false;
			}

			auto /*(const)AVCodec* */ codec(avcodec_find_decoder(static_cast<AVCodecID>(encode_operation.source_stream->getCodec())));
			if (!codec) {
				return false;
			}

			QScopedPointer<AVCodecParserContext, AVCodecParserContextDeleter> parser_context(nullptr);
			if (!encode_operation.source_stream->isPCM()) {
				parser_context.reset(av_parser_init(codec->id));
				if (!parser_context) {
					return false;
				}
			}

			QScopedPointer<SwrContext, SwrContextDeleter> swr_context(swr_alloc_set_opts(nullptr, encode_operation.codec_context->channel_layout, encode_operation.codec_context->sample_fmt, encode_operation.codec_context->sample_rate, static_cast<uint64_t>(encode_operation.source_stream->getChannelLayout()), static_cast<AVSampleFormat>(encode_operation.source_stream->getSampleFormat()), encode_operation.source_stream->getSampleRate(), 0, nullptr));
			if (!swr_context) {
				return false;
			}
			if (swr_init(swr_context.get()) < 0) {
				return false;
			}

			QScopedPointer<AVCodecContext, AVCodecContextDeleter> codec_context(avcodec_alloc_context3(codec));
			if (!codec_context) {
				return false;
			}
			codec_context->bit_rate = encode_operation.source_stream->getBitRate();
			codec_context->sample_rate = encode_operation.source_stream->getSampleRate();
			codec_context->sample_fmt = static_cast<AVSampleFormat>(encode_operation.source_stream->getSampleFormat());
			codec_context->channel_layout = encode_operation.source_stream->getChannelLayout();
			codec_context->channels = av_get_channel_layout_nb_channels(codec_context->channel_layout);

			if (avcodec_open2(codec_context.get(), codec, nullptr) < 0) {
				return false;
			}

			/// @brief Packet for holding encoded input
			QScopedPointer<AVPacket, AVPacketDeleter> in_packet(av_packet_alloc());
			if (!in_packet) {
				return false;
			}			

			/// @brief Packet for holding encoded output
			QScopedPointer<AVPacket, AVPacketDeleter> out_packet(av_packet_alloc());
			if (!out_packet) {
				return false;
			}

			/// @brief Frame containing input raw audio
			QScopedPointer<AVFrame, AVFrameDeleter> input_frame(av_frame_alloc());
			if (!input_frame) {
				return false;
			}
			const int input_frame_size(av_get_bytes_per_sample(codec_context->sample_fmt) * codec_context->channels);

			/// @brief Frame containing converted input raw audio
			QScopedPointer<AVFrame, AVFrameDeleter> resampled_frame(av_frame_alloc());
			if (!resampled_frame) {
				return false;
			}
			resampled_frame->nb_samples     = encode_operation.codec_context->frame_size;
			resampled_frame->channel_layout = encode_operation.codec_context->channel_layout;
			resampled_frame->format         = encode_operation.codec_context->sample_fmt;
			resampled_frame->sample_rate    = encode_operation.codec_context->sample_rate;
			/// @note Allocate the samples of the created frame. This call will make sure that the audio frame can hold as many samples as specified
			if (av_frame_get_buffer(resampled_frame.get(), 0) < 0) {
				/// @note Could not allocate output frame samples
				return false;
			}

			int output_pts(0);
			AVFrame* output_frame(nullptr);			

			while (encode_operation.source_stream->isReadable()) {
				QScopedPointer<uint8_t> padded_buffer(new uint8_t[input_frame_size + AV_INPUT_BUFFER_PADDING_SIZE]);
				memset(padded_buffer.get() + input_frame_size, 0, AV_INPUT_BUFFER_PADDING_SIZE);
				qint64 read_size(encode_operation.source_stream->read(reinterpret_cast<char*>(padded_buffer.get()), input_frame_size));
				if (0 == read_size) {
					break;
				}
				in_packet->size = read_size;
				in_packet->data = padded_buffer.get();
				int  decode_retval(avcodec_send_packet(codec_context.get(), in_packet.get()));

				if (decode_retval < 0) {
					if (decode_retval == AVERROR(EAGAIN)) { /// < input is not accepted in the current state - user must read output with avcodec_receive_frame() (once all output is read, the packet should be resent, and the call will not fail with EAGAIN)
						/// @brief Not ready
						break;
					} else if (decode_retval == AVERROR_EOF) { /// < the decoder has been flushed, and no new packets can be sent to it (also returned if more than 1 flush packet is sent)
						break;
					} else if (decode_retval == AVERROR(EINVAL)) { /// < codec not opened, it is an encoder, or requires flush
						return false;
					} else if (decode_retval == AVERROR(ENOMEM)) { /// < failed to add packet to internal queue, or similar
						return false;
					} else {
						//char error[1024];
						//av_make_error_string(error, 1024, decode_retval);
						/// @note General error
						return false;
					}
				}

				/// @brief read all the output frames (in general there may be any number of them
				decode_retval = avcodec_receive_frame(codec_context.get(), input_frame.get());
				if (decode_retval < 0) {
					if (decode_retval == AVERROR(EAGAIN)) {
						break;
					} else if (decode_retval == AVERROR(EINVAL)) {
						return false;
					} else if (decode_retval == AVERROR_EOF) {
						break;
					} else {
						/// @note General error
						return false;
					}
				}

				/// @note or input_frame if we do not need a resample
//				output_frame = input_frame.get();
				output_frame = nullptr;

				/// @brief Resampling (CHTOBONSDOH!)
				if (!output_frame) {
					uint8_t* converted_data(nullptr);
					if (av_samples_alloc(&converted_data, nullptr, encode_operation.codec_context->channels, resampled_frame->nb_samples, encode_operation.codec_context->sample_fmt, 0) < 0) {
						/// @brief Could not allocate samples
						return false;
					}
					int samples_count(swr_convert(swr_context.get(), nullptr, 0, (const uint8_t**)input_frame->data, input_frame->nb_samples));
					if (samples_count < 0) {
						/// @brief Could not convert
						return false;
					}
					while (true) {
						samples_count = swr_get_out_samples(swr_context.get(), 0);
						if (samples_count < encode_operation.codec_context->frame_size * encode_operation.codec_context->channels) {
							break;
						}
						samples_count = swr_convert(swr_context.get(), &converted_data, resampled_frame->nb_samples, nullptr, 0);
						size_t buffer_size = av_samples_get_buffer_size(nullptr, encode_operation.codec_context->channels, resampled_frame->nb_samples, encode_operation.codec_context->sample_fmt, 0);
						if (buffer_size < 0) {
							/// @brief Invalid buffer size
							return false;
						}
						if (avcodec_fill_audio_frame(resampled_frame.get(), encode_operation.codec_context->channels, encode_operation.codec_context->sample_fmt, converted_data, buffer_size, 0) < 0) {
							/// @brief Could not fill frame
							return false;
						}
						output_frame = resampled_frame.get();
					}
				}
				if (!output_frame) {
					continue;
				}

				/// @note Set a timestamp based on the sample rate for the container
				output_frame->pts = output_pts;
				output_pts += output_frame->nb_samples;

				/// @note Send the frame for encoding
				int encode_retval(avcodec_send_frame(encode_operation.codec_context.get(), output_frame));
				if (encode_retval < 0) {
					/// @brief Error sending the frame to the encoder
					return false;
				}
				/// @note Read all the available output packets (in general there may be any number of them)
				while (encode_retval >= 0) {
					encode_retval = avcodec_receive_packet(encode_operation.codec_context.get(), out_packet.get());
					if (encode_retval == AVERROR(EAGAIN) || encode_retval == AVERROR_EOF) {
						/// @note No more packets are available
						break;
					} else if (encode_retval < 0) {
						/// @note Error encoding audio frame
						return false;
					}
					out_packet->stream_index = encode_operation.target_stream->index;
					if (av_interleaved_write_frame(_format_context.get(), out_packet.get()) < 0) {
						return false;
					}
				}
			}

			int flush_retval(avcodec_send_frame(encode_operation.codec_context.get(), nullptr));
			if (flush_retval < 0) {
				/// @note Error sending the frame to the encoder
				return false;
			}

			/// @brief read all the available output packets (in general there may be any number of them
			while (flush_retval >= 0) {
				flush_retval = avcodec_receive_packet(encode_operation.codec_context.get(), out_packet.get());
				if (flush_retval == AVERROR(EAGAIN) || flush_retval == AVERROR_EOF) {
					break;
				} else if (flush_retval < 0) {
					/// @brief Error encoding audio frame
					return false;
				}
				out_packet->stream_index = encode_operation.target_stream->index;
				av_packet_rescale_ts(out_packet.get(), encode_operation.codec_context->time_base, encode_operation.target_stream->time_base);
				if (av_interleaved_write_frame(_format_context.get(), out_packet.get()) < 0) {
					return false;
				}
			}
		}

		_format_context.reset();
		return true;
	}

	bool AudioFile::encode(QSharedPointer<AudioStream> source_stream, AudioStream::Codec target_codec, AudioStream::ChannelLayout target_channel_layout, std::int64_t target_bit_rate, int target_sample_rate, AudioStream::SampleFormat target_sample_format)
	{
		if (!_format_context) {
			return false;
		}

		/// @brief Output codec
		auto/* [const]AVCodec* */ codec(avcodec_find_encoder(static_cast<AVCodecID>(target_codec)));
		if (!codec) {
			return false;
		}

		QSharedPointer<AVCodecContext> codec_context(avcodec_alloc_context3(codec), AVCodecContextDeleter::cleanup);
		if (!codec_context) {
			return false;
		}
		AVStream* target_stream(avformat_new_stream(_format_context.get(), nullptr));
		if (!target_stream) {
			return false;
		}

		codec_context->sample_fmt = AV_SAMPLE_FMT_NONE;
		codec_context->bit_rate = target_bit_rate;
		codec_context->sample_rate = 0;
		codec_context->channel_layout = AV_CH_LAYOUT_NATIVE;
		codec_context->channels = av_get_channel_layout_nb_channels(codec_context->channel_layout);

		/// @brief Sample format
		if (target_sample_format == AudioStream::SampleFormat::None) {
			const enum AVSampleFormat* codec_format(codec->sample_fmts);
			codec_context->sample_fmt = *codec_format;
		} else {
			const enum AVSampleFormat preferred_sample_format(static_cast<AVSampleFormat>(target_sample_format));
			const enum AVSampleFormat* codec_format(codec->sample_fmts);
			while (*codec_format != AV_SAMPLE_FMT_NONE) {
				if (*codec_format == preferred_sample_format) {
					codec_context->sample_fmt = preferred_sample_format;
					//break;
				}
				codec_format++;
			}
		}
		if (AV_SAMPLE_FMT_NONE == codec_context->sample_fmt) {
			return false;
		}

		/// @brief Sample rate
		if (!codec->supported_samplerates) {
			if (target_sample_rate == 0) {
				codec_context->sample_rate = 44100;
			}
		} else {
			int best_samplerate = 0;
			const int* sample(codec->supported_samplerates);
			while (*sample) {
				if (!best_samplerate || abs(44100 - *sample) < abs(44100 - best_samplerate)) {
					if (0 == target_sample_rate) {
						best_samplerate = *sample;
					} else if (target_sample_rate == *sample) {
						best_samplerate = *sample;
						break;
					}
				}
				++sample;
			}
			if (best_samplerate == 0) {
				return false;
			} else {
				codec_context->sample_rate = best_samplerate;
			}
		}

		/// @brief Channel Layout
		uint64_t best_channel_layout = 0;
		if (!codec->channel_layouts) {
			if (target_channel_layout == AudioStream::ChannelLayout::Default || target_channel_layout == AudioStream::ChannelLayout::Mono) {
				best_channel_layout = AV_CH_LAYOUT_MONO;
			}
		} else {
			int best_nb_channels = 0;
			const uint64_t* channel(codec->channel_layouts);
			while (*channel) {
				if (target_channel_layout == AudioStream::ChannelLayout::Default) {
					int nb_channels = av_get_channel_layout_nb_channels(*channel);
					if (nb_channels > best_nb_channels) {
						best_channel_layout = *channel;
						best_nb_channels = nb_channels;
					}
				} else {
					if (AudioStream::ChannelLayout::Mono == target_channel_layout && *channel == AV_CH_LAYOUT_MONO) {
						best_channel_layout = AV_CH_LAYOUT_MONO;
						break;
					} else if (AudioStream::ChannelLayout::Stereo == target_channel_layout && *channel == AV_CH_LAYOUT_STEREO) {
						best_channel_layout = AV_CH_LAYOUT_STEREO;
						break;
					}
				}
				++channel;
			}
		}
		if (best_channel_layout != 0) {
			codec_context->channel_layout = best_channel_layout;
			codec_context->channels = av_get_channel_layout_nb_channels(codec_context->channel_layout);
		} else{
			return false;
		}
		codec_context->time_base = AVRational{1, codec_context->sample_rate};

		/// @brief some formats want stream headers to be separate
		if (_format_context->oformat->flags & AVFMT_GLOBALHEADER) {
			  codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
		}

		AVDictionary* raw_options(nullptr);
		av_dict_set(&raw_options, "strict", "experimental", 0);

		/// @brief Open output codec
		const int open_retval(avcodec_open2(codec_context.get(), codec, &raw_options));
		QScopedPointer<AVDictionary, AVDictionaryDeleter> scoped_options(raw_options);
		if (open_retval < 0) {
			return false;
		}

		QSharedPointer<AVCodecParameters> codec_parameters(avcodec_parameters_alloc(), AVCodecParametersDeleter::cleanup);
		avcodec_parameters_from_context(codec_parameters.get(), codec_context.get());
		target_stream->codecpar = codec_parameters.get();

		_encode_vector.push_back(EncodeOperation{target_stream, source_stream, codec_context, codec_parameters});

		return true;
	}
} } } }
