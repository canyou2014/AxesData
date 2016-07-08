package com.cloudminds.smartrobot.fragment;

import android.hardware.SensorManager;
import android.os.Environment;

public class MyConstans {
	public static String MAC;
	public static String IMEI;


	public static String STATUS = "off";

	
	public static String ROOT = Environment.getExternalStorageDirectory().getPath() + "//";

    public static String DATA_PATH = Environment.getExternalStorageDirectory().getPath() + "/" + "com.123" +"/"+"com.123.sensor"
            + "/";
     public static String DATA_PATH0 = Environment.getExternalStorageDirectory().getPath() + "/" +  "com.123" +"/"+"com.123.time"
     + "/";


	public static int[] DELAY = { SensorManager.SENSOR_DELAY_GAME, SensorManager.SENSOR_DELAY_NORMAL,
			SensorManager.SENSOR_DELAY_UI };

	
	public static final int[] SAMPLING_RATE = { 1000, 2000, 3000 };
	public static final String RECORD_FILE_NAME = DATA_PATH0 + "record.txt";
	
	
}
