package systems.cadabra.queen;

import java.lang.String;
import java.lang.Throwable;

import android.util.Log;

import androidx.annotation.NonNull;

import ru.rustore.sdk.core.feature.model.FeatureAvailabilityResult;

public class RSMAvailabilityListener implements ru.rustore.sdk.core.tasks.OnCompleteListener<FeatureAvailabilityResult>
{
	private static native void Back(long native_back, boolean success);

	private final long _native_backend;

	public RSMAvailabilityListener(final long native_backend)
	{
		this._native_backend = native_backend;
	}

	@Override
	public void onSuccess(FeatureAvailabilityResult result)
	{
		if (result instanceof FeatureAvailabilityResult.Available) {
			Log.d("RSMAvailabilityListener::onSuccess: ", "Available");
			Back(_native_backend, true);
		} else {
			Log.w("RSMAvailabilityListener::onSuccess: ", "Unavailable");
			Back(_native_backend, false);
		}
	}

	@Override
	public void onFailure(@NonNull Throwable throwable)
	{
		Log.w("RSMAvailabilityListener::onFailure: ", throwable.toString());
		Back(_native_backend, false);
	}
}
