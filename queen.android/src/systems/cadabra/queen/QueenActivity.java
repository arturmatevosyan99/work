package systems.cadabra.queen;

import java.util.Arrays;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileInputStream;
import java.io.BufferedInputStream;

import java.net.URLConnection;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Bundle;
import android.os.Build;
import android.os.ParcelFileDescriptor;
import android.os.Vibrator;
import android.os.VibrationEffect;
import android.util.Log;
import android.provider.Settings;
import android.provider.MediaStore;
import android.net.Uri;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.Context;
import android.content.BroadcastReceiver;
import android.content.ActivityNotFoundException;
import android.content.ContentResolver;
import android.content.DialogInterface;
import android.content.IntentSender.SendIntentException;
import android.content.pm.ActivityInfo;
import android.text.InputType;
import android.text.Html;
import android.view.WindowManager;
import android.view.Window;
import android.view.WindowInsets;
import android.view.View;
import android.view.DisplayCutout;
import android.widget.EditText;
import android.widget.TextView;
import android.graphics.Rect;

import androidx.localbroadcastmanager.content.LocalBroadcastManager;
import androidx.annotation.NonNull;
import androidx.core.content.FileProvider;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.fitness.Fitness;
import com.google.android.gms.fitness.FitnessOptions;
import com.google.android.gms.fitness.data.DataType;
import com.google.android.gms.fitness.data.Subscription;

import com.google.android.play.core.appupdate.AppUpdateInfo;
import com.google.android.play.core.appupdate.AppUpdateManager;
import com.google.android.play.core.appupdate.AppUpdateManagerFactory;
import com.google.android.play.core.install.model.AppUpdateType;
import com.google.android.play.core.install.model.UpdateAvailability;
import com.google.android.play.core.install.InstallState;
import com.google.android.play.core.install.model.InstallStatus;
import com.google.android.play.core.install.InstallStateUpdatedListener;

public class QueenActivity extends org.qtproject.qt5.android.bindings.QtActivity
{
	public static final int APP_UPDATE_REQUEST_CODE = 0x99999;
	public static final int FIT_PERMISSION_REQUEST_CODE = 0x10001;
	public static final int ALBUM_REQUEST_CODE = 0x20001;
	public static final int CAMERA_REQUEST_CODE = 0x20002;

	public static native void ThrowException(String message);
	public static native void UpdateApplication(boolean ready);
	public static native void AccessGoogleFit(boolean success);
	public static native void PickAlbumItem(String path);
	public static native void PickAlbumDescriptor(int handle);
	public static native void ResolvePayment(String acquiring, int id, boolean success, String message);
	public static native void AlertInput(String text, boolean success, String ident);
	public static native void AlertAction(int index, String ident);

	private AppUpdateManager _appupdate_manager;
	private InstallStateUpdatedListener _appupdate_listener;

	/**
	 * @todo Migrate to C++
	 */
	FitnessOptions _fitness_options = FitnessOptions.builder()
		.addDataType(DataType.TYPE_STEP_COUNT_DELTA) /// < Number of new steps since the last reading. Activity steps (int—count)
		//.addDataType(DataType.TYPE_BASAL_METABOLIC_RATE) /// < In the com.google.calories.bmr data type, each data point represents the basal metabolic rate of energy expenditure at rest of the user at the time of the reading, in kcal per day.
		//.addDataType(DataType.TYPE_BODY_FAT_PERCENTAGE) /// < In the com.google.body.fat.percentage data type, each data point represents a measurement of the total fat mass in a person's body as a percentage of the total body mass.
		.addDataType(DataType.TYPE_CALORIES_EXPENDED) /// < Total calories expended over a time interval.	Activity	calories (float—kcal)
		.addDataType(DataType.TYPE_CYCLING_PEDALING_CADENCE) /// < Instantaneous pedaling rate in crank revolutions per minute.	Activity	rpm (float—rpm)
		//.addDataType(DataType.TYPE_CYCLING_PEDALING_CUMULATIVE) /// < In the com.google.cycling.pedaling.cadence data type, each data point represents an instantaneous measurement of the pedaling rate in crank revolutions per minute.
		//.addDataType(DataType.TYPE_CYCLING_WHEEL_REVOLUTION) /// < In the com.google.cycling.wheel_revolution.cumulative data type, each data point represents the number of revolutions taken from the start of the count.
		///.addDataType(DataType.TYPE_CYCLING_WHEEL_RPM) /// < Instantaneous wheel speed.	Location	rpm (float—rpm)
		//.addDataType(DataType.TYPE_DISTANCE_DELTA) /// < Distance covered since the last reading.	Location	distance (float—meters)
		//.addDataType(DataType.TYPE_HEART_POINTS) /// < In the com.google.heart_minutes data type, each data point represents derived heart points.
		///.addDataType(DataType.TYPE_HEART_RATE_BPM) /// < Heart rate in beats per minute.	Body	bpm (float—bpm)
		//.addDataType(DataType.TYPE_HEIGHT) /// < In the com.google.height data type, each data point represents the height of the user at the time of the reading, in meters.
		//.addDataType(DataType.TYPE_HYDRATION) /// < In the com.google.hydration data type, the field_volume in the data point represents the volume, in liters, of water consumed by a user as part of a single drink.

		/// @note Whilst any application can write data to a public data type, there are certain data types which can only be read by the application that wrote the data.
		//.addDataType(DataType.TYPE_LOCATION_SAMPLE) /// <com.google.location.sample	The user's current location.	latitude (float—degrees) longitude (float—degrees) accuracy (float—meters) altitude (float—meters)
		//.addDataType(DataType.AGGREGATE_LOCATION_BOUNDING_BOX) /// < com.google.location.bounding_box	A bounding box for the user's location over a time interval.	low_latitude (float—degrees) low_longitude (float—degrees) high_latitude (float—degrees) high_longitude (float—degrees)

		//.addDataType(DataType.TYPE_LOCATION_TRACK) /// < The com.google.location.track data type represents a location point that is part of a track and which may have inexact timestamps.
		//.addDataType(DataType.TYPE_MOVE_MINUTES) /// < In the com.google.active_minutes data type, each data point represents derived active minutes.
		///.addDataType(DataType.TYPE_NUTRITION) /// < com.google.nutrition	Food item information	Nutrition	nutrients (Map<String, float>—calories/grams/IU) meal_type (int—enum) food_item (String—n/a)
		.addDataType(DataType.TYPE_POWER_SAMPLE) /// < Instantaneous power generated while performing an activity.	Activity	watts (float—watts)
		///.addDataType(DataType.TYPE_SPEED) /// < com.google.speed	Instantaneous speed over ground.	Location	speed (float—m/s)
		.addDataType(DataType.TYPE_STEP_COUNT_CADENCE) /// < Instantaneous cadence in steps per minute.	Activity	rpm (float—steps/min)
		///.addDataType(DataType.TYPE_WEIGHT) /// < com.google.weight	The user's weight.	Body	weight (float—kg)
		.addDataType(DataType.TYPE_WORKOUT_EXERCISE) /// < com.google.activity.exercise	A user's continuous workout routine.	Activity	exercise (int—enum) repetitions (int—count) resistance type (int—enum) resistance (float—kg) duration (int—milliseconds)
		.addDataType(DataType.AGGREGATE_ACTIVITY_SUMMARY) /// < com.google.activity.summary	Total time and number of segments in a particular activity for a time interval.	Activity	activity (int—enum) duration (int—ms) num_segments (int—cou
		//.addDataType(DataType.AGGREGATE_BASAL_METABOLIC_RATE_SUMMARY) /// < In the com.google.bmr.summary data type, each data point represents the average, maximum and minimum basal metabolic rate, in kcal per day, over the time interval of the data point.
		.addDataType(DataType.AGGREGATE_BODY_FAT_PERCENTAGE_SUMMARY) /// < In the com.google.body.fat_percentage.summary data type, each data point represents the average, maximum and minimum percentage over the time interval of the data point.
		//.addDataType(DataType.AGGREGATE_CALORIES_EXPENDED) /// < Aggregate calories expended, in kcal, during a time interval.
		//.addDataType(DataType.AGGREGATE_DISTANCE_DELTA) /// < Aggregate distance, in meters, during a time interval.
		//.addDataType(DataType.AGGREGATE_HEART_POINTS) /// < Aggregates of heart points, including the sum of intensity.
		///.addDataType(DataType.AGGREGATE_HEART_RATE_SUMMARY) /// < com.google.heart_rate.summary	Average, maximum, and minimum beats per minute for a time interval.	Body	average (float—bpm) max (float—bpm) min (float—bpm)
		//.addDataType(DataType.AGGREGATE_HEIGHT_SUMMARY) /// < In the com.google.height.summary data type, each data point represents the average, maximum and minimum height, in meters, over the time interval of the data point.
		//.addDataType(DataType.AGGREGATE_HYDRATION) /// < In the com.google.hydration data type, the field_volume in the data point represents the volume, in liters, of water consumed by a user as part of a single drink.
		//.addDataType(DataType.AGGREGATE_MOVE_MINUTES) /// < Aggregate number of move minutes during a time interval.
		///.addDataType(DataType.AGGREGATE_NUTRITION_SUMMARY) /// < com.google.nutrition.summary	User's nutrition intake during a time interval.	Nutrition	nutrients (Map<String, float>—calories/grams/IU) meal_type (int—enum) food_item (String—n/a)
		.addDataType(DataType.AGGREGATE_POWER_SUMMARY) /// < com.google.power.summary	Average, maximum, and minimum power generated while performing an activity.	Activity	average (float—watts) max (float—watts) min (float—watts)
		///.addDataType(DataType.AGGREGATE_SPEED_SUMMARY) /// < com.google.speed.summary	Average, maximum, and minimum speed over ground over a time interval.	Location	average (float—m/s) max (float—m/s) min (float—m/s)
		//.addDataType(DataType.AGGREGATE_STEP_COUNT_DELTA) /// < Aggregate number of steps during a time interval.
		///.addDataType(DataType.AGGREGATE_WEIGHT_SUMMARY) /// < com.google.weight.summary	Average, maximum, and minimum weight over a time interval.	Body	average (float—kg) max (float—kg) min (float—kg)
	.build();

	@Override
	public void onCreate(Bundle state)
	{
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
			getWindow().getAttributes().layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_ALWAYS;
		}
		super.onCreate(state);
	}

	@Override
	protected void onDestroy()
	{
		stopAppliactionUpdate();
		super.onDestroy();
	}

	@Override
	protected void onStart()
	{
		super.onStart();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		probeApplicationUpdate();
	}

	@Override
	protected void onStop()
	{
		super.onStop();
	}

	/**
	 * @todo Move to C++ with QAndroidActivityResultReceiver
	 */
	@Override
	protected void onActivityResult(int request_code, int result_code, Intent data)
	{
		Log.w("QueenActivity::onActivityResult", Integer.toString(request_code) + ":" + Integer.toString(result_code));
		if (result_code == Activity.RESULT_OK) {
			if (request_code == QueenActivity.FIT_PERMISSION_REQUEST_CODE) {
				Log.w("QueenActivity::onActivityResult", "Access to Google Fit is granted");
				QueenActivity.AccessGoogleFit(true);
			} else if (request_code == QueenActivity.ALBUM_REQUEST_CODE) {
				Uri image_uri =  data.getData();
				if (image_uri != null) {
					try {
						ParcelFileDescriptor file_descriptor = getContentResolver().openFileDescriptor(image_uri, "r");
						QueenActivity.PickAlbumDescriptor(file_descriptor.detachFd());
					} catch (FileNotFoundException e) {
						Log.w("QueenActivity::onActivityResult", e.toString());
						QueenActivity.ThrowException(e.toString());
					}
				}
			} else if (request_code == QueenActivity.APP_UPDATE_REQUEST_CODE) {
				Log.w("QueenActivity::onActivityResult", "Update is installed");
			}
		} else if (request_code == QueenActivity.APP_UPDATE_REQUEST_CODE) {
			Log.w("QueenActivity::onActivityResult", "Update is failed");
			stopAppliactionUpdate();
			QueenActivity.ThrowException("Update is failed");
		} else if (data != null) {
			Bundle bundle = data.getExtras();
			if (bundle != null) {
				Log.w("QueenActivity::onActivityResult", "Dump of the Intent extras");
				for (String key : bundle.keySet()) {
					Log.e("QueenActivity::onActivityResult", key + " : " + (bundle.get(key) != null ? bundle.get(key) : "NULL"));
				}
			}
		}
	}

	/**
	 * @todo Migrate to C++
	 */
	public String generateId()
	{
		return Settings.Secure.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
	}

	/**
	 * @todo Migrate to C++
	 */
	public void keepScreen(boolean onoff)
	{
		if (onoff) {
			getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		} else {
			getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		}
	}

	/**
	 * @todo Migrate to C++
	 */
	public void orientScreen(int orientation)
	{
		setRequestedOrientation(orientation);
	}

	/**
	 * @todo Migrate to C++
	 */
	public void checkApplicationUpdate()
	{
		if (probeApplicationUpdate()) {
			return ;
		}
		_appupdate_manager = AppUpdateManagerFactory.create(this.getApplicationContext());
		com.google.android.play.core.tasks.Task<AppUpdateInfo> task = _appupdate_manager.getAppUpdateInfo();
		task.addOnSuccessListener
		(
			new com.google.android.play.core.tasks.OnSuccessListener<AppUpdateInfo>()
			{
				@Override
				public void onSuccess(AppUpdateInfo info)
				{
					if (info.updateAvailability() == UpdateAvailability.UPDATE_AVAILABLE) {
						Log.w("QueenActivity::checkApplicationUpdate", "Update is available");
						QueenActivity.UpdateApplication(true);
					} else {
						Log.w("QueenActivity::checkApplicationUpdate", "Update is not available");
						QueenActivity.UpdateApplication(false);
					}
					stopAppliactionUpdate();
				}
			}
		);
		task.addOnFailureListener
		(
			new com.google.android.play.core.tasks.OnFailureListener()
			{
				@Override
				public void onFailure (Exception e)
				{
					Log.w("QueenActivity::checkApplicationUpdate",  e.toString());
					QueenActivity.UpdateApplication(false);
					stopAppliactionUpdate();
					QueenActivity.ThrowException(e.toString());
				}
			}
		);
	}

	/**
	 * @todo Migrate to C++
	 */
	public void runApplicationUpdate()
	{
		if (probeApplicationUpdate()) {
			return ;
		}
		_appupdate_manager = AppUpdateManagerFactory.create(this.getApplicationContext());
		_appupdate_manager.getAppUpdateInfo()
		.addOnSuccessListener
		(
			new com.google.android.play.core.tasks.OnSuccessListener<AppUpdateInfo>()
			{
				@Override
				public void onSuccess(AppUpdateInfo info)
				{
					if (info.isUpdateTypeAllowed(AppUpdateType.IMMEDIATE)) {
						Log.w("QueenActivity::runApplicationUpdate", "Immediate update is allowed");
						try {
							_appupdate_manager.startUpdateFlowForResult(info, AppUpdateType.IMMEDIATE, QueenActivity.this, APP_UPDATE_REQUEST_CODE);
						} catch (SendIntentException e) {
							Log.w("QueenActivity::runApplicationUpdate", e.toString());
							stopAppliactionUpdate();
						}
					} else if (info.isUpdateTypeAllowed(AppUpdateType.FLEXIBLE)) {
						Log.w("QueenActivity::runApplicationUpdate", "Flexible update is allowed");
						try {
							_appupdate_listener = new InstallStateUpdatedListener()
							{
								@Override
								public void onStateUpdate(InstallState install_state) {
									/// @brief Show module progress, log state, or install the update
									if (install_state.installStatus() == InstallStatus.DOWNLOADED) {
										/// @brief After the update is downloaded, show a notification and request user confirmation to restart the app.
										stopAppliactionUpdate();
									}
								}
							};
							_appupdate_manager.registerListener(_appupdate_listener);
							_appupdate_manager.startUpdateFlowForResult(info, AppUpdateType.FLEXIBLE, QueenActivity.this, APP_UPDATE_REQUEST_CODE);
						} catch (SendIntentException e) {
							Log.w("QueenActivity::runApplicationUpdate", e.toString());
							stopAppliactionUpdate();
						}
					} else {
						Log.w("QueenActivity::runApplicationUpdate", "Update is not allowed");
						stopAppliactionUpdate();
					}
				}
			}
		);
	}

	/**
	 * @todo Migrate to C++
	 */
	private boolean probeApplicationUpdate()
	{
		if (_appupdate_manager == null) {
			return false;
		}
		_appupdate_manager.getAppUpdateInfo()
		.addOnSuccessListener
		(
			new com.google.android.play.core.tasks.OnSuccessListener<AppUpdateInfo>()
			{
				@Override
				public void onSuccess(AppUpdateInfo info)
				{
					/// @note Flexible
					/// @brief If the update is downloaded but not installed, notify the user to complete the update.
					if (info.installStatus() == InstallStatus.DOWNLOADED) {
						 /// @todo Displays the snackbar notification and call to action - needed only for Flexible app update
	/*
						Snackbar snackbar = Snackbar.make(drawer_layout, getString(R.string.update_downloaded), Snackbar.LENGTH_INDEFINITE);
						snackbar.setAction(
											R.string.restart,
											new View.OnClickListener()
											{
												@Override
												public void onClick(View view) {
													_appupdate_manager.completeUpdate();
												}
											}
						);
						snackbar.setActionTextColor(getResources().getColor(R.color.action_color));
						snackbar.show();
	*/
						stopAppliactionUpdate();
					}
					/// @note Immediate
					if (info.updateAvailability() == UpdateAvailability.DEVELOPER_TRIGGERED_UPDATE_IN_PROGRESS) {
						/// @brief If an in-app update is already running, resume the update
//						startAppUpdateImmediate(appUpdateInfo);
					}
				}
			}
		);
		return true;
	}

	/**
	 * @todo Migrate to C++
	 */
	private void stopAppliactionUpdate()
	{
		if (_appupdate_manager != null) {
			if (_appupdate_listener != null) {
				_appupdate_manager.unregisterListener(_appupdate_listener);
			}
			_appupdate_manager = null;
		}
	}

	/**
	 * @todo Migrate to C++
	 */
	public void requestFitAccess()
	{
		if (!hasFitAccess()) {
			Log.w("QueenActivity::requestFitAccess", "Requesting...");
			GoogleSignIn.requestPermissions(this, QueenActivity.FIT_PERMISSION_REQUEST_CODE, GoogleSignIn.getLastSignedInAccount(this), _fitness_options);
		} else {
			Log.w("QueenActivity::requestFitAccess", "Done");
			QueenActivity.AccessGoogleFit(true);
		}
	}

	/**
	 * @todo Migrate to C++
	 */
	public Intent createAlbumIntent()
	{
		Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
		intent.setType("image/*");
		return Intent.createChooser(intent, "Select Image");
	}

	/**
	 * @todo Migrate to C++
	 */
	public void openAlbum()
	{
		Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
		intent.setType("image/*");
		intent.setAction(Intent.ACTION_GET_CONTENT);
		startActivityForResult(intent, ALBUM_REQUEST_CODE);
	}

	/**
	 * @todo Migrate to C++
	 */
	private void openCamera()
	{
		Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		if (intent.resolveActivity(getPackageManager()) != null) {
			startActivityForResult(intent, CAMERA_REQUEST_CODE);
		}
	}

	/**
	 * @todo Migrate to C++
	 */
	public boolean openFile(String url)
	{
		Log.w("QueenActivity::openFile", url);

		File file = new File(url);
		Intent target = new Intent(Intent.ACTION_VIEW);
//		target.setType(URLConnection.guessContentTypeFromName(file.getName()));
		target.setData(FileProvider.getUriForFile(this, getApplicationContext().getPackageName() + ".provider", file));
		target.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
//		target.setFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);

		Intent intent = Intent.createChooser(target, "Open File");
		try {
			startActivity(intent);
		} catch (ActivityNotFoundException e) {
			Log.w("QueenActivity::openFile", e.toString());
			QueenActivity.ThrowException(e.toString());
			return false;
		}
		return true;
	}

	/**
	 * @todo Migrate to C++
	 */
	public boolean shareFile(String url) throws Exception
	{
		Log.w("QueenActivity::shareFile", url);

		File file = new File(url);
		Intent target = new Intent(Intent.ACTION_VIEW);
		target.setType(URLConnection.guessContentTypeFromStream(new BufferedInputStream(new FileInputStream(file))));
		target.setData(FileProvider.getUriForFile(this, getApplicationContext().getPackageName() + ".provider", file));
		target.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
//		target.setFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);

		Intent intent = Intent.createChooser(target, "Share File");
		try {
			startActivity(intent);
		} catch (ActivityNotFoundException e) {
			Log.w("QueenActivity::shareFile", e.toString());
			QueenActivity.ThrowException(e.toString());
			return false;
		}
		return true;
	}

	/**
	 * @todo Migrate to C++
	 */
	public void vibrate()
	{
		Vibrator v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
			v.vibrate(VibrationEffect.createOneShot(250, VibrationEffect.DEFAULT_AMPLITUDE));
		} else {
			v.vibrate(500);
		}
	}

	/**
	 * @obsolete
	 */
	public void alertInput(String title, String placeholder, final String ident)
	{
		final EditText input = new EditText(this);
		input.setInputType(InputType.TYPE_CLASS_TEXT);
		input.setHint(placeholder);

		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setTitle(title);
		builder.setView(input);
		builder.setPositiveButton(
									"Ok",
									new DialogInterface.OnClickListener()
									{
										@Override
										public void onClick(DialogInterface dialog, int which)
										{
											QueenActivity.AlertInput(input.getText().toString(), true, ident);
										}
									}
		);
		builder.setNegativeButton(
									"Cancel",
									new DialogInterface.OnClickListener()
									{
										@Override
										public void onClick(DialogInterface dialog, int which)
										{
											QueenActivity.AlertInput(input.getText().toString(), false, ident);
										}
									}
		);
		builder.show();
	}

	/**
	 * @obsolete
	 */
	public void alertAction(String title, String message, String[] actionArray, boolean need_cancel, final String ident)
	{
		TextView message_view = new TextView(this);
		message_view.setPadding(15, 15, 15, 0);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
			message_view.setText(Html.fromHtml("<h2>" + title + "</h2><br><p>" + message +"</p>", Html.FROM_HTML_MODE_COMPACT));
		} else { 
			message_view.setText(Html.fromHtml("<h2>" + title + "</h2><br><p>" + message +"</p>"));
		}

		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setCustomTitle(message_view);
		builder.setItems(
							actionArray,
							new DialogInterface.OnClickListener()
							{
								@Override
								public void onClick(DialogInterface dialog, int which)
								{
									QueenActivity.AlertAction(which, ident);
								}
							}
		);
		if (need_cancel) {
			builder.setNegativeButton(
										"Cancel",
										new DialogInterface.OnClickListener()
										{
											@Override
											public void onClick(DialogInterface dialog, int id)
											{
												QueenActivity.AlertAction(-1, ident);
												dialog.dismiss();
											}
										}
			);
		}
		builder.show();
	}

	public boolean hasFitAccess()
	{
		return GoogleSignIn.hasPermissions(GoogleSignIn.getLastSignedInAccount(this), _fitness_options);
	}
}
