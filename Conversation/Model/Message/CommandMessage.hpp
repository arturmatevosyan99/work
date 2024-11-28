#ifndef Cadabra_Queen_Conversation_Model_Message_CommandMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_CommandMessage_hpp

#include "../AbstractMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class CommandMessage : public AbstractMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString argument READ getArgument NOTIFY imported)
		Q_PROPERTY(QString action READ getAction NOTIFY imported)
		Q_PROPERTY(QString output READ getOutput NOTIFY imported)
		Q_PROPERTY(int success READ getSuccess NOTIFY imported)
		Q_PROPERTY(bool active READ hasAction NOTIFY imported)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		CommandMessage(QObject* parent = nullptr);
		virtual ~CommandMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _argument;
		QString _output;
		QString _action;
		int _success;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
		virtual bool clipBody() const override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
		const QString& getArgument() const;
		const QString& getOutput() const;
		const QString& getAction() const;
		const int& getSuccess() const;
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

	/** @name Slots */
	/** @{ */
	public:
		virtual bool hasClipboard() const override;
		bool hasAction() const;
	/** @} */
	};
} } } }

#endif
