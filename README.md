My goal is to make an OSC server optimized for VRChat/ChilloutVR, that is flexible and modular enough to use with DIY haptics devices. This repository is the firmware side.

Features:

- ESP32 simple firmware that allows to control the PWM of some GPIO pins via the network.
- Easy to understand and test
- More features to come

This firmware is used in complement to Daky OSC (Python server): https://github.com/dakyneko/DakyHapticsOSC

# Building

- First configure the firmware: in ESP32 open main.ino, at the top adapt the TODO section. This may include Wifi name, password, motor mapping and more.
- Follow one section below with your preferred dev tool:

## Arduino IDE
- select your ESP32 board
- compile and flash

## PlatformIO

- `pip install platformio`
- `cd ESP32`
- check platformio.ini to see which board is supported, if necessary add your own (environment -e) it may just work.
- `pio run --target upload -e esp32_devkit --upload-port=/dev/ttyACM0` (adapt the -e option to your board and the upload port)

# Testing

- `pip install -r requirements.txt`
- read `test_udp.py` and understand what you can use
- `python -i test_udp.py ipaddress port` (adapt ip/hostname and port of your ESP32, default port is 1234)
- this opens python REPL, you can try `blink()` first then test each motor `set_motor(0, 50)` values are from 0 to 255 (`uint8_t`)
