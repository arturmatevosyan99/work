#ifndef Cadabra_Queen_Conversation_Model_Message_AuthorizationMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_AuthorizationMessage_hpp

#include "../AbstractMessage.hpp"

#include <QUrl>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class AuthorizationMessage : public AbstractMessage
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT		
		Q_PROPERTY(QString comment MEMBER _comment)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		AuthorizationMessage(QObject* parent = nullptr);
		virtual ~AuthorizationMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		bool _force;
		bool _hard_reset;
		QString _comment;
		Repository::Meta::Session::Id _id;
		Repository::Meta::Session::Handle _handle;
		Repository::Meta::Subject::AvatarScheme _scheme;
	/** @} */

	/** @name Procedures */
	/** @{ */		
	public:
		Q_INVOKABLE bool authorize();

	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
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
