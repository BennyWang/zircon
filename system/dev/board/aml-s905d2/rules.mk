# Copyright 2018 The Fuchsia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_TYPE := driver

MODULE_SRCS += \
    $(LOCAL_DIR)/aml.c \
    $(LOCAL_DIR)/aml-gpio.c \
    $(LOCAL_DIR)/aml-i2c.c \
    $(LOCAL_DIR)/aml-usb.c \

MODULE_STATIC_LIBS := \
    system/dev/soc/amlogic \
    system/ulib/ddk \
    system/ulib/sync

MODULE_LIBS := \
    system/ulib/driver \
    system/ulib/c \
    system/ulib/zircon

MODULE_HEADER_DEPS := \
    system/dev/soc/amlogic

include make/module.mk
