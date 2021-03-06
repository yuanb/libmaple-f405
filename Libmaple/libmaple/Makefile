# Try "make help" for more information on BOARD and MEMORY_TARGET;
# these default to a Maple Flash build.
#BOARD ?= maple
#BOARD ?= aeroquad32
#BOARD ?= aeroquad32f1
#BOARD ?= discovery_f4
#BOARD ?= aeroquad32mini
#BOARD ?= freeflight
BOARD ?= netduinoplus2
#V=1

#CPUTYPE ?= cortex-m3 (USE THIS FOR STM32F1/STM32F2)
CPUTYPE ?= cortex-m4 #(USE THIS FOR STM32F4, no h/w FPU)
#CPUTYPE ?= cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__FPU_PRESENT #(USE THIS FOR STM32F4, h/w FPU Enabled) 
#NOTE:  To get this to work, the CRT libraries must also be built for the FPU
#These were found at:  C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\ARMGCC\3.3.1.128\ARMGCCToolchain\lib\gcc\arm-none-eabi\4.7.0\fpu
#CPUARCH ?= armv7-m (USE THIS FOR STM32F1/STM32F2)
CPUARCH ?= armv7e-m #(USE THIS FOR STM32F4)

.DEFAULT_GOAL := sketch

##
## Useful paths, constants, etc.
##

ifeq ($(LIB_MAPLE_HOME),)
SRCROOT := .
else
SRCROOT := $(LIB_MAPLE_HOME)
endif
BUILD_PATH = build
LIBMAPLE_PATH := $(SRCROOT)/libmaple
WIRISH_PATH := $(SRCROOT)/wirish
SUPPORT_PATH := $(SRCROOT)/support
# Support files for linker
LDDIR := $(SUPPORT_PATH)/ld
# Support files for this Makefile
MAKEDIR := $(SUPPORT_PATH)/make

# USB ID for DFU upload
VENDOR_ID  := 1EAF
PRODUCT_ID := 0003

##
## Target-specific configuration.  This determines some compiler and
## linker options/flags.
##

MEMORY_TARGET ?= jtag
#MEMORY_TARGET ?= flash

# $(BOARD)- and $(MEMORY_TARGET)-specific configuration
include $(MAKEDIR)/target-config.mk

##
## Compilation flags
##

GLOBAL_FLAGS    := -D$(VECT_BASE_ADDR)					     \
		   -DBOARD_$(BOARD) -DMCU_$(MCU)			     \
		   -DERROR_LED_PORT=$(ERROR_LED_PORT)			     \
		   -DERROR_LED_PIN=$(ERROR_LED_PIN)			     \
		   -D$(DENSITY) -D$(MCU_FAMILY) 

ifeq ($(BOARD), freeflight)
GLOBAL_FLAGS += -DDISABLEUSB
endif

ifeq ($(BOARD), aeroquad32)
GLOBAL_FLAGS += -DF_CPU=168000000UL
endif

ifeq ($(BOARD), discovery_f4)
GLOBAL_FLAGS += -DF_CPU=168000000UL
endif

ifeq ($(BOARD), netduinoplus2)
GLOBAL_FLAGS += -DF_CPU=168000000UL
endif

EXTRAINCDIRS += wirish/boards
EXTRAINCDIRS += libraries

ifeq ($(MCU_FAMILY), STM32F2)
	EXTRAINCDIRS += \
		libmaple/usbF4/STM32_USB_Device_Library/Core/inc \
		libmaple/usbF4/STM32_USB_Device_Library/Class/cdc/inc \
		libmaple/usbF4/STM32_USB_OTG_Driver/inc \
		libmaple/usbF4/VCP
endif

		   
#GLOBAL_FLAGS += -DDISABLEUSB
#GLOBAL_FLAGS += -DUSB_DISC_OD
		   
GLOBAL_CFLAGS   := -O0 -g3 -gdwarf-2  -mcpu=$(CPUTYPE) -mthumb -march=$(CPUARCH) \
		   -nostdlib -ffunction-sections -fdata-sections	     \
		   -Wl,--gc-sections $(GLOBAL_FLAGS)
GLOBAL_CXXFLAGS := -fno-rtti -fno-exceptions -Wall $(GLOBAL_FLAGS)
GLOBAL_ASFLAGS  := -mcpu=$(CPUTYPE) -march=$(CPUARCH) -mthumb		     \
		   -x assembler-with-cpp $(GLOBAL_FLAGS)
LDFLAGS  = -T$(LDDIR)/$(LDSCRIPT) -L$(LDDIR)    \
            -mcpu=$(CPUTYPE) -mthumb -Xlinker     \
            --gc-sections --print-gc-sections --march=$(CPUARCH) -Wall

##
## Build rules and useful templates
##

include $(SUPPORT_PATH)/make/build-rules.mk
include $(SUPPORT_PATH)/make/build-templates.mk

##
## Set all submodules here
##

# Try to keep LIBMAPLE_MODULES a simply-expanded variable
ifeq ($(LIBMAPLE_MODULES),)
	LIBMAPLE_MODULES := $(SRCROOT)/libmaple
else
	LIBMAPLE_MODULES += $(SRCROOT)/libmaple
endif
LIBMAPLE_MODULES += $(SRCROOT)/wirish
# Official libraries:
LIBMAPLE_MODULES += $(SRCROOT)/libraries/Servo
LIBMAPLE_MODULES += $(SRCROOT)/libraries/LiquidCrystal
LIBMAPLE_MODULES += $(SRCROOT)/libraries/Wire

# Experimental libraries:
LIBMAPLE_MODULES += $(SRCROOT)/libraries/FreeRTOS
LIBMAPLE_MODULES += $(SRCROOT)/libraries/mapleSDfat
LIBMAPLE_MODULES += $(SRCROOT)/libraries/EtherCard
LIBMAPLE_MODULES += $(SRCROOT)/libraries/EEPROM
LIBMAPLE_MODULES += $(SRCROOT)/libraries/i2cdevlib
LIBMAPLE_MODULES += $(SRCROOT)/libraries/L3G4200D

# Call each module's rules.mk:
$(foreach m,$(LIBMAPLE_MODULES),$(eval $(call LIBMAPLE_MODULE_template,$(m))))

##
## Targets
##

# main target
include $(SRCROOT)/build-targets.mk

.PHONY: install sketch clean help debug cscope tags ctags ram flash jtag doxygen mrproper

# Target upload commands
UPLOAD_ram   := $(SUPPORT_PATH)/scripts/reset.py && \
                sleep 1                  && \
                $(DFU) -a0 -d $(VENDOR_ID):$(PRODUCT_ID) -D $(BUILD_PATH)/$(BOARD).bin -R
UPLOAD_flash := $(SUPPORT_PATH)/scripts/reset.py && \
                sleep 1                  && \
                $(DFU) -a1 -d $(VENDOR_ID):$(PRODUCT_ID) -D $(BUILD_PATH)/$(BOARD).bin -R
UPLOAD_jtag  := $(OPENOCD_WRAPPER) flash

all: library

# Conditionally upload to whatever the last build was
install: INSTALL_TARGET = $(shell cat $(BUILD_PATH)/build-type 2>/dev/null)
install: $(BUILD_PATH)/$(BOARD).bin
	@echo Install target: $(INSTALL_TARGET)
	$(UPLOAD_$(INSTALL_TARGET))

# Force a rebuild if the target changed
PREV_BUILD_TYPE = $(shell cat $(BUILD_PATH)/build-type 2>/dev/null)
build-check:
ifneq ($(PREV_BUILD_TYPE), $(MEMORY_TARGET))
	$(shell rm -rf $(BUILD_PATH))
endif

sketch: build-check MSG_INFO $(BUILD_PATH)/$(BOARD).bin

clean:
	rm -rf build

mrproper: clean
	rm -rf doxygen

help:
	@echo ""
	@echo "  libmaple Makefile help"
	@echo "  ----------------------"
	@echo "  "
	@echo "  Programming targets:"
	@echo "      sketch:   Compile for BOARD to MEMORY_TARGET (default)."
	@echo "      install:  Compile and upload code over USB, using Maple bootloader"
	@echo "  "
	@echo "  You *must* set BOARD if not compiling for Maple (e.g."
	@echo "  use BOARD=maple_mini for mini, etc.), and MEMORY_TARGET"
	@echo "  if not compiling to Flash."
	@echo "  "
	@echo "  Valid BOARDs:"
	@echo "      maple, maple_mini, maple_RET6, maple_native"
	@echo "  "
	@echo "  Valid MEMORY_TARGETs (default=flash):"
	@echo "      ram:    Compile sketch code to ram"
	@echo "      flash:  Compile sketch code to flash"
	@echo "      jtag:   Compile sketch code for jtag; overwrites bootloader"
	@echo "  "
	@echo "  Other targets:"
	@echo "      debug:  Start OpenOCD gdb server on port 3333, telnet on port 4444"
	@echo "      clean: Remove all build and object files"
	@echo "      help: Show this message"
	@echo "      doxygen: Build Doxygen HTML and XML documentation"
	@echo "      mrproper: Remove all generated files"
	@echo "  "

debug:
	$(OPENOCD_WRAPPER) debug

cscope:
	rm -rf *.cscope
	find . -name '*.[hcS]' -o -name '*.cpp' | xargs cscope -b

tags:
	etags `find . -name "*.c" -o -name "*.cpp" -o -name "*.h"`
	@echo "Made TAGS file for EMACS code browsing"

ctags:
	ctags-exuberant -R .
	@echo "Made tags file for VIM code browsing"

ram:
	@$(MAKE) MEMORY_TARGET=ram --no-print-directory sketch

flash:
	@$(MAKE) MEMORY_TARGET=flash --no-print-directory sketch

jtag:
	@$(MAKE) MEMORY_TARGET=jtag --no-print-directory sketch

doxygen:
	doxygen $(SUPPORT_PATH)/doxygen/Doxyfile
