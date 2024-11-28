#ifndef Cadabra_Queen_Repository_Model_AbstractProxy_hpp
#define Cadabra_Queen_Repository_Model_AbstractProxy_hpp

#include "Application/Domain.hpp"

#include "../Meta/EssenceToken.hpp"
#include "../Data/Entity/AbstractRecord.hpp"
#include "../Net/AbstractCursor.hpp"
#include "../Model/AbstractIterator.hpp"

#include <QAbstractItemModel>
#include <QQmlListProperty>
#include <QPair>
#include <QList>
#include <QCache>
#include <QFlags>
#include <QThread>
#include <QPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class AbstractProxy : public QAbstractItemModel
	{
	/** @name Aliases */
	/** @{ */
	public:
		using TokenVector = QVector<Meta::EssenceToken>;
		using OrderVector = QVector<int>;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class RefreshFlag : int
		{
			Manual = 0x00,
			DynamicToken = 0x01,
			PermanentToken = 0x02
		};
		Q_DECLARE_FLAGS(RefreshFlags, RefreshFlag)
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_FLAG(RefreshFlags)
		Q_PROPERTY(bool isReady READ isReady NOTIFY readyChanged)
		Q_PROPERTY(bool isLive READ isLive WRITE setLive NOTIFY liveChanged)
		Q_PROPERTY(bool isReverse READ isReverse WRITE setReverse NOTIFY reverseChanged)
		Q_PROPERTY(QQmlListProperty<Cadabra::Queen::Repository::Meta::EssenceToken> tokens READ getDynamicTokenList NOTIFY dynamicTokenChanged)
		/// @todo better use QList
//		Q_PROPERTY(QQmlListProperty<int> orders READ getDynamicOrderList NOTIFY dynamicOrderChanged)
		Q_PROPERTY(Cadabra::Queen::Repository::ProxyIndexFlags alternativeIndexes READ getAlternativeIndexFlagSet NOTIFY indexAltered)
		Q_PROPERTY(Cadabra::Queen::Repository::ProxyIndexFlags preferredIndexes READ getPreferredIndexFlagSet WRITE setPreferredIndexFlagSet NOTIFY indexPreferred)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static Cadabra::Queen::Repository::Meta::EssenceToken* DynamicToken(QQmlListProperty<Cadabra::Queen::Repository::Meta::EssenceToken>* list, int index);
		static int DynamicTokenListSize(QQmlListProperty<Cadabra::Queen::Repository::Meta::EssenceToken>* list);

		static int* DynamicOrder(QQmlListProperty<int>* list, int index);
		static int DynamicOrderListSize(QQmlListProperty<int>* list);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractProxy(Realm* parent = nullptr);
		virtual ~AbstractProxy();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Realm* _realm;

		bool _is_ready;
		bool _is_live;
		bool _is_reverse;
		QFlags<RefreshFlag> _refresh_flagset;

		QFlags<ProxyIndex> _alternative_index;
		QFlags<ProxyIndex> _preferred_index;

		QCache<QModelIndex, QVariantMap> _active_cache;
		QSharedPointer<QAbstractItemModel> _active_model;
		QSharedPointer<Model::AbstractIterator> _map_iterator;
		QSharedPointer<Data::AbstractRecord> _data_record;
		QSharedPointer<Net::AbstractCursor> _net_cursor;

		QObject _property_list_parent;
		TokenVector _dynamic_token_vector;
		TokenVector _permanent_token_vector;
		OrderVector _dynamic_order_vector;
		OrderVector _permanent_order_vector;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool load();
		Q_INVOKABLE bool unload();
		Q_INVOKABLE bool update();
		Q_INVOKABLE bool tokenize(const QString& raw_token, bool permanent = false);
		Q_INVOKABLE bool assort(const QString& role_name, Qt::SortOrder sort_order, bool permanent = false);

	public :
		bool reload();
		bool refresh(RefreshFlag mode = RefreshFlag::Manual);
		virtual void realize(Realm* realm, bool background_mode = false);

	protected:
		virtual Model::AbstractIterator* allocateIterator() const;
		virtual Data::AbstractRecord* allocateRecord() const;
		virtual Net::AbstractCursor* allocateCursor() const;

		virtual bool filterIterator(Model::AbstractIterator* iterator) const;
		virtual bool filterRecord(Data::AbstractRecord* record) const;
		virtual bool filterCursor(Net::AbstractCursor* cursor) const;

	private:
		void resetActiveModel(QSharedPointer<QAbstractItemModel> candidate = nullptr);
		QSharedPointer<QAbstractItemModel> filter();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QStringList makeTokenList() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	protected:
		Realm* getRealm() const;

	public:		
		QAbstractItemModel* getActiveModel() const;
		virtual AbstractMap* getSourceModel() const = 0;
		ProxyIndexFlags getAlternativeIndexFlagSet() const;
		ProxyIndexFlags getPreferredIndexFlagSet() const;
		RefreshFlags getRefreshFlagSet() const;

		const AbstractProxy::TokenVector& getTokenVector(bool permanent = false) const;
		const AbstractProxy::TokenVector& getPermanentTokenVector() const;
		const AbstractProxy::TokenVector& getDynamicTokenVector() const;

		const AbstractProxy::OrderVector& getOrderVector(bool permanent = false) const;
		const AbstractProxy::OrderVector& getPermanentOrderVector() const;
		const AbstractProxy::OrderVector& getDynamicOrderVector() const;

		QQmlListProperty<Cadabra::Queen::Repository::Meta::EssenceToken> getDynamicTokenList();
		QQmlListProperty<int> getDynamicOrderList();

		virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const final;
		virtual QModelIndex parent(const QModelIndex& child) const final;
		virtual int rowCount(const QModelIndex& parent) const override;
		virtual int columnCount(const QModelIndex& parent = QModelIndex()) const final;
		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Mutators */
	/** @{ */
	private:
		void setReady(bool onoff = true);

	public:
		void setLive(bool onoff = true);
		void setReverse(bool onoff);
		void setPreferredIndex(ProxyIndex preference, bool onoff);
		void setPreferredIndex(ProxyIndex preference);
		void setPreferredIndexFlagSet(ProxyIndexFlags preference);
		void setRefreshMode(RefreshFlag mode, bool onoff = true);

		Q_INVOKABLE bool addToken(const QList<Meta::EssenceToken> token_list, bool permanent = false);
		Q_INVOKABLE bool addToken(Meta::EssenceToken::Type token_type, const QString& token_value, bool permanent = false);
		Q_INVOKABLE bool replaceToken(int index, Meta::EssenceToken::Type token_type, const QString& token_value, bool permanent = false);
		Q_INVOKABLE bool removeToken(int index, bool permanent = false);
		Q_INVOKABLE void clearTokenVector(bool permanent = false);

		Q_INVOKABLE bool addOrder(int role, Qt::SortOrder order, bool permanent = false);
		Q_INVOKABLE bool resetOrder(int index, int role, Qt::SortOrder order, bool permanent = false);
		Q_INVOKABLE bool removeOrder(int index, bool permanent = false);
		Q_INVOKABLE void clearOrderVector(bool permanent = false);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void readyChanged(bool onoff);
		void liveChanged(bool onoff);
		void reverseChanged(bool onoff);

		void indexPreferred();
		void indexAltered();

		void permanentTokenChanged(int index, int count, bool onoff);
		void dynamicTokenChanged(int index, int count, bool onoff);

		void permanentOrderChanged(int index, bool onoff);
		void dynamicOrderChanged(int index, bool onoff);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onSourceReseted();
		void onSourceChanged(const QModelIndex& top_left, const QModelIndex& bottom_right, const QVector<int>& role_vector = QVector<int>());
		void onActiveResized(int row_count);
		void onActiveIndexed(QSharedPointer<Core::Data::Rowset> row_set, int begin_offset, int end_offset);
		void onActiveTransformed(QSet<int> inserted_index_set, QSet<int> removed_index_set, QSet<int> updated_index_set, QMap<int, int> moved_index_map);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasRealm() const;
		bool hasSourceModel() const;
		bool hasActiveModel() const;
		bool isReady() const;
		bool isLive() const;
		bool isReverse() const;
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Repository::Model::AbstractProxy::RefreshFlags)

#endif
