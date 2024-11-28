#ifndef Cadabra_Queen_Core_System_AudioStream_hpp
#define Cadabra_Queen_Core_System_AudioStream_hpp

#include <QBuffer>
#include <QScopedPointer>

extern "C" {
#include <libavformat/avformat.h>
}

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class AudioStream : public QBuffer
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Codec : std::uint32_t {
			PCMSigned16LittleEndian = AV_CODEC_ID_PCM_S16LE,
			PCMSigned16BigEndian = AV_CODEC_ID_PCM_S16BE,
			PCMUnsigned16LittleEndian = AV_CODEC_ID_PCM_U16LE,
/*
			AV_CODEC_ID_PCM_U16BE
			AV_CODEC_ID_PCM_S8
			AV_CODEC_ID_PCM_U8
			AV_CODEC_ID_PCM_MULAW
			AV_CODEC_ID_PCM_ALAW
			AV_CODEC_ID_PCM_S32LE
			AV_CODEC_ID_PCM_S32BE
			AV_CODEC_ID_PCM_U32LE
			AV_CODEC_ID_PCM_U32BE
			AV_CODEC_ID_PCM_S24LE
			AV_CODEC_ID_PCM_S24BE
			AV_CODEC_ID_PCM_U24LE
*/
			PCMUnsigned24BigEndian = AV_CODEC_ID_PCM_U24BE,
/*
			AV_CODEC_ID_PCM_S24DAUD
			AV_CODEC_ID_PCM_ZORK
			AV_CODEC_ID_PCM_S16LE_PLANAR
			AV_CODEC_ID_PCM_DVD
			AV_CODEC_ID_PCM_F32BE
*/
			PCMFloat32LittleEndian = AV_CODEC_ID_PCM_F32LE,
/*			AV_CODEC_ID_PCM_F32LE
			AV_CODEC_ID_PCM_F64BE
			AV_CODEC_ID_PCM_F64LE
			AV_CODEC_ID_PCM_BLURAY
*/
			PCMLXF = AV_CODEC_ID_PCM_LXF,

			MP2 = AV_CODEC_ID_MP2,
			Opus = AV_CODEC_ID_OPUS
		};

		enum SampleFormat {
			None = AV_SAMPLE_FMT_NONE,
			Unsigned8 = AV_SAMPLE_FMT_U8,
			Signed16 = AV_SAMPLE_FMT_S16,
			Signed32 = AV_SAMPLE_FMT_S32,
			Float = AV_SAMPLE_FMT_FLT,
			Double = AV_SAMPLE_FMT_DBL,
			UnsignedPlanar8 = AV_SAMPLE_FMT_U8P,
			SignedPlanar16 = AV_SAMPLE_FMT_S16P,
			SignedPlanar32 = AV_SAMPLE_FMT_S32P,
			FloatPlanar = AV_SAMPLE_FMT_FLTP,
			DoublePlanar = AV_SAMPLE_FMT_DBLP,
			Signed64 = AV_SAMPLE_FMT_S64,
			SignedPlanar64 = AV_SAMPLE_FMT_S64P
		};

		enum ChannelLayout : int {
			Default = 0,
			Mono = 1,
			Stereo = 2
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static QStringList CodecList();

		static int DeviceWrite(void* opaque, uint8_t* buffer, int size);
		static int DeviceRead(void* opaque, uint8_t* buffer, int size);
		static int64_t DeviceSeek(void* opaque, int64_t offset, int whence);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit AudioStream(AudioStream::Codec codec, AudioStream::ChannelLayout channel_layout, std::int64_t bit_rate, int sample_rate, AudioStream::SampleFormat sample_format);
		virtual ~AudioStream() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const Codec _codec;
		const SampleFormat _sample_format;
		const int _sample_rate;
		const std::int64_t _bit_rate;
		const ChannelLayout _channel_layout;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		AudioStream::Codec getCodec() const;
		AudioStream::SampleFormat getSampleFormat() const;
		int getSampleRate() const;
		std::int64_t getBitRate();
		AudioStream::ChannelLayout getChannelLayout() const;
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
		bool isPCM() const;
	/** @} */
	};
} } } }

#endif
