package systems.cadabra.queen;

import java.lang.String;
import java.lang.Throwable;

import android.util.Log;

import androidx.annotation.NonNull;

public class RSMTokenListener implements ru.rustore.sdk.core.tasks.OnCompleteListener<String>
{
	private static native void Back(long native_back, String token);

	private final long _native_backend;

	public RSMTokenListener(final long native_backend)
	{
		this._native_backend = native_backend;
	}

	@Override
	public void onSuccess(String result)
	{
		Log.w("RSMTokenListener::onSuccess: ", result);
		Back(_native_backend, result);
	}

	@Override
	public void onFailure(@NonNull Throwable throwable)
	{
		Log.w("RSMTokenListener::onFailure: ", throwable.toString());
		Back(_native_backend, "");
	}
}
