#ifndef Cadabra_Queen_Repository_Model_Application_Interface_URLInterface_hpp
#define Cadabra_Queen_Repository_Model_Application_Interface_URLInterface_hpp

#include "../AbstractInterface.hpp"

#include <QtPlugin>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class URLInterface : public virtual Repository::Model::AbstractInterface
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Type : qint8
		{
			Unaccessible = -1,
			Unknown = 0,
			YouTube,
			CadabraTube,
			HyperLink,
			File,
			RTSP
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static Type SchemeType(const QUrl& url);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		URLInterface() = default;
		virtual ~URLInterface() = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Core::Net::CRPDelay fetch(const QString& name);
		Core::Net::CRPDelay sign(const QString& name);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onFetched(Type type, const QUrl& url);
		virtual void onSigned(Type type, const QUrl& url, const QDateTime& life_time, const QVariantMap& token_map, const QString& view_preset);

	private:
		void onFetch(const Core::Net::CRPReaction& reaction);
		void onSign(const Core::Net::CRPReaction& reaction);
	/** @} */
	};
} } } }

Q_DECLARE_INTERFACE(Cadabra::Queen::Repository::Model::URLInterface, "cadabra.queen.repository.model.url/1.0");

#endif
