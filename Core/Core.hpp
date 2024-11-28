#ifndef Cadabra_Queen_Core_Core_hpp
#define Cadabra_Queen_Core_Core_hpp

#include "../Body.hpp"
#include "../Bone.hpp"

#include "View/StatusBar.hpp"
#include "View/Theme.hpp"

#include "Meta/Scheme.hpp"

namespace Cadabra { namespace Queen { namespace Core {
	class Extension : public Queen::Bone
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Core::View::StatusBar* statusBar READ getStatusBar CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Core::View::Theme* theme READ getTheme CONSTANT)
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
		virtual bool tweakApplicationEngine(QQmlApplicationEngine& engine) override;
		virtual bool tweakRootContext(QQmlContext& context) override;
		virtual bool tweakRootObject(QObject& object) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Core::View::StatusBar* getStatusBar() const;
		Cadabra::Queen::Core::View::Theme* getTheme() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	protected:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onAttach(Skeleton& skeleton) override;
		virtual void onDetach(Skeleton& skeleton) override;
		virtual void onPreDisclose(Cupboard& cupboard) override;
		virtual void onPostDisclose(Cupboard& cupboard) override;
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

namespace Cadabra { namespace Queen { namespace Core { Q_NAMESPACE
	enum class ApplicationState : int
	{
		SuspendedApplication = Qt::ApplicationSuspended, /// < About to suspend SOD! = save our data
		HiddenApplication = Qt::ApplicationHidden, /// < Background
		InactiveApplication = Qt::ApplicationInactive, /// < Visible, but probably interrupted by call
		ActiveApplication = Qt::ApplicationActive
	};
	Q_ENUM_NS(ApplicationState)

	enum class ApplicationMessage : int
	{
		DebugMessage = QtMsgType::QtDebugMsg,
		InfoMessage = QtMsgType::QtInfoMsg,
		WarningMessage = QtMsgType::QtWarningMsg,
		CriticalMessage = QtMsgType::QtCriticalMsg,
		FatalMessage = QtMsgType::QtFatalMsg,
		SystemMessage = QtMsgType::QtSystemMsg
	};
	Q_ENUM_NS(ApplicationMessage)

	enum class TestVariation : int
	{
		Unit = 1, /// < 1+1==2
		Integration	= 2, /// < PetrI и поздней Gherkin(но не факт)
		System = 3, /// <  Выполняемое на полной, интегрированной системе
		Death = 4, /// < Похожи на интеграционные или комплект интеграционных для определения максимальной нагрузки на бэкенд
		Proof = 5, /// < Подтверждения CAP, CAI, CC-issues
		Development = 6, /// < Для удобной разработки (например, пользовательских интерфейсов)
		Custom = 7 /// < Для велосипедов с некруглыми колесами
	};
	Q_ENUM_NS(TestVariation)

	enum class TestDialect : int
	{
		None = 0,
		QML = 1,
		GraphML = 2,
		Gherkin = 3,
		Python = 4
	};
	Q_ENUM_NS(TestDialect)

	enum class TestModule : int
	{
		Extension = 0,
		Git,
		Web,
		FTP,
		FS,
		QRC
	};
	Q_ENUM_NS(TestModule)

	enum class TestFactory : int
	{
		General = 0,
		PetrI = 1,
	};
	Q_ENUM_NS(TestFactory)
} } }

#endif
