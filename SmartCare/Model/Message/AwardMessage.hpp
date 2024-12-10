#ifndef Cadabra_Queen_SmartCare_Model_Message_AwardMessage_hpp
#define Cadabra_Queen_SmartCare_Model_Message_AwardMessage_hpp

#include "../../../Conversation/Model/Message/CommandMessage.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class AwardMessage : public Conversation::Model::CommandMessage
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd subjectId READ getSubjectQd NOTIFY imported)
		Q_PROPERTY(QString crn READ getCRN NOTIFY imported)
		Q_PROPERTY(QString motive READ getMotive NOTIFY imported)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		AwardMessage(QObject* parent = nullptr);
		virtual ~AwardMessage() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Repository::Meta::Subject::Id getSubjectId() const;
		Core::Meta::Qd getSubjectQd() const;
		const QString& getCRN() const;
		const QString& getMotive() const;
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
