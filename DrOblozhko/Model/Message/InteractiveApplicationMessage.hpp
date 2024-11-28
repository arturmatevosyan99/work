#ifndef Cadabra_Queen_DrOblozhko_Model_Message_InteractiveApplicationMessage_hpp
#define Cadabra_Queen_DrOblozhko_Model_Message_InteractiveApplicationMessage_hpp

#include "../../../Conversation/Model/Message/AppletMessage.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	class InteractiveApplicationMessage : public Conversation::Model::AppletMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(int index READ getIndex NOTIFY imported)
		Q_PROPERTY(bool active READ isActive NOTIFY activated)
		Q_PROPERTY(bool isReviewed READ isReviewed WRITE setReview NOTIFY imported)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		InteractiveApplicationMessage(QObject* parent = nullptr);
		virtual ~InteractiveApplicationMessage() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Repository::Meta::Subject::Id _subject_id;
		bool _review;
		bool _local_review;
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
		int getIndex() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setReview(bool review);
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
		bool isReviewed() const;
	/** @} */
	};
} } } }

#endif
