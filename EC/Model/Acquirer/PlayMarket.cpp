#include "PlayMarket.hpp"

#include <QLocale>
#include <QtAndroid>
#include <QAndroidJniEnvironment>

namespace Cadabra { namespace Queen { namespace EC { namespace Model {
/*
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
			}

			if (success) {
				payment->successed();
			} else if (environment->GetStringLength(message) > 0) {
				payment->faulted(environment->GetStringUTFChars(message, nullptr));
			} else {
				payment->canceled();
			}
		}

	private:
		QMap<int, TinkoffAcquirer*> _payment_map;

		TinkoffResolver()
		{
			 QtAndroid::runOnAndroidThreadSync(
												[]()
												{
													JNINativeMethod methods[] {
																				{"ResolvePayment", "(Ljava/lang/String;IZLjava/lang/String;)V", reinterpret_cast<void*>(Cadabra::Queen::EC::Model::TinkoffResolver::CompletePayment)}
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
*/

	Meta::InappReceipt InappCharger::Receipt()
	{
		return Meta::InappReceipt();
	}

	InappCharger::InappCharger(const QString& name, const QJsonObject& specification, QObject* parent)
	:
		AbstractAcquirer(name, parent),
		_shared_secret(""),
		_debug(false)
	{
		QJsonObject::ConstIterator s(specification.find("playmarket"));
		if (s != specification.constEnd() && s->isObject()) {
			const QJsonObject spec(s->toObject());
			spec.value("shared_secret").toString("");
			spec.value("debug").toBool(true);
		}
	}

	bool InappCharger::pay(const SD::Meta::LegalCounterparty& legal_counterparty, const QVector<SD::Meta::PurchaseItem>& item_vector, const SD::Meta::Money& total)
	{
		return false;
	}

	const QString& InappCharger::getEngine() const
	{
		static const QString retval("ec.playmarket");
		return retval;
	}

	QSet<QString> InappCharger::getCurrencyCodeSet() const
	{
		return {QLocale::system().currencySymbol(QLocale::CurrencySymbolFormat::CurrencyIsoCode)};
	}
} } } }
