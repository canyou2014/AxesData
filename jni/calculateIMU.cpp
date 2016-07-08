#include <string.h>
#include <jni.h>
#include <stdlib.h>
#include <fcntl.h>
#include <android/log.h>
#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <pthread.h>
#include  "imutransformation.h"


extern "C" {
JNIEXPORT void JNICALL Java_com_cloudminds_smartrobot_fragment_HomeFragment_calculateinit(JNIEnv* env,
		jobject thiz, jdoubleArray myarray) {
	double latitude = myarray[0];
	double attitude = myarray[1];
	double t = myarray[2];

	imutransformation myimu(latitude, attitude, t);

	return (*env)->NewStringUTF(env, "calculateinit");

}


}



