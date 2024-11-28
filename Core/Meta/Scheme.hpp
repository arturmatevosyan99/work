/**
 @file Scheme.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Meta_Scheme_hpp
#define Cadabra_Queen_Core_Meta_Scheme_hpp

#include "Qd.hpp"
#include "PropertyList.hpp"
#include "Filter.hpp"
#include "Order.hpp"
#include "../Model/SortFilterProxyModel.hpp"

#include <QObject>
#include <QHash>
#include <QPair>
#include <QQmlEngine>
#include <QMetaType>
#include <QSortFilterProxyModel>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	using SchemeId = qint64;

	class AbstractScheme;

	template <typename T>
	class SchemeProduct
	{
		template <typename U>
		static void detect(...);

		template <typename U>
		static typename U::_ detect(typename U::_*);

	public:
		using Type = decltype(detect<T>(nullptr));
	};

	template <typename T>
	class Scheme
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Salt = quint32;
		using Id = SchemeId;
		using Type = int;
		using Model = typename SchemeProduct<T>::Type;
		using FactoryFunction = std::function<QSharedPointer<Model>()>;
		using FactoryTable = QHash<Id, FactoryFunction>;
		using SchemeTable = QMultiHash<Id, T>;
		using TypeTable = QHash<Type, QPair<typename FactoryTable::ConstIterator, typename SchemeTable::ConstIterator>>;
		using Iterator = typename SchemeTable::ConstIterator;
	/** @} */

	/** @name Constructors */
	/** @{ */
	private:
		Scheme() = default;
		virtual ~Scheme() = default;
	/** @} */
		
	/** @name Statics */
	/** @{ */
	private:
		template <typename M>
		static QSharedPointer<Model> Make()
		{
			return QSharedPointer<M>::create();
		}

	public:
		static Id Season(quint32 extension_hash, Salt module_salt)
		{
			return (static_cast<quint64>(extension_hash) << 32) | static_cast<quint64>(module_salt);
		}

		template <typename M>
		static typename std::enable_if<std::is_convertible<T*, AbstractScheme*>::value, Id>::type
		Validate(Salt salt, T&& scheme)
		{
			if (!IsMutable) {
				return 0;
			}

			const Id scheme_id(Season(scheme.getExtensionHash(), salt));
			typename SchemeTable::Iterator i(SchemeMap.find(scheme_id));
			if (i == SchemeMap.end()) {
				TypeMap.insert(qMetaTypeId<M*>(), qMakePair(FactoryMap.insert(scheme_id, &Scheme<T>::Make<M>), SchemeMap.insert(scheme_id, std::move(scheme))));
			} else {
				SchemeMap.insert(scheme_id, std::move(scheme));
			}
			return scheme_id;
		}

		template <typename M>
		static Id Ident()
		{
			typename TypeTable::ConstIterator t(TypeMap.find(qMetaTypeId<M*>()));
			return TypeMap.constEnd() == t ? 0 : t.value().first.key();
		}

		static Id Ident(const QMetaType& meta_type)
		{
			typename TypeTable::ConstIterator t(TypeMap.find(meta_type.id()));
			return TypeMap.constEnd() == t ? 0 : t.value().first.key();
		}

		static Iterator Iterate(const Qd& qd)
		{
			return SchemeMap.find(qd.get());
		}

		static QVariant Variant(const Qd& qd)
		{
			Iterator i(SchemeMap.find(qd.get()));
			return SchemeMap.constEnd() != i ? QVariant::fromValue(i.value()) : QVariant();
		}

		static QVariant Variant(const int& index)
		{
			Iterator i(SchemeMap.cbegin() + index);
			return SchemeMap.constEnd() != i ? QVariant::fromValue(i.value()) : QVariant();
		}

		static QVariant Variant(const QMetaType& meta_type)
		{
			typename TypeTable::ConstIterator i(TypeMap.find(meta_type.id()));
			return TypeMap.constEnd() != i ? QVariant::fromValue(i.value().second.value()) : QVariant();
		}

		static QVariant Variant(const QModelIndex& index, const int& role)
		{
			Iterator i(SchemeMap.cbegin() + index.row());
			if (SchemeMap.constEnd() == i) {
				return QVariant();
			}
			switch (role) {
				case Qt::UserRole: return QVariant::fromValue(Qd(i.key()));
				default: return i->data(role);
			}
		}

		static QVariant Variant(const QMetaType& meta_type, const int& role)
		{
			typename TypeTable::ConstIterator i(TypeMap.find(meta_type.id()));
			if (TypeMap.constEnd() == i) {
				return QVariant();
			}
			switch (role) {
				case Qt::UserRole: return QVariant::fromValue(Qd(i->second.key()));
				default: return i->second->data(role);
			}
		}

		static QHash<int, QByteArray> RoleHash()
		{
			QHash<int, QByteArray> retval;
			retval.insert(Qt::UserRole, "hash");
			Iterator i(SchemeMap.cbegin());
			if (SchemeMap.constEnd() != i) {
				retval.insert(i->roleNames());
			}
			return retval;
		}

		static bool Exists(const Iterator& i)
		{
			return SchemeMap.constEnd() != i;
		}

		static QList<Iterator> List(const QString& pattern)
		{
			QList<Iterator> retval;
			for (Iterator i = SchemeMap.constBegin(); i != SchemeMap.constEnd(); ++i) {
				if (i.value() == pattern) {
					retval.push_back(i);
				}
			}
			return retval;
		}

		static int Count()
		{
			return SchemeMap.count();
		}

		static void Close()
		{
			IsMutable = false;
		}

		static QSharedPointer<Model> Produce(const Qd& qd)
		{
			typename QHash<Id, FactoryFunction>::ConstIterator f(FactoryMap.find(qd.get()));
			return (f == FactoryMap.constEnd()) ? nullptr : f.value()();
		}
	/** @} */
		
	/** @name Properties */
	/** @{ */
	private:
		static bool IsMutable;
		static SchemeTable SchemeMap;
		static TypeTable TypeMap;
		static FactoryTable FactoryMap;
	/** @} */
	};
	template <typename T>
	typename Scheme<T>::SchemeTable Scheme<T>::SchemeMap = {};

	template <typename T>
	typename Scheme<T>::TypeTable Scheme<T>::TypeMap = {};

	template <typename T>
	typename Scheme<T>::FactoryTable Scheme<T>::FactoryMap = {};

	template <typename T>
	bool Scheme<T>::IsMutable = true;

	class SchemeFactory : public QObject
	{
	friend class SchemeList;
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	private:
		SchemeFactory() = default;
		virtual ~SchemeFactory() = default;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		template <typename T>
		static typename std::enable_if<std::is_convertible<T*, AbstractScheme*>::value, bool>::type
		Bind(const QString& alias)
		{
			if (!IsMutable) {
				return false;
			}
			Map.insert(
						alias,
						{
							&Scheme<T>::Count,
							&Scheme<T>::RoleHash,
							static_cast<SchemeId(*)(const QMetaType&)>(&Scheme<T>::Ident),
							static_cast<QVariant(*)(const int&)>(&Scheme<T>::Variant),
							static_cast<QVariant(*)(const QMetaType&)>(&Scheme<T>::Variant),
							static_cast<QVariant(*)(const Qd&)>(&Scheme<T>::Variant),
							static_cast<QVariant(*)(const QModelIndex&, const int&)>(&Scheme<T>::Variant)
						}
			);
			return true;
		}
		static void Close();
		static QObject* QInstance(QQmlEngine* engine, QJSEngine* scriptEngine);
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using CountFunction = std::function<int()>;
		using RoleFunction = std::function<QHash<int, QByteArray>()>;
		using IdentFunction = std::function<SchemeId(const QMetaType&)>;
		using IndexFunction = std::function<QVariant(const int&)>;
		using TypeFunction = std::function<QVariant(const QMetaType&)>;
		using KeyFunction = std::function<QVariant(const Qd&)>;
		using DataFunction = std::function<QVariant(const QModelIndex&, const int&)>;
		struct Tuple {
			CountFunction count;
			RoleFunction role;
			IdentFunction ident;
			IndexFunction index;
			TypeFunction type;
			KeyFunction key;
			DataFunction data;
		};
		using Table = QHash<QString, Tuple>;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		static bool IsMutable;
		static Table Map;
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
		Q_INVOKABLE int count(QString factory_alias);
		Q_INVOKABLE QVariantList list(QString factory_alias, QString filter = "");
		Q_INVOKABLE QVariant load(QString factory_alias, QVariant scheme_id);
		Q_INVOKABLE QVariant load(QString factory_alias, QString extension_alias, QString model_alias);
		Q_INVOKABLE QVariant ident(QString factory_alias, QString extension_alias, QString model_alias);
	/** @} */
	};

	class SchemeList : public QAbstractListModel
	{
	/** @name Constructors */
	/** @{ */
	public:
		SchemeList(const QString& factory_alias, QObject* parent = nullptr);
		virtual ~SchemeList() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		SchemeFactory::Table::ConstIterator _iterator;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual void load();

	public:
		virtual void refresh(bool force = false);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getFactoryAlias() const;
		virtual int rowCount(const QModelIndex &parent) const override;
		virtual QVariant data(const QModelIndex &index, int role) const override;
	/** @} */
	};

	class SchemeModel : public Model::SortFilterProxyModel
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString factoryAlias READ getFactoryAlias WRITE setFactoryAlias NOTIFY factoryReset)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Meta::Filter> filterList READ getFilterList CONSTANT)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Meta::Order> orderList READ getOrderList CONSTANT)
//		Q_CLASSINFO("DefaultProperty", "filterList")
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SchemeModel(QObject* parent = nullptr);
		virtual ~SchemeModel() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QScopedPointer<SchemeList> _scheme_list;
		Meta::TemplatedPropertyList<Filter> _filter_list;
		Meta::TemplatedPropertyList<Order> _order_list;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void filter();

	protected:
		virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
		virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;

	private:
		void validate(Filter* filter);
		void validate(Order* order);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QHash<int, QByteArray> roleNames() const override;

		QString getFactoryAlias() const;
		QQmlListProperty<Filter> getFilterList();
		QQmlListProperty<Order> getOrderList();
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void addFilter(Filter* filter);
		void addOrder(Order* order);

		bool setFactoryList(SchemeList* list);
		bool setFactoryAlias(const QString& alias);
	/** @} */

	/** @name Slots */
	/** @{ */
	protected slots:
		void onFilterListCleared();
		void onFilterListExpanded();
		void onFilterListUpdated();
		void onOrderListCleared();
		void onOrderListExpanded();
		void onOrderListUpdated();
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void factoryReset();
	/** @} */
	};
} } } }

#endif
