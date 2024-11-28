package systems.cadabra.queen;

import android.content.DialogInterface;

public class DialogClickListener implements DialogInterface.OnClickListener
{
	private static native void Back(long native_back, int item_index);

	private final long _native_backend;

	public DialogClickListener(final long native_backend)
	{
		this._native_backend = native_backend;
	}

	@Override
	public void onClick(DialogInterface dialog, int which)
	{
		Back(_native_backend, which);
	}
}
