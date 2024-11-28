#ifndef Cadabra_Queen_DrOblozhko_Model_Message_VideoLessonMessage_hpp
#define Cadabra_Queen_DrOblozhko_Model_Message_VideoLessonMessage_hpp

#include "../../../Conversation/Model/Message/AppletMessage.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	class VideoLessonMessage : public Conversation::Model::AppletMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(double progress READ getProgress WRITE setProgress NOTIFY imported)
		Q_PROPERTY(int index READ getIndex NOTIFY imported)
		Q_PROPERTY(bool active READ isActive NOTIFY activated)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		VideoLessonMessage(QObject* parent = nullptr);
		virtual ~VideoLessonMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Repository::Meta::Subject::Id _subject_id;
		double _progress;
		double _local_progress;
		int _index;
		bool _active;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void fetch();
		Q_INVOKABLE void sign();

	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;

	private:
		void activate(bool onoff);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Repository::Meta::Subject::Id getSubjectId() const;
		double getProgress() const;
		int getIndex() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setProgress(double progress);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void activated(bool onoff);
		void accessed(QUrl url);
		void denied();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onFetched(quint8 domain, quint32 code, QString message);
		void onFetched(QJsonValue payload);

		void onSigned(quint8 domain, quint32 code, QString message);
		void onSigned(QJsonValue payload);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isActive() const;
		bool isFetched() const;
	/** @} */
	};
} } } }

#endif
