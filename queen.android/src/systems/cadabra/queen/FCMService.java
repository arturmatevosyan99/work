package systems.cadabra.queen;

import java.util.Map;
import java.lang.String;

import android.util.Log;
import android.content.Intent;

import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

public class FCMService extends FirebaseMessagingService
{
	private static native void Token(String token);
	private static native void Message(String title, String icon, String sound, String body);

	@Override
	public void onNewToken(String token)
	{
		Log.w("FCMService::onNewToken", token);
		FCMService.Token(token);
		super.onNewToken(token);
	}

	@Override
	public void onMessageReceived(RemoteMessage remote_message)
	{
		Log.d("FCMService::onMessageReceived", "from=" + remote_message.getFrom());
		if (remote_message.getData().size() > 0) {
			Log.d("FCMService::onMessageReceived", "message data payload=" + remote_message.getData());
		}
		if (remote_message.getNotification() != null) {
			Log.d("FCMService::onMessageReceived", "message notification title=" + remote_message.getNotification().getTitle());
			Log.d("FCMService::onMessageReceived", "message notification icon=" + remote_message.getNotification().getIcon());
			Log.d("FCMService::onMessageReceived", "message notification sound=" + remote_message.getNotification().getSound());
			Log.d("FCMService::onMessageReceived", "message notification body=" + remote_message.getNotification().getBody());

			FCMService.Message
			(
				remote_message.getNotification().getTitle(),
				remote_message.getNotification().getIcon(),
				remote_message.getNotification().getSound(),
				remote_message.getNotification().getBody()
			);
		}
		super.onMessageReceived(remote_message);
	}
}
