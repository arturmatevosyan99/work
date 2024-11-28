#ifndef Cadabra_Queen_Core_Net_Call_Grab_YoutubeGrab_hpp
#define Cadabra_Queen_Core_Net_Call_Grab_YoutubeGrab_hpp

#include "../MediaGrab.hpp"

#include <QObject>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QVector>
#include <QDate>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class YoutubeGrab : public MediaGrab
	{
	/** @name Statics */
	/** @{ */
	private:
	/** @} */

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
		explicit YoutubeGrab(const QString& video_id);
		virtual ~YoutubeGrab() override = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QString _video_id;		
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool fetch() override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
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

	/** @name Hooks */
	/** @{ */
	private:
		void onVideoInfo(QSharedPointer<QNetworkReply> reply);
		void onPlayerInfo(QSharedPointer<QNetworkReply> reply);

	protected:
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url) override;
	/** @} */
	};
} } } }

#endif
