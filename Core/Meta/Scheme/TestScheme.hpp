/**
 @file TestScheme.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Meta_Scheme_TestScheme_hpp
#define Cadabra_Queen_Core_Meta_Scheme_TestScheme_hpp

#include "../AbstractScheme.hpp"

#include "../../Core.hpp"
#include "../../Test/AbstractCase.hpp"

#include <QString>
#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class TestScheme : public Core::Meta::AbstractScheme
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			Variation = Core::Meta::AbstractScheme::Role::CustomRole,
			Factory,
			Dialect,
			Module,
			ModuleDisplayName,
			ModuleAbsolutePath,
			ModelFileName
		};
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using _ = Test::AbstractCase;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_GADGET
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TestScheme();
		TestScheme(const QString& extname, const QString& modelname, TestVariation variation, TestDialect dialect = TestDialect::None, TestFactory factory = TestFactory::General);
		TestScheme(const QString& vendorname, const QString& modulename, const QString& modelname, TestVariation variation, TestDialect dialect = TestDialect::None, TestFactory factory = TestFactory::General);
		TestScheme(const TestScheme& origin);
		TestScheme(TestScheme&& origin);
		virtual ~TestScheme() = default;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		TestScheme& operator=(const TestScheme& origin);
		TestScheme& operator=(TestScheme&& origin);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		TestVariation _variation;
		TestDialect _dialect;
		TestFactory _factory;
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	private:
		Q_INVOKABLE virtual bool load(const Cadabra::Queen::Core::Meta::Qd& qd) override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		Q_INVOKABLE QString makeModuleDisplayName() const;
		Q_INVOKABLE QString makeModuleAbsolutePath() const;
		Q_INVOKABLE QString makeModelFileName() const;
		Q_INVOKABLE QString makeModelAbsolutePath() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QHash<int, QByteArray> roleNames() const override;
		virtual QVariant data(int role) const override;

		TestVariation getVariation() const;
		TestDialect getDialect() const;
		TestModule getModule() const;
		TestFactory getFactory() const;
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

Q_DECLARE_METATYPE(Cadabra::Queen::Core::Meta::TestScheme)

#endif
