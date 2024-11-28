#ifndef Cadabra_Queen_Repository_Net_Call_ObjectDataset_hpp
#define Cadabra_Queen_Repository_Net_Call_ObjectDataset_hpp

#include "../../../Core/Net/ServiceCall.hpp"
#include "../../Meta/Object.hpp"
#include "../../Meta/Session.hpp"
#include "../../Meta/EssenceToken.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Net {
	class ObjectDataset : public Core::Net::ServiceCall
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @{ */

	/** @name Constructors */
	/** @{ */
	public:
/*
		ObjectDataset(Core::Net::Endpoint* endpoint, Meta::Session::Id session_id, Meta::Session::Handle session_handle, Meta::ObjectId object_id);
		ObjectDataset(Core::Net::Endpoint* endpoint, Meta::Session::Id session_id, Meta::Session::Handle session_handle, Meta::ObjectHandle application_handle);
		ObjectDataset(Core::Net::Endpoint* endpoint, QUrlQuery&& moniker);
		virtual ~ObjectDataset() override = default;
*/
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QUrlQuery _query;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
/*
		void sort(Meta::EssenceToken filter_token, Meta::EssenceToken sort_token);
		void slice(qsizetype offset, qsizetype limit);
		void page(QIODevice* input, const QString& file_name = "");
*/
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
/*
		const QByteArray& getData() const;
*/
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
/*
		void finished(bool code);
		void uploaded(QString file_name);
		void downloaded(QString file_name);
		void faulted(QNetworkReply::NetworkError error, QString message);
		void saved(QString file_name, int size);
*/
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
/*
		virtual void onComplete() override;
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
		virtual void onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url) override;
*/
	/** @} */
	};
} } } }

#endif
