# VL53L0X Laser-Ranging Sensor Port
## Compatibility
This project is tested with nRF5_SDK_15.0.0. SDK version below 15.0.0 is not supported.
## How to
Simply copy all the files to your project directory. Add .c files and .h directory path to your project. Then you need to modify Pin definitions in vl53l0x_platform.c to comply with your hardware settings.
Finally call VL53L0X_init() function in main() to start continuous ranging test.
## Notice
You need to be aware of TWIM and GPIOTE module reinitialization.
