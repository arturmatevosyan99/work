#ifndef Cadabra_Queen_Repository_Model_Job_Vacuum_hpp
#define Cadabra_Queen_Repository_Model_Job_Vacuum_hpp

#include "../AbstractJob.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Vacuum : public AbstractJob
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
		Vacuum(QObject* parent = nullptr);
		virtual ~Vacuum() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
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
