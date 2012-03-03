LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_C_INCLUDES += \
	external/dbus \
	system/bluetooth/bluez-clean-headers
LOCAL_CFLAGS += -DHAVE_BLUETOOTH
LOCAL_SRC_FILES:= raw.cpp
LOCAL_SHARED_LIBRARIES := libcutils liblog libbluetooth libdbus libbluedroid
LOCAL_MODULE:= bt-test
LOCAL_MODULE_PATH := $(TARGET_OUT_EXECUTABLES)
LOCAL_MODULE_TAGS := eng
include $(BUILD_EXECUTABLE)
