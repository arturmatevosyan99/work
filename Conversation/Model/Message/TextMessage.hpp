#ifndef Cadabra_Queen_Conversation_Model_Message_TextMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_TextMessage_hpp

#include "../AbstractMessage.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class TextMessage : public AbstractMessage
	{
	/** @name Statics */
	/** @{ */
	public:
		static bool EmailStyle(QString& input);
		static bool HyperlinkStyle(QString& input);
		static bool GeneralStyle(QString& input);
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class Format : char
		{
			Auto = 'A',
			Plain = 'P',
			Styled = 'S',
			Rich = 'R',
			Markdown = 'M'
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Format)
		Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
		Q_PROPERTY(Format format READ getFormat WRITE setFormat NOTIFY textChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TextMessage(QObject* parent = nullptr);
		virtual ~TextMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _text;
		QString _formatted_text;
		Format _format;
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
		QString getText() const;
		Format getFormat() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setText(const QString& text);
		void setFormat(const Format& format);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void textChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
		virtual bool isEditable() const override;
		virtual bool hasClipboard() const override;
	/** @} */
	};
} } } }

#endif
