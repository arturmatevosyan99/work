#ifndef Cadabra_Queen_Mentalgami_Net_Call_WelcomeCall_hpp
#define Cadabra_Queen_Mentalgami_Net_Call_WelcomeCall_hpp

#include "../../../Core/Net/ServiceCall.hpp"

#include "../../../Core/Meta/Qd.hpp"
#include "../../../Core/Meta/MapModel.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Net {
	class WelcomeCall : public Core::Net::ServiceCall
	{
	/** @name Aliases */
	/** @{ */
	public:
		using MapModel = Core::Meta::TemplatedMapModel<Core::Meta::Qd, QString>;
	/** @{ */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @{ */

	/** @name Constructors */
	/** @{ */
	public:
		WelcomeCall(Core::Net::Endpoint* endpoint = nullptr);
		virtual ~WelcomeCall() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Core::Meta::TemplatedMapModel<Core::Meta::Qd, QString> _map_model;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void request(const QString& alias);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QAbstractListModel& getModel();
		const MapModel::Map& getMap() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void responded(const QString& message);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
	/** @} */
	};
} } } }

#endif
