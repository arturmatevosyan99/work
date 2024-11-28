#include "TinkoffAcquirer.hpp"

#import <ASDKCore/ASDKCore.h>
#import <ASDKUI/ASDKUI.h>
#import <UIKit/UIKit.h>
#import <PassKit/PassKit.h>
#import <Contacts/Contacts.h>

#include <QDebug>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	const QString TinkoffAcquirer::WalletPaymentIntefaceName = "Pay";

	bool TinkoffAcquirer::pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total, PaymentInterface payment_interface)
	{
		if (PaymentInterface::Card != payment_interface && (TinkoffAcquirer::AppMerchantId.isEmpty() || [ASDKPaymentFormStarter isPayWithAppleAvailable] == NO)) {
			if (PaymentInterface::Auto == payment_interface) {
				payment_interface = PaymentInterface::Card;
			} else {
				resolve(tr("Apple Pay is not available, please use different method"));
				return false;
			}
		}

		if (PaymentInterface::Auto == payment_interface) {
			return pay(legal_counterparty, item_vector, total, true);
		}

		NSPersonNameComponents* contact_name = [[NSPersonNameComponents alloc] init];
		contact_name.givenName = legal_counterparty.getName().toNSString();
		PKContact* shipping_contact = [[PKContact alloc] init];
		shipping_contact.name = contact_name;
		NSMutableDictionary* receipt = [NSMutableDictionary dictionary];
		if (legal_counterparty.hasEmail()) {
			shipping_contact.emailAddress = legal_counterparty.getEmail().toNSString();
			[receipt setObject: legal_counterparty.getEmail().toNSString() forKey: @"Email"];
		} else if (legal_counterparty.hasPhone()) {
			shipping_contact.phoneNumber = [CNPhoneNumber phoneNumberWithStringValue: legal_counterparty.getPhone().toNSString()];
			[receipt setObject: legal_counterparty.getPhone().toNSString() forKey: @"Phone"];
		} else {
			shipping_contact.emailAddress = @"patient@oblozhko.ru";
			/// @xxx Иначе не пройдет платеж
			[receipt setObject: @"patient@oblozhko.ru" forKey: @"Email"];
		}

		NSMutableArray* basket = [NSMutableArray array];
		for (const SD::Meta::PurchaseItem& item : item_vector) {
			NSMutableDictionary* basket_item = [NSMutableDictionary dictionary];
			[basket_item setObject: item.getName().toNSString() forKey: @"Name"];
			[basket_item setObject: [NSNumber numberWithUnsignedLongLong: item.getCentum()] forKey: @"Price"];
			[basket_item setObject: [NSNumber numberWithDouble: item.getQuantity()] forKey: @"Quantity"];
			[basket_item setObject: [NSNumber numberWithUnsignedLongLong: (item.getCost().multiply(item.getQuantity()).getCentum())]  forKey: @"Amount"];
			[basket_item setObject: @"none"  forKey: @"Tax"];
			[basket addObject: basket_item];
		}
		[receipt setObject: [NSArray arrayWithArray:basket] forKey: @"Items"];
		[receipt setObject: @"patent" forKey: @"Taxation"];

		ASDKStringKeyCreator* public_key_source(nil);
		ASDKAcquiringSdk* sdk(nil);
		ASDKPaymentFormStarter* payment_controller(nil);
		@try {
			public_key_source = [[[ASDKStringKeyCreator alloc] initWithPublicKeyString: _public_key.toNSString()] autorelease];
			sdk = [ASDKAcquiringSdk
									acquiringSdkWithTerminalKey: _terminal_key.toNSString()
									payType: nil // @"O" // @"T"
									password: _password.toNSString()
									publicKeyDataSource: public_key_source
			];
			[sdk setDebug: NO];
			[sdk setTestDomain: NO];
			[sdk setLogger: nil];

			payment_controller = [ASDKPaymentFormStarter paymentFormStarterWithAcquiringSdk: sdk];
			if (PaymentInterface::Wallet == payment_interface) {
				[payment_controller
									payWithApplePayFromViewController: [[UIApplication sharedApplication].keyWindow rootViewController]
									amount: [NSNumber numberWithDouble: total.getCost()]
									orderId: makeOrderNo().toNSString()
									description: TinkoffAcquirer::AppMerchantName.isEmpty() ? _order_description.toNSString() : TinkoffAcquirer::AppMerchantName.toNSString()
									customerKey: _customer_key.toNSString()
									sendEmail: !legal_counterparty.getEmail().isEmpty()
									email: legal_counterparty.getEmail().toNSString()
									appleMerchantId: TinkoffAcquirer::AppMerchantId.toNSString()
									shippingMethods: nil
									shippingContact: shipping_contact
									shippingEditableFields: PKAddressFieldName|PKAddressFieldEmail|PKAddressFieldPhone
									recurrent: _is_recurrent ? YES : NO
									additionalPaymentData: @{}
									receiptData: receipt
									shopsData: nil
									shopsReceiptsData: nil
									success:^(ASDKPaymentInfo* paymentInfo)
									{
										resolve(true);
									}
									cancelled:^
									{
										resolve(false);
									}
									error:^(ASDKAcquringSdkError* error)
									{
										resolve(QString("TKS SDK Exception(%1): %2").arg(QString::fromNSString(error.errorMessage)).arg(QString::fromNSString(error.errorDetails)));
									}
				];
/*
				[payment_controller
									payUsingApplePayFromViewController: [[UIApplication sharedApplication].keyWindow rootViewController]
									amount: [NSNumber numberWithDouble: total.getCost()]
									orderId: makeOrderNo().toNSString()
									description: _order_description.toNSString()
									customerKey: _customer_key.toNSString()
									sendEmail: !legal_counterparty.getEmail().isEmpty()
									email: legal_counterparty.getEmail().toNSString()
									appleMerchantId: TinkoffAcquirer::AppMerchantId.toNSString()
									shippingMethods: nil
									shippingContact: shipping_contact
									shippingEditableFields: [NSSet setWithObjects:PKContactFieldName, PKContactFieldEmailAddress, PKContactFieldPhoneNumber, nil]
									recurrent: _is_recurrent ? YES : NO
									additionalPaymentData: @{}
									receiptData: receipt
									shopsData: nil
									shopsReceiptsData: nil
									success:^(ASDKPaymentInfo* paymentInfo)
									{
										resolve(true);
									}
									cancelled:^
									{
										resolve(false);
									}
									error:^(ASDKAcquringSdkError* error)
									{
										resolve(QString("TKS SDK Exception(%1): %2").arg(QString::fromNSString(error.errorMessage)).arg(QString::fromNSString(error.errorDetails)));
									}
				];
*/
			} else {
				[shipping_contact release];
				[payment_controller
									presentPaymentFormFromViewController: [[UIApplication sharedApplication].keyWindow rootViewController]
									orderId: makeOrderNo().toNSString()
									amount: [NSNumber numberWithDouble: total.getCost()]
									title: _order_title.toNSString()
									description: _order_description.toNSString()
									cardId: _customer_card_id.isEmpty() ? @"" : _customer_card_id.toNSString() // nil - новая нужно вводить реквизиты карты, @"" - последняя сохраненная, @"836252" - карта по CardId
									email: legal_counterparty.getEmail().toNSString()
									customerKey: _customer_key.toNSString()
									recurrent: _is_recurrent ? YES : NO
									makeCharge: NO
									additionalPaymentData: @{}
									receiptData: receipt
									shopsData: nil
									shopsReceiptsData: nil
									success:^(ASDKPaymentInfo* paymentInfo)
									{
										resolve(true);
									}
									cancelled:^
									{
										resolve(false);
									}
									error:^(ASDKAcquringSdkError* error)
									{
										resolve(QString("TKS SDK Exception(%1): %2").arg(QString::fromNSString(error.errorMessage)).arg(QString::fromNSString(error.errorDetails)));
									}
				];
			}
		} @catch (NSException* e) {
			resolve(QString("TKS SDK Exception(%1): %2").arg(QString::fromNSString(e.name)).arg(QString::fromNSString(e.reason)));
			return false;
		}
		return true;
	}
} } } }
