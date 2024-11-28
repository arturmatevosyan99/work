/**
 @file RenderScheme.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Meta_Scheme_RenderScheme_hpp
#define Cadabra_Queen_Core_Meta_Scheme_RenderScheme_hpp

#include "../AbstractScheme.hpp"

#include <QUrl>
#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class RenderScheme : public Core::Meta::AbstractScheme
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			Path = Core::Meta::AbstractScheme::Role::CustomRole,
			Filename,
			URL,
			MajorVersion,
			MinorVersion,
			CustomRole
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		RenderScheme();
		RenderScheme(const QString& path, int version_major, int version_minor, const QString& extname, const QString& modelname, const QString& qmlfilename);
		RenderScheme(const RenderScheme& origin);
		RenderScheme(RenderScheme&& origin);
		virtual ~RenderScheme() = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:		
		Q_INVOKABLE QUrl makeURL(const QString& suffix = "") const;
	/** @} */
		
	/** @name Operators */
	/** @{ */
	public:
		RenderScheme& operator=(const RenderScheme& origin);
		RenderScheme& operator=(RenderScheme&& origin);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _path;
		QString _qml_filename;
		int _version_major;
		int _version_minor;
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Converters */
	/** @{ */
	public:
	/** @} */
		
	/** @name Getters */
	/** @{ */
	public:
		virtual QHash<int, QByteArray> roleNames() const override;
		virtual QVariant data(int role) const override;

		const QString& getPath() const;
		const QString& getName() const;
	/** @} */
		
	/** @name Formatters */
	/** @{ */
	public:
	/** @} */
		
	/** @name States */
	/** @{ */
	public:
	/** @} */
		
	/** @name Hooks */
	/** @{ */
	public:
	/** @} */
		
	/** @name Templates */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
