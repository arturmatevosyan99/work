#ifndef Cadabra_Queen_Conversation_Model_Assist_TextPush_hpp
#define Cadabra_Queen_Conversation_Model_Assist_TextPush_hpp

#include "../AbstractAssist.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class TextPush : public AbstractAssist
	{
	/** @name Aliases */
	/** @{ */
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY labelChanged)
		Q_PROPERTY(QString value READ getValue WRITE setValue NOTIFY valueChanged)
        Q_PROPERTY(QString prefix READ getPrefix WRITE setPrefix NOTIFY prefixChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TextPush(QObject* parent = nullptr);
		virtual ~TextPush() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
        QString _prefix;
		QString _label;
		QString _value;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;

        Q_INVOKABLE QString text();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getLabel() const;
		const QString& getValue() const;
        const QString& getPrefix() const;
		virtual Meta::Assist::TargetLayout getDefaultTargetLayout() const override;
		virtual QString getPreviewText() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
        void setLabel(const QString& label);
		void setValue(const QString& value);
        void setPrefix(const QString& prefix);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void labelChanged();
		void valueChanged();
        void prefixChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
    private slots:
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
