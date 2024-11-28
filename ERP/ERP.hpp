#ifndef Cadabra_Queen_ERP_ERP_hpp
#define Cadabra_Queen_ERP_ERP_hpp

#include "../Bone.hpp"

#include <QtTest/QtTest>

namespace Cadabra { namespace Queen { namespace ERP {
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

namespace Cadabra { namespace Queen { namespace ERP { Q_NAMESPACE
	enum class DocumentState : char
	{
		Unknown = '0',
		Nulled = 'N', /// < Удалить выше не Issued нельзя
		Issued = 'I',
		Archived = 'A',
		Held = 'H' /// < Проведен
	};
	Q_ENUM_NS(DocumentState)

	enum class DocumentDirection : char
	{
		Unknown = '0',
		Domestic = 'D',
		Incoming = 'I',
		Outcoming = 'O'
	};
	Q_ENUM_NS(DocumentDirection)
} } }

#endif
