#include "Reception.hpp"

#include "../../../Repository/Model/Application/Session.hpp"

#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	Reception::Reception(QObject* parent)
	:
		Repository::Model::AbstractApplication(parent)
	{
	}

	Reception::~Reception()
	{
	}

	Cadabra::Queen::Repository::Model::AbstractJob* Reception::cleanup(const QString& mode)
	{
		QSharedPointer<Cleanup> retval(new Cleanup(getDescriptor()->getObjectId(), mode));
		if (!getSession()->launch(retval)) {
			return nullptr;
		}
		QQmlEngine::setObjectOwnership(retval.data(), QQmlEngine::CppOwnership);
		return retval.data();
	}

	bool Reception::onHandle()
	{
		return true;
	}

	bool Reception::onOpen()
	{
		return true;
	}

	bool Reception::onSetup()
	{
		return true;
	}

	void Reception::customEvent(QEvent* event)
	{
		AbstractApplication::customEvent(event);
	}
} } } }
