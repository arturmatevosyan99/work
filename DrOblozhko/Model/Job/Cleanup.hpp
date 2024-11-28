#ifndef Cadabra_Queen_DrOblozhko_Model_Job_Cleanup_hpp
#define Cadabra_Queen_DrOblozhko_Model_Job_Cleanup_hpp

#include "../../../Repository/Model/AbstractJob.hpp"

#include "../../../Repository/Meta/Object.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	class Cleanup : public Repository::Model::AbstractJob
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Cleanup(QObject* parent = nullptr);
		Cleanup(Repository::Meta::Object::Id object_id, const QString alias, QObject* parent = nullptr);
		virtual ~Cleanup() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Repository::Meta::Object::Id _reception_id;
		QString _mode;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void setReceptionId(Repository::Meta::Object::Id object_id);
		void setModeAlias(const QString& alias);
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
