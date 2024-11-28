#import "AppStore.h"

#include "AppStore.hpp"
#include "../../../Skeleton.hpp"
#include "../../../Core/System/Native.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QIODevice>

@interface AppStoreKitDelegate()
@end

@implementation AppStoreKitDelegate
+(instancetype) defaultInstance
{
	static AppStoreKitDelegate* Instance = nil;
	static dispatch_once_t Guard;

	dispatch_once
	(
		&Guard,
		^{
			Instance = [[AppStoreKitDelegate alloc] init];
		}
	);
	return Instance;
}

-(instancetype) init
{
	if (self = [super init]) {
		_request = nil;
		_response = nil;
		_payment = nil;
		_transaction = nil;
		self.onProductResponse = nil;
		self.onPaymentTransaction = nil;
		self.onError = nil;

		[[SKPaymentQueue defaultQueue] addTransactionObserver: self];
	}
	return self;
}

-(void) dealloc
{
	[[SKPaymentQueue defaultQueue] removeTransactionObserver: self];
	[self reset];
	[_onProductResponse release];
	[_onPaymentTransaction release];
	[_onError release];
	[super dealloc];
}

-(void) productsRequest:(SKProductsRequest*)request didReceiveResponse:(SKProductsResponse*)response
{
	@synchronized (self) {
		_response = [response retain];
		if (self.onProductResponse != nil) {
			self.onProductResponse(self.response.products);
		}
	}
}

-(void) request:(SKRequest*)request didFailWithError:(NSError*)error
{
	if (self.onError != nil) {
		self.onError(error);
	}
}

-(void) paymentQueue:(SKPaymentQueue*)queue updatedTransactions:(NSArray *)transactions
{
	for (SKPaymentTransaction* action in transactions) {
		if (_payment != nil && [[action.payment productIdentifier] isEqualToString: [_payment productIdentifier]]) {
			switch (action.transactionState) {
				case SKPaymentTransactionStatePurchased:
					_transaction = [action retain];
					if (self.onPaymentTransaction) {
						self.onPaymentTransaction(_transaction);
					}
					break;

				case SKPaymentTransactionStateFailed:
					_transaction = [action retain];
					if (self.onError) {
						self.onError(action.error);
					}
					break;

				case SKPaymentTransactionStateRestored:
					[queue finishTransaction: action];
					break;

				case SKPaymentTransactionStateDeferred:
					/// @note A transaction that is in the queue, but its final status is pending external action such as Ask to Buy.
					break;

				case SKPaymentTransactionStatePurchasing:
					/// @note A transaction that is being processed by the App Store.
					break;

				default:
					break;
			}
		}
	}
}

-(bool) requestProduct:(NSMutableSet*)set
{
	@synchronized (self) {
		if (_request) {
			return false;
		}

		_request = [SKProductsRequest alloc];
		[_request initWithProductIdentifiers: set];
		[_request setDelegate: self];
		[_request start];
	}

	return true;
}

-(bool) enqueuePayment
{
	@synchronized (self) {
		if (_payment || !_response) {
			return false;
		} else if ([SKPaymentQueue canMakePayments]) {
			/// @??? а можно несколько за один раз?
			_payment = [[SKPayment paymentWithProduct: self.response.products.firstObject] retain];
			[[SKPaymentQueue defaultQueue] addPayment: self.payment];
			return true;
		}

		/// @note InApp purchases are disabled in Settings.app
		if (self.onError != nil) {
			self.onError([NSError errorWithDomain: @"In-app purchases are not allowed, please check you settings" code: 1 userInfo: nil]);
		}
	}
	return false;
}

-(void) reset
{
	@synchronized (self) {
		if (_request) {
			[_request release];
			_request = nil;
		}

		if (_payment) {
			[_payment release];
			_payment = nil;
		}

		if (_transaction) {
			[[SKPaymentQueue defaultQueue] finishTransaction: _transaction];
			[_transaction release];
			_transaction = nil;
		}

		if (_response) {
			[_response release];
			_response = nil;
		}
	}
}

@end

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	Meta::InappReceipt InappCharger::Receipt()
	{
		QFile receipt(QUrl::fromNSURL([[NSBundle mainBundle] appStoreReceiptURL]).toLocalFile());
		if (!receipt.open(QIODevice::ReadOnly) || receipt.size() == 0) {
			return Meta::InappReceipt();
		}

		return Meta::InappReceipt(
									Meta::InappReceipt::Distributor::AppStore,
									Core::System::Native::Instance()->getBundleIdentifier(),
									QString::fromUtf8(receipt.readAll().toBase64()),
									Cadabra::Queen::Skeleton::DebugMode
		);
	}

	InappCharger::InappCharger(const QString& name, const QJsonObject& specification, QObject* parent)
	:
		AbstractAcquirer(name, parent),

		_shared_secret(""),
		_debug(Cadabra::Queen::Skeleton::DebugMode)
	{
		static const QString node_array[] = {"appstore", "application"};
		for (const QString* node = std::begin(node_array); node != std::end(node_array); std::advance(node, 1)) {
			QJsonObject::ConstIterator s(specification.find(*node));
			if (s != specification.constEnd() && s->isObject()) {
				const QJsonObject spec(s->toObject());
				_shared_secret = spec.value(Core::System::Native::Instance()->getBundleIdentifier()).toString("");
				_debug = spec.value("debug").toBool(true);
			}
		}
	}

	InappCharger::~InappCharger()
	{

	}

	bool InappCharger::pay(const SD::Meta::LegalCounterparty&, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total)
	{
		if (_shared_secret.isEmpty()) {
			return false;
		}

		NSMutableSet* sku_set = [NSMutableSet set];
		for (const SD::Meta::PurchaseItem& item : item_vector) {
			if (!item.getSKU().isEmpty()) {
				[sku_set addObject: item.getSKU().toNSString()];
			}
		}
		if (sku_set.count == 0) {
			return false;
		}

		track(item_vector, total);

		__block AppStoreKitDelegate* delegate = [[[AppStoreKitDelegate alloc] init] autorelease];
		[delegate setOnProductResponse:	^(NSArray<SKProduct*>* products)
										{
											[delegate enqueuePayment];
										}
		];
		[delegate setOnPaymentTransaction: ^(SKPaymentTransaction* transaction)
											{
												Meta::InappReceipt receipt(InappCharger::Receipt());
												if (!receipt.isInvalid()) {
													QJsonObject payload(receipt.makeJSON());
													payload.insert("order_id", QString::number(_order_id));
													payload.insert("transaction_id", QString::fromNSString([transaction transactionIdentifier]));

													QUrlQuery query;
													query.addQueryItem("id", QString::number(_shop_id));
													query.addQueryItem("alias", _name);

													QUrl url(_shop_endpoint->makeBaseURL());
													url.setPath("/ec/hook/acquirer");
													url.setQuery(query);

													QNetworkRequest request(url);
													request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

													QNetworkReply* future_reply(_shop_endpoint->post(request, QJsonDocument(payload).toJson()));
													QObject::connect(future_reply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::errorOccurred), std::bind(&InappCharger::onReceiptValidated, this, future_reply, std::placeholders::_1));
													QObject::connect(future_reply, &QNetworkReply::finished, std::bind(&InappCharger::onReceiptValidated, this, future_reply, QNetworkReply::NetworkError::NoError));
												} else {
													resolve("Could not fetch the appstore receipt");
												}
												[delegate release];
											}
		];
		[delegate setOnError: ^(NSError* error)
								{
									resolve(QString::fromNSString(error.localizedDescription));
									[delegate release];
								}
		];

		return ([delegate requestProduct: sku_set] && [delegate retain]);
	}

	bool InappCharger::reprice(const QMap<Repository::Meta::Essence::Id, QString>& item_map)
	{
		NSMutableDictionary* sku_dictionary = [NSMutableDictionary dictionary];
		NSMutableSet* sku_set = [NSMutableSet set];
		for (QMap<Repository::Meta::Essence::Id, QString>::ConstIterator i = item_map.constBegin(); i != item_map.constEnd(); ++i) {
			if (!i.value().isEmpty()) {
				[sku_set addObject: i.value().toNSString()];
				[sku_dictionary setObject: [NSNumber numberWithLongLong: i.key()] forKey: i.value().toNSString()];
			}
		}
		if (sku_set.count == 0) {
			return false;
		}

		__block AppStoreKitDelegate* delegate = [[[AppStoreKitDelegate alloc] init] autorelease];
		[delegate setOnProductResponse: ^(NSArray<SKProduct*>* products)
										{
											NSMutableDictionary* dictionary = sku_dictionary;
											NSUInteger count = [products count];
											QMap<Repository::Meta::Essence::Id, SD::Meta::Money> reprice_map;
											for (NSUInteger index = 0; index < count; ++index) {
												SKProduct* product = [products objectAtIndex: index];
												NSNumber* price_id = [dictionary objectForKey: product.productIdentifier];
												reprice_map.insert([price_id longLongValue], SD::Meta::Money(product.price.doubleValue * 100, QString::fromNSString(product.priceLocale.currencyCode)));
												[dictionary removeObjectForKey: product.productIdentifier];
											}
											resolve(reprice_map);
											[delegate release];
										}
		];
		[delegate setOnError: ^(NSError* error) {
									resolve(QString::fromNSString(error.localizedDescription));
									[delegate release];
								}
		];

		return ([delegate requestProduct: sku_set] && [delegate retain]);
	}

	const QString& InappCharger::getEngine() const
	{
		static const QString retval("ec.appstore");
		return retval;
	}

	QSet<QString> InappCharger::getCurrencyCodeSet() const
	{
		return {"AST"};
	}

	QFlags<AbstractAcquirer::Option> InappCharger::getOptionFlagSet() const
	{
		return {Option::PriceAdjustment, Option::SinglePurchaseItem, Option::WithoutDiscount};
	}

	void InappCharger::onReceiptValidated(QNetworkReply* reply, QNetworkReply::NetworkError error)
	{
		reply->deleteLater();
		if (QNetworkReply::NetworkError::NoError != error || reply->error() != QNetworkReply::NetworkError::NoError) {
			resolve(QString("Could not validate AppStore receipt"));
		} else {
			resolve(true);
		}
	}
} } } }
