#ifndef Cadabra_Queen_Core_System_AudioRecorder_hpp
#define Cadabra_Queen_Core_System_AudioRecorder_hpp

#include "AudioFile.hpp"
#include "AudioStream.hpp"
#include "AudioRole.hpp"

#include <QScopedPointer>
#include <QSharedPointer>
#include <QAudioRecorder>
#include <QAudioInput>
#include <QTime>
#include <QTemporaryFile>
#include <QFile>
#include <QString>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class AudioRecorder : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class State {
			On,
			Off,
			Suspend
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(State)
		Q_PROPERTY(QBuffer* buffer READ getBuffer NOTIFY flushed)
		Q_PROPERTY(QTime duration READ makeDurationTime NOTIFY durationChanged)
		Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY durationChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AudioRecorder(QObject* parent = nullptr);
		virtual ~AudioRecorder() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QSharedPointer<Core::System::AudioFile> _file;
		QScopedPointer<QAudioInput> _input;
		QSharedPointer<Core::System::AudioStream> _stream;
		qint64 _time_limit;
		qint64 _actual_duration;
		Core::System::AudioRole::Mode _role_mode;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool start(qint64 limit = 0); /// < milliseconds
		Q_INVOKABLE bool pause();
		Q_INVOKABLE bool stop();
		Q_INVOKABLE bool flush();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		qint64 getActualDuration() const;
		QBuffer* getBuffer() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	public:
		QTime makeDurationTime() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void stateChanged(State value);
		void durationChanged(qint64 duration);
		void flushed();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onStateChanged(QAudio::State state);
		void onNotified();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isBusy() const;
		bool isEmpty() const;
	/** @} */
	};
} } } }

#endif
