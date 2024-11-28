#ifndef Cadabra_Queen_Repository_Meta_FolderIndex_hpp
#define Cadabra_Queen_Repository_Meta_FolderIndex_hpp

#include "Object.hpp"

#include <QAbstractListModel>
#include <QSharedPointer>
#include <QMultiMap>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class FolderIndex : public QAbstractListModel
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Map = QMultiMap<Meta::Object::Id, QSharedPointer<const Meta::Object>>;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			Id = Qt::UserRole + 1,
			ParentId,
			Name,
			ApplicationScheme,
			Option,
			Picture,
			CreationTimestamp,
			ModificationTimestamp
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
		FolderIndex(QObject* parent = nullptr);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Map _object_map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool parse(const QJsonValue& value);
		bool parse(const QJsonArray& value);
		void add(const QSharedPointer<const Meta::Object>& object);
		void add(const Map& object_map);
		void remap(const Map& object_map);
		void swap(Map&& object_map);
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual int rowCount(const QModelIndex &parent) const;
		virtual QVariant data(const QModelIndex &index, int role) const;
		virtual QHash<int, QByteArray> roleNames() const;
	/** @} */
	};
} } } }

#endif
