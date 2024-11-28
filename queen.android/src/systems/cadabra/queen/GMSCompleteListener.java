package systems.cadabra.queen;

import java.lang.String;

import android.util.Log;
import androidx.annotation.NonNull;

public class GMSCompleteListener implements com.google.android.gms.tasks.OnCompleteListener<String>
{
	private static native void Back(long native_back, String token);

	private final long _native_backend;

	public GMSCompleteListener(final long native_backend)
	{
		this._native_backend = native_backend;
	}

	@Override
	public void onComplete(@NonNull com.google.android.gms.tasks.Task<String> task)
	{
		if (!task.isSuccessful()) {
			Log.w("GMSCompleteListener::onComplete: ", task.getException().toString());
			Back(_native_backend, task.getException().toString());
		} else {
			Log.w("GMSCompleteListener::onComplete: ", task.getResult());
			Back(_native_backend, task.getResult());
		}
	}
}
