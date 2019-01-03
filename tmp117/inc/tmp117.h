/**************************************************************
 * FILENAME: tmp117.h
 * PURPOSE:  Register map of TMP117 digital temperature sensor
 * CONTACT:  zjutujunbo@gmail.com
 * AUTHOR:   junbo tu
 * VERSION:  1.0
 * DATE:     2019/01/03
 *************************************************************/
#ifndef TMP117_H__
#define TMP117_H__
/*************************************************************/
#include <stdint.h>
/*************************************************************/
/* Device I2C Address */
#define TMP117_ADDR  0x90 // ADDR PIN -> GND
#define TMP117_ADDR2 0x92 // ADDR PIN -> VDD
#define TMP117_ADDR3 0x94 // ADDR PIN -> SDA
#define TMP117_ADDR4 0x96 // ADDR PIN -> SCL
/*************************************************************/
/* Register Address */
#define TMP117_DEVID         0x0F // Device ID (Read Only)
#define TMP117_TEMP          0x00 // Temperature data (Read Only)
#define TMP117_CONFIG        0x01 // Configuration
#define TMP117_HIGH_LIMIT    0x02 // High limit
#define TMP117_LOW_LIMIT     0x03 // Low limit
#define TMP117_EEPROM_UNLOCK 0x04 // EEPROM Unlock
#define TMP117_EEPROM1       0x05 // EEPROM1
#define TMP117_EEPROM2       0x06 // EEPROM2
#define TMP117_OFFSET        0x07 // Temperature Offset
#define TMP117_EEPROM3       0x08 // EEPROM3
/*************************************************************/
/* Register Value */
/* TMP117_DEVID */
#define TMP117_ID 0x0117

/* TMP117_CONFIG */
#define TMP117_CONTINUOUS_CONVERSION 0x0000
#define TMP117_SHUTDOWM 0x0400
#define TMP117_ONE_SHOT 0x0600
#define TMP117_FREQ_15_5MS 0x0000
#define TMP117_FREQ_125MS 0x0080
#define TMP117_FREQ_250MS 0x0100
#define TMP117_FREQ_500MS 0x0180
#define TMP117_FREQ_1S 0x0200
#define TMP117_FREQ_4S 0x0280
#define TMP117_FREQ_8S 0x0300
#define TMP117_FREQ_16S 0x0380
#define TMP117_AVG_NO 0x0000
#define TMP117_AVG_8 0x0020
#define TMP117_AVG_32 0x0040
#define TMP117_AVG_64 0x0060
#define TMP117_MODE_THERM 0x0010
#define TMP117_MODE_ALERT 0x0000
#define TMP117_PIN_ACTIVE_HIGH 0x0008
#define TMP117_PIN_ACTIVE_LOW 0x0000
#define TMP117_PIN_DATA_READY 0x0004
#define TMP117_PIN_ALERT 0x0000
#define TMP117_SOFT_RESET 0x0002

/*************************************************************/
void TMP117_WrReg(uint8_t reg, uint16_t data);
void TMP117_RdReg(uint8_t reg, uint16_t *p_data);
void TMP117_init(void);

#endif // TMP117_H__
