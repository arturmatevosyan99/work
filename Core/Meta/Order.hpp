/**
 @file Order.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Meta_Order_hpp
#define Cadabra_Queen_Core_Meta_Order_hpp

#include <QQmlParserStatus>
#include <QObject>
#include <QString>
#include <QVariant>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class Order : public QObject, public QQmlParserStatus
	{
	friend class SchemeModel;
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_INTERFACES(QQmlParserStatus)
		Q_PROPERTY(QString roleName READ getRoleName WRITE setRoleName NOTIFY roleNameChanged)
		Q_PROPERTY(Qt::SortOrder roleOrder READ getRoleOrder WRITE setRoleOrder NOTIFY roleOrderChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Order(QObject* parent = nullptr);
		Order(QObject* parent, const QString& name, Qt::SortOrder order);
		Order(const Order& origin);
		virtual ~Order() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		int _role_index;
		QString _role_name;
		Qt::SortOrder _role_order;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void roleNameChanged();
		void roleOrderChanged();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getRoleName() const;
		const Qt::SortOrder& getRoleOrder() const;
		const int& getRoleIndex() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setRoleName(const QString& name);
		void setRoleOrder(Qt::SortOrder order);
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
