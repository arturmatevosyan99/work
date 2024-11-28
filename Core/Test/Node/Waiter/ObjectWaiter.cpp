#include "ObjectWaiter.hpp"

#include <QDebug>
#include <QScopedPointer>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	ObjectWaiter::ObjectWaiter(QObject* parent)
	:
		AbstractWaiter(parent)
	{
		QObject::connect(this, &AbstractWaiter::set, this, &ObjectWaiter::onSet);
	}

	bool ObjectWaiter::bind()
	{
		if (!_target || (_slot_map.isEmpty() && _callback_map.isEmpty())) {
			return false;
		}

		const QMetaObject* meta_object(_target->metaObject());
		if (!meta_object) {
			return false;
		}

		for (int m = 0, count = meta_object->methodOffset() + meta_object->methodCount(); m < count; ++m) {
			const QMetaMethod meta_method(meta_object->method(m));
			if (meta_method.methodType() == QMetaMethod::MethodType::Signal) {
				/// @brief Соединяем сигналы _target со слотами, размеченными непосредственно внутри ObjectWaiter
				QByteArray method_name(meta_method.name());
				QMap<QByteArray, QMetaMethod>::ConstIterator m(_slot_map.find(method_name));
				if (_slot_map.constEnd() != m) {
					std::unique_ptr<int[]> argv(new int[meta_method.parameterCount()]{});
					for (int p = 0; p < meta_method.parameterCount(); ++p) {
						int parameter_type(meta_method.parameterType(p));
						if (QMetaType::UnknownType != parameter_type) {
							argv[p] = parameter_type;
						} else {
							void* arg[] = { &parameter_type, &p };
							QMetaObject::metacall(_target, QMetaObject::RegisterMethodArgumentMetaType, meta_method.methodIndex(), arg);
							if (parameter_type == -1) {
								argv[p] = QMetaType::UnknownType;
								qWarning(
										"ObjectWaiter: Unable to handle parameter '%s' of type '%s' of method '%s', use qRegisterMetaType to register it.",
										meta_method.parameterNames().at(p).constData(),	meta_method.parameterTypes().at(p).constData(),	meta_method.name().constData()
								);
							} else {
								argv[p] = parameter_type;
							}
						}
					}

					if (!QMetaObject::connect(_target, meta_method.methodIndex(), this, m.value().methodIndex(), Qt::DirectConnection, argv.release())) {
						qWarning("ObjectWaiter: Unable to make a dynamic connect");
					}

					if (!QObject::connect(_target, meta_method, this, QMetaMethod::fromSignal(&AbstractWaiter::done))) {
						qWarning("ObjectWaiter: Unable to make the connection and won't be able to emit a stop signal at the end");
					}
				}

				/// @brief Соединяем сигналы _target со слотами, размеченными внутри WaiterCallback
				if (_callback_map.contains(method_name)) {
					QPointer<WaiterCallback> callback_pointer(_callback_map.value(method_name));

					if (callback_pointer) {
						callback_pointer->setSignalSpy(_target, meta_method);

						if (!QObject::connect(_target, meta_method, callback_pointer, QMetaMethod::fromSignal(&WaiterCallback::done))) {
							qWarning("ObjectWaiter: Unable to make a dynamic connect");
						} else {
							_callback_history_set.insert(method_name);
						}
					}

					/// @todo Имеет смысл стрелять сигналом AbstractWaiter::done только один раз
					if (!QObject::connect(_target, meta_method, this, QMetaMethod::fromSignal(&AbstractWaiter::done))) {
						qWarning("ObjectWaiter: Unable to make the connection and won't be able to emit a stop signal at the end");
					}
				}
			}
		}
		return true;
	}

	bool ObjectWaiter::disconnect()
	{
		if (!_target) {
			return false;
		}
		_target->disconnect(this);
		for (const QString& method_name : qAsConst(_callback_history_set)) {
			_target->disconnect(_callback_map.value(method_name));
		}
		return true;
	}

	QQmlListProperty<Cadabra::Queen::Core::Test::WaiterCallback> ObjectWaiter::makeCallbackPropertyList()
	{
		return _callback_property_list;
	}

	QObject* ObjectWaiter::getTarget()
	{
		return _target;
	}

	void ObjectWaiter::setTarget(QObject* target)
	{
		if (_target != target) {
			disconnect();

			_target = target;
			emit targetChanged();
		}
	}

	void ObjectWaiter::classBegin()
	{

	}

	void ObjectWaiter::componentComplete()
	{
		const QMetaObject* meta_object(metaObject());
		if (!meta_object) {
			return ;
		}

		/// @brief Запоминаем все слоты, в которые готовы принимать сигналы от будущего _target, размеченные непосредственно внутри ObjectWaiter
		for (int m = meta_object->methodOffset(), count = meta_object->methodOffset() + meta_object->methodCount(); m < count; ++m) {
			const QMetaMethod meta_method(meta_object->method(m));
			if (meta_method.methodType() == QMetaMethod::MethodType::Slot) {
				QByteArray slot_name(meta_method.name().remove(0, 2));
				if (!slot_name.isEmpty()) {
					slot_name.front() = QChar(slot_name.front()).toLower().toLatin1();
					_slot_map.insert(slot_name, meta_method);
				}
			}
		}

		/// @brief Запоминаем все слоты, в которые готовы принимать сигналы от будущего _target, размеченные внутри WaiterCallback
		for (WaiterCallback* callback : qAsConst(_callback_property_list)) {
			if (_callback_map.contains(callback->getName())) {
				qWarning() << "The slot with the name" << callback->getName() << "already exists, another one will not be processed";
			} else {
				_callback_map.insert(callback->getName(), callback);
			}
		}
	}

	void ObjectWaiter::onSet(bool onoff)
	{
		if (onoff) {
			bind();
		} else {
			disconnect();
		}
	}
} } } }
