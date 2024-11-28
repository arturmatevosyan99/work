#include "TestThread.hpp"

#include "../../Stress/Stress.hpp"

#include "../../Core/System/Alert.hpp"
#include "../../Core/Meta/Scheme/TestScheme.hpp"
#include "../../Core/Test/Case/PetriCase.hpp"
#include "../../Core/Test/Case/GeneralCase.hpp"

#include <QMap>
#include <QDebug>
#include <QElapsedTimer>

namespace Cadabra { namespace Queen { namespace Stress { namespace System {
	TestThread::TestThread(QObject* parent)
	:
		QThread(parent),

		_active_case_index(-1)
	{
		QObject::connect(&_context, &TestContext::engineWarned, this, &TestThread::onContextEngineWarned);

		QObject::connect(this, &TestThread::ran, this, &TestThread::onRan);
		QObject::connect(this, &QThread::started, this, &TestThread::onStarted);
		QObject::connect(this, &QThread::finished, this, &TestThread::onFinished);

		QObject::connect(Skeleton::ActiveInstance(), &Skeleton::debugMessaged, this, &TestThread::onDiagnosticMessaged, Qt::DirectConnection);
		QObject::connect(Skeleton::ActiveInstance(), &Skeleton::infoMessaged, this, &TestThread::onDiagnosticMessaged, Qt::DirectConnection);
		QObject::connect(Skeleton::ActiveInstance(), &Skeleton::warningMessaged, this, &TestThread::onDiagnosticMessaged, Qt::DirectConnection);
		QObject::connect(Skeleton::ActiveInstance(), &Skeleton::criticalMessaged, this, &TestThread::onFailed, Qt::DirectConnection);
		QObject::connect(Skeleton::ActiveInstance(), &Skeleton::fatalMessaged, this, &TestThread::onFailed, Qt::DirectConnection);

		/// @ocd _context.moveToThread(thread());

		/// @todo Inject Grokhin&GraphML engines in the _qml_context as well

		/** @todo Support various schemes like git://, ftp:// and etc.
		 * QNetworkAccessManager *QQmlEngine::networkAccessManager() const
		 * Returns a common QNetworkAccessManager which can be used by any QML type instantiated by this engine.
		 * If a QQmlNetworkAccessManagerFactory has been set and a QNetworkAccessManager has not yet been created,
		 * the QQmlNetworkAccessManagerFactory will be used to create the QNetworkAccessManager;
		 * otherwise the returned QNetworkAccessManager will have no proxy or cache set.
		 * See also setNetworkAccessManagerFactory().
		 */

		qDebug() << "currentThread: " << QThread::currentThread() << "this: " << this;
	}

	TestThread::~TestThread()
	{
	}

	void TestThread::run()
	{
		QElapsedTimer timer;
		QMultiMap<QString, QString> testrun_map;
		timer.start();
		for (IndexMap::ConstIterator i = _index_map.cbegin(); i != _index_map.cend(); ++i) {
			const QString test_name(i->second.makeModuleDisplayName() + "/" + i->second.getModelName());
			signalLog("Testing " + test_name + "...");
			emit ran(i.key());
			if (i->second.getVariation() == Core::TestVariation::Development) {
				signalLog("... Skipped!");
				emit completed(i.key(), true);
				testrun_map.insert("Skipped", test_name);
				continue;
			} else if (i->first != 0) {
				_case_vector.append(Core::Meta::Scheme<Core::Meta::TestScheme>::Produce(i->first));
			} else {
				switch (i->second.getFactory()) {
					case Core::TestFactory::PetrI:
						_case_vector.append(QSharedPointer<Core::Test::PetriCase>::create());
						break;

					case Core::TestFactory::General:
					default:
						_case_vector.append(QSharedPointer<Core::Test::GeneralCase>::create());
				}
			}
			if (_case_vector.last() == nullptr) {
				_case_vector.removeLast();
				signalLog("... Missed!");
				emit completed(i.key(), false);
				testrun_map.insert("Missed", test_name);
			} else {
				_context.put(_case_vector.last().get());
				const bool runcode(_case_vector.last()->run(i->second.makeModelAbsolutePath(), i->second.getDialect()));
				if (_context.interrupt(false)) {
					signalLog("... Interrupted!");
					emit completed(i.key(), false);
					testrun_map.insert("Interrupted", test_name);
				} else if (!runcode) {
					signalLog("... Failed!");
					emit completed(i.key(), false);
					testrun_map.insert("Failed", test_name);
				} else {
					signalLog("... Successed!");
					emit completed(i.key(), true);
					testrun_map.insert("Successed", test_name);
				}
			}
		}

		for (const QString& testing_state : {"Skipped", "Missed", "Interrupted", "Failed", "Successed"}) {
			QDebug warning_output(qWarning().nospace());
			warning_output << testing_state << ":";
			for (
				QMultiMap<QString, QString>::ConstIterator t = testrun_map.find(testing_state);
				t != testrun_map.constEnd();
				++t
			) {
				warning_output << " " << *t;
			}
		}

		_index_map.clear();
		qWarning
		(
			"Overall results: %lld/%d/%d/%d/%d/%d",
			timer.elapsed(),
			testrun_map.count("Skipped"),
			testrun_map.count("Missed"),
			testrun_map.count("Interrupted"),
			testrun_map.count("Failed"),
			testrun_map.count("Successed")
		);

		/// @todo У каждого запуска должен быть изолированный экземпляр Alert и этой проблемы существовать не будет
		if (Core::System::Alert::Instance()->flush()) {
			qWarning("Alert queue was not empty and was cleaned up therefore");
		}
	}

	void TestThread::signalLog(const QString& message)
	{
		emit logged(message);
	}

	int TestThread::start(Cadabra::Queen::Stress::Model::CheckList* check_list, bool filter)
	{
		QSortFilterProxyModel filtered_check_list;
		filtered_check_list.setSourceModel(check_list);
		if (filter) {
			filtered_check_list.setFilterFixedString("true");
			filtered_check_list.setFilterRole(Model::CheckList::Role::CheckState);
		}

		const int count(filtered_check_list.rowCount());
		for (int i = 0; i < count; ++i) {
			const QModelIndex index(filtered_check_list.mapToSource(filtered_check_list.index(i, 0)));
			const QVariant scheme(check_list->data(index, Qt::UserRole));
			if (scheme.canConvert<Core::Meta::Qd>()) {
				const Core::Meta::SchemeId scheme_id(scheme.toLongLong());
				const Core::Meta::Scheme<Core::Meta::TestScheme>::Iterator t(Core::Meta::Scheme<Core::Meta::TestScheme>::Iterate(scheme_id));
				_index_map[index] = qMakePair(scheme_id, Core::Meta::Scheme<Core::Meta::TestScheme>::Exists(t) ? *t : Core::Meta::TestScheme{});
			} else if (scheme.canConvert<Core::Meta::TestScheme>()) {
				_index_map[index] = qMakePair(0, scheme.value<Core::Meta::TestScheme>());
			} else {
				check_list->setStatusState(index, "fail");
				continue;
			}
			check_list->setStatusState(index, "busy");
		}
		if (count > 0) {
			_log_map.clear();
			_context.clear();
			QThread::start();
		}
		return count;
	}

	void TestThread::truncateLog()
	{
		_log_map.clear();
		QFile dummy_file;
		Cadabra::Queen::Skeleton::ActiveInstance()->rotateLog(dummy_file);
	}

	QString TestThread::fetchLog(int index) const
	{
		return _log_map.value(index, "");
	}

	void TestThread::onRan(QModelIndex index)
	{
		_active_case_index = index.row();
	}

	void TestThread::onCompleted(QModelIndex index, bool result)
	{
		Q_UNUSED(index) Q_UNUSED(result)

		_active_case_index = -1;
	}

	void TestThread::onDiagnosticMessaged(QString file_name, QString function_name, int code_line, QString message)
	{
		Q_UNUSED(file_name) Q_UNUSED(function_name) Q_UNUSED(code_line)

		if (_active_case_index != -1) {
			_log_map[_active_case_index].append(message + "\n");
		}
		signalLog(message);
	}

	void TestThread::onFailed(QString file_name, QString function_name, int code_line, QString message)
	{
		signalLog(QString("Interrupting on %1 in %2 at %3 with message: %4").arg(file_name, function_name).arg(code_line).arg(message));
		_context.interrupt(true);
	}

	void TestThread::onContextEngineWarned(const QList<QQmlError>& warning_list)
	{
		for (const QQmlError& error : warning_list) {
			signalLog(error.toString());
		}
	}

	void TestThread::onStarted()
	{
		emit changed();
	}

	void TestThread::onFinished()
	{
		_case_vector.clear();
		emit changed();
	}
} } } }
