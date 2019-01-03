/**************************************************************
 * FILENAME: tmp117.c
 * PURPOSE:  NRF51/NRF52 platform porting
 * CONTACT:  colincr77@gmail.com
 * AUTHOR:   Shaoling Zhu
 * VERSION:  1.0
 * DATE:     2019/01/03
 *************************************************************/
/* TMP117 header file */
#include "tmp117.h"

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
#define ADDR     TMP117_ADDR
#define PIN_SCL  3
#define PIN_SDA  4
#define PIN_INT 5
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

}

static void gpio_enable(void)
{
    nrfx_gpiote_in_event_enable(PIN_INT, true);
}

static void gpio_init(void)
{
    uint32_t err_code;
    nrfx_gpiote_in_config_t cfg_in = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);

    if (!nrfx_gpiote_is_init()) {
        err_code = nrfx_gpiote_init();
        APP_ERROR_CHECK(err_code);
    }
    err_code = nrfx_gpiote_in_init(PIN_INT, &cfg_in, gpio_handler);
    APP_ERROR_CHECK(err_code);
}
/*************************************************************/
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!! *
 * Call this function in Main() *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!! */
void TMP117_init(void)
{
    uint8_t data;

    twim_init();
    gpio_init();
    nrf_delay_ms(2); // Wait 1.5ms at least

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