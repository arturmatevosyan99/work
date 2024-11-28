#include "AbstractInterface.hpp"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QUuid>
#include <algorithm>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	AbstractInterface::AbstractInterface()
	:
		_call_map(this, &AbstractInterface::onCall)
	{

	}

	AbstractInterface::~AbstractInterface()
	{

	}

	QSharedPointer<Repository::Net::ObjectFile> AbstractInterface::makeFile()
	{
		AbstractDescriptor* descriptor(getDescriptor());
		if (!descriptor) {
			return nullptr;
		}

		QSharedPointer<Repository::Net::ObjectFile> retval
		(
			new Repository::Net::ObjectFile
			(
				descriptor->getRealm()->getEndpoint(),
				descriptor->makeMoniker()
			)
		);
		_call_map(retval);
		return retval;
	}

	QSharedPointer<Core::Net::CRPCall> AbstractInterface::makeCall(bool anonymous)
	{
		AbstractDescriptor* descriptor(getDescriptor());
		if (!descriptor) {
			return nullptr;
		}

		QSharedPointer<Core::Net::CRPCall> retval
		(
			new Core::Net::CRPCall
			(
				descriptor->getRealm()->getEndpoint(),
				"/repository/object/reflector",
				anonymous ? "" : descriptor->getMoniker()
			)
		);
		_call_map(retval);
		return retval;
	}

	QSharedPointer<Core::Net::CRPDeferredCall> AbstractInterface::makeDeferredCall(bool anonymous)
	{
		AbstractDescriptor* descriptor(getDescriptor());
		if (!descriptor) {
			return nullptr;
		}

		QSharedPointer<Core::Net::CRPDeferredCall> retval
		(
			new Core::Net::CRPDeferredCall
			(
				descriptor->getRealm()->getEndpoint(),
				"/repository/object/reflector",
				anonymous ? "" : descriptor->getMoniker()
			)
		);
		_call_map(retval);
		return retval;
	}

	QSharedPointer<Core::Net::CRPCall> AbstractInterface::makeCall(const Meta::ObjectId& object_id)
	{
		AbstractDescriptor* descriptor(getDescriptor());
		if (!descriptor) {
			return nullptr;
		}

		QSharedPointer<Core::Net::CRPCall> retval
		(
			new Core::Net::CRPCall
			(
				descriptor->getRealm()->getEndpoint(),
				"/repository/object/reflector",
				descriptor->getMoniker() + QString::number(object_id).prepend('#')
			)
		);
		_call_map(retval);
		return retval;
	}

	QSharedPointer<Core::Net::CRPDeferredCall> AbstractInterface::makeDeferredCall(const Meta::ObjectId& object_id)
	{
		AbstractDescriptor* descriptor(getDescriptor());
		if (!descriptor) {
			return nullptr;
		}

		QSharedPointer<Core::Net::CRPDeferredCall> retval
		(
			new Core::Net::CRPDeferredCall
			(
				descriptor->getRealm()->getEndpoint(),
				"/repository/object/reflector",
				descriptor->getMoniker() + QString::number(object_id).prepend('#')
			)
		);
		_call_map(retval);
		return retval;
	}

	AbstractDescriptor* AbstractInterface::getDescriptor() const
	{
		return nullptr;
	}

	void AbstractInterface::onCall(bool start)
	{

	}

	bool AbstractInterface::isEmptyCallMap() const
	{
		return !_call_map;
	}

	AbstractInterface::CallMap::CallMap(AbstractInterface* interface, void(AbstractInterface::*method)(bool))
	:
		QObject(),

		_interface(interface),
		_method(method)
	{

	}

	AbstractInterface::CallMap::CallMap::operator bool() const
	{
		return !_map.isEmpty();
	}

	bool AbstractInterface::CallMap::CallMap::operator!() const
	{
		return _map.isEmpty();
	}

	void AbstractInterface::CallMap::CallMap::operator()(const QSharedPointer<Core::Net::ServiceCall>& call)
	{
		if (!call) {
			return ;
		}
		_map.insert(reinterpret_cast<quintptr>(call.get()), call);
		call->setParent(this);
		call->installEventFilter(this);
		(_interface->*_method)(true);
		qDebug() << "AbstractInterface::CallMap(" << this << ") +" << reinterpret_cast<quintptr>(call.get());
	}

	bool AbstractInterface::CallMap::CallMap::eventFilter(QObject* target, QEvent* event)
	{
		if (event->type() == QEvent::Type::SockClose && _map.remove(reinterpret_cast<quintptr>(target)) > 0) {
			(_interface->*_method)(false);
			qDebug() << "AbstractInterface::CallMap(" << this << ") -" << reinterpret_cast<quintptr>(target);
			return true;
		}
		return false;
	}
} } } }
