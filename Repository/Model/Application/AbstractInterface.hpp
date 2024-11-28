/**
 @file AbstractInterface.hpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Repository_Model_Application_AbstractInterface_hpp
#define Cadabra_Queen_Repository_Model_Application_AbstractInterface_hpp

#include "../../../Core/Net/Call/CRPInterface.hpp"

#include "../AbstractDescriptor.hpp"

#include "../../Net/Call/ObjectFile.hpp"

#include "../../../Core/Net/Call/CRPCall.hpp"
#include "../../../Core/Net/Call/CRPDeferredCall.hpp"
#include "../../../Core/Net/Call/CRPDelay.hpp"

#include <QMap>
#include <QVector>
#include <QSharedPointer>
#include <functional>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	/**
	 * @brief Abstract Application Interface
	 * @todo Реализовать возможность вызова anonymous только через AceEndpoint
	 */
	class AbstractInterface : public Core::Net::CRPInterface
	{
	/** @name Classes */
	/** @{ */
	private:
		class CallMap : public QObject
		{
		/** @name Constructors */
		/** @{ */
		public:
			CallMap(AbstractInterface* interface, void(AbstractInterface::*method)(bool));
			virtual ~CallMap() = default;
		/** @} */

		/** @name Properties */
		/** @{ */
		public:
			AbstractInterface* _interface;
			void(AbstractInterface::*_method)(bool);
			QMap<quintptr, QSharedPointer<Core::Net::ServiceCall>> _map;
		/** @} */

		/** @name Operators */
		/** @{ */
		public:
			operator bool() const;
			bool operator!() const;
			void operator()(const QSharedPointer<Core::Net::ServiceCall>& call);
		/** @} */

		/** @name Hooks */
		/** @{ */
		public:
			virtual bool eventFilter(QObject* target, QEvent* event);
		/** @} */
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractInterface();
		virtual ~AbstractInterface();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		CallMap _call_map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	protected:
		QSharedPointer<Repository::Net::ObjectFile> makeFile();
		QSharedPointer<Core::Net::CRPCall> makeCall(bool anonymous = false);
		QSharedPointer<Core::Net::CRPDeferredCall> makeDeferredCall(bool anonymous = false);

	public:
		/**
		 * @brief Вызов дочернему/зависимому объекту, который будет обработан servlet'ом приложения по умолчанию
		 * @param object_id: Объект может не поддерживать переадресацию запросов ко всем или некоторым дочерним/зависымым объектам
		 * @todo Закрыть в private
		 * @return CRP вызов
		 */
		QSharedPointer<Core::Net::CRPCall> makeCall(const Meta::ObjectId& object_id);
		QSharedPointer<Core::Net::CRPDeferredCall> makeDeferredCall(const Meta::ObjectId& object_id);
	/** @} */

	/** @name Getters */
	/** @{ */
	protected:
		virtual AbstractDescriptor* getDescriptor() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onCall(bool start);
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		bool isEmptyCallMap() const;
	/** @} */
	};
} } } }

#endif
