/**************************************************************
 * FILENAME: adxl345.c
 * PURPOSE:  NRF51/NRF52 platform porting
 * CONTACT:  colincr77@gmail.com
 * AUTHOR:   Shaoling Zhu
 * VERSION:  1.0
 * DATE:     2018/05/09
 *************************************************************/
/* ADXL345 header file */
#include "adxl345.h"

/* Nordic header files */
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrfx_gpiote.h"
#include "nrfx_twim.h"
/*************************************************************/
/* !!!!!!!!!!!!!!!!! *
 * Modify these pins *
 * !!!!!!!!!!!!!!!!! */
#define ADDR     ADXL345_ADDR
#define PIN_SCL  3
#define PIN_SDA  4
#define PIN_INT1 5
/*************************************************************/
static nrfx_twim_t m_twim = NRFX_TWIM_INSTANCE(0);
/*************************************************************/
static void twim_init(void)
{
    uint32_t err_code;
    nrfx_twim_config_t config = {
        .scl                = PIN_SCL,
        .sda                = PIN_SDA,
        .frequency          = NRF_TWIM_FREQ_400K,
        .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
        .hold_bus_uninit    = false
    };

    err_code = nrfx_twim_init(&m_twim, &config, NULL, NULL);
    APP_ERROR_CHECK(err_code);
    nrfx_twim_enable(&m_twim);
}
/*************************************************************/
static void gpio_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    uint8_t data;
    uint8_t buffer[6];
    int16_t number[3];
    float grav[3];

    ADXL345_ReadMulti(ADXL345_DATAX0, buffer, 6);
    number[0] = (uint16_t)buffer[1]<<8|buffer[0];
    number[1] = (uint16_t)buffer[3]<<8|buffer[2];
    number[2] = (uint16_t)buffer[5]<<8|buffer[4];
    grav[0] = (float)number[0]*3.9f;
    grav[1] = (float)number[1]*3.9f;
    grav[2] = (float)number[2]*3.9f;
    NRF_LOG_RAW_INFO("x:" NRF_LOG_FLOAT_MARKER "mg ", NRF_LOG_FLOAT(grav[0]));
    NRF_LOG_RAW_INFO("y:" NRF_LOG_FLOAT_MARKER "mg ", NRF_LOG_FLOAT(grav[1]));
    NRF_LOG_RAW_INFO("z:" NRF_LOG_FLOAT_MARKER "mg\r\n", NRF_LOG_FLOAT(grav[2]));
}

static void gpio_enable(void)
{
    nrfx_gpiote_in_event_enable(PIN_INT1, true);
}

static void gpio_init(void)
{
    uint32_t err_code;
    nrfx_gpiote_in_config_t cfg_in = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);

    if (!nrfx_gpiote_is_init()) {
        err_code = nrfx_gpiote_init();
        APP_ERROR_CHECK(err_code);
    }
    err_code = nrfx_gpiote_in_init(PIN_INT1, &cfg_in, gpio_handler);
    APP_ERROR_CHECK(err_code);
}
/*************************************************************/
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!! *
 * Call this function in Main() *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!! */
void ADXL345_init(void)
{
    uint8_t data;

    twim_init();
    gpio_init();
    nrf_delay_ms(2);

    // Secure hardware link
    ADXL345_RdByte(ADXL345_DEVID, &data);
    if (data == 0xE5) {
        NRF_LOG_INFO("ADXL345 Init");
    } else {
        NRF_LOG_INFO("ADXL345 Error");
        return;
    }

    // Configuration
    ADXL345_WrByte(ADXL345_RESERVED, ADXL345_RESET); // Reset
    nrf_delay_ms(2);
    ADXL345_WrByte(ADXL345_BW_RATE, ADXL345_RATE_12_5);
    ADXL345_WrByte(ADXL345_DATA_FORMAT, ADXL345_RANGE_16G|ADXL345_FULL_RES);
    ADXL345_WrByte(ADXL345_INT_ENABLE, ADXL345_DATA_READY);
    ADXL345_WrByte(ADXL345_POWER_CTL, ADXL345_MEASURE);

    gpio_enable();
}

void ADXL345_WriteMulti(uint8_t reg, uint8_t *p_data, uint8_t len)
{
    uint32_t err_code;
    uint8_t buffer[len+1];

    buffer[0] = reg;
    memcpy(&buffer[1], p_data, len);
    err_code = nrfx_twim_tx(&m_twim, ADDR, buffer, len+1, false);
    // APP_ERROR_CHECK(err_code);
}

void ADXL345_ReadMulti(uint8_t reg, uint8_t *p_data, uint8_t len)
{
    uint32_t err_code;

    err_code = nrfx_twim_tx(&m_twim, ADDR, &reg, 1, false);
    // APP_ERROR_CHECK(err_code);
    err_code = nrfx_twim_rx(&m_twim, ADDR, p_data, len);
    // APP_ERROR_CHECK(err_code);
}

void ADXL345_WrByte(uint8_t reg, uint8_t data)
{
    uint32_t err_code;
    uint8_t buffer[2];

    buffer[0] = reg;
    buffer[1] = data;
    err_code = nrfx_twim_tx(&m_twim, ADDR, buffer, 2, false);
    // APP_ERROR_CHECK(err_code);
}

void ADXL345_RdByte(uint8_t reg, uint8_t *p_data)
{
    uint32_t err_code;

    err_code = nrfx_twim_tx(&m_twim, ADDR, &reg, 1, false);
    // APP_ERROR_CHECK(err_code);
    err_code = nrfx_twim_rx(&m_twim, ADDR, p_data, 1);
    // APP_ERROR_CHECK(err_code);
}
