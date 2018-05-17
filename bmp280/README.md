# BMP280 Digital Pressure Sensor Port
## Compatibility
This project is tested with nRF5_SDK_15.0.0. SDK version below 15.0.0 is not supported.

## How to
1.Connect PIN2 on BMP280 to VDD for using I2C interface.

2.Connect PIN5 on BMP280 to the ground for using I2C primary address.

3.Copy all the files to your project directory. Add .c files and .h directory path to your project. Then you need to modify Pin definitions in bmp280_platform.c to comply with your hardware settings. Finally call BMP280_init() function in main() to start pressure.

## Notice
You need to be aware of TWIM module reinitialization.
