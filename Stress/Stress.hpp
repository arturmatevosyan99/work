#ifndef Cadabra_Queen_Stress_Test_hpp
#define Cadabra_Queen_Stress_Test_hpp

#include "../Bone.hpp"

#include "../Core/Core.hpp"
#include "../Core/Meta/Qd.hpp"
#include "../Core/Meta/Scheme.hpp"

#include "Model/CheckList.hpp"
#include "Model/LogList.hpp"

#include <QSet>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Stress {
	class Extension : public Queen::Bone
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Stress::Model::CheckList* checkList READ getCheckList CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Stress::Model::LogList* logList READ getLogList CONSTANT)
		Q_PROPERTY(QString debuggerOption READ getDebuggerOption CONSTANT)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QMap<QString, Core::TestVariation> VariationMap;
		static QByteArray DebuggerOption();
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Extension();
		virtual ~Extension() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Cadabra::Queen::Stress::Model::CheckList _check_list;
		Cadabra::Queen::Stress::Model::LogList _log_list;
		QThreadPool _thread_pool;
		QMap<Core::Meta::Qd, QFuture<bool>> _probe_map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool registerMetaObject() override;
		virtual bool registerMetaType() override;
		virtual bool registerQMLType() override;
		virtual bool registerMetaTypeStreamOperator() override;
		virtual bool registerTestCase() override;

		void run();
		bool run(Core::Meta::Qd& scheme_id);
		Q_INVOKABLE bool attach(QUrl url, bool persistent = false, bool onoff = true);
		Q_INVOKABLE bool replicate(QUrl url, bool persistent = false, bool onoff = true);
		Q_INVOKABLE bool debug(const QString& argument);
		Q_INVOKABLE bool vary(const QString& key, const QVariant& value);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Model::CheckList* getCheckList();
		Model::LogList* getLogList();
		QString getDebuggerOption();
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onTestStarted();
		void onTestFinished();
		void onTestRan(QModelIndex index);
		void onTestCompleted();
		void onTestSkipped();
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onAttach(Skeleton& skeleton) override;
		virtual void onDetach(Skeleton& skeleton) override;
	/** @} */
	};
} } }

namespace Cadabra { namespace Queen { namespace Stress { Q_NAMESPACE
	enum LogFormat : char
	{
		Text = 'T',
		JSON = 'J',
		XML = 'X'
	};
	Q_ENUM_NS(LogFormat)
} } }

QDataStream& operator<<(QDataStream& out, const QSet<QUrl>& set);
QDataStream& operator>>(QDataStream& in, QSet<QUrl>& set);
Q_DECLARE_METATYPE(QSet<QUrl>)

#endif
