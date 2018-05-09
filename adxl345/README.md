# ADXL345 Accelerometer Sensor Port
## Compatibility
This project is tested with nRF5_SDK_15.0.0. SDK version below 15.0.0 is not supported.

## How to
Simply copy all the files to your project directory. Add .c files and .h directory path to your project. Then you need to modify Pin definitions in adxl345.c to comply with your hardware settings. Finally call ADXL345_init() function in main() to start accerelation test.

## Notice
You need to be aware of TWIM and GPIOTE module reinitialization.
