#ifdef Cadabra_Queen_EC_Model_Acquiring_AppStore_hpp
class AppStoreKitDelegate;
#else

#ifndef Cadabra_Queen_EC_Model_Acquiring_AppStore_h
#define Cadabra_Queen_EC_Model_Acquiring_AppStore_h

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface AppStoreKitDelegate : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver>

@property (nonatomic, copy, nullable) void (^onProductResponse)(NSArray<SKProduct*>* products);
@property (nonatomic, copy, nullable) void (^onPaymentTransaction)(SKPaymentTransaction* transaction);
@property (nonatomic, copy, nullable) void (^onError)(NSError* error);

@property (nonatomic, retain, readonly, nullable) SKProductsRequest* request;
@property (nonatomic, retain, readonly, nullable) SKProductsResponse* response;
@property (nonatomic, retain, readonly, nullable) SKPayment* payment;
@property (nonatomic, retain, readonly, nullable) SKPaymentTransaction* transaction;

+ (instancetype) defaultInstance;

- (bool) requestProduct:(nonnull NSMutableSet*)set;
- (bool) enqueuePayment;
- (void) reset;
@end

#endif

#endif
