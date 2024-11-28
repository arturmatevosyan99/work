package systems.cadabra.queen;

import java.util.List;
import java.lang.String;

import android.util.Log;
import android.content.Intent;

import androidx.annotation.NonNull;

import ru.rustore.sdk.core.tasks.OnCompleteListener;
import ru.rustore.sdk.pushclient.RuStorePushClient;
import ru.rustore.sdk.pushclient.messaging.exception.RuStorePushClientException;
import ru.rustore.sdk.pushclient.messaging.model.RemoteMessage;
import ru.rustore.sdk.pushclient.messaging.service.RuStoreMessagingService;

public class RSMService extends RuStoreMessagingService
{
	private static native void Token(String token);
	private static native void Message(String title, String icon, String sound, String body);

	@Override
	public void onNewToken(@NonNull String token)
	{
		Log.w("RSMService::onNewToken", token);
		RSMService.Token(token);
		super.onNewToken(token);
	}

	@Override
	public void onMessageReceived(@NonNull RemoteMessage remote_message)
	{
		Log.w("RSMService::onMessageReceived", remote_message.getMessageId());
		if (remote_message.getData().size() > 0) {
			Log.d("RSMService::onMessageReceived", remote_message.getData().toString());
		}
		if (remote_message.getNotification() != null) {
			Log.w("RSMService::onMessageReceived", remote_message.getNotification().toString());

			RSMService.Message
			(
				remote_message.getNotification().getTitle(),
				remote_message.getNotification().getIcon(),
				"", // remote_message.getNotification().getSound(),
				remote_message.getNotification().getBody()
			);
		}
		super.onMessageReceived(remote_message);
	}

	@Override
	public void onDeletedMessages()
	{
		Log.w("RSMService::onDeletedMessages", "1");
	}

	@Override
	public void onError(@NonNull final List<? extends RuStorePushClientException> errors)
	{
		Log.e("RSMService::onError", "1");
		for (RuStorePushClientException error : errors) {
			error.printStackTrace();
		}
	}
}
