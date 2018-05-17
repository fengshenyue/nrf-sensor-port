/**************************************************************
 * FILENAME: bmp280_platform.c
 * PURPOSE:  NRF51/NRF52 platform porting
 * CONTACT:  colincr77@gmail.com
 * AUTHOR:   Shaoling Zhu
 * VERSION:  1.0
 * DATE:     2018/05/17
 *************************************************************/
/* BMP280 Header Files */
#include "bmp280.h"
#include "bmp280_defs.h"

/* Nordic Header Files */
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrfx_twim.h"
/*************************************************************/
/* !!!!!!!!!!!!!!!!! *
 * Modify these pins *
 * !!!!!!!!!!!!!!!!! */
#define PIN_SCL 3
#define PIN_SDA 4
/*************************************************************/
static nrfx_twim_t m_twim = NRFX_TWIM_INSTANCE(0);
static struct bmp280_dev bmp;
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
static int8_t BMP280_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    uint32_t err_code;
    uint8_t buffer[len+1];

    buffer[0] = reg_addr;
    memcpy(&buffer[1], data, len);
    err_code = nrfx_twim_tx(&m_twim, dev_id, buffer, len+1, false);
    // APP_ERROR_CHECK(err_code);

    return BMP280_OK;
}

static int8_t BMP280_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    uint32_t err_code;

    err_code = nrfx_twim_tx(&m_twim, dev_id, &reg_addr, 1, false);
    // APP_ERROR_CHECK(err_code);
    err_code = nrfx_twim_rx(&m_twim, dev_id, data, len);
    // APP_ERROR_CHECK(err_code);

    return BMP280_OK;
}
/*************************************************************/
static void BMP280_conf(void)
{
    struct bmp280_config conf;

    bmp280_get_config(&conf, &bmp);

    /* Overwrite the desired settings */
    conf.filter = BMP280_FILTER_COEFF_16;
    conf.os_pres = BMP280_OS_16X;
    conf.os_temp = BMP280_OS_2X;
    conf.odr = BMP280_ODR_0_5_MS;

    bmp280_set_config(&conf, &bmp);

    /* Always set the power mode after setting the configuration */
    bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);
}

static void BMP280_getdata(void)
{
    uint8_t meas_dur;
    struct bmp280_uncomp_data ucomp_data;

    meas_dur = bmp280_compute_meas_time(&bmp);
    NRF_LOG_RAW_INFO("Measurement duration: %dms\r\n", meas_dur);

    while (1) {
        bmp.delay_ms(100);
        bmp280_get_uncomp_data(&ucomp_data, &bmp);
        int32_t temp32 = bmp280_comp_temp_32bit(ucomp_data.uncomp_temp, &bmp);
        uint32_t pres32 = bmp280_comp_pres_32bit(ucomp_data.uncomp_press, &bmp);
        uint32_t pres64 = bmp280_comp_pres_64bit(ucomp_data.uncomp_press, &bmp);
        double temp = bmp280_comp_temp_double(ucomp_data.uncomp_temp, &bmp);
        double pres = bmp280_comp_pres_double(ucomp_data.uncomp_press, &bmp);
        NRF_LOG_RAW_INFO("UT:%d, UP:%d, T32:%d ", ucomp_data.uncomp_temp, ucomp_data.uncomp_press, temp32);
        NRF_LOG_RAW_INFO("P32:%d, P64:%d, P64N:%d ", pres32, pres64, pres64/256);
        NRF_LOG_RAW_INFO("T:" NRF_LOG_FLOAT_MARKER " ", NRF_LOG_FLOAT(temp));
        NRF_LOG_RAW_INFO("P:" NRF_LOG_FLOAT_MARKER "\r\n", NRF_LOG_FLOAT(pres));
    }
}
/*************************************************************/
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!! *
 * Call this function in Main() *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!! */
void BMP280_init(void)
{
    int8_t rslt;

    twim_init();
    nrf_delay_ms(2);

    /* Sensor interface over I2C with primary slave address  */
    bmp.dev_id = BMP280_I2C_ADDR_PRIM;
    bmp.intf = BMP280_I2C_INTF;
    bmp.read = BMP280_i2c_read;
    bmp.write = BMP280_i2c_write;
    bmp.delay_ms = nrf_delay_ms;

    rslt = bmp280_init(&bmp);

    if (rslt == BMP280_OK) {
        /* Sensor chip ID will be printed if initialization was successful */
        NRF_LOG_INFO("Device found with chip id 0x%x", bmp.chip_id);
    } else {
        NRF_LOG_INFO("Device not found");
        return;
    }

    BMP280_conf();
    BMP280_getdata();
}
