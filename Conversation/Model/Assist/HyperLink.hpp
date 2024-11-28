#ifndef Cadabra_Queen_Conversation_Model_Assist_HyperLink_hpp
#define Cadabra_Queen_Conversation_Model_Assist_HyperLink_hpp

#include "../AbstractAssist.hpp"

#include <QUrl>
#include <QString>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class HyperLink : public AbstractAssist
	{
	/** @name Aliases */
	/** @{ */
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
		Q_PROPERTY(QUrl target READ getTarget WRITE setTarget NOTIFY targetChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		HyperLink(QObject* parent = nullptr);
		virtual ~HyperLink() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _title;
		QUrl _target;
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
		const QString& getTitle() const;
		const QUrl& getTarget() const;
		virtual Meta::Assist::TargetLayout getDefaultTargetLayout() const override;
		virtual QString getPreviewText() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setTitle(const QString& title);
		void setTarget(const QUrl& target);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void titleChanged();
		void targetChanged();
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
