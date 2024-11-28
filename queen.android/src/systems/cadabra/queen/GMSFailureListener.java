package systems.cadabra.queen;

import java.lang.String;

import android.util.Log;

public class GMSFailureListener implements com.google.android.gms.tasks.OnFailureListener
{
	private static native void Back(long native_back, String message);

	private final long _native_backend;

	public GMSFailureListener(final long native_backend)
	{
		this._native_backend = native_backend;
	}

	@Override
	public void onFailure(Exception e)
	{
		Log.w("GMSFailureListener::onFailure: ", e.toString());
		Back(_native_backend, e.toString());
	}
}
