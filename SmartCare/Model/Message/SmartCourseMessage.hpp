#ifndef Cadabra_Queen_SmartCare_Model_Message_SmartCourseMessage_hpp
#define Cadabra_Queen_SmartCare_Model_Message_SmartCourseMessage_hpp

#include "../../../Conversation/Model/Message/AppletMessage.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class SmartCourseMessage : public Conversation::Model::AppletMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd subjectId READ getSubjectQd NOTIFY imported)
		Q_PROPERTY(QString defaultTrackKey READ getDefaultTrackKey NOTIFY imported)
		Q_PROPERTY(QString artwork READ getArtwork NOTIFY imported)
		Q_PROPERTY(QUrl artworkURL READ getArtworkURL NOTIFY imported)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		SmartCourseMessage(QObject* parent = nullptr);
		virtual ~SmartCourseMessage() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Repository::Meta::Subject::Id _subject_id;
		QString _default_track_key;
		QString _artwork;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Repository::Meta::Subject::Id getSubjectId() const;
		Core::Meta::Qd getSubjectQd() const;
		const QString& getDefaultTrackKey() const;
		const QString& getArtwork() const;
		QUrl getArtworkURL() const;
	/** @} */

	/** @name Setters */
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
