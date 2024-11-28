#ifndef Cadabra_Queen_DrOblozhko_Model_Application_Reception_hpp
#define Cadabra_Queen_DrOblozhko_Model_Application_Reception_hpp

#include "../../../Repository/Model/AbstractApplication.hpp"

#include "../../Model/Job/Cleanup.hpp"

#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	class Reception : public Repository::Model::AbstractApplication
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Reception(QObject* parent = nullptr);
		virtual ~Reception() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE Cadabra::Queen::Repository::Model::AbstractJob* cleanup(const QString& mode);
	/** @} */

	/** @name Getters */
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

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void customEvent(QEvent* event) override;
		virtual bool onHandle() override;
		virtual bool onOpen() override;
		virtual bool onSetup() override;
//		virtual bool onClose() override;
//		virtual bool onTeardown() override;
	/** @} */
	};
} } } }

#endif
