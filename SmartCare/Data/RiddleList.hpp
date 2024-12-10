#ifndef Cadabra_Queen_SmartCare_Data_RiddleList_hpp
#define Cadabra_Queen_SmartCare_Data_RiddleList_hpp

#include <QAbstractListModel>

#include "../../Core/Meta/Qd.hpp"

#include <tuple>
#include <QJsonObject>
#include <QQmlComponent>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Data {
	class RiddleList : public QAbstractListModel
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Tuple = std::tuple<Core::Meta::Qd, QJsonObject, QQmlComponent*>;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static QVector<RiddleList::Tuple> Parse(const QJsonValue& value);
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		RiddleList(QObject* parent = nullptr);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QVector<Tuple> _vector;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool append(QVector<Tuple>&& vector);
		Q_INVOKABLE bool compose(QQmlComponent* component);
		Q_INVOKABLE bool removeTail(int index);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isEmpty() const;
	/** @} */
	};
} } } }

#endif
