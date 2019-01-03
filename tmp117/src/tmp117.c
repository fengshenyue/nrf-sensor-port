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
#define PIN_SCL   3
#define PIN_SDA   4
#define PIN_ALERT 5
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
    int16_t data;
    float temp;

    TMP117_RdReg(TMP117_TEMP, (uint16_t *)&data); // Read temperature
    temp = (float)data*0.0078125f; // Conversion
    NRF_LOG_INFO("Temp:" NRF_LOG_FLOAT_MARKER "C", NRF_LOG_FLOAT(temp));
}

static void gpio_enable(void)
{
    nrfx_gpiote_in_event_enable(PIN_ALERT, true);
}

static void gpio_init(void)
{
    uint32_t err_code;
    nrfx_gpiote_in_config_t cfg_in = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);

    if (!nrfx_gpiote_is_init()) {
        err_code = nrfx_gpiote_init();
        APP_ERROR_CHECK(err_code);
    }
    err_code = nrfx_gpiote_in_init(PIN_ALERT, &cfg_in, gpio_handler);
    APP_ERROR_CHECK(err_code);
}
/*************************************************************/
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!! *
 * Call this function in Main() *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!! */
void TMP117_init(void)
{
    uint16_t data;

    twim_init();
    gpio_init();
    nrf_delay_ms(2); // Wait 1.5ms at least

    // Secure hardware link
    TMP117_RdReg(TMP117_DEVID, &data);
    if (data == TMP117_ID) {
        NRF_LOG_INFO("TMP117 Init");
    } else {
        NRF_LOG_INFO("TMP117 Error");
        return;
    }

    // Configuration
    TMP117_WrReg(TMP117_CONFIG, TMP117_FREQ_1S|TMP117_AVG_8|TMP117_MODE_THERM|TMP117_PIN_DATA_READY);

    // Enable GPIO interrupt
    gpio_enable();
}

void TMP117_WrReg(uint8_t reg, uint16_t data)
{
    uint8_t buffer[3];

    buffer[0] = reg;
    buffer[1] = data>>8;
    buffer[2] = data&0x00FF;
    nrfx_twim_tx(&m_twim, ADDR>>1, buffer, 3, false);
}

void TMP117_RdReg(uint8_t reg, uint16_t *p_data)
{
    uint8_t buffer[2];

    nrfx_twim_tx(&m_twim, ADDR>>1, &reg, 1, false);
    nrfx_twim_rx(&m_twim, ADDR>>1, buffer, 2);
    *p_data = ((uint16_t)buffer[0]<<8)|(uint16_t)buffer[1];
}
