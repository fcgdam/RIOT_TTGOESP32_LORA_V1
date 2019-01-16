# name of your application
APPLICATION = TTN_DemoApp

# Use the TTGO ESP32 Lora V1 board:
BOARD ?= esp32_ttgo_lora_v1

# Define here the type of node activation OTAA or ABP:  ABP=0, OTAA=1
NODEACTIVATION ?= 0            
# NODEACTIVATION ?= 1          

# This has to be the absolute path to the RIOT base directory:
# This variable is defined at shell level in my case, uncomment to point to your RIOT-OS install directory
#RIOTBASE ?= $(CURDIR)/../..
RIOTBASE ?= $(RIOT_BASE)

BOARD_INSUFFICIENT_MEMORY := nucleo-f031k6 nucleo-f042k6 nucleo-l031k6

# Pre-set this information so when the board boots up, a set of default parameters are already set
# TTN information for OTTA activation.
# WARNING: Make sure there are NO trailing spaces.
DEVEUI ?= 0000000000000000
APPEUI ?= 0000000000000000
APPKEY ?= 00000000000000000000000000000000

# TTN information for ABP activation.
DEVADDR ?= 00000000
NWKSKEY ?= 00000000000000000000000000000000
APPSKEY ?= 00000000000000000000000000000000

# Default radio driver is Semtech SX1276 
DRIVER ?= sx1276

# Default region is Europe and default band is 868MHz
REGION ?= EU868

# Include the Semtech-loramac package
USEPKG += semtech-loramac
#USEPKG += u8g2

FEATURES_REQUIRED += periph_gpio periph_i2c

USEMODULE += $(DRIVER)
USEMODULE += fmt

# include the shell:
USEMODULE += shell
USEMODULE += shell_commands
# additional modules for debugging:
USEMODULE += ps

FEATURES_REQUIRED += periph_rtc

CFLAGS += -DREGION_$(REGION)
CFLAGS += -DDEVEUI=\"$(DEVEUI)\" -DAPPEUI=\"$(APPEUI)\" -DAPPKEY=\"$(APPKEY)\"
CFLAGS += -DDEVADDR=\"$(DEVADDR)\" -DAPPSKEY=\"$(APPSKEY)\" -DNWKSKEY=\"$(NWKSKEY)\"
CFLAGS += -DNODEACTIVATION=$(NODEACTIVATION)
CFLAGS += -DLORAMAC_ACTIVE_REGION=LORAMAC_REGION_$(REGION)

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
