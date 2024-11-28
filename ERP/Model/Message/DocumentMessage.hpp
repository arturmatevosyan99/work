#ifndef Cadabra_Queen_ERP_Model_Message_DocumentMessage_hpp
#define Cadabra_Queen_ERP_Model_Message_DocumentMessage_hpp

#include "../../../Conversation/Model/AbstractMessage.hpp"

#include "../../../ERP/Model/AbstractDocument.hpp"

namespace Cadabra { namespace Queen { namespace ERP { namespace Model {
	class DocumentMessage : public Conversation::Model::AbstractMessage
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(bool isValid READ hasDocument NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::ERP::Model::AbstractDocument* document READ getDocument NOTIFY imported)
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		DocumentMessage(QObject* parent = nullptr);
		virtual ~DocumentMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual Q_INVOKABLE void view();
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
		virtual QString getDescription() const;
		virtual ERP::Model::AbstractDocument* getDocument() const;
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
		bool hasDocument() const;
	/** @} */
	};
} } } }

#endif
