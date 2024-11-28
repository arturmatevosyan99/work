#ifndef Cadabra_Queen_Conversation_Model_Job_Spread_hpp
#define Cadabra_Queen_Conversation_Model_Job_Spread_hpp

#include "../../../Repository/Model/AbstractJob.hpp"

#include "../../../Repository/Meta/Object.hpp"
#include "../../../Repository/Meta/EssenceToken.hpp"

#include <QList>
#include <QList>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class Spread : public Repository::Model::AbstractJob
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Spread(QObject* parent = nullptr);
		virtual ~Spread() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _message_text;
		QVector<Repository::Meta::EssenceToken> _token_vector;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	private:
		virtual QJsonObject makeEnvironment() const override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void addChannelId(Repository::Meta::Object::Id channel_id);
		void addChannelToken(const Repository::Meta::EssenceToken& token);
		void addChannelTokenVector(const QVector<Repository::Meta::EssenceToken>& token);
		void setMessageText(const QString& text);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */
	};
} } } }

#endif
