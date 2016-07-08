package com.cloudminds.smartrobot.fragment;
import com.cloudminds.axesdata.R;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;


public class DialogUitl {

	public static ProgressDialog createProgressDialog(Context context){
		ProgressDialog dialog = new ProgressDialog(context);
		dialog.setMessage(context.getString(R.string.saving_data));
		dialog.setCanceledOnTouchOutside(false);
		return dialog;
	}
	

	public static AlertDialog createAlertDialog(Context context, String content){
		AlertDialog dialog = new AlertDialog.Builder(context).setMessage(content)
			.setPositiveButton(context.getString(R.string.saving_data), null).create();
		return dialog;
	}
	

	public static AlertDialog createAlertDialog(Context context, String content, 
			DialogInterface.OnClickListener clickListener){
		AlertDialog dialog = new AlertDialog.Builder(context).setMessage(content)
			.setPositiveButton(context.getString(R.string.saving_data), clickListener).create();
		return dialog;
	}
}
