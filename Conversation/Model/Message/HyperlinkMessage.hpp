#ifndef Cadabra_Queen_Conversation_Model_Message_HyperlinkMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_HyperlinkMessage_hpp

#include "../AbstractMessage.hpp"

#include <QString>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class HyperlinkMessage : public AbstractMessage
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString title READ getTitle NOTIFY imported)
		Q_PROPERTY(QUrl url READ getURL NOTIFY imported)
		Q_PROPERTY(bool isHyper READ hasURL NOTIFY imported)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		HyperlinkMessage(QObject* parent = nullptr);
		virtual ~HyperlinkMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:		
		QString _title;
		QUrl _url;
	/** @} */

	/** @name Procedures */
	/** @{ */		
	public:
		Q_INVOKABLE bool browse();

	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;

	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
		const QString& getTitle() const;
		const QUrl& getURL() const;
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
		bool hasURL() const;
	/** @} */
	};
} } } }

#endif
