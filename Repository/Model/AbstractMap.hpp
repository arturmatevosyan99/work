#ifndef Cadabra_Queen_Repository_Model_AbstractMap_hpp
#define Cadabra_Queen_Repository_Model_AbstractMap_hpp

#include "Roaster.hpp"

#include "../Repository.hpp"
#include "../Meta/Essence.hpp"
#include "../Meta/Tag.hpp"
#include "../Meta/EssenceToken.hpp"

#include "../Data/Entity/AbstractRecord.hpp"
#include "../../Core/Data/Provider.hpp"
#include "../../Core/Data/Factory.hpp"

#include <QMetaType>
#include <QAbstractItemModel>
#include <QWeakPointer>
#include <QSharedPointer>
#include <QAtomicPointer>
#include <QMap>
#include <QMultiMap>
#include <QSet>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QJsonValue>
#include <QPair>
#include <QImage>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class MapObserver;
	/**
	 * @brief The AbstractMap class
	 * Identity Map implementation
	 */
	class AbstractMap : public QAbstractItemModel
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static const QSet<Meta::Essence::Id> DummyTagSet;
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using TagMap = QMap<Meta::Essence::Id, QSet<Meta::Essence::Id>>;
		using OptionMap = QMap<Meta::Essence::Id, QJsonObject>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractMap(QObject* parent);
		virtual ~AbstractMap();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		bool _lazy_mode;
		Roaster* _roaster;
		TagMap _tag_map;
		OptionMap _option_map;
		QAtomicPointer<MapObserver> _map_observer;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool load(Roaster* roaster, bool lazy_mode);
		virtual bool unload();

		virtual qsizetype bulk(const QJsonArray& array);
		virtual bool apply(Meta::Essence::Id id, const QDateTime& modification_datetime, const QJsonObject& dump) = 0;
		virtual bool remove(Meta::Essence::Id id);

		virtual bool opt(Meta::Essence::Id id, const QString& key, const QVariant& value);
		virtual bool tag(Meta::Essence::Id id, Meta::Essence::Id tag_id, bool onoff = true);
		virtual bool tag(Meta::Essence::Id id);

		bool observe(EssenceIncidentFlags flag_set = {});

	protected:
		virtual bool fetch(const QList<Meta::Essence::Id>& id_list);
		virtual bool opt(Meta::Essence::Id id);
		virtual bool opt(Meta::Essence::Id id, const QJsonObject& object);
		virtual bool opt(Meta::Essence::Id id, const QString& key, const QVariant& value, QVariant& backup);

	public slots:
		virtual bool fetch(QSharedPointer<Core::Data::Rowset> row_set);
	/** @} */

	/** @name Getters */
	/** @{ */
	protected:
		Core::Data::Provider* getDataProvider() const;

	public:
		Roaster* getRoaster() const;

		virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		virtual QModelIndex parent(const QModelIndex& child) const override;
		virtual int rowCount(const QModelIndex& parent) const override = 0;
		virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant key(const QModelIndex& index) const;
		virtual Meta::Essence::Id mapKey(const int& row) const;
		virtual QVariant data(const QVariantMap& key, int role) const;
		virtual QVariant data(const QVariant& key, int role) const;
		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual QVariant data(const Meta::Essence::Id& id, int role) const;
		virtual QHash<int, QByteArray> roleNames() const override;
		virtual QJsonObject optionObject(const Meta::Essence::Id& id) const;
		virtual QVariant option(Meta::Essence::Id id, const QString& key) const;
		virtual const TagMap::mapped_type& tagSet(const Meta::Essence::Id& id) const;
		virtual const TagMap::mapped_type& tagSet(const Meta::Essence::Id& id);
		virtual const TagMap::mapped_type& tagSet(const QModelIndex& index) const;
		virtual const TagMap::mapped_type& tagSet(const QModelIndex& index);
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QSharedPointer<Data::AbstractRecord> makeDataRecord() const = 0;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void altered(Cadabra::Queen::Repository::Meta::Essence::Id id, Cadabra::Queen::Repository::EssenceAlter alter);
		void patched(Cadabra::Queen::Repository::Meta::EssenceAlterMap map);
	/** @} */

	/** @name Slots */
	/** @{ */
	public slots:;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isLoaded() const;
		bool isLazy() const;
		virtual bool isExist(const Meta::Essence::Id& id) const = 0;
	/** @} */
	};

	/**
	 * @brief
	 */
	class MapObserver : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		MapObserver(AbstractMap* map);
		virtual ~MapObserver();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QMap<Meta::Essence::Id, EssenceAlterFlags> _alter_map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void flush();
		const QMap<Meta::Essence::Id, EssenceAlterFlags>& getMap() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onAltered(Meta::Essence::Id id, EssenceAlter alter);
	/** @} */
	};
} } } }

#endif
