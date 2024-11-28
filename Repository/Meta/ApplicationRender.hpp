/**
 @file ApplicationRender.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_ApplicationRender_hpp
#define Cadabra_Queen_Repository_Meta_ApplicationRender_hpp

#include "../../Core/Meta/Scheme/RenderScheme.hpp"

#include "../Model/AbstractApplication.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class ApplicationRender : public Core::Meta::RenderScheme
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using _ = Model::AbstractApplication;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			IconURL = Core::Meta::RenderScheme::Role::CustomRole
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ApplicationRender();
		ApplicationRender(const QString& extname, const QString& modelname, int version_major, int version_minor, const QString& filename);
		ApplicationRender(const ApplicationRender& origin);
		ApplicationRender(ApplicationRender&& origin);
		virtual ~ApplicationRender() override = default;
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
		Q_INVOKABLE QUrl makeIconURL() const;
	/** @} */
		
	/** @name Operators */
	/** @{ */
	public:
		ApplicationRender& operator=(const ApplicationRender& origin);
		ApplicationRender& operator=(ApplicationRender&& origin);
	/** @} */
		
	/** @name Properties */
	/** @{ */
	public:
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	private:
		Q_INVOKABLE virtual bool load(const Cadabra::Queen::Core::Meta::Qd& qd) override;
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

Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Meta::ApplicationRender)

#endif
