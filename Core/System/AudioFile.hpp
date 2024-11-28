#ifndef Cadabra_Queen_Core_System_AudioFile_hpp
#define Cadabra_Queen_Core_System_AudioFile_hpp

#include "AudioStream.hpp"

#include <QBuffer>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QByteArray>
#include <QVector>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class AudioFile : public QBuffer
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Container
		{
			WAV,
			OGG
		};

	private:
		struct AVFormatInputContextDeleter
		{
			static inline void cleanup(AVFormatContext* context)
			{
				if (context) {
//					avformat_close_input(&context);
					avformat_free_context(context);
				}
			}
		};

		struct AVFormatOutputContextDeleter
		{
			static inline void cleanup(AVFormatContext* context)
			{
				if (context) {
					av_write_trailer(context);
//					avio_close(context->pb);
				}
			}
		};

		struct AVIOContextDeleter
		{
			static inline void cleanup(AVIOContext* context)
			{
				if (context) {
					avio_context_free(&context);
				}
			}
		};

		struct AVCodecContextDeleter
		{
			static inline void cleanup(AVCodecContext* context)
			{
				if (context) {
					avcodec_close(context);
					avcodec_free_context(&context);
				}
			}
		};

		struct AVIOBufferDeleter
		{
			static inline void cleanup(unsigned char* buffer)
			{
				if (buffer) {
					av_freep(&buffer);
				}
			}
		};

		struct AVFrameDeleter
		{
			static inline void cleanup(AVFrame* frame)
			{
				if (frame) {
/*
					if (frame->data[0]) {
						av_free(frame->data[0]);
					}
*/
					av_frame_free(&frame);
				}
			}
		};

		struct AVPacketDeleter
		{
			static inline void cleanup(AVPacket* packet)
			{
				if (packet) {
					av_packet_free(&packet);
				}
			}
		};

		struct AVCodecParserContextDeleter
		{
			static inline void cleanup(AVCodecParserContext* parser)
			{
				if (parser) {
					av_parser_close(parser);
				}
			}
		};

		struct AVDictionaryDeleter
		{
			static inline void cleanup(AVDictionary* dictionary)
			{
				if (dictionary) {
					av_dict_free(&dictionary);
				}
			}
		};

		struct AVCodecParametersDeleter
		{
			static inline void cleanup(AVCodecParameters* parameters)
			{
				if (parameters) {
					avcodec_parameters_free(&parameters);
				}
			}
		};

		struct SwrContextDeleter
		{
			static inline void cleanup(SwrContext* context)
			{
				if (context) {
					swr_close(context);
					swr_free(&context);
				}
			}
		};

	private:
		struct EncodeOperation
		{
			AVStream* target_stream;
			QSharedPointer<AudioStream> source_stream;
			QSharedPointer<AVCodecContext> codec_context;
			QSharedPointer<AVCodecParameters> codec_parameters;
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static QStringList MuxerList();
		static QStringList DemuxerList();

	public:
		static const std::size_t BufferSize = 4096;
//		static const int AudioSampleCountPerFrame = 960;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit AudioFile();
		virtual ~AudioFile() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	private:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QScopedPointer<AVFormatContext, AVFormatOutputContextDeleter> _format_context;
		QScopedPointer<AVIOContext, AVIOContextDeleter> _io_context;
		QByteArray _buffer;
		Container _container;
		int _channel_count;
		int _bit_rate;

		QVector<EncodeOperation> _encode_vector;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool initialize(Container container, int channel_count, int bit_rate);
		bool flush();
		bool encode(QSharedPointer<AudioStream> source_stream, AudioStream::Codec target_codec, AudioStream::ChannelLayout target_channel_layout = AudioStream::ChannelLayout::Mono, std::int64_t target_bit_rate = 16000, int target_sample_rate = 48000, AudioStream::SampleFormat target_sample_format = AudioStream::SampleFormat::None);

	private:
		bool tuneFrame(AVFrame* frame, AVCodecContext* codec_context);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
