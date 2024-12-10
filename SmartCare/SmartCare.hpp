#ifndef Cadabra_Queen_SmartCare_SmartCare_hpp
#define Cadabra_Queen_SmartCare_SmartCare_hpp

#include "../Bone.hpp"

#include <QtTest/QtTest>

namespace Cadabra { namespace Queen { namespace SmartCare {
	class Extension : public Queen::Bone
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Extension();
		virtual ~Extension() override;
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

#ifdef QT_TESTLIB_LIB
	/** @name Procedures */
	/** @{ */
	public:
		virtual bool registerTestCase() override;
	/** @} */
#endif
	};
} } }

namespace Cadabra { namespace Queen { namespace SmartCare { Q_NAMESPACE
	enum class TrackRenderLayout : int
	{
		PlaylistLayout = 0b0000,
		PopupviewLayout = 0b0001,
		StackviewLayout = 0b0010,
		StackeditLayout = 0b0100
	};
	Q_DECLARE_FLAGS(TrackRenderLayouts, TrackRenderLayout)
	Q_FLAG_NS(TrackRenderLayouts)
} } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::SmartCare::TrackRenderLayouts)
Q_DECLARE_METATYPE(Cadabra::Queen::SmartCare::TrackRenderLayouts)

#endif
