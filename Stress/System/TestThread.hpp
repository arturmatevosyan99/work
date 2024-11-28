#ifndef Cadabra_Queen_Stress_TestThread_hpp
#define Cadabra_Queen_Stress_TestThread_hpp

#include "TestContext.hpp"

#include "../Model/CheckList.hpp"

#include "../../Core/Meta/Scheme.hpp"
#include "../../Core/Meta/Scheme/TestScheme.hpp"
#include "../../Core/Test/AbstractCase.hpp"

#include <QMap>
#include <QPair>
#include <QVector>
#include <QThread>

namespace Cadabra { namespace Queen { namespace Stress { namespace System {
	class TestThread : public QThread
	{
	/** @name Class */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(bool isRunning READ isRunning NOTIFY changed)
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using IndexMap = QMap<QModelIndex, QPair<Core::Meta::SchemeId, Core::Meta::TestScheme>>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TestThread(QObject* parent = nullptr);
		virtual ~TestThread();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		IndexMap _index_map;
		QVector<QSharedPointer<Core::Test::AbstractCase>> _case_vector;

		TestContext _context;

		QMap<int, QString> _log_map;
		int _active_case_index;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual void run() override;

	public:
		Q_INVOKABLE int start(Cadabra::Queen::Stress::Model::CheckList* check_list, bool filter = true);
		Q_INVOKABLE void signalLog(const QString& message);
		Q_INVOKABLE void truncateLog();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Q_INVOKABLE QString fetchLog(int index = -1) const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void changed();
		void ran(QModelIndex index);
		void completed(QModelIndex index, bool result);
		void logged(QString message);
	/** @} */

	/** @name Slots */
	/** @{ */
	public slots:
		void onRan(QModelIndex index);
		void onCompleted(QModelIndex index, bool result);
		void onDiagnosticMessaged(QString file_name, QString function_name, int code_line, QString message);
		void onFailed(QString file_name, QString function_name, int code_line, QString message);
		void onContextEngineWarned(const QList<QQmlError>& warning_list);

	private slots:
		void onStarted();
		void onFinished();
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
