# RIOT_TTGOESP32_LORA_V1

This repository contains a RIOT-OS board definition for the TTGO ESP32 board, Version 1, that has support for LORA through the SX127X transceiver and a SSD1306 OLED.

The board definition is not yet totally complete. Only the following points are correctly implemented/checked:

- ESP32 clock frequency: Necessary for correct serial output.
- On board led PIN definition
- SX1276 transceiver pin definitions
- SSD1306 pin definitions
- I2C pins definitions
- SPI pins definitions

Other pins are probably not corrected and where not checked yet.

# What is working?

The following peripheral devices work:

- SSD1306 OLed
- LORA SX1276 works but several issues connecting to lorawan/TTN do exist (timing issues).

Under the examples directory there are examples for each peripheral.

# How to use it?

The following steps are required:

- Checkout the RIOT-OS repository to a local machine.
- I recommend to install the docker RIOT-OS BUILD container: https://hub.docker.com/r/riot/riotbuild/ for building RIOT based applications
- Checkout this repository
- Copy this repository board definition to the boards directory of the cloned RIOT-OS repository
- Enter the example directoy application.
- Execute make flash term

