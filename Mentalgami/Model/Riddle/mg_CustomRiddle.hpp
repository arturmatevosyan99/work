#ifndef Cadabra_Queen_Mentalgami_Model_Question_CustomRiddle_hpp
#define Cadabra_Queen_Mentalgami_Model_Question_CustomRiddle_hpp

#include "../mg_AbstractRiddle.hpp"

#include <QBitArray>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class CustomRiddle : public AbstractRiddle
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
		Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
		Q_PROPERTY(Cadabra::Queen::Mentalgami::Meta::RiddleInformation::Specifications specifications READ getSpecifications WRITE setSpecifications NOTIFY specificationsChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CustomRiddle(const QString& text = QString(), const QString& descriptor = QString());
		virtual ~CustomRiddle() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QBitArray _busyness_plan;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& body) override;
		virtual QVariant exportAnswer() const override;
		virtual bool importAnswer(const QVariant& answer) override;
		virtual QVariant makeDefault() const override;

		Q_INVOKABLE virtual bool startBusyness(int length = 1);
		Q_INVOKABLE virtual bool completeBusynessPhase(int phase_number);
		Q_INVOKABLE virtual void stopBusyness();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getType() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	protected:
		virtual void setBusy(bool onoff) override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool isBusy() const override;

	private:
		bool isBusynessInProcess() const;
	/** @} */
};
} } } }

#endif
