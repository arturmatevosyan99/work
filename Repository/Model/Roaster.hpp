#ifndef Cadabra_Queen_Repository_Model_Roaster_hpp
#define Cadabra_Queen_Repository_Model_Roaster_hpp

#include "TagSet.hpp"
#include "../Meta/Essence.hpp"
#include "../../Core/Data/Provider.hpp"

#include <QObject>
#include <QSharedPointer>
#include <QMap>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Roaster : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT		
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using TagMap = QMap<Meta::Essence::Id, QObject*>; /// @todo QtConcurrentMap
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Roaster();
		virtual ~Roaster() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Core::Data::Provider* _data_provider;
		TagMap _tag_map;
		Meta::Essence::Id _last_load_id;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		TagMap::ConstIterator operator[](const Meta::Essence::Id& id);
		TagMap::ConstIterator operator[](const Meta::Essence::Id& id) const;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		TagSet makeSet(const QSet<Meta::Essence::Id>& id_set);
		QSet<Meta::Essence::Id> filterSet(const QString& pattern) const;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool load(Core::Data::Provider* provider);
		bool load(qsizetype& count);
		bool unload();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Core::Data::Provider* getDataProvider() const;
		TagMap::ConstIterator get(const Meta::Essence::Id& id) const;
		TagMap::ConstIterator get(const Meta::Essence::Id& id);
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		bool emplace(Meta::Essence::Id id, const QString& alias);
		bool import(const QJsonArray& rowset);
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
		bool isLoaded() const;
		bool isValid(const TagMap::ConstIterator& iteartor) const;
	/** @} */
	};
} } } }

#endif
