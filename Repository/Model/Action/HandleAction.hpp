#ifndef Cadabra_Queen_Repository_Model_Action_HandleAction_hpp
#define Cadabra_Queen_Repository_Model_Action_HandleAction_hpp

#include "../AbstractAction.hpp"

#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class HandleAction : public Repository::Model::AbstractAction
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class Cause : std::uint8_t
		{
			Validation = 0,
			Invalidation = 1,
			Revalidation = 2
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		HandleAction();
		HandleAction(const HandleAction& origin);
		HandleAction(HandleAction&& origin);
		virtual ~HandleAction() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:		
		Cause _cause;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool import(const QJsonValue& payload) override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:		
		const HandleAction::Cause& getCause() const;
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
