#include "TinkoffAcquirer.hpp"

#include <QtAndroid>
#include <QAndroidJniEnvironment>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
	const QString TinkoffAcquirer::WalletPaymentIntefaceName = "Google Pay";

	class TinkoffResolver
	{
	public:
		static TinkoffResolver& Instance()
		{
			static TinkoffResolver instance;
			return instance;
		}

		static void CompletePayment(JNIEnv* environment, jobject self, jstring acquiring, int payment_id, bool success, jstring message)
		{
			if (QString(environment->GetStringUTFChars(acquiring, nullptr)).compare("TKS") != 0) {
				return ;
			}
			TinkoffAcquirer* payment(Instance().extract(payment_id));
			if (!payment) {
				return ;
			} else if (success) {
				emit payment->successed();
			} else if (environment->GetStringLength(message) > 0) {
				emit payment->faulted(environment->GetStringUTFChars(message, nullptr));
			} else {
				emit payment->canceled();
			}
		}

	private:
		QMap<int, TinkoffAcquirer*> _payment_map;

		TinkoffResolver()
		{
			 QtAndroid::runOnAndroidThreadSync
			(
				[]()
				{
					JNINativeMethod methods[] {
												{
													"ResolvePayment",
													"(Ljava/lang/String;IZLjava/lang/String;)V",
													reinterpret_cast<void*>(Cadabra::Queen::EC::Model::TinkoffResolver::CompletePayment)
												}
					};

					QAndroidJniObject java_class("systems.cadabra.queen.QueenActivity");
					QAndroidJniEnvironment jni_environment;
					jclass object_class = jni_environment->GetObjectClass(java_class.object<jobject>());
					jni_environment->RegisterNatives(object_class, methods, sizeof(methods) / sizeof(methods[0]));
					jni_environment->DeleteLocalRef(object_class);
				}
			);
		}
		~TinkoffResolver()
		{
		}

	public:
		bool observe(int payment_id, TinkoffAcquirer* acquirer)
		{
			return _payment_map.contains(payment_id) ? false : _payment_map.end() != _payment_map.insert(payment_id, acquirer);
		}

		TinkoffAcquirer* extract(int payment_id)
		{
			return _payment_map.take(payment_id);
		}
	};

	bool TinkoffAcquirer::pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total, PaymentInterface payment_interface)
	{
		QAndroidJniObject j_customer_key = QAndroidJniObject::fromString(_customer_key);
		QAndroidJniObject j_customer_card_id = QAndroidJniObject::fromString(_customer_card_id);
		QAndroidJniObject j_customer_email = QAndroidJniObject::fromString(legal_counterparty.getEmail());
		QAndroidJniObject j_customer_phone = QAndroidJniObject::fromString(legal_counterparty.hasPhone() ? legal_counterparty.getPhone() : "");
		QAndroidJniObject j_public_key = QAndroidJniObject::fromString(_public_key);
		QAndroidJniObject j_terminal_key = QAndroidJniObject::fromString(_terminal_key);
		QAndroidJniObject j_password = QAndroidJniObject::fromString(_password);
		QAndroidJniObject j_order_id = QAndroidJniObject::fromString(makeOrderNo());
//		QAndroidJniObject j_payment_total = QAndroidJniObject::fromString();
		QAndroidJniObject j_payment_title = QAndroidJniObject::fromString(_order_title);
		QAndroidJniObject j_payment_description = QAndroidJniObject::fromString(_order_description);
//		QAndroidJniObject j_is_recurrent = QAndroidJniObject::fromString();

		QAndroidJniEnvironment j_environment;
		jclass tax_enum = j_environment->FindClass("ru/tinkoff/acquiring/sdk/Tax");
		jfieldID tax_none_field = j_environment->GetStaticFieldID(tax_enum , "NONE", "Lru/tinkoff/acquiring/sdk/Tax;");
		jobject tax_none = j_environment->GetStaticObjectField(tax_enum, tax_none_field);
		QAndroidJniObject j_tax_none = QAndroidJniObject::fromLocalRef(tax_none);

		jobjectArray item_array = j_environment->NewObjectArray(item_vector.count(), j_environment->FindClass("ru/tinkoff/acquiring/sdk/Item"), nullptr);
		int index(0);
		for (const SD::Meta::PurchaseItem& i : item_vector) {
			QAndroidJniObject j_item_name(QAndroidJniObject::fromString(i.getName()));
			double item_quantity(i.getQuantity());
			QAndroidJniObject j_item_price("java/lang/Long", "(J)V", static_cast<jlong>(i.getCentum()));
			QAndroidJniObject j_item_amount("java/lang/Long", "(J)V", static_cast<jlong>(i.getCost().multiply(i.getQuantity()).getCentum()));
			QAndroidJniObject item("ru/tinkoff/acquiring/sdk/Item", "(Ljava/lang/String;Ljava/lang/Long;DLjava/lang/Long;Lru/tinkoff/acquiring/sdk/Tax;)V", j_item_name.object<jstring>(), j_item_price.object(), item_quantity, j_item_amount.object(), j_tax_none.object());
			j_environment->SetObjectArrayElement(item_array, index++, item.object());
		}
		QAndroidJniObject j_item_array = QAndroidJniObject::fromLocalRef(item_array);
		bool google_pay_onoff(payment_interface == PaymentInterface::Wallet ? true : false);

		track(item_vector, total);

		jint payment_id;
		QtAndroid::runOnAndroidThreadSync
		(
			[&]()
			{
				payment_id = QtAndroid::androidActivity().callMethod<jint>
				(
					"payWithTinkoff",
					"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Lru/tinkoff/acquiring/sdk/Item;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;ZZ)I",
					j_customer_key.object<jstring>(),
					j_customer_card_id.object<jstring>(),
					j_customer_email.object<jstring>(),
					j_customer_phone.object<jstring>(),
					j_public_key.object<jstring>(),
					j_terminal_key.object<jstring>(),
					j_password.object<jstring>(),
					j_item_array.object<jobjectArray>(),
					j_order_id.object<jstring>(),
					static_cast<jlong>(total.getCentum()),
					j_payment_title.object<jstring>(),
					j_payment_description.object<jstring>(),
					_is_recurrent,
					google_pay_onoff
				);
			}
		);
		return TinkoffResolver::Instance().observe(payment_id, this);
	}
} } } }
