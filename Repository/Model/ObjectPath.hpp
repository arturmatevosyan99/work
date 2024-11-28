/**
 @file ObjectPath.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Model_ObjectPath_hpp
#define Cadabra_Queen_Repository_Model_ObjectPath_hpp

#include "Map/ObjectMap.hpp"
#include "../Meta/Object.hpp"
#include "../../Core/Meta/Qd.hpp"

#include <QVariant>
#include <QVector>
#include <QSharedPointer>
#include <QMetaType>
#include <QDataStream>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class ObjectPath : public QObject
	{
	/** @name Statics */
	/** @{ */
	private:
		static const QSharedPointer<const Meta::Object> RootObject;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class Shortcut : std::int8_t
		{
			Root,
			Back
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Shortcut)
		Q_PROPERTY(int depth READ getDepth NOTIFY changed)
		Q_PROPERTY(QString breadcrumbs READ breadCrumbs NOTIFY changed)
		Q_PROPERTY(QString name READ getLocalizedName NOTIFY changed)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ObjectPath(ObjectMap* map = nullptr);
		ObjectPath(const ObjectPath& origin);
		ObjectPath(ObjectPath&& origin);
		~ObjectPath() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	public:
		ObjectMap* _map;
		QVector<QSharedPointer<const Meta::Object>> _vector;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool go(Meta::Object::Id id);
		bool go(QSharedPointer<const Meta::Object> object);
		Q_INVOKABLE bool go(Shortcut shortcut);
		Q_INVOKABLE bool backward();
		Q_INVOKABLE bool forward(Cadabra::Queen::Core::Meta::Qd object_qd);
		QString breadCrumbs() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QVector<QSharedPointer<const Meta::Object>>& getVector() const;
		int getDepth() const;
		QString getLocalizedName() const;
	/** @} */

	public:
		void setMap(ObjectMap* map);

	/** @name Signals*/
	/** @{ */
	signals:
		void changed();
	/** @} */

	/** @name Converters */
	/** @{ */
	public:
	/** @} */

	/** @name Friends */
	/** @{ */
	friend QDebug operator<<(QDebug debug, const ObjectPath& object_path);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasMap() const;
	/** @} */
	};
} } } }

#endif
