#include <string.h>
#include <jni.h>
#include <stdlib.h>
#include <fcntl.h>
#include <android/log.h>
#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <pthread.h>
#include  "libsensors.h"



jstring Java_com_cloudminds_smartrobot_fragment_HomeFragment_getAxisData(JNIEnv* env,
		jobject thiz, jobject versionObj) {

	struct axes_data axis;

	get_axes_data(&axis);

	jfieldID jfield;
	jclass objectClass = (*env)->FindClass(env,
			"com/cloudminds/smartrobot/bean/AxisData");
	jfield = (*env)->GetFieldID(env, objectClass, "seq", "I");
	(*env)->SetIntField(env, versionObj, jfield, axis.seq);

	jfield = (*env)->GetFieldID(env, objectClass, "acc_x", "F");
	(*env)->SetFloatField(env, versionObj, jfield, axis.acc_x);
	jfield = (*env)->GetFieldID(env, objectClass, "acc_y", "F");
	(*env)->SetFloatField(env, versionObj, jfield, axis.acc_y);
	jfield = (*env)->GetFieldID(env, objectClass, "acc_z", "F");
	(*env)->SetFloatField(env, versionObj, jfield, axis.acc_z);

	jfield = (*env)->GetFieldID(env, objectClass, "gyro_x", "F");
	(*env)->SetFloatField(env, versionObj, jfield, axis.gyro_x);
	jfield = (*env)->GetFieldID(env, objectClass, "gyro_y", "F");
	(*env)->SetFloatField(env, versionObj, jfield, axis.gyro_y);
	jfield = (*env)->GetFieldID(env, objectClass, "gyro_z", "F");
	(*env)->SetFloatField(env, versionObj, jfield, axis.gyro_z);

	jfield = (*env)->GetFieldID(env, objectClass, "comp_x", "F");
	(*env)->SetFloatField(env, versionObj, jfield, axis.comp_x);
	jfield = (*env)->GetFieldID(env, objectClass, "comp_y", "F");
	(*env)->SetFloatField(env, versionObj, jfield, axis.comp_y);
	jfield = (*env)->GetFieldID(env, objectClass, "comp_z", "F");
	(*env)->SetFloatField(env, versionObj, jfield, axis.comp_z);

	return (*env)->NewStringUTF(env, "getAxisData");
}

jstring
Java_com_cloudminds_smartrobot_fragment_HomeFragment_getUltrasonicData
  (JNIEnv* env, jobject thiz,jobject obj){

	struct ultrasonic_data ultra;
	get_ultrasonic_data(&ultra);

	jfieldID jfield;
	jclass objectClass = (*env)->FindClass(env,
			"com/cloudminds/smartrobot/bean/UltrasonicData");
	jfield = (*env)->GetFieldID(env, objectClass, "seq", "I");
	(*env)->SetIntField(env, obj, jfield, ultra.seq);

	jfield = (*env)->GetFieldID(env, objectClass, "distances", "[F");

	jfloatArray jarr1 = (*env)->NewFloatArray(env, 4);
	(*env)->SetFloatArrayRegion(env, jarr1, 0, 4, ultra.distance);

	(*env)->SetObjectField(env, obj, jfield, jarr1);

	 return (*env)->NewStringUTF(env, "getUltrasonicData");
}

jstring Java_com_cloudminds_smartrobot_fragment_HomeFragment_getIoData(JNIEnv* env,
		jobject thiz, jobject obj) {

	struct io_data iodata;
	get_io_data(&iodata);

	jfieldID jfield;
	jclass objectClass = (*env)->FindClass(env,
			"com/cloudminds/smartrobot/bean/IoData");
	jfield = (*env)->GetFieldID(env, objectClass, "seq", "I");
	(*env)->SetIntField(env, obj, jfield, iodata.seq);
	jfield = (*env)->GetFieldID(env, objectClass, "io_status", "I");
	(*env)->SetIntField(env, obj, jfield, iodata.io_status);
	return (*env)->NewStringUTF(env, "getUltrasonicData");
}

jstring Java_com_cloudminds_smartrobot_fragment_HomeFragment_getBatteryData(JNIEnv* env,
		jobject thiz, jobject obj) {

	struct battery_data battery;
	get_battery_data(&battery);

	jfieldID jfield;
	jclass objectClass = (*env)->FindClass(env,
			"com/cloudminds/smartrobot/bean/BatteryData");
	jfield = (*env)->GetFieldID(env, objectClass, "seq", "I");
	(*env)->SetIntField(env, obj, jfield, battery.seq);
	jfield = (*env)->GetFieldID(env, objectClass, "capacity", "F");
	(*env)->SetFloatField(env, obj, jfield, battery.capacity);
	jfield = (*env)->GetFieldID(env, objectClass, "residue_capacity", "F");
	(*env)->SetFloatField(env, obj, jfield, battery.residue_capacity);
	jfield = (*env)->GetFieldID(env, objectClass, "voltage", "F");
	(*env)->SetFloatField(env, obj, jfield, battery.voltage);
	jfield = (*env)->GetFieldID(env, objectClass, "current", "F");
	(*env)->SetFloatField(env, obj, jfield, battery.current);
	jfield = (*env)->GetFieldID(env, objectClass, "status", "I");
	(*env)->SetIntField(env, obj, jfield, battery.status);
	return (*env)->NewStringUTF(env, "getUltrasonicData");
}


jstring Java_com_cloudminds_smartrobot_fragment_HomeFragment_getversion(JNIEnv* env,
		jobject thiz, jobject versionObj) {

	struct version ver;
	get_version(&ver);

	jclass objectClass = (*env)->FindClass(env,
			"com/cloudminds/smartrobot/bean/Version");
	jfieldID imajor = (*env)->GetFieldID(env, objectClass, "major", "I");
	jfieldID iminor = (*env)->GetFieldID(env, objectClass, "minor", "I");
	(*env)->SetIntField(env, versionObj, imajor, ver.major);
	(*env)->SetIntField(env, versionObj, iminor, ver.minor);

	return (*env)->NewStringUTF(env, "getversion");
}






jstring Java_com_cloudminds_smartrobot_fragment_HomeFragment_sensorsInit(JNIEnv* env,
		jobject thiz) {
		set_axes_hz(99.0);
	    int ret = 0;
	    struct version ver;
	    ret = get_version(&ver);
	    struct axes_data naxes;
		ret = sensors_init("/dev/ttyACM0");
		set_axes_hz(99.0);

	return (*env)->NewStringUTF(env, "sensorsInit");
}



