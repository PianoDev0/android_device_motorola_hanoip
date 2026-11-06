LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := MotoCamera5
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := APPS
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_PRODUCT_MODULE := true
LOCAL_PRIVILEGED_MODULE := true
LOCAL_DEX_PREOPT := false
LOCAL_OVERRIDES_PACKAGES := Camera2 Aperture

my_intermediates := $(call local-intermediates-dir)
LOCAL_PREBUILT_MODULE_FILE := $(my_intermediates)/$(LOCAL_MODULE).apk

MOTOCAMERA5_PARTS := \
    $(LOCAL_PATH)/MotoCamera5.apk.partaa \
    $(LOCAL_PATH)/MotoCamera5.apk.partab \
    $(LOCAL_PATH)/MotoCamera5.apk.partac \
    $(LOCAL_PATH)/MotoCamera5.apk.partad \
    $(LOCAL_PATH)/MotoCamera5.apk.partae \
    $(LOCAL_PATH)/MotoCamera5.apk.partaf

$(LOCAL_PREBUILT_MODULE_FILE): $(MOTOCAMERA5_PARTS)
	@mkdir -p $(dir $@)
	cat $(MOTOCAMERA5_PARTS) > $@

include $(BUILD_PREBUILT)
