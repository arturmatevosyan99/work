#ifndef Cadabra_Queen_Core_Meta_DynamicRole_hpp
#define Cadabra_Queen_Core_Meta_DynamicRole_hpp

#include <QObject>
#include <QString>
#include <QVariant>
#include <QQmlParserStatus>

namespace Cadabra { namespace Queen { namespace Core { namespace Model {
	class SortFilterProxyModel;
} } } }

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class DynamicRole : public QObject, public QQmlParserStatus
	{	
	/** @name Friends */
	/** @{ */
		friend class Model::SortFilterProxyModel;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_INTERFACES(QQmlParserStatus)
		Q_PROPERTY(int roleIndex READ getRoleIndex)
		Q_PROPERTY(QString roleName READ getRoleName WRITE setRoleName NOTIFY roleNameChanged)
		Q_PROPERTY(QVariant roleValue READ getRoleValue WRITE setRoleValue NOTIFY roleValueChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		DynamicRole(const DynamicRole& origin);
		DynamicRole(QObject* parent = nullptr);
		DynamicRole(QObject* parent, const QString& role_name, const QVariant& role_value = QVariant());
		virtual ~DynamicRole() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		int _role_index;
		QString _role_name;
		QVariant _role_value;
		QMap<int, QVariant> _role_map;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void roleNameChanged();
		void roleValueChanged();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const int& getRoleIndex() const;
		const QString& getRoleName() const;
		const QVariant& getRoleValue() const;
		const QVariant getRoleValue(int index) const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setRoleName(const QString& name);
		void setRoleValue(const QVariant& value);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void classBegin() override;
		virtual void componentComplete() override;
	/** @} */
	};
} } } }

#endif
