#include "Basket.hpp"

#include "../Model/Application/OnlineShop.hpp"

namespace Cadabra { namespace Queen { namespace EC { namespace Data {
	/// @todo Перенести в Model, так как нереляционная модель

	const QMap<QString, int> Basket::CurrencyFixedPriorityMap = {
																{QLocale::system().currencySymbol(QLocale::CurrencySymbolFormat::CurrencyIsoCode).toUpper(), -1},
																{"AST", -2}, {"PMT", -2}, {"AGT", -2},
	};
	Basket::Basket(QObject* parent)
	:
		Stock(parent),

		_currency_code(QLocale::system().currencySymbol(QLocale::CurrencySymbolFormat::CurrencyIsoCode).toUpper()),
		_reprice_force(false)
	{
		QObject::connect(this, &Basket::acquirerChanged, this, &Basket::onAcquirerChanged);
	}

	bool Basket::parse(const QJsonObject& object)
	{
		QJsonObject::ConstIterator ia(object.find("items"));
		QJsonObject::ConstIterator da(object.find("discounts"));
		QJsonObject::ConstIterator ra(object.find("reprices"));
		QJsonObject::ConstIterator qa(object.find("quantities"));
		QJsonObject::ConstIterator cc(object.find("currency_code"));
		QJsonObject::ConstIterator ae(object.find("acquirer_engine"));
//		QJsonObject::ConstIterator an(object.find("acquirer_name"));

		if (!da->isArray() || !ra->isArray() || !ia->isArray() || !qa->isArray() || !ae->isString() || object.constEnd() == cc) {
			return false;
		} else if (!setCurrency(cc->toString("XXX"))) {
			return false;
		} else if (!setAcquirer(ae->toString("unknown"))) {
			return false;
		}

		beginResetModel();
		_source_map = ParseArray(ia->toArray());

		QJsonArray quantity_array(qa->toArray());
		for (QJsonArray::ConstIterator q = quantity_array.constBegin(); q != quantity_array.constEnd(); ++q) {
			QJsonObject quantity(q->toObject());
			QJsonObject::ConstIterator p(quantity.find("price_id"));
			QJsonObject::ConstIterator v(quantity.find("value"));
			if (p->isString() && v->isDouble()) {
				_quantity_map[Repository::Meta::Essence::ToId(p->toString())] = v->toDouble();
			}
		}

		QJsonArray discount_array(da->toArray());
		for (QJsonArray::ConstIterator d = discount_array.constBegin(); d != discount_array.constEnd(); ++d) {
			QJsonObject discount(d->toObject());
			QJsonObject::ConstIterator p(discount.find("price_id"));
			QJsonObject::ConstIterator v(discount.find("value"));
			if (p->isString() && v->isDouble()) {
				_discount_map[Repository::Meta::Essence::ToId(p->toString())] = v->toDouble();
			}
		}

		QMap<Repository::Meta::Essence::Id, SD::Meta::Money> reprice_map;
		QJsonArray reprices_array(ra->toArray());
		for (QJsonArray::ConstIterator r = reprices_array.constBegin(); r != reprices_array.constEnd(); ++r) {
			QJsonObject reprice(r->toObject());
			QJsonObject::ConstIterator p(reprice.find("price_id"));
			QJsonObject::ConstIterator c(reprice.find("cost"));
			if (p->isString() && c->isObject()) {
				_reprice_map[Repository::Meta::Essence::ToId(p->toString())] = SD::Meta::Money(c->toObject());
			}
		}

		endResetModel();

		return true;
	}

	QJsonObject Basket::dump() const
	{
		QJsonObject retval(Stock::dump());

		QJsonArray quantity_array;
		for (QuantityMap::ConstIterator q = _quantity_map.constBegin(); q != _quantity_map.constEnd(); ++q) {
			quantity_array.push_back(
									QJsonObject
									{
										{"price_id", QString::number(q.key())},
										{"value", q.value()}
									}
			);
		}

		QJsonArray discount_array;
		for (DiscountMap::ConstIterator d = _discount_map.constBegin(); d != _discount_map.constEnd(); ++d) {
			discount_array.push_back(
									QJsonObject
									{
										{"price_id", QString::number(d.key())},
										{"value", d.value()}
									}
			);
		}

		QJsonArray reprice_array;
		for (RepriceMap::ConstIterator r = _reprice_map.constBegin(); r != _reprice_map.constEnd(); ++r) {
			reprice_array.push_back(
									QJsonObject
									{
										{"price_id", QString::number(r.key())},
										{"cost", static_cast<QJsonObject>(r.value())}
									}
			);
		}

		retval.insert("acquirer_engine", _acquirer ? _acquirer->getEngine() : "");
		retval.insert("acquirer_name", _acquirer ? _acquirer->getName() : "");
		retval.insert("currency_code", _currency_code);
		retval.insert("quantities", quantity_array);
		retval.insert("discounts", discount_array);
		retval.insert("reprices", reprice_array);

		return retval;
	}

	bool Basket::setPaymentMethod()
	{
		/// @brief 0 Нет валюты, эквайринга, map (без QFlags<Option> нет возможности определить)
		if (_currency_code.isEmpty() && _acquirer.isNull() && _source_map.isEmpty()) {
			return false;
		}

		QString currency_code;
		QScopedPointer<Model::AbstractAcquirer> acquirer;
		/// @brief 1 Нет валюты, есть эквайринг и _map (валюта первого Item в _map должна совпадать c поддерживаемыми валютами эквайринга, иначе false)
		if (_currency_code.isEmpty() && !_acquirer.isNull() && !_source_map.isEmpty()) {
			currency_code = _source_map.cbegin()->cost.getCurrencyCode();
			if (!_acquirer->getCurrencyCodeSet().contains(currency_code)) {
				return false;
			}
		}
		/// @brief 2 Нет валюты, эквайринга, но есть _map (по валюте первого Item в _map)
		else if (_currency_code.isEmpty() && _acquirer.isNull() && !_source_map.isEmpty()) {
			currency_code = _source_map.cbegin()->cost.getCurrencyCode();
			for (const QMap<QString, std::function<Model::AbstractAcquirer*(const QString&, const QJsonObject&, QObject*)>>::mapped_type& factory : Model::OnlineShop::AcquirerFactoryMap) {
				acquirer.reset(factory("local", {}, nullptr));
				if (acquirer && acquirer->getCurrencyCodeSet().contains(currency_code)) {
					break;
				} else {
					acquirer.reset();
				}
			}
			if (acquirer.isNull()) {
				return false;
			}
		}
		/// @brief 3 Нет валюты, есть эквайринг, нет _map (валюта локали или первая валюта)
		else if (_currency_code.isEmpty() && !_acquirer.isNull() && _source_map.isEmpty()) {
			currency_code = QLocale::system().currencySymbol(QLocale::CurrencySymbolFormat::CurrencyIsoCode).toUpper();
			if (_acquirer->getCurrencyCodeSet().isEmpty()) {
				return false;
			} else if (!_acquirer->getCurrencyCodeSet().contains(currency_code)) {
				currency_code = *_acquirer->getCurrencyCodeSet().cbegin();
			}
		}
		/// @brief 4 Есть валюта, нет эквайринга
		else if (_currency_code.isEmpty() && _acquirer.isNull()) {
			if (!_source_map.isEmpty() && _source_map.first().cost.getCurrencyCode() != _currency_code) {
				return false;
			}
			for (const QMap<QString, std::function<Model::AbstractAcquirer*(const QString&, const QJsonObject&, QObject*)>>::mapped_type& factory : Model::OnlineShop::AcquirerFactoryMap) {
				acquirer.reset(factory("local", {}, nullptr));
				if (acquirer && acquirer->getCurrencyCodeSet().contains(_currency_code)) {
					break;
				} else {
					return false;
				}
			}
		}

		if (!currency_code.isEmpty()) {
			_currency_code = currency_code;
		}
		if (!acquirer.isNull()) {
			_acquirer.reset(acquirer.take());
			emit acquirerChanged();
		}
		if (!currency_code.isEmpty()) {
			emit currencyChanged();
		}
		return true;
	}

	bool Basket::setPaymentMethod(QFlags<Model::AbstractAcquirer::Option>& option_flagset)
	{
		/**
		 * @brief Нужно реализовать алгоритм определения валюты из заданых преференций/ограничений учитывая
		 * указанные системы оплаты
		 */
		QMultiMap<int, QString> engine_priority_map;
		QMultiMap<int, QString> currency_priority_map;
		for (const SD::Meta::PaymentPreference& payment_preference : getPaymentPreferenceVector()) {
			if (payment_preference.getType() == SD::Meta::PaymentPreference::Type::Voucher) {
				option_flagset.setFlag(Model::AbstractAcquirer::Option::WithoutDiscount, false);
			} else if (payment_preference.getType() == SD::Meta::PaymentPreference::Type::Currency) {
				QString currency_code(payment_preference.getAlias().toUpper());
				if (SD::Meta::Money::CurrencySignMap.contains(currency_code)) {
					currency_priority_map.insert(Basket::CurrencyFixedPriorityMap.value(currency_code, currency_priority_map.count()), currency_code);
				}
			} else if (payment_preference.getType() == SD::Meta::PaymentPreference::Type::Engine) {
				engine_priority_map.insert(engine_priority_map.count(), payment_preference.getAlias().toLower());
			}
		}

		QString currency_code;
		QScopedPointer<Model::AbstractAcquirer> acquirer;

		/// @brief 1 Нет предпочтений - используем валюту QLocale и кейс 3
		if (currency_priority_map.isEmpty() && engine_priority_map.isEmpty()) {
			currency_priority_map.insert(0, QLocale::system().currencySymbol(QLocale::CurrencySymbolFormat::CurrencyIsoCode).toUpper());
		}
		/// @brief 2 Нет предпочтительных валют, есть предпочтительные системы оплат - ищем ту систему, которая подходит под option_flagset и берем у нее валюту которая совпадает с QLocale или первую
		if (currency_priority_map.isEmpty() && !engine_priority_map.isEmpty()) {
			currency_code = QLocale::system().currencySymbol(QLocale::CurrencySymbolFormat::CurrencyIsoCode).toUpper();
			for (const QMultiMap<int, QString>::mapped_type& engine_alias : engine_priority_map) {
				if (Model::OnlineShop::Acquire(acquirer, "local", engine_alias, {})) {
					if (!acquirer || !acquirer->test(option_flagset) || !acquirer->getCurrencyCodeSet().contains(currency_code)) {
						acquirer.reset();
					} else {
						currency_code = *acquirer->getCurrencyCodeSet().cbegin();
					}
				}
			}
		}
		/// @brief 3 Есть предпочтительные валюты, нет предпочтительных систем оплат - выбираем ту систему, которая подходит под option_flagset и берем у нее предпочитаемую валюту
		else if (!currency_priority_map.isEmpty() && engine_priority_map.isEmpty()) {
			for (const QMap<QString, std::function<Model::AbstractAcquirer*(const QString&, const QJsonObject&, QObject*)>>::mapped_type& factory : Model::OnlineShop::AcquirerFactoryMap) {
				acquirer.reset(factory("local", {}, nullptr));
				if (!acquirer || !acquirer->test(option_flagset)) {
					acquirer.reset();
				} else {
					for (const QMultiMap<int, QString>::mapped_type& code : currency_priority_map) {
						if (acquirer->getCurrencyCodeSet().contains(code)) {
							currency_code = code;
							break;
						}
					}
					if (currency_code.isEmpty()) {
						acquirer.reset();
					} else {
						break;
					}
				}
			}
		}
		/// @brief 4 Есть предпочтительные валюты, есть предпочтительные систем оплат - выбираем ту систему, которая подходит под option_flagset и берем у нее одну из приоритетных валют
		else if (!currency_priority_map.isEmpty() && !engine_priority_map.isEmpty()) {
			for (const QMultiMap<int, QString>::mapped_type& a_code : engine_priority_map) {
				if (Model::OnlineShop::Acquire(acquirer, "local", a_code, {}) && acquirer->test(option_flagset)) {
					for (const QMultiMap<int, QString>::mapped_type& c_code : currency_priority_map) {
						if (acquirer->getCurrencyCodeSet().contains(c_code)) {
							currency_code = c_code;
							break;
						}
					}
					if (currency_code.isEmpty()) {
						acquirer.reset();
					} else {
						break;
					}
				} else {
					acquirer.reset();
				}
			}
		}
		return setCurrency(currency_code) && setAcquirer(std::move(acquirer));
	}

	bool Basket::setCurrency(const QString& code)
	{
		if (code.isEmpty() || !SD::Meta::Money::CurrencySignMap.contains(code)) {
			return false;
		} else if (code == _currency_code) {
			return true;
		} else if (_source_map.isEmpty()) {
			_currency_code = code;
		} else if (_source_map.first().cost.getCurrencyCode() != code) {
			Stock::clear();
		} else {
			_currency_code = code;
			return true;
		}
		if (!_acquirer.isNull() && !_acquirer->getCurrencyCodeSet().contains(_currency_code)) {
			_acquirer.reset();
			emit acquirerChanged();
		}
		emit currencyChanged();
		return true;
	}

	bool Basket::setAcquirer(const QString& engine_alias)
	{
		QScopedPointer<Model::AbstractAcquirer> acquirer;
		return (!Model::OnlineShop::Acquire(acquirer, "local", engine_alias, {})) ? false : setAcquirer(std::move(acquirer));
	}

	bool Basket::setAcquirer(QScopedPointer<Model::AbstractAcquirer>&& acquirer)
	{
		if (acquirer.isNull()) {
			return false;
		} else if (!_acquirer.isNull() && (acquirer->getEngine() == _acquirer->getEngine() && acquirer->getName() == _acquirer->getName())) {
			return true;
		}
		_acquirer.reset(acquirer.take());
		emit acquirerChanged();

		if (!_currency_code.isEmpty() && _acquirer->getCurrencyCodeSet().contains(_currency_code)) {
			return true;
		} else if (!_currency_code.isEmpty() && _source_map.isEmpty()) {
			_currency_code.clear();
			emit currencyChanged();
			return true;
		} else if (!_source_map.isEmpty() && _acquirer->getCurrencyCodeSet().contains(_source_map.first().cost.getCurrencyCode())) {
			_currency_code = _source_map.first().cost.getCurrencyCode();
			emit currencyChanged();
			return true;
		} else if (!_source_map.isEmpty()) {
			Stock::clear();
			return true;
		}
		return true;
	}

	void Basket::enableRepriceForce(bool onoff)
	{
		_reprice_force = onoff;
	}

	bool Basket::insert(const Item& item)
	{
		if (item.cost.getCurrencyCode() == _currency_code) {
			return Stock::insert(item);
		} else if (_currency_code.isEmpty()) {
			setCurrency(item.cost.getCurrencyCode());
			return Stock::insert(item);
		}
		return false;
	}

	bool Basket::repriceItem(const Cadabra::Queen::Repository::Meta::Essence::Id& id, const Cadabra::Queen::SD::Meta::Money& price)
	{
		SourceMap::ConstIterator i(_source_map.find(id));
		if (_source_map.constEnd() == i) {
			return false;
		}
		QModelIndex index(createIndex(std::distance(_source_map.cbegin(), i), 0));
		_reprice_map[id] = price;
		emit dataChanged(index, index, {Stock::Role::Cost});
		return true;
	}

	bool Basket::discountItem(const Cadabra::Queen::Repository::Meta::Essence::Id& id, const double& value)
	{
		SourceMap::ConstIterator i(_source_map.find(id));
		if (_source_map.constEnd() == i) {
			return false;
		}
		QModelIndex index(createIndex(std::distance(_source_map.cbegin(), i), 0));
		_discount_map[id] = value;
		emit dataChanged(index, index, {Basket::Role::Discount});
		return true;
	}

	void Basket::addItem(const Item& item, double quantity)
	{
		if (quantity <= 0) {
			return;
		}

		SourceMap::ConstIterator s(_source_map.lowerBound(item.price_id));
		if (_source_map.constEnd() == s || item.price_id != s->price_id) {
			std::iterator_traits<SourceMap::ConstIterator>::difference_type index((_source_map.isEmpty() == 0 || _source_map.cbegin() == s) ? 0 : (std::distance(_source_map.cbegin(), s) + 1));
			if (insert(item)) {
				beginInsertRows(QModelIndex(), index, index);
				_quantity_map[item.price_id] = quantity;
				endInsertRows();
			}
		} else {
			if (_quantity_map.value(item.price_id, 0) != quantity) {
				_quantity_map[item.price_id] = quantity;
				QModelIndex index(createIndex(std::distance(_source_map.cbegin(), s), 0));
				emit dataChanged(index, index, {Basket::Role::Quantity});
			}
		}
	}

	void Basket::addStock(Data::Stock* stock, double quantity)
	{
		if (nullptr == stock || stock->getSourceMap().isEmpty() || quantity <= 0) {
			return ;
		}
		for (const Stock::Item& stock_item : stock->getSourceMap()) {
			_quantity_map[stock_item.price_id] = quantity;
		}
		add(stock->getSourceMap());
	}

	void Basket::addStockItem(Data::Stock* stock, QVariant price_qd, double quantity)
	{
		if (nullptr == stock || !price_qd.isValid() || quantity <= 0) {
			return ;
		}
		Stock::SourceMap::ConstIterator i(stock->getSourceMap().find(Core::Meta::Qd(price_qd).toInt64()));
		if (stock->getSourceMap().constEnd() != i) {
			_quantity_map[i.key()] = quantity;
			add(i.value());
		}
	}

	void Basket::resetPaymentPreference()
	{
		_payment_preference_vector.clear();
	}

	void Basket::addPaymentPreference(SD::Meta::PaymentPreference::Type type, const QString& alias)
	{
		_payment_preference_vector.push_back(SD::Meta::PaymentPreference(type, alias));
	}

	void Basket::addPaymentPreference(const SD::Meta::PaymentPreference& preference)
	{
		_payment_preference_vector.push_back(preference);
	}

	const QString& Basket::getCurrencyCode() const
	{
		return _currency_code;
	}

	const QVector<SD::Meta::PaymentPreference>& Basket::getPaymentPreferenceVector() const
	{
		return _payment_preference_vector;
	}

	double Basket::getQuantity(const SourceMap::ConstIterator& m) const
	{
		return (_source_map.constEnd() == m) ? 0 : _quantity_map.value(m->price_id, 0);
	}

	double Basket::getQuantity(const int& index) const
	{
		return getQuantity(_source_map.constBegin() + index);
	}

	double Basket::getDiscountRate(const SourceMap::ConstIterator& m) const
	{
		return ((_source_map.constEnd() == m) || (m->catalogue_id != 0)) ? 0 : _discount_map.value(m->unit_id, 0);
	}

	double Basket::getDiscountRate(const int& index) const
	{
		return getDiscountRate(_source_map.constBegin() + index);
	}

	SD::Meta::Money Basket::getDiscountedCost(const SourceMap::ConstIterator& m) const
	{
		return ((_source_map.constEnd() == m) || (m->catalogue_id != 0)) ? m->cost : m->cost.discount(_discount_map.value(m->price_id, 0));
	}

	SD::Meta::Money Basket::getDiscountedCost(const int& index) const
	{
		return getDiscountedCost(_source_map.constBegin() + index);
	}

	SD::Meta::Money Basket::getImportCost(const SourceMap::ConstIterator& m) const
	{
		return _reprice_map.value(m->price_id, SD::Meta::Money{0, _currency_code});
	}

	SD::Meta::Money Basket::getImportCost(const int& index) const
	{
		return getImportCost(_source_map.constBegin() + index);
	}

	SD::Meta::Money Basket::getCost(const SourceMap::ConstIterator& m) const
	{
		return _reprice_map.value(m->price_id, getDiscountedCost(m)).multiply(_quantity_map.value(m->price_id, 0));
	}

	QVariant Basket::data(const QModelIndex& index, int role) const
	{
		if (!index.isValid()) {
			return QVariant();
		}
		switch (role) {
			case Role::Quantity: return getQuantity(index.row());
			case Role::Discount: return getDiscountRate(index.row());
			default: return Stock::data(index, role);
		}
		return QVariant{};
	}

	QHash<int, QByteArray> Basket::roleNames() const
	{
		QHash<int, QByteArray> retval(Stock::roleNames());
		retval[Role::Quantity] = "quantity";
		retval[Role::Discount] = "discount";
		return retval;
	}

	QVector<SD::Meta::PaymentPreference> Basket::makePaymentPreferenceVector() const
	{
		QVector<SD::Meta::PaymentPreference> retval;

		if (!_acquirer.isNull()) {
			if (!_acquirer->getName().isEmpty()) {
				retval.push_back(SD::Meta::PaymentPreference{SD::Meta::PaymentPreference::Type::Terminal, _acquirer->getName()});
			}
			if (!_acquirer->getEngine().isEmpty()) {
				retval.push_back(SD::Meta::PaymentPreference{SD::Meta::PaymentPreference::Type::Engine, _acquirer->getEngine()});
			}
		}

		return retval;
	}

	void Basket::onAcquirerChanged()
	{
		if (!_acquirer) {
			return ;
		}
		QObject::connect(_acquirer.get(), &Model::AbstractAcquirer::repriced, this, &Basket::onSourceMapRepriced);
	}

	void Basket::onSourceMapInserted(SourceMap::ConstIterator b, SourceMap::ConstIterator e)
	{
		if (_source_map.constEnd() == b || _acquirer == nullptr || !_acquirer->getOptionFlagSet().testFlag(Model::AbstractAcquirer::Option::PriceAdjustment)) {
			return;
		}
		QMap<Repository::Meta::Essence::Id, QString> item_map;
		do {
			if (_reprice_force || !_reprice_map.contains(b->price_id)) {
				item_map.insert(b->price_id, b->sku);
				_reprice_map.insert(b->price_id, SD::Meta::Money());
			}
		} while (b++ != e);
		if (!item_map.isEmpty() && !_acquirer->reprice(item_map)) {
			_reprice_map.clear();
		}
	}

	void Basket::onSourceMapRepriced(QMap<Cadabra::Queen::Repository::Meta::Essence::Id, Cadabra::Queen::SD::Meta::Money> map)
	{
		if (map.isEmpty()) {
			emit faulted(Cadabra::Queen::EC::PaymentFault::AcquiringError, tr("Prices are not available at this moment"));
			return ;
		}

		QSet<Repository::Meta::Essence::Id> retval;
		for (QMap<Cadabra::Queen::Repository::Meta::Essence::Id, Cadabra::Queen::SD::Meta::Money>::ConstIterator r = map.constBegin(); map.constEnd() != r; ++r) {
			if (repriceItem(r.key(), r.value())) {
				retval.insert(r.key());
			}
		}
		emit itemsRepriced(retval);
	}

	void Basket::onModelBeforeReset()
	{
		_discount_map.clear();
		_reprice_map.clear();
		_quantity_map.clear();
	}

	void Basket::onModelAfterReset()
	{
		onSourceMapInserted(_source_map.cbegin(), --_source_map.cend());
		Stock::onModelAfterReset();
	}

	void Basket::onModelDataChanged(const QModelIndex& top_left, const QModelIndex& bottom_right, const QVector<int>& roles)
	{
		/// @note а как у них могла измениться цена и скидка?
/*
		for (Stock::SourceMap::ConstIterator k1(_source_map.begin() + top_left.row()), k2(_source_map.begin() + bottom_right.row()); k1 != k2; ++k1) {
			_quantity_map.remove(k1.key());
			_discount_map.remove(k1.key());
			_reprice_map.remove(k1.key());
		}
*/
		Stock::onModelDataChanged(top_left, bottom_right, roles);
	}

	void Basket::onModelRowsInserted(const QModelIndex& parent, int first, int last)
	{
		Stock::onModelRowsInserted(parent, first, last);
		onSourceMapInserted(_source_map.cbegin() + first, _source_map.cbegin() + last);
	}

	void Basket::onModelRowsRemoved(const QModelIndex& parent, int first, int last)
	{
		for (Stock::SourceMap::ConstIterator k1(_source_map.begin() + first), k2(_source_map.begin() + last); k1 != k2; ++k1) {
			_quantity_map.remove(k1.key());
			_discount_map.remove(k1.key());
			_reprice_map.remove(k1.key());
		}
		Stock::onModelRowsRemoved(parent, first, last);
	}

	bool Basket::isEmpty() const
	{
		if (Stock::isEmpty()) {
			return true;
		} else if (std::any_of(_reprice_map.cbegin(), _reprice_map.cend(), [](const SD::Meta::Money& price){ return price.isUndefined(); } )) {
			return true;
		}
		return false;
	}
} } } }
