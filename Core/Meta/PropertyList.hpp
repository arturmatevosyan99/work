/**
 @file PropertyList.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Meta_PropertyList_hpp
#define Cadabra_Queen_Core_Meta_PropertyList_hpp

#include <QObject>
#include <QMap>
#include <QSet>
#include <QList>
#include <QMultiMap>
#include <QQmlListProperty>

#include <utility>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class AbstractPropertyList : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors*/
	/** @{ */
	public:
		AbstractPropertyList(QObject* parent = nullptr);
		virtual ~AbstractPropertyList();
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
		virtual bool eventFilter(QObject* object, QEvent* event) override;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void cleared();
		void expanded();
		void updated();
	/** @} */
	};

	template <typename T>
	class TemplatedPropertyList : public AbstractPropertyList
	{
	/** @name Aliases */
	/** @{ */
	private:
		using Data = QPair<QList<T*>, QMultiMap<QString, T*>>;
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
		static T* Item(QQmlListProperty<T>* list, int index)
		{
			return reinterpret_cast<Data*>(list->data)->first.value(index);
		}

		static int Size(QQmlListProperty<T>* list)
		{
			return reinterpret_cast<Data*>(list->data)->first.count();
		}

		static void Clear(QQmlListProperty<T>* list)
		{
			Data* data(reinterpret_cast<Data*>(list->data));
			if (!data) {
				return ;
			}
			qDeleteAll(data->first.begin(), data->first.end());
			data->first.clear();
			data->second.clear();
			AbstractPropertyList* delegate(qobject_cast<AbstractPropertyList*>(list->object));
			if (delegate) {
				emit delegate->cleared();
			}
		}

		static void Append(QQmlListProperty<T>* list, T* item)
		{
			Data* data(reinterpret_cast<Data*>(list->data));
			if (!data) {
				return ;
			}
			data->first.append(item);
			data->second.insert(!item ? "" : item->objectName(), item);
			AbstractPropertyList* delegate(qobject_cast<AbstractPropertyList*>(list->object));
			if (delegate) {
				if (item) {
					item->installEventFilter(delegate);
				}
				emit delegate->expanded();
			}
		}

		static void Replace(QQmlListProperty<T>* list, int index, T* item)
		{
			Data* data(reinterpret_cast<Data*>(list->data));
			const int count(!data ? 0 : data->first.count());
			if (index >= count) {
				return ;
			}
			data->first.append(item);
			data->first.swapItemsAt(index, count);
			QScopedPointer<T> item2(data->first.takeLast());
			AbstractPropertyList* delegate(qobject_cast<AbstractPropertyList*>(list->object));
			if (delegate) {
				if (item) {
					item->installEventFilter(delegate);
				}
				if (item2) {
					item2->removeEventFilter(delegate);
				}
				emit delegate->updated();
			}
		}

		static void Pop(QQmlListProperty<T>* list)
		{
			Data* data(reinterpret_cast<Data*>(list->data));
			if (!data || data->first.isEmpty()) {
				return ;
			}
			QScopedPointer<T> item(data->first.takeLast());
			data->second.remove(item->objectName(), item.data());
			AbstractPropertyList* delegate(qobject_cast<AbstractPropertyList*>(list->object));
			if (delegate) {
				if (item) {
					item->removeEventFilter(delegate);
				}
				emit delegate->expanded();
			}
		}
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TemplatedPropertyList(QObject* parent = nullptr)
		:
			AbstractPropertyList(parent)
		{

		}

		virtual ~TemplatedPropertyList() override
		{
			qDeleteAll(_data.first.begin(), _data.first.end());
		}
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Data _data;
	/** @} */

	/** @name Operators  */
	/** @{ */
	public:
		/// @todo different access levels
		operator QQmlListProperty<T>()
		{
			return QQmlListProperty<T>
			(
				this,
				&_data,
				&TemplatedPropertyList<T>::Append,
				&TemplatedPropertyList<T>::Size,
				&TemplatedPropertyList<T>::Item,
				&TemplatedPropertyList<T>::Clear,
				&TemplatedPropertyList<T>::Replace,
				&TemplatedPropertyList<T>::Pop
			);
		}

		operator QList<T*>&()
		{
			return _data.first;
		}

		operator const QList<T*>&() const
		{
			return qAsConst(_data.first);
		}
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void push_back(typename QList<T*>::const_reference reference)
		{
			_data.first.push_back(reference);
			_data.second.insert(reference->objectName(), reference);
			if (reference) {
				reference->installEventFilter(this);
			}
			emit expanded();
		}

		void merge(TemplatedPropertyList<T>& source)
		{
			for (typename QList<T*>::const_reference reference : source._data.first) {
				_data.first.push_back(reference);
				_data.second.insert(reference->objectName(), reference);
				if (reference) {
					reference->installEventFilter(this);
				}
			}
			source._data.first.clear();
			emit expanded();
		}
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		int length() const
		{
			return _data.first.length();
		}

		QList<typename Data::second_type::mapped_type> index(const QString& key) const
		{
			QList<typename Data::second_type::mapped_type> retval;
			typename Data::second_type::ConstIterator i(_data.second.find(key));
			while (i != _data.second.cend() && i.key() == key) {
				retval.push_back((i++).value());
			}
			return retval;
		}

		typename Data::second_type::mapped_type map(const QString& key) const
		{
			typename Data::second_type::ConstIterator i(_data.second.find(key));
			return _data.second.constEnd() != i ? i.value() : nullptr;
		}

		typename QList<T*>::reference last()
		{
			return _data.first.last();
		}

		typename QList<T*>::const_reference last() const
		{
			return qAsConst(_data.first).last();
		}

		typename QList<T*>::reference first()
		{
			return _data.first.first();
		}

		typename QList<T*>::const_reference first() const
		{
			return qAsConst(_data.first).first();
		}

		typename QList<T*>::iterator begin()
		{
			return _data.first.begin();
		}

		typename QList<T*>::const_iterator begin() const
		{
			return qAsConst(_data.first).begin();
		}

		typename QList<T*>::iterator end()
		{
			return _data.first.end();
		}

		typename QList<T*>::const_iterator end() const
		{
			return qAsConst(_data.first).end();
		}

		typename QList<T*>::reference back()
		{
			return _data.first.back();
		}

		typename QList<T*>::const_reference back() const
		{
			return qAsConst(_data.first).back();
		}

		typename QList<T*>::const_reference at(int index) const
		{
			return qAsConst(_data.first).at(index);
		}

		typename QList<T*>::reference operator[](int index)
		{
			return _data.first[index];
		}
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isEmpty() const
		{
			return _data.first.isEmpty();
		}
	/** @} */
	};
} } } }

#endif
