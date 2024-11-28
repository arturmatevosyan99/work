#ifndef Cadabra_Queen_EC_EC_hpp
#define Cadabra_Queen_EC_EC_hpp

#include "../Bone.hpp"

#include <QtTest/QtTest>

namespace Cadabra { namespace Queen { namespace EC {
	class Extension : public Queen::Bone
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Extension();
		virtual ~Extension() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */	
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool registerMetaObject() override;
		virtual bool registerMetaType() override;
		virtual bool registerMetaTypeConverter() override;
		virtual bool registerMetaTypeStreamOperator() override;
		virtual bool registerQMLType() override;
		virtual bool registerAceAction() override;
		virtual bool registerAceScheme() override;
		virtual bool registerQMLSingletonType() override;
		virtual bool tweakRootContext(QQmlContext& context) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	protected:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

#ifdef QT_TESTLIB_LIB
	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
#endif
	};
} } }

namespace Cadabra { namespace Queen { namespace EC { Q_NAMESPACE
	enum class PaymentFault
	{
		UnknownError,
		UnreachableService,
		IssueError,
		AcquiringError
	};
	Q_ENUM_NS(PaymentFault)
} } }

#endif
