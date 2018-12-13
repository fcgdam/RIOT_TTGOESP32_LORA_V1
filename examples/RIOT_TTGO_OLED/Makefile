# name of your application
APPLICATION = ttgo_oled_test

# If no BOARD is found in the environment, use this default:
BOARD ?= esp32_ttgo_lora_v1

# This has to be the absolute path to the RIOT base directory:
#RIOTBASE ?= $(CURDIR)/../..
RIOTBASE ?= $(RIOT_BASE)

# Uncomment these lines if you want to use platform support from external
# repositories:
#RIOTCPU ?= $(CURDIR)/../../RIOT/thirdparty_cpu
#RIOTBOARD ?= $(CURDIR)/../../RIOT/thirdparty_boards

# Uncomment this to enable scheduler statistics for ps:
#USEMODULE += schedstatistics

# If you want to use native with valgrind, you should recompile native
# with the target all-valgrind instead of all:
# make -B clean all-valgrind

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# Packages to include
USEPKG += u8g2

FEATURES_REQUIRED += periph_gpio periph_i2c

include $(RIOTBASE)/Makefile.include
