LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := sensors
LOCAL_SRC_FILES := libsensors.so
#LOCAL_SRC_FILES := ../../sojni/libs/armeabi/libinit.so
include $(PREBUILT_SHARED_LIBRARY)
#
include $(CLEAR_VARS)
LOCAL_MODULE := senser
#LOCAL_SHARED_LIBRARY := my-init    
LOCAL_SHARED_LIBRARIES := sensors
LOCAL_SRC_FILES := senser.c
include $(BUILD_SHARED_LIBRARY)


#include $(CLEAR_VARS)
#LOCAL_MODULE := imutransformation
#LOCAL_SRC_FILES := imutransformation.so
##LOCAL_SRC_FILES := ../../sojni/libs/armeabi/libinit.so
#include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := calculateIMU
#LOCAL_SHARED_LIBRARY := my-init    
#LOCAL_SHARED_LIBRARIES := imutransformation
LOCAL_SRC_FILES := calculateIMU.cpp
LOCAL_SRC_FILES += imutransformation.cpp
include $(BUILD_SHARED_LIBRARY)