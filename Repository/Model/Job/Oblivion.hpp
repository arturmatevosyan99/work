#ifndef Cadabra_Queen_Repository_Model_Job_Oblivion_hpp
#define Cadabra_Queen_Repository_Model_Job_Oblivion_hpp

#include "../AbstractJob.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Oblivion : public AbstractJob
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
		Oblivion(QObject* parent = nullptr);
		virtual ~Oblivion() override;
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
