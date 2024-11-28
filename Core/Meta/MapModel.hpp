#ifndef Cadabra_Queen_Core_Meta_MapModel_hpp
#define Cadabra_Queen_Core_Meta_MapModel_hpp

#include <QAbstractListModel>

#include <QMap>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class MapModel : public QAbstractListModel
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			Key = Qt::UserRole + 1,
			Value
		};

		struct Down
		{
		/** @name Constructors */
		/** @{ */
			Down(const Down& origin) = delete;
			Down(MapModel* origin);
			Down(Down&& origin);
			~Down();
		/** @} */

		/** @name Properties */
		/** @{ */
			MapModel* map_model;
		/** @} */

		/** @name Procedures */
		/** @{ */
			void detach();
		/** @} */
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Role)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		MapModel(QObject* parent = nullptr);
		virtual ~MapModel() override = default;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
		Q_INVOKABLE virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
		Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	/** @} */
	};

	template <typename K, typename V>
	class TemplatedMapModel : public MapModel
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Map = QMap<K, V>;
	/** @{ */

	/** @name Constructors */
	/** @{ */
	public:
		TemplatedMapModel(QObject* parent = nullptr, Map* source_map = nullptr)
		:
			MapModel(parent),

			_owner(!source_map),
			_source_map(!source_map ? new Map() : source_map)
		{

		}
		virtual ~TemplatedMapModel() override
		{
			if (_owner) {
				delete _source_map;
			}
		}
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const bool _owner;
		Map* _source_map;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		Down cleanUp()
		{
			Down retval(this);
			_source_map->clear();
			return retval;
		}
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Map& getMap()
		{
			return *_source_map;
		}

		const Map& getMap() const
		{
			return *_source_map;
		}

		virtual QHash<int, QByteArray> roleNames() const override
		{
			QHash<int, QByteArray> retval(QAbstractListModel::roleNames());
			retval[Role::Key] = "id";
			retval[Role::Value] = "name";
			return retval;
		}

		virtual int rowCount(const QModelIndex& parent) const override
		{
			return parent.isValid() ? 0 : _source_map->size();
		}

		virtual QVariant data(const QModelIndex& index, int role) const override
		{
			if (!index.isValid()) {
				return QVariant();
			} else if (Qt::DisplayRole == role) {
				role = index.column() == 0 ? Role::Key : Role::Value;
			}

			switch (role) {
				case Role::Key:
					return QVariant::fromValue((_source_map->constBegin() + index.row()).key());

				case Role::Value:
					return (_source_map->constBegin() + index.row()).value();

				default:
					return QVariant();
			}
		}
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isEmpty() const
		{
			return _source_map->isEmpty();
		}
	/** @} */
	};
} } } }

#endif
