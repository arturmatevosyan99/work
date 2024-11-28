#include "FitnessTracker.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	QObject* FitnessTracker::QInstance(QQmlEngine* engine, QJSEngine* scriptEngine)
	{
		Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)

		FitnessTracker* retval(FitnessTracker::Instance());

		QQmlEngine::setObjectOwnership(retval, QQmlEngine::CppOwnership);
		return retval;
	}
} } } }
