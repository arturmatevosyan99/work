#ifndef Cadabra_Queen_Conversation_Model_Message_AppletMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_AppletMessage_hpp

#include "../AbstractMessage.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class AppletMessage : public AbstractMessage
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd id READ getQd NOTIFY imported)
		Q_PROPERTY(QString applicationScheme READ getApplicationScheme NOTIFY imported)
		Q_PROPERTY(QString name READ getName NOTIFY imported)
		Q_PROPERTY(QString description READ getDescription NOTIFY imported)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		AppletMessage(QObject* parent = nullptr, Meta::Message::SchemeIdent scheme = -8034428181974876110);
		virtual ~AppletMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Repository::Meta::ObjectId _id;
		Repository::Meta::Object::ApplicationScheme _application_scheme;
		QString _name;
		QString _description;
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
		virtual QString getPreviewText() const override;
		Repository::Meta::ObjectId getId() const;
		Cadabra::Queen::Core::Meta::Qd getQd() const;
		Repository::Meta::Object::ApplicationScheme getApplicationScheme() const;
		const QString& getName() const;
		const QString& getDescription() const;
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
