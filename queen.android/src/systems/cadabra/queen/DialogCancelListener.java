package systems.cadabra.queen;

import android.content.DialogInterface;

public class DialogCancelListener implements DialogInterface.OnCancelListener
{
	private static native void Back(long native_back);

	private final long _native_backend;

	public DialogCancelListener(final long native_backend)
	{
		this._native_backend = native_backend;
	}

	@Override
	public void onCancel(DialogInterface dialog)
	{
		Back(_native_backend);
	}
}
