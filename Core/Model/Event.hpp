#ifndef Cadabra_Queen_Core_Model_Event_hpp
#define Cadabra_Queen_Core_Model_Event_hpp

#include "AbstractEvent.hpp"

#include <QEvent>
#include <QMetaType>
#include <QSharedPointer>
#include <QJsonValue>
#include <type_traits>

namespace Cadabra { namespace Queen { namespace Core { namespace Model {
	template <typename E>
	class Event : public QEvent
	{
	/** @name Statics */
	/** @{ */
	public:
		static QEvent::Type Type(const QString& name)
		{
			return static_cast<QEvent::Type>(QEvent::User + QMetaType::type(name.toLocal8Bit().constData()));
		}

		template <typename A, typename std::enable_if<std::is_convertible<A*, E*>::value, int>::type = 0>
		static A* TypeCast(Event* event)
		{
			return static_cast<A*>(event->_event.data());
		}

		template <typename A>
		static A* TypeCast(QEvent* event)
		{
			return TypeCast<A>(static_cast<Core::Model::Event<E>*>(event));
		}
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		template <typename A>
		class Action
		{
		public:
			static int RegisterMetaType(const char* type)
			{
				int hint(qRegisterMetaType<A>(type));
				return Action<A>::RegisterEventType(hint);
			}

			static QEvent::Type RegisterEventType(int hint = 0)
			{
				static const QEvent::Type retval(static_cast<QEvent::Type>(QEvent::User + hint));
				return retval;
			}
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Event(const QString& name)
		:
			QEvent(Event::Type(name)),

			_name(name),
			_id(type() > QEvent::User ? static_cast<int>(type() - QEvent::User) : QMetaType::UnknownType),
			_event((_id == QMetaType::UnknownType) ? nullptr : static_cast<E*>(QMetaType::create(_id)))
		{

		}

		template <typename A, typename std::enable_if<std::is_convertible<A*, E*>::value, int>::type = 0>
		Event(A* instance)
		:
			QEvent(Action<A>::RegisterEventType()),

			_name(""),
			_id(type() > QEvent::User ? static_cast<int>(type() - QEvent::User) : QMetaType::UnknownType),
			_event(instance)
		{

		}

		Event(const Event& origin)
		:
			QEvent(Event::Type(origin._name)),

			_name(origin._name),
			_id(origin._id),
			_event(origin._event)
		{

		}

		virtual ~Event()
		{
/*
			QMetaType::destroy(_id, _action);
			_action = nullptr;
*/
		}
	/** @} */

	/** @name Properties */
	/** @{ */
	protected:
		const QString _name;
		const int _id;
		QSharedPointer<E> _event;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
