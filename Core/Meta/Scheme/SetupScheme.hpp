/**
 @file SetupScheme.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Meta_Scheme_SetupScheme_hpp
#define Cadabra_Queen_Core_Meta_Scheme_SetupScheme_hpp

#include "../AbstractScheme.hpp"

#include "../../Data/AbstractSetup.hpp"

#include <QMap>
#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class SetupScheme : public Core::Meta::AbstractScheme
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using _ = Data::AbstractSetup;
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
		SetupScheme();
		SetupScheme(const QString& extname, const QString& modelname, const QMap<QString, qulonglong>& dependency_map = {});
		SetupScheme(const SetupScheme& origin);
		SetupScheme(SetupScheme&& origin);
		virtual ~SetupScheme() override = default;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		SetupScheme& operator=(const SetupScheme& origin);
		SetupScheme& operator=(SetupScheme&& origin);
	/** @} */
		
	/** @name Properties */
	/** @{ */
	private:
		QMap<QString, qulonglong> _dependency_map;
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

Q_DECLARE_METATYPE(Cadabra::Queen::Core::Meta::SetupScheme)

#endif
