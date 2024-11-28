/**
 @file WebDAVTestDirectory.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Stress_Meta_TestSchemeCache_WebDAVTestDirectory_hpp
#define Cadabra_Queen_Stress_Meta_TestSchemeCache_WebDAVTestDirectory_hpp

#include "../TestSchemeCache.hpp"

#include "../../../Qaos/DAV/WebCollection.hpp"

#include <QSet>
#include <QScopedPointer>

namespace Cadabra { namespace Queen { namespace Stress { namespace Meta {
	class WebDAVTestDirectory : public TestSchemeCache
	{
	/** @name Constructors */
	/** @{ */
	public:
		WebDAVTestDirectory(const QUrl& url, QObject* parent = nullptr);
		virtual ~WebDAVTestDirectory() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QScopedPointer<Qaos::DAV::WebConnection> _connection;
		QMap<QString, Qaos::DAV::WebCollection> _set;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual void load() override;

	public:
		bool collect(const QString& path);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onFault(Qaos::DAV::WebCollection* const collection);
		void onSuccess(Qaos::DAV::WebCollection* const collection);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getFactoryAlias() const override;
	/** @} */
	};
} } } }

namespace Qaos { namespace DAV {
	inline uint qHash(const Qaos::DAV::WebCollection& collection)
	{
		return ::qHash(collection.getPath());
	}
} }

#endif
