#
#             LUFA Library
#     Copyright (C) Dean Camera, 2015.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega32u2
ARCH         = AVR8

BOARD        = GSCHEIDUINO
#BOARD        = ATEVAL


ifeq ($(BOARD), GSCHEIDUINO)
  F_CPU        = 16000000
else
  F_CPU        = 16000000
endif
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = USBtoSerial
SRC          = $(TARGET).c Descriptors.c Lib/V2Protocol.c Lib/V2ProtocolParams.c Lib/ISP/ISPProtocol.c Lib/ISP/ISPTarget.c Lib/XPROG/XPROGProtocol.c \
               Lib/XPROG/XPROGTarget.c Lib/XPROG/XMEGANVM.c Lib/XPROG/TINYNVM.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = ../../LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/ -Wall -Werror
LD_FLAGS     =

# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk
