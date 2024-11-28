/**
 @file Filter.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Meta_Filter_hpp
#define Cadabra_Queen_Core_Meta_Filter_hpp

#include <QQmlParserStatus>
#include <QObject>
#include <QString>
#include <QVariant>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class Filter : public QObject, public QQmlParserStatus
	{
	friend class SchemeModel;
	/** @name Classes */
	/** @{ */
	public:
		enum Condition
		{
			Equal,
			NotEqual,
			Less,
			LessOrEqual,
			Greater,
			GreaterOrEqual
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_INTERFACES(QQmlParserStatus)
		Q_ENUM(Condition)
		Q_PROPERTY(Condition condition READ getCondition WRITE setCondition NOTIFY conditionChanged)
		Q_PROPERTY(QString roleName READ getRoleName WRITE setRoleName NOTIFY roleNameChanged)
		Q_PROPERTY(QVariant roleValue READ getRoleValue WRITE setRoleValue NOTIFY roleValueChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Filter(QObject* parent, Condition condition, const QString& role_name, const QVariant& role_value);
		Filter(QObject* parent = nullptr);
		Filter(const Filter& origin);
		virtual ~Filter() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		int _role_index;
		QString _role_name;
		QVariant _role_value;
		Condition _condition;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		bool operator==(const QVariant& rhs) const;
		bool operator!=(const QVariant& rhs) const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void conditionChanged();
		void roleNameChanged();
		void roleValueChanged();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getRoleName() const;
		const QVariant& getRoleValue() const;
		const int& getRoleIndex() const;
		const Condition& getCondition() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setRoleName(const QString& name);
		void setRoleValue(const QVariant& value);
		void setCondition(const Condition& condition);
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
