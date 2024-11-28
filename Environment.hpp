#ifndef Cadabra_Queen_Environment_hpp
#define Cadabra_Queen_Environment_hpp

#include <QAbstractListModel>

namespace Cadabra { namespace Queen {
	class Environment : public QAbstractListModel
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			VariableRole = Qt::UserRole + 1,
			ValueRole
		};
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static Environment& GlobalInstance();
	/** @} */

	/** @name Aliases */
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
		Environment(QObject* parent = nullptr);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QVariantMap _map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void updateModel();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Q_INVOKABLE QVariant getVariable(const QString& key, const QVariant& default_value = QVariant{}) const;

		virtual int rowCount(const QModelIndex& parent) const override;
		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */


	/** @name Mutators */
	/** @{ */
	public:
		Q_INVOKABLE bool setVariable(const QString& key, const QVariant& value);

		virtual bool setData(const QModelIndex& index, const QVariant& value, int role = ValueRole) override;
	/** @} */

	};
} }

#endif
