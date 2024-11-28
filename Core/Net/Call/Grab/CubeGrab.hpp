#ifndef Cadabra_Queen_Core_Net_Call_Grab_CubeGrab_hpp
#define Cadabra_Queen_Core_Net_Call_Grab_CubeGrab_hpp

#include "../MediaGrab.hpp"

#include <QUrl>
#include <QDir>
#include <QDate>
#include <QString>
#include <QVector>
#include <QUrlQuery>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class CubeGrab : public MediaGrab
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Plane : char
		{
			Video = 'v',
			Audio = 'a',
			Post = 'p',
			File = 'f',
			Application = 'w'
		};
	/** @} */

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
		explicit CubeGrab(const QUuid& uid, Plane plane, const QString& api_key = "0:0:0:0:0");
		virtual ~CubeGrab() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QDir _cache_path;
		const QUuid _uid;
		const Plane _plane;
		const QString _api_key;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool fetch() override;

	private:
		bool add(const QJsonDocument& document);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QFile makeCacheFile() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	private:
		void onSuccess(const QJsonDocument& document);
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
	protected:
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onRedirect(const QScopedPointer<QNetworkReply>& reply, const QUrl& url) override;
	/** @} */
	};
} } } }

#endif
