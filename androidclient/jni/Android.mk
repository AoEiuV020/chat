LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= chat-jni.c
LOCAL_MODULE:= chat-jni 

include $(BUILD_SHARED_LIBRARY)
