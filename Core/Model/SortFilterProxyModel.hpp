#ifndef Cadabra_Queen_Core_Model_SortFilterProxyModel_hpp
#define Cadabra_Queen_Core_Model_SortFilterProxyModel_hpp

#include "../Meta/DynamicRole.hpp"
#include "../Meta/PropertyList.hpp"

#include <QSortFilterProxyModel>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class DynamicRole;
} } } }

namespace Cadabra { namespace Queen { namespace Core { namespace Model {
	class SortFilterProxyModel : public QSortFilterProxyModel
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QAbstractItemModel* sourceModel READ getSource WRITE setSource NOTIFY sourceChanged)
		Q_PROPERTY(QString filterPattern READ getPattern WRITE setPattern NOTIFY patternChanged)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Core::Meta::DynamicRole> roleList READ getRoleList CONSTANT)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static constexpr int MaxDataRole = static_cast<int>(std::numeric_limits<int>::max());
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SortFilterProxyModel(QObject* parent = nullptr);
		virtual ~SortFilterProxyModel() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Meta::TemplatedPropertyList<Meta::DynamicRole> _role_list;
		QHash<int, QByteArray> _role_hash;
	/** @} */

	/** @name Factories*/
	/** @{ */
	public:
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		void validate();

	private:
		void validate(Meta::DynamicRole* role);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QAbstractItemModel* getSource();
		QString getPattern() const;

		QQmlListProperty<Meta::DynamicRole> getRoleList();
		Q_INVOKABLE int getRoleHashKey(const QString& name);

		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		Q_INVOKABLE void setSource(QAbstractItemModel* model);
		Q_INVOKABLE void setPattern(const QString& pattern);

		void addRole(Meta::DynamicRole* role);

		virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	/** @} */

	/** @{ */
	signals:
		void validated();
		void sourceChanged();
		void patternChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	protected slots:
		void onRoleListCleared();
		void onRoleListExpanded();
		void onRoleListUpdated();
	/** @} */
	};
} } } }

#endif
