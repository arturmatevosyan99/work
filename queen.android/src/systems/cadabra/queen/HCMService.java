package systems.cadabra.queen;

import java.lang.String;

import android.util.Log;
import android.content.Intent;

import com.huawei.hms.push.HmsMessageService;
import com.huawei.hms.push.RemoteMessage;

public class HCMService extends HmsMessageService
{
	private static native void Token(String token);
	private static native void Message(String title, String icon, String sound, String body);

	@Override
	public void onNewToken(String token)
	{
		Log.w("HCMService::onNewToken", token);
		HCMService.Token(token);
		super.onNewToken(token);
	}

	@Override
	public void onMessageReceived(RemoteMessage remote_message)
	{
		if (remote_message == null) {
			Log.e("HCMService::onMessageReceived", "null");
			return;
		}
		Log.i("HCMService::onMessageReceived", remote_message.getMessageId());

		Log.i
		(
			"HCMService::onMessageReceived",
			"get Data: " + remote_message.getData()
			+ "\n getFrom: " + remote_message.getFrom()
			+ "\n getTo: " + remote_message.getTo()
			+ "\n getMessageId: " + remote_message.getMessageId()
			+ "\n getSentTime: " + remote_message.getSentTime()
			+ "\n getDataMap: " + remote_message.getDataOfMap()
			+ "\n getMessageType: " + remote_message.getMessageType()
			+ "\n getTtl: " + remote_message.getTtl()
			+ "\n getToken: " + remote_message.getToken()
		);
		HCMService.Message
		(
			remote_message.getNotification().getTitle(),
			remote_message.getNotification().getIcon(),
			remote_message.getNotification().getSound(),
			remote_message.getNotification().getBody()
		);
		super.onMessageReceived(remote_message);
	}
}
