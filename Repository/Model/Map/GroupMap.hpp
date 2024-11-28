#ifndef Cadabra_Queen_Repository_Model_Essence_GroupMap_hpp
#define Cadabra_Queen_Repository_Model_Essence_GroupMap_hpp

#include "../AbstractMap.hpp"

#include "../../Meta/Group.hpp"
#include "../../Data/Entity/Record/Group.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class GroupMap : public AbstractMap
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using Map = QMap<Meta::Group::Id, QSharedPointer<Meta::Group>>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		GroupMap(QObject* parent = nullptr);
		virtual ~GroupMap() override;
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

		virtual bool tag(Meta::Essence::Id id, Meta::Essence::Id tag_id, bool onoff = true) override;
		virtual bool opt(Meta::Essence::Id id, const QString& key, const QVariant& value) override;

		virtual bool apply(Meta::Group::Id id, const QDateTime& modification_datetime, const QJsonObject& dump) override;
		virtual bool remove(Meta::Essence::Id id) override;

		QSharedPointer<const Meta::Group> upsert(Meta::Group::Id id, const QString& name, const QString& description, const QString& picture, int creation_timestamp, int modification_timestamp);

	protected:
		virtual bool fetch(const QList<Meta::Essence::Id>& id_list) override;

	private:
		bool fetch(QSharedPointer<Data::Group> data);

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
		QSharedPointer<const Meta::Group> getGroup(Meta::Group::Id id);

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
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool isExist(const Meta::Essence::Id& id) const override;
	/** @} */
	};
} } } }

#endif
