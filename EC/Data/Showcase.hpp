#ifndef Cadabra_Queen_EC_Data_Showcase_hpp
#define Cadabra_Queen_EC_Data_Showcase_hpp

#include "../../Repository/Meta/Essence.hpp"

#include "Stock.hpp"

#include <QAbstractListModel>
#include <QMap>
#include <QStringList>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace EC { namespace Data {
	class Showcase : public QAbstractListModel
	{			
	/** @name Classes */
	/** @{ */
	public:
		enum class Catalogue : char
		{
			Order = 'O',
			Category = 'C',
			Bundle = 'B',
			Nomenclature = 'N'
		};

		enum Role
		{			
			Code = Qt::UserRole + 1,
			Caption,
			Description,
			Banner,
			Total
		};

		struct Item
		{			
			QString code;
			QString caption;
			QString description;
			QSharedPointer<Stock> stock;
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Catalogue)
		Q_PROPERTY(Catalogue catalogue MEMBER _catalogue)
		Q_PROPERTY(QStringList filter MEMBER _filter)
		Q_PROPERTY(bool isActive MEMBER _is_active)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static Showcase::Item Parse(const QJsonObject& object);
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using Map = QMap<QString, Item>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Showcase(QObject* parent = nullptr);
		Showcase(Showcase::Catalogue catalogue, QObject* parent = nullptr);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Map _map;
		Catalogue _catalogue;
		QStringList _filter;
		bool _is_active;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void add(const QJsonObject& object);
		void add(const Item& item);
		void add(const Map& map);
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void clear();
		void setActive(bool onoff = false);
		void setCatalogue(const Catalogue& catalogue);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Q_INVOKABLE Cadabra::Queen::EC::Data::Stock* stock(int index);

		const Catalogue& getCatalogue() const;
		const Map& getItemMap() const;
		const QStringList& getFilterStringList() const;
		QStringList& getFilterStringList();

		virtual int rowCount(const QModelIndex &parent) const;
		virtual QVariant data(const QModelIndex &index, int role) const;
		virtual QHash<int, QByteArray> roleNames() const;
	/** @} */

	/** @name States */
	/** @{ */
	signals:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isActive() const;
	/** @} */
	};
} } } }

#endif
