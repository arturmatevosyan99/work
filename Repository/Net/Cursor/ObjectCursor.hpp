#ifndef Cadabra_Queen_Repository_Net_Cursor_ObjectCursor_hpp
#define Cadabra_Queen_Repository_Net_Cursor_ObjectCursor_hpp

#include "../AbstractCursor.hpp"
#include "../../Meta/Object.hpp"
#include "../../Meta/Session.hpp"

#include <QMimeDatabase>
#include <QByteArray>
#include <QBuffer>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Repository { namespace Net {
	class ObjectCursor : public AbstractCursor
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @{ */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
/*
		ObjectCursor(Core::Net::Endpoint* endpoint, Meta::Session::Id session_id, Meta::Session::Handle session_handle, Meta::ObjectId object_id);
		ObjectCursor(Core::Net::Endpoint* endpoint, Meta::Session::Id session_id, Meta::Session::Handle session_handle, Meta::ObjectHandle application_handle);
		ObjectCursor(Core::Net::Endpoint* endpoint, QUrlQuery&& moniker);
		virtual ~ObjectCursor() override = default;
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
		QByteArray _data;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void slice(const QString& file_name = "");
		void page(QIODevice* input, const QString& file_name = "");
		void sort(QBuffer* input, const QString& file_name = "");
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
