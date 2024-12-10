#ifndef Cadabra_Queen_SmartCare_Model_Application_Interface_QuizInterface_hpp
#define Cadabra_Queen_SmartCare_Model_Application_Interface_QuizInterface_hpp

#include "../../../../Repository/Model/Application/AbstractInterface.hpp"

#include "../../AbstractRiddle.hpp"

#include <QtPlugin>
#include <QString>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class QuizInterface : public virtual Repository::Model::AbstractInterface
	{
	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		QuizInterface() = default;
		virtual ~QuizInterface() = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Core::Net::CRPDelay name();
		Core::Net::CRPDelay ask(int limit = 0, int offset = 0);
		Core::Net::CRPDelay answer(AbstractRiddle* riddle);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onName(const Core::Net::CRPReaction& reaction);
		virtual void onNamed(quint8 domain, quint32 code, const QString& message);
		virtual void onNamed(const QString& retval);

		virtual void onAsk(const Core::Net::CRPReaction& reaction);
		virtual void onAsked(quint8 domain, quint32 code, const QString& message);
		virtual void onAsked(const QJsonValue& retval);

		virtual void onAnswer(const Core::Net::CRPReaction& reaction);
		virtual void onAnswered(quint8 domain, quint32 code, const QString& message);
		virtual void onAnswered(const QJsonValue& riddles);
	/** @} */
	};
} } } }

Q_DECLARE_INTERFACE(Cadabra::Queen::SmartCare::Model::QuizInterface, "cadabra.queen.smartcare.model.quiz/1.0");

#endif
