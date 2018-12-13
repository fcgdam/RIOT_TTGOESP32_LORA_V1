TTGO SSD1301 OLED 
=================

This example contains a simple code to test the TTGO ESP32 LORA V1 board OLed screen.
The screen is based on the SSD1306 chip, and is connected to the ESP32 cpu by I2C (Address 0x3C).

The screen only works when the I2C speed is set to NORMAL (see the board peripheral definition).
Other speeds do not work.

Usage
=====

Copy the TTGO board definition to the RIOT boards subdirectory.
If not wanting to mess with your RIOT installation, the TTGO Board definition can be 
accessed by defining a path for the Third-party boards.

The ESP-IDF SDK also needs to be installed, so that flashing is possible.
I'm using the DOCKER image that has all the required frameworks for compiling RIOT applications.
If not using DOCKER, remove the export for BUILD_IN_DOCKER

Build, flash and start the application:
```
export BOARD=esp32_ttgo_lora_v1
export RIOT_BASE=/opt/RIOT
export ESP32_SDK_DIR=/opt/esp-idf
export BUILD_IN_DOCKER=1
make flash term
```

The `term` make target starts a terminal emulator for your board. It
connects to a default port so you can interact with the shell, usually
that is `/dev/ttyUSB0`. If your port is named differently, the
`PORT=/dev/yourport` variable can be used to override this.

