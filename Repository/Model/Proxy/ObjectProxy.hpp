#ifndef Cadabra_Queen_Repository_Model_Proxy_ObjectProxy_hpp
#define Cadabra_Queen_Repository_Model_Proxy_ObjectProxy_hpp

#include "../AbstractProxy.hpp"

#include "../ObjectPath.hpp"
#include "../../Meta/Object.hpp"
#include "../../Meta/Subject.hpp"
#include "../../Meta/Group.hpp"

#include <QWeakPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class ObjectProxy : public AbstractProxy
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class PathFilter
		{
			None,
			ObjectPath
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::Model::ObjectPath* path READ getPath CONSTANT)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ObjectProxy(Realm* parent = nullptr);
		virtual ~ObjectProxy();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		ObjectPath _path;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual Model::AbstractIterator* allocateIterator() const override;
		virtual Data::AbstractRecord* allocateRecord() const override;
		virtual Net::AbstractCursor* allocateCursor() const override;

		virtual bool filterIterator(Model::AbstractIterator* iterator) const override;
		virtual bool filterRecord(Data::AbstractRecord* record) const override;
		virtual bool filterCursor(Net::AbstractCursor* cursor) const override;
	/** @} */

	/** @name Getters */
	/** @{ */	
	public:
		virtual AbstractMap* getSourceModel() const override;
		ObjectPath* getPath();
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void addOrder(Meta::Object::Role role, Qt::SortOrder order = Qt::SortOrder::AscendingOrder);

		void addPathFilter(PathFilter filter);
		void addPathFilter(const QString& path);
		void addParentIdFilter(const Meta::Object::Id& object_id);

		void addApplicationFilter(const Meta::Object::ApplicationScheme& scheme_ident);
		void addApplicationFilter(const QString& scheme_alias, bool ident_convert = false);

		void addObjectFilter(const QString& name);
		void addObjectFilter(const Meta::Object::Id& id);

		void addOwnerFilter(const QString& name);
		void addOwnerFilter(const Meta::Object::Id& id);

		void addOwnersFilter(const QString& name);
		void addOwnersFilter(const Meta::Group::Id& id);
	/** @} */

	/** @name Factories */
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
	/** @} */
	};
} } } }

#endif
