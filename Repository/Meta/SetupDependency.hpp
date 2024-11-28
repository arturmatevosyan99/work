/**
 @file SetupDependency.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_SetupDependency_hpp
#define Cadabra_Queen_Repository_Meta_SetupDependency_hpp

#include "../../Core/Meta/AbstractScheme.hpp"

#include "../Data/AbstractSetup.hpp"

#include <QMap>
#include <QString>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class SetupDependency : public Core::Meta::AbstractScheme
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			Revision = Core::Meta::AbstractScheme::Role::CustomRole,
			Set
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using _ = Data::AbstractSetup; /// @todo Может быть не только Data::Setup (по идее)
		using Map = QMap<QPair<QString, QString>, Core::Meta::Scheme<SetupDependency>::Salt>;
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		SetupDependency();
		SetupDependency(const QString& extname, const QString& modelname, const Map& map = {});
		SetupDependency(const SetupDependency& origin);
		SetupDependency(SetupDependency&& origin);
		virtual ~SetupDependency() override = default;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		SetupDependency& operator=(const SetupDependency& origin);
		SetupDependency& operator=(SetupDependency&& origin);
	/** @} */
		
	/** @name Properties */
	/** @{ */
	private:
		Map _map;
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	private:
		Q_INVOKABLE virtual bool load(const Cadabra::Queen::Core::Meta::Qd& qd) override;
	/** @} */

	/** @name Converters */
	/** @{ */
	public:
		QSet<Core::Meta::Scheme<SetupDependency>::Id> makeSchemeSet() const;
	/** @} */
		
	/** @name Converters */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const SetupDependency::Map& getMap() const;
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

Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Meta::SetupDependency)

#endif
