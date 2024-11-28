#ifndef Cadabra_Queen_Conversation_Conversation_hpp
#define Cadabra_Queen_Conversation_Conversation_hpp

#include "../Bone.hpp"

#include <QtTest/QtTest>

namespace Cadabra { namespace Queen { namespace Conversation {
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
	/** @name Procedures */
	/** @{ */
	public:
		virtual bool registerTestCase() override;
	/** @} */
#endif
	};
} } }

namespace Cadabra { namespace Queen { namespace Conversation { Q_NAMESPACE
	enum class RenderLayout : int
	{
		FeedLayout		 = 0b0000,
		GridLayout		 = 0b0001, /// < 1
		CarouselLayout	 = 0b0010, /// < 2
		VerticalLayout	 = 0b0100, /// < 4
		HorizontalLayout = 0b1000  /// < 8
	};
	Q_DECLARE_FLAGS(RenderLayouts, RenderLayout)
	Q_FLAG_NS(RenderLayouts)
} } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Conversation::RenderLayouts)
Q_DECLARE_METATYPE(Cadabra::Queen::Conversation::RenderLayouts)

#endif
