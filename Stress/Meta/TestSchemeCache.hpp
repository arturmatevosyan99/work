/**
 @file TestSchemeCache.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Stress_Meta_TestSchemeCache_hpp
#define Cadabra_Queen_Stress_Meta_TestSchemeCache_hpp

#include "../../Core/Meta/Scheme.hpp"

#include "../Stress.hpp"
#include "../../Core/Meta/Scheme/TestScheme.hpp"

#include <QMap>
#include <QPair>

namespace Cadabra { namespace Queen { namespace Stress { namespace Meta {
	class TestSchemeCache : public Core::Meta::SchemeList
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using Cache = QVector<Core::Meta::TestScheme>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TestSchemeCache(QObject* parent = nullptr);
		virtual ~TestSchemeCache() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Cache _cache;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void refresh(bool force = false) override;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void append(Cache&& vector);
		void append(const QString& extension, const QString& module, const QString& model, Core::TestVariation variation, Core::TestDialect dialect, Core::TestFactory factory);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		int getCacheSize() const;
		virtual int rowCount(const QModelIndex &parent) const override;
		virtual QVariant data(const QModelIndex &index, int role) const override;
	/** @} */
	};
} } } }

#endif
