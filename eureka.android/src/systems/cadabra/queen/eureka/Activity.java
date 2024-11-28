package systems.cadabra.queen.eureka;

import systems.cadabra.queen.QueenActivity;

import java.lang.Runnable;
import java.util.Arrays;
import java.util.List;
import java.util.Currency;
import java.math.BigDecimal;

import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.widget.Toast;
import android.content.Intent;

import com.google.android.gms.wallet.WalletConstants;

import ru.tinkoff.acquiring.sdk.GooglePayParams;
import ru.tinkoff.acquiring.sdk.PayFormActivity;
import ru.tinkoff.acquiring.sdk.Money;
import ru.tinkoff.acquiring.sdk.OnPaymentListener;
import ru.tinkoff.acquiring.sdk.Item;
import ru.tinkoff.acquiring.sdk.Receipt;
import ru.tinkoff.acquiring.sdk.Taxation;
import ru.tinkoff.acquiring.sdk.Tax;
import ru.tinkoff.acquiring.sdk.Journal;

public class Activity extends QueenActivity implements OnPaymentListener
{
	static final List<String> eCurrencyList = Arrays.asList("AST", "CCC", "AGT", "PMT", "XXX");

	public static final int TINKOFF_PAYMENT_CODE = 0x80001;

	private int _tinkoff_payment_index = -1;

	public void trackUser()
	{
	}

	public boolean trackUserPurchase(String payment_system, String sku, double quantity, long price, String currency_code)
	{
		return false;
	}

	public boolean trackUserEvent(String message)
	{
		return false;
	}

	public int payWithTinkoff(String customer_key, String customer_card_id, String customer_email, String customer_phone, String public_key, String terminal_key, String password, Item[] item_array, String order_id, long payment_total, String payment_title, String payment_description, boolean is_recurrent, boolean with_google_pay)
	{
//		Journal.setDebug(true);
//		Journal.setDeveloperMode(true);
		Receipt receipt = new Receipt(item_array, customer_email, Taxation.ENVD);
		receipt.setPhone(customer_phone);

		GooglePayParams google_pay_params = null;
		if (with_google_pay) {
			google_pay_params = new GooglePayParams.Builder()
															.setAddressRequired(false)
															.setPhoneRequired(false)
															.setEnvironment(WalletConstants.ENVIRONMENT_PRODUCTION)
															.build();
		}

		PayFormActivity
			.init(terminal_key, password, public_key) // данные продавца
			.prepare(
						order_id,					// ID заказа в вашей системе
						Money.ofCoins(payment_total),// сумма для оплаты
						payment_title,				// название платежа, видимое пользователю
						payment_description,		// описание платежа, видимое пользователю
						customer_card_id,			// ID карточки
						customer_email,				// E-mail клиента для отправки уведомления об оплате
						is_recurrent,				// флаг определяющий является ли платеж рекуррентным [1]
						true						// флаг использования безопасной клавиатуры [2]
			)
//			.setCustomerKey(customer_key)			// уникальный ID пользователя для сохранения данных его карты
			.setGooglePayParams(google_pay_params)
			.setReceipt(receipt)
			.startActivityForResult(this, TINKOFF_PAYMENT_CODE);

		return ++_tinkoff_payment_index;
	}

	protected void onActivityResult(int request_code, int result_code, Intent data)
	{
		Log.d("Activity::onActivityResult", Integer.toString(result_code));
		if (request_code == TINKOFF_PAYMENT_CODE) {
			PayFormActivity.dispatchResult(result_code, data, this);
			return ;
		}
		super.onActivityResult(request_code, result_code, data);
	}

	@Override
	public void onSuccess(long payment_id, String card_id)
	{
		Log.e("Activity::onSuccess", Long.toString(payment_id));
		QueenActivity.ResolvePayment("TKS", _tinkoff_payment_index, true, "");
	}

	@Override
	public void onCancelled()
	{
		Log.e("Activity::onCancelled", "");
		QueenActivity.ResolvePayment("TKS", _tinkoff_payment_index, false, "");
	}

	@Override
	public void onError(Exception e)
	{
		Log.e("Activity::onError", e.getMessage(), e);
		QueenActivity.ResolvePayment("TKS", _tinkoff_payment_index, false, e.getMessage());
	}
}
