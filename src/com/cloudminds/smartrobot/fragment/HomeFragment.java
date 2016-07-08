package com.cloudminds.smartrobot.fragment;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.highgui.Highgui;

import com.cloudminds.axesdata.R;
import com.cloudminds.smartrobot.bean.AxisData;

import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.hardware.Camera.Size;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SubMenu;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.widget.Toast;

public class HomeFragment extends Activity  implements CvCameraViewListener2{

	
	 public native String getAxisData(AxisData axisData);
	 public native String setAxisHz();
	 public native String sensorsInit();
	 public native String calculateinit(double[] d);
		
		
		static {
//			System.loadLibrary("senser");
			System.loadLibrary("calculateIMU");
	    }
	
	
    private static final String TAG = "OCVSample::Activity";
    private Timer timer;
    private long startTime;
    private int sitime;
    private Context context;
    private boolean flag;
    private StringBuilder sb,sb1;
    private GetDataTask getDataTask;
    private Tutorial3View mOpenCvCameraView;
    private List<Size> mResolutionList;
    private MenuItem[] mEffectMenuItems;
    private SubMenu mColorEffectsMenu;
    private MenuItem[] mResolutionMenuItems;
    private MenuItem[] mFPSMenuItems, mStopMenuItems;
    private SubMenu mResolutionMenu;
    private SubMenu mFPSMenu, mStopMenu;
    private List<Integer> mFrameRate;
    private Mat mRgba;
    private boolean startState = false;
    private Vector<Mat> mRgbaList;
    private Vector<String> mTime;
    private Mat[] aRgba1, aRgba2;
    private long[] mTime1, mTime2;
    private int sized;
    private int countd,count;
    private int[] doo = new int[6];
    int[] fpsd;
    private int maxx = 0, minn = 10000;
    private  AxisData axisData;
    private String strCaptureFilePath = Environment.getExternalStorageDirectory() + "/dataset/";
    private String nowtime,  currentTime;
    List<int[]> dFrameRate = new ArrayList<int[]>();   
    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    mOpenCvCameraView.enableView();
                    //mOpenCvCameraView.setOnTouchListener(Tutorial3Activity.this);
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };
    private final static int STOP_SAMPLING = 0, START_SAMPLING = 1, Save_camera = 2;
    private Handler handler = new Handler() {
        public void handleMessage(android.os.Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case STOP_SAMPLING:
                    
                    stopSensor();

                    break;
                case START_SAMPLING:
                    
                    startSensor();

                    break;
                case Save_camera:
                	new SaveDataTask1().execute();

            }
    }};
    
//    
   

    public HomeFragment() {
        Log.i(TAG, "Instantiated new " + this.getClass());
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.tutorial3_surface_view);
     
        mOpenCvCameraView = (Tutorial3View) findViewById(R.id.tutorial3_activity_java_surface_view);
        mOpenCvCameraView.enableFpsMeter();
        mOpenCvCameraView.setMaxFrameSize(640, 480);
        mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);

        mOpenCvCameraView.setCvCameraViewListener(this);
        
        
        File file = new File(strCaptureFilePath + "Camera/");
        if (!file.exists()) {
            file.mkdirs();
        }
        File file1 = new File(strCaptureFilePath + "Sensor/");
        if (!file1.exists()) {
            file1.mkdirs();
        }

        context = this;
        sb = new StringBuilder();
        sb1 = new StringBuilder();
        axisData  = new AxisData();
		sensorsInit();

        
    }
    public void startSensor(){
    	 flag = true;
    	 mRgbaList = new Vector<Mat>();
    	 mTime = new Vector<String>();
//    	 aRgba1 = new Mat[200];
//    	 aRgba2 = new Mat[200];
//    	 mTime1 = new long[200];
//    	 mTime2 = new long[200];
//    	 countd = 0;
    	 sb1.append("#timestamp [ns],filename" + "\n");
    	 sb.append("#timestamp [ns],w_RS_S_x [rad s^-1],w_RS_S_y [rad s^-1],w_RS_S_z [rad s^-1],a_RS_S_x [m s^-2],a_RS_S_y [m s^-2],a_RS_S_z [m s^-2]" + "\n");
    	 sitime = 5;
    	 timer = new Timer();
         getDataTask = new GetDataTask();
         startTime = System.currentTimeMillis();
         Toast.makeText(context, context.getString(R.string.reading), Toast.LENGTH_SHORT).show();
         timer.scheduleAtFixedRate(getDataTask, 0, sitime);//
     }

    public void stopSensor() {
    	flag = false;
    	if (timer != null) {
            timer.cancel();
        }
        if (getDataTask != null) {
            getDataTask.cancel();  
        }
        new SaveDataTask().execute();
        new SaveDataTask1().execute();
        

    }
    private class GetDataTask extends TimerTask {             //

        @Override
        public void run() {
            currentTime = Long.toString(System.currentTimeMillis());
            getAxisData(axisData);
            sb.append(axisData.toString() + "\n");
//            sb.append(currentTime +"," + "123" + "\n");

         
        }

    }
	
		
	
    @Override
    public void onPause()
    {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this, mLoaderCallback);
    }

    public void onDestroy() {
        super.onDestroy();
        //handler.sendEmptyMessage(STOP_SAMPLING);
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    public void onCameraViewStarted(int width, int height) {
//    	mRgba = new Mat(height, width, CvType.CV_8UC4);
    }

    public void onCameraViewStopped() {
    	 mRgba.release();
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
    	mRgba = inputFrame.gray();
        
        if(flag == true)
        {	
        	nowtime = Long.toString(System.currentTimeMillis() * 1000000);
        	sb1.append(nowtime + "," + nowtime + ".jpg" + "\n");
//        	Highgui.imwrite(strCaptureFilePath +"Camera/"+ nowtime +".jpg", mRgba); 
        	mRgbaList.add(mRgba.clone());
            mTime.add(nowtime);

//        }
//        else if (flag == false && mTime.size() > 0)
//        {
//        	sized = mRgbaList.size();
//        	for(int i = 0;i < sized; i++ )
//        	{
//        		Highgui.imwrite(strCaptureFilePath +"Camera/"+ mTime.get(i) +".jpg", mRgbaList.get(i));  
//        	}
//        	
        }
        return mRgba;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        mResolutionMenu = menu.addSubMenu("Resolution");
        mResolutionList = mOpenCvCameraView.getResolutionList();
        mResolutionMenuItems = new MenuItem[mResolutionList.size()];

        ListIterator<Size> resolutionItr = mResolutionList.listIterator();
        int idx = 0;
        while(resolutionItr.hasNext()) {
            Size element = resolutionItr.next();
            mResolutionMenuItems[idx] = mResolutionMenu.add(1, idx, Menu.NONE,
                    Integer.valueOf(element.width).toString() + "x" + Integer.valueOf(element.height).toString());
            idx++;
         }
     
        mFPSMenu = menu.addSubMenu("fps");
         
        dFrameRate = mOpenCvCameraView.getFramRate();
        
        
        fpsd = dFrameRate.get(0);
        mFPSMenuItems = new MenuItem[6];
        int d = fpsd[0];
        int delta = (fpsd[1] - fpsd[0])/5;
        idx = 0;
        while( idx < 6 )
        {	
        	doo[idx] = d;
        	mFPSMenuItems[idx] = mFPSMenu.add(2, idx, Menu.NONE,
                    Integer.valueOf(doo[idx]).toString());
        	idx ++;
        	d += delta;
        }
        
        //mFPSMenuItems[0] = mFPSMenu.add(2, 0, Menu.NONE, Integer.valueOf(dFrameRate.get(0)[0]));
        mStopMenu = menu.addSubMenu("action");
        mStopMenuItems = new MenuItem[2];
        mStopMenuItems[0] = mStopMenu.add(3, 0, Menu.NONE,
                "Start");
        mStopMenuItems[1] = mStopMenu.add(3, 1, Menu.NONE,
                "Stop");
        return true;
    }

    public boolean onOptionsItemSelected(MenuItem item) {
        Log.i(TAG, "called onOptionsItemSelected; selected item: " + item);
        if (item.getGroupId() == 1)
        {
 
            int id = item.getItemId();
            Size resolution = mResolutionList.get(id);
            mOpenCvCameraView.setResolution(resolution);
            resolution = mOpenCvCameraView.getResolution();
            String caption = Integer.valueOf(resolution.width).toString() + "x" + Integer.valueOf(resolution.height).toString();
            Toast.makeText(this, caption, Toast.LENGTH_SHORT).show();
        }
        else if(item.getGroupId() == 2){
        	int id = item.getItemId();
        	int f = doo[id];
        	
        	mOpenCvCameraView.setFrameRate(f, f);
        }
        else if(item.getGroupId() == 3)
        	{int id = item.getItemId();
        	if(id == 0)
        	{
        		flag = true;
        		startState = true;
//        		startSensor();
        		handler.sendEmptyMessage(START_SAMPLING);
        		
        	}
        	else
        	{	
//        		if(startState == true)
//        		{
        		flag = false;
        		startState = false;
//        		stopSensor();
        		handler.sendEmptyMessage(STOP_SAMPLING);
        			
//        			
//        		}
//        		else{
//        			Toast.makeText(context, context.getString(R.string.please_start), Toast.LENGTH_SHORT).show();
//        		
//        			}
        	}
        	}
        
        return true;
    }
    private class SaveDataTask extends AsyncTask<Void, Void, Void> {
        ProgressDialog dialog;
        boolean saveSuccess = false;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            dialog = DialogUitl.createProgressDialog(context);
            dialog.show();
        }

        @Override
        protected Void doInBackground(Void... params) {

            String content = sb.toString();

            saveSuccess = FileUtil.savaZip(strCaptureFilePath + "Sensor/" + (System.currentTimeMillis())
            		+ "_sensor" + ".zip", content);
            sb.setLength(0);
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            dialog.dismiss();
            if (saveSuccess) {
                Toast.makeText(context, context.getString(R.string.saving_sensor_success), Toast.LENGTH_SHORT).show();//显示数据保存成功
            } else {
                Toast.makeText(context, context.getString(R.string.saving_sensor_failed), Toast.LENGTH_SHORT).show();
            }
        }

    }
   
    private class SaveDataTask1 extends AsyncTask<Void, Void, Void> {
        ProgressDialog dialog1;
        boolean saveSuccess1 = false;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            dialog1 = DialogUitl.createProgressDialog(context);
            dialog1.show();
        }

        @Override
        protected Void doInBackground(Void... params) {

        	sized = mRgbaList.size();

        	for(int i = 0;i < sized; i++ )
        	{
        		Highgui.imwrite(strCaptureFilePath +"Camera/"+ mTime.get(i) +".jpg", mRgbaList.get(i));  
        	}
        	String content1 = sb1.toString();

            FileUtil.savaZip(strCaptureFilePath + "Camera/" + (System.currentTimeMillis())
                        + "_camera" + ".zip", content1);
            
            sb1.setLength(0);
        	saveSuccess1 = true;
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            dialog1.dismiss();
            if (saveSuccess1) {
                Toast.makeText(context, context.getString(R.string.saving_camera_success), Toast.LENGTH_SHORT).show();//显示数据保存成功
            } else {
                Toast.makeText(context, context.getString(R.string.saving_camera_failed), Toast.LENGTH_SHORT).show();
            }
        }

    }
   
}
