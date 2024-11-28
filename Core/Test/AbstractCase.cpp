#include "AbstractCase.hpp"

#include <QMutex>
#include <QWaitCondition>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	AbstractCase::AbstractCase(QObject* parent)
	:
		QObject(parent)
	{
	}

	bool AbstractCase::run()
	{
		return false;
	}

	bool AbstractCase::run(const QString& filename, TestDialect dialect)
	{
		bool retval(true);
		switch (dialect) {
/*
			case Core::TestDialect::Gherkin:
			{
				GherkinScenario scenario(QUrl(filename));
				retval = dial(scenario);
				break;
			}

			case Core::TestDialect::GraphML:
			{
				GraphML graph(QUrl(filename))
				retval = dial(graph);
				break;
			}
*/
			case Core::TestDialect::QML:
			{
				QQmlEngine* engine(QtQml::qmlEngine(this));
				QScopedPointer<QMutex> mutex(new QMutex()); mutex->lock();
				QQmlComponent component(engine);
				QObject::connect(&component, &QQmlComponent::statusChanged, std::bind(&AbstractCase::onStatusChanged, this, std::cref(component)));
				component.loadUrl(QUrl(filename), QQmlComponent::CompilationMode::Asynchronous);
				qDebug() << "Loading QML Component from..." << filename;
				quint8 deadline_counter(0);
				while (QQmlComponent::Loading == component.status()) {
					if (!_loader.wait(mutex.get(), QDeadlineTimer(20000))) {
						qDebug() << "Still loading...";
						if (++deadline_counter > 3) {
							qDebug() << "Component loading interrupted due to timeout...";
							mutex->unlock();
							return false;
						}
					}
				}
				mutex->unlock();
				if (QQmlComponent::Error == component.status()) {
					qDebug() << "Error during creating QML component:" << component.errors();
					return false;
				} else if (QQmlComponent::Null == component.status()) {
					qWarning() << "QML component is unexpectedly null for some reason, no sense to wait incubation anymore...";
					return false;
				}
				QQmlIncubator incubator(QQmlIncubator::IncubationMode::Asynchronous);
				retval = dial(incubator);
				if (retval) {
					component.create(incubator, QtQml::qmlContext(this));
					while (!incubator.isReady()) {
						QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
						if (incubator.isError()) {
							qDebug() << "QML incubation error:" << incubator.errors();
							return false;
						}
					}
					retval = dial(qAsConst(incubator));
				}
				break;
			}

			default:
				/// @log Not implemented yet
				break;
		}

		if (!retval) {
			/// @log Dial error
			return false;
		} else if (!run()) {
			/// @log Run error
			return false;
		}
		return true;
	}

	bool AbstractCase::dial(QQmlIncubator& incubator)
	{
		return true;
	}

	bool AbstractCase::dial(const QQmlIncubator& incubator)
	{
		return true;
	}

	const Context& AbstractCase::getContext() const
	{
		return _context;
	}

	Context& AbstractCase::getContext()
	{
		return _context;
	}

	void AbstractCase::onStatusChanged(const QQmlComponent& component)
	{
		switch (component.status()) {
			case QQmlComponent::Status::Loading:
				qDebug() << QString("Loading QML progress: %1%...").arg(component.progress() * 100);
				return ;

			case QQmlComponent::Status::Error:
				qCritical() << "Error during loading QML:" << component.errors();

			default:
				_loader.wakeOne();
		}
	}
} } } }
