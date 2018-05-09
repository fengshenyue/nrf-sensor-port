/**************************************************************
 * FILENAME: adxl345.h
 * PURPOSE:  Register map of ADXL345 digital accelerameter
 * CONTACT:  colincr77@gmail.com
 * AUTHOR:   Shaoling Zhu
 * VERSION:  1.0
 * DATE:     2018/05/09
 *************************************************************/
#ifndef ADXL345_H__
#define ADXL345_H__
/*************************************************************/
#include <stdint.h>
/*************************************************************/
/* Device I2C Address */
#define ADXL345_ADDR        0x53 // ADDR PIN -> GND
#define ADXL345_ADDR2       0x1D // ADDR PIN -> VDD
/*************************************************************/
/* Register Address */
#define ADXL345_DEVID          0x00 // Device ID (Read Only)
#define ADXL345_RESERVED       0x01 // Reserved
#define ADXL345_THRESH_TAP     0x1D // Tap threshold
#define ADXL345_OFSX           0x1E // X-axis offset
#define ADXL345_OFSY           0x1F // Y-axis offset
#define ADXL345_OFSZ           0x20 // Z-axis offset
#define ADXL345_DUR            0x21 // Tap duration
#define ADXL345_LATENT         0x22 // Tap latency
#define ADXL345_WINDOW         0x23 // Tap window
#define ADXL345_THRESH_ACT     0x24 // Activity threshold
#define ADXL345_THRESH_INACT   0x25 // Inactivity threshold
#define ADXL345_TIME_INACT     0x26 // Inactivity time
#define ADXL345_ACT_INACT_CTL  0x27 // Axis enable control for activity and inactivity detection
#define ADXL345_THRESH_FF      0x28 // Free-fall threshold
#define ADXL345_TIME_FF        0x29 // Free-fall time
#define ADXL345_TAP_AXES       0x2A // Axis control for single tap/double tap
#define ADXL345_ACT_TAP_STATUS 0x2B // Source of single tap/double tap (Read Only)
#define ADXL345_BW_RATE        0x2C // Data rate and power mode control
#define ADXL345_POWER_CTL      0x2D // Power-saving features control
#define ADXL345_INT_ENABLE     0x2E // Interrupt enable control
#define ADXL345_INT_MAP        0x2F // Interrupt mapping control
#define ADXL345_INT_SOURCE     0x30 // Source of interrupts (Read Only)
#define ADXL345_DATA_FORMAT    0x31 // Data format control
#define ADXL345_DATAX0         0x32 // X-Axis Data 0 (Read Only)
#define ADXL345_DATAX1         0x33 // X-Axis Data 1 (Read Only)
#define ADXL345_DATAY0         0x34 // Y-Axis Data 0 (Read Only)
#define ADXL345_DATAY1         0x35 // Y-Axis Data 1 (Read Only)
#define ADXL345_DATAZ0         0x36 // Z-Axis Data 0 (Read Only)
#define ADXL345_DATAZ1         0x37 // Z-Axis Data 1 (Read Only)
#define ADXL345_FIFO_CTL       0x38 // FIFO control
#define ADXL345_FIFO_STATUS    0x39 // FIFO status (Read Only)
/*************************************************************/
/* Register Value */
/* ADXL345_DEVID */
#define ADXL345_ID 0xE5

/* ADXL345_RESERVED */
#define ADXL345_RESET 0x52 // Wait 1.1ms for initialization

/* ADXL345_ACT_INACT_CTL */
#define ADXL345_INACT_Z_ENABLE 0x01
#define ADXL345_INACT_Y_ENABLE 0x02
#define ADXL345_INACT_X_ENABLE 0x04
#define ADXL345_INACT_AC       0x08
#define ADXL345_ACT_Z_ENABLE   0x10
#define ADXL345_ACT_Y_ENABLE   0x20
#define ADXL345_ACT_X_ENABLE   0x40
#define ADXL345_ACT_AC         0x80

/* ADXL345_TAP_AXES */
#define ADXL345_TAP_Z_ENABLE 0x01
#define ADXL345_TAP_Y_ENABLE 0x02
#define ADXL345_TAP_X_ENABLE 0x04
#define ADXL345_TAP_SUPPRESS 0x08

/* ADXL345_ACT_TAP_STATUS */
#define ADXL345_TAP_Z_SOURCE 0x01
#define ADXL345_TAP_Y_SOURCE 0x02
#define ADXL345_TAP_X_SOURCE 0x04
#define ADXL345_ASLEEP       0x08
#define ADXL345_ACT_Z_SOURCE 0x10
#define ADXL345_ACT_Y_SOURCE 0x20
#define ADXL345_ACT_X_SOURCE 0x40

/* ADXL345_BW_RATE */
#define ADXL345_RATE_3200 0x0F
#define ADXL345_RATE_1600 0x0E
#define ADXL345_RATE_800  0x0D
#define ADXL345_RATE_400  0x0C
#define ADXL345_RATE_200  0x0B
#define ADXL345_RATE_100  0x0A
#define ADXL345_RATE_50   0x09
#define ADXL345_RATE_25   0x08
#define ADXL345_RATE_12_5 0x07
#define ADXL345_RATE_6_25 0x06
#define ADXL345_RATE_3_13 0x05
#define ADXL345_RATE_1_56 0x04
#define ADXL345_RATE_0_78 0x03
#define ADXL345_RATE_0_39 0x02
#define ADXL345_RATE_0_20 0x01
#define ADXL345_RATE_0_10 0x00
#define ADXL345_LOW_POWER 0x10

/* ADXL345_POWER_CTL */
#define ADXL345_WAKEUP_8HZ 0x00
#define ADXL345_WAKEUP_4HZ 0x01
#define ADXL345_WAKEUP_2HZ 0x02
#define ADXL345_WAKEUP_1HZ 0x03
#define ADXL345_SLEEP      0x04
#define ADXL345_MEASURE    0x08
#define ADXL345_AUTO_SLEEP 0x10
#define ADXL345_LINK       0x20

/* ADXL345_INT_ENABLE & ADXL345_INT_MAP & ADXL345_INT_SOURCE */
#define ADXL345_OVERRUN    0x01
#define ADXL345_WATERMARK  0x02
#define ADXL345_FREE_FALL  0x04
#define ADXL345_INACTIVITY 0x08
#define ADXL345_ACTIVITY   0x10
#define ADXL345_DOUBLE_TAP 0x20
#define ADXL345_SINGLE_TAP 0x40
#define ADXL345_DATA_READY 0x80

/* ADXL345_DATA_FORMAT */
#define ADXL345_RANGE_2G   0x00
#define ADXL345_RANGE_4G   0x01
#define ADXL345_RANGE_8G   0x02
#define ADXL345_RANGE_16G  0x03
#define ADXL345_DATA_MSB   0x04
#define ADXL345_FULL_RES   0x08
#define ADXL345_INT_INVERT 0x20
#define ADXL345_SPI_3WIRE  0x40
#define ADXL345_SELF_TEST  0x80

/* ADXL345_FIFO_CTL */
#define ADXL345_TRIGGER_INT2      0x20
#define ADXL345_FIFO_MODE_FIFO    0x40
#define ADXL345_FIFO_MODE_STREAM  0x80
#define ADXL345_FIFO_MODE_TRIGGER 0xC0

/* ADXL345_FIFO_STATUS */
#define ADXL345_FIFO_TRIG 0x80
/*************************************************************/
void ADXL345_WriteMulti(uint8_t reg, uint8_t *p_data, uint8_t len);
void ADXL345_ReadMulti(uint8_t reg, uint8_t *p_data, uint8_t len);
void ADXL345_WrByte(uint8_t reg, uint8_t data);
void ADXL345_RdByte(uint8_t reg, uint8_t *p_data);

#endif // ADXL345_H__