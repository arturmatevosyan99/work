#ifndef Cadabra_Queen_Repository_Net_Call_ObjectFile_hpp
#define Cadabra_Queen_Repository_Net_Call_ObjectFile_hpp

#include "../../../Core/Net/ServiceCall.hpp"
#include "../../Meta/Object.hpp"
#include "../../Meta/Session.hpp"

#include <QMimeDatabase>
#include <QByteArray>
#include <QBuffer>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Repository { namespace Net {
	class ObjectFile : public Core::Net::ServiceCall
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @{ */

	/** @name Statics */
	/** @{ */
	private:
		static const QMimeDatabase MimeDatabase;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ObjectFile(Core::Net::Endpoint* endpoint, Meta::Session::Id session_id, Meta::Session::Handle session_handle, Meta::ObjectId object_id);
		ObjectFile(Core::Net::Endpoint* endpoint, Meta::Session::Id session_id, Meta::Session::Handle session_handle, Meta::ObjectHandle application_handle);
		ObjectFile(Core::Net::Endpoint* endpoint, QUrlQuery&& moniker);
		virtual ~ObjectFile() override = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QUrlQuery _query;
		QByteArray _data;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void download(const QString& file_name = "");
		void upload(QIODevice* input, const QString& file_name = "");
		void upload(QBuffer* input, const QString& file_name = "");
		void load(QFile* input);
		bool save(const QString& filename);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QByteArray& getData() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void finished(bool code);
		void uploaded(QString file_name);
		void downloaded(QString file_name);
		void faulted(QNetworkReply::NetworkError error, QString message);
		void saved(QString file_name, int size);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onComplete() override;
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
		virtual void onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url) override;
	/** @} */
	};
} } } }

#endif
