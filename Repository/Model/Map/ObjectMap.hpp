#ifndef Cadabra_Queen_Repository_Model_Essence_ObjectMap_hpp
#define Cadabra_Queen_Repository_Model_Essence_ObjectMap_hpp

#include "../AbstractMap.hpp"

#include "../../Meta/Object.hpp"
#include "../../Data/Entity/Record/Object.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class ObjectMap : public AbstractMap
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Recursion
		{
			None, /// < Self
			Children,
			Family, /// < Self + Children
			Descendant,
			Tree /// < Self + Desendant
		};
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using Map = QMap<Meta::Object::Id, QSharedPointer<Meta::Object>>; /// @todo QtConcurrentMap
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ObjectMap(QObject* parent = nullptr);
		virtual ~ObjectMap() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Map _map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool load(Roaster* roaster, bool lazy_mode = false) override;
		virtual bool unload() override;

		virtual bool apply(Meta::Essence::Id id, const QDateTime& modification_datetime, const QJsonObject& dump) override;
		virtual bool remove(Meta::Essence::Id id) override;

		virtual bool tag(Meta::Essence::Id id, Meta::Essence::Id tag_id, bool onoff = true) override;
		virtual bool opt(Meta::Essence::Id id, const QString& key, const QVariant& value) override;

		QSharedPointer<const Meta::Object> upsert(Meta::Object::Id id, Meta::Object::Id parent_id, Meta::Object::ApplicationScheme application_scheme, const QString& name, bool favourite, bool localization, const QString& picture, Meta::Object::PermissionRule permission, qint64 creation_timestamp, qint64 modification_timestamp);
		void access(Meta::Object::Id id);

		bool feed(QSharedPointer<Data::Object> data, int first_row_index, int last_row_index);

	protected:
		virtual bool fetch(const QList<Meta::Essence::Id>& id_list) override;
		bool fetch(Meta::Essence::Id parent_id, Recursion recursion);

	private:
		bool fetch(QSharedPointer<Data::Object> data);

	public slots:
		virtual bool fetch(QSharedPointer<Core::Data::Rowset> row_set) override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QSharedPointer<Data::AbstractRecord> makeDataRecord() const override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QSharedPointer<const Meta::Object> getObject(Meta::Object::Id id);

		virtual int rowCount(const QModelIndex& parent) const override;
		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual QVariant data(const Meta::Essence::Id& id, int role) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	public slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool isExist(const Meta::Essence::Id& id) const override;
	/** @} */
	};
} } } }

#endif
