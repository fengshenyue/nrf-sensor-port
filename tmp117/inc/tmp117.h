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
#define TMP117_ID_H 0x01
#define TMP117_ID_L 0x17

/* TMP117_CONFIG_H */
#define TMP117_CFG_HIGH_ALERT 0x80
#define TMP117_CFG_LOW_ALERT 0x40
#define TMP117_CFG_DATA_READY 0x20
#define TMP117_CFG_EEPROM_BUSY 0x10
#define TMP117_CFG_MOD1 0x08
#define TMP117_CFG_MOD0 0x04
#define TMP117_CFG_CONV2 0x02
#define TMP117_CFG_CONV1 0x01
/* TMP117_CONFIG_L */
#define TMP117_CFG_CONV0 0x80
#define TMP117_CFG_AVG1 0x40
#define TMP117_CFG_AVG0 0x20
#define TMP117_CFG_TNA 0x10
#define TMP117_CFG_POL 0x08
#define TMP117_CFG_DRNALERT_EN 0x04
#define TMP117_CFG_SOFT_RESET 0x02

/* TMP117_EEPROM_UNLOCK_H */
#define TMP117_ROM_EUN 0x80
#define TMP117_ROM_BUSY 0x40

/*************************************************************/
void TMP117_WriteMulti(uint8_t reg, uint8_t *p_data, uint8_t len);
void TMP117_ReadMulti(uint8_t reg, uint8_t *p_data, uint8_t len);
void TMP117_WrByte(uint8_t reg, uint8_t data);
void TMP117_RdByte(uint8_t reg, uint8_t *p_data);
void TMP117_init(void);

#endif // TMP117_H__