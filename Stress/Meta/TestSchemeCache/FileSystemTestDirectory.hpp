/**
 @file FileSystemTestDirectory.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Stress_Meta_TestSchemeCache_FileSystemTestDirectory_hpp
#define Cadabra_Queen_Stress_Meta_TestSchemeCache_FileSystemTestDirectory_hpp

#include "../TestSchemeCache.hpp"

#include <QDir>

namespace Cadabra { namespace Queen { namespace Stress { namespace Meta {
	class FileSystemTestDirectory : public TestSchemeCache
	{
	/** @name Constructors */
	/** @{ */
	public:
		FileSystemTestDirectory(const QUrl& path, QObject* parent = nullptr);
		virtual ~FileSystemTestDirectory() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QUrl _path;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual void load() override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getFactoryAlias() const override;
	/** @} */
	};
} } } }

#endif
