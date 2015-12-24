LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= chat-jni.c
LOCAL_MODULE:= chat-jni 
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
LOCAL_C_INCLUDES += system/core/include/cutils
LOCAL_SHARED_LIBRARIES := libcutil

include $(BUILD_SHARED_LIBRARY)
