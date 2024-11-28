#ifndef Cadabra_Queen_Stress_Model_CheckList_hpp
#define Cadabra_Queen_Stress_Model_CheckList_hpp

#include <QAbstractListModel>

#include "../../Core/Meta/Scheme.hpp"

#include <QUrl>
#include <QList>
#include <QItemSelectionModel>

namespace Cadabra { namespace Queen { namespace Stress { namespace Model {
	class CheckList : public QAbstractListModel
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			SchemeModel = Qt::EditRole,
			FactoryAlias = Qt::DisplayRole,
			CheckState = Qt::CheckStateRole,
			StatusState = Qt::StatusTipRole
		};
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
		CheckList(QObject* parent = nullptr);
		virtual ~CheckList() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QStringList _filter_list;
		QList<Core::Meta::SchemeModel*> _model_list;
		QItemSelectionModel _selection_list;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool append(QUrl url);
		bool append(Core::Meta::SchemeList* list);
		bool append(Core::Meta::SchemeModel* model);
		bool append(QScopedPointer<Core::Meta::SchemeModel>&& model);
		bool remove(const QUrl& url);
		void filter(const QStringList& filter_list);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		int getCount() const;
		Core::Meta::SchemeModel* getFirst() const;
		Core::Meta::SchemeModel* getLast() const;
		Core::Meta::SchemeModel* get(int index) const;

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		Q_INVOKABLE virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		Q_INVOKABLE virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
		Q_INVOKABLE void setCheckState(const QModelIndex& index, bool value);
		Q_INVOKABLE void setStatusState(const QModelIndex& index, QString value);
	/** @} */

	/** @name Slots */
	/** @{ */
	public slots:
		void rowsInserted(int list_index, const QModelIndex& parent_index, int start, int end);
	/** @} */
	};
} } } }

#endif
