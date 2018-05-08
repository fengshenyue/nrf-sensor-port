/**************************************************************
 * FILENAME: vl53l0x_platform.c
 * PURPOSE:  NRF51/NRF52 platform porting
 * CONTACT:  colincr77@gmail.com
 * AUTHOR:   Shaoling Zhu
 * VERSION:  1.0
 * DATE:     2018/05/07
 *************************************************************/
/* VL53L0X header files */
#include "vl53l0x_platform.h"
#include "vl53l0x_api.h"

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
#define PIN_SCL   3
#define PIN_SDA   4
#define PIN_GPIO1 5
#define PIN_XSHUT 6
/*************************************************************/
static nrfx_twim_t m_twim = NRFX_TWIM_INSTANCE(0);
static VL53L0X_Dev_t m_dev = {
    .I2cDevAddr      = 0x52>>1, // Nordic API will left shift the address by 1 bit. So we need to compensate by right shift 1 bit.
    .comms_type      = 1,
    .comms_speed_khz = 400
};
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
    uint32_t err_code;
    VL53L0X_RangingMeasurementData_t data;

    err_code = VL53L0X_GetRangingMeasurementData(&m_dev, &data);
    APP_ERROR_CHECK(err_code);
    VL53L0X_ClearInterruptMask(&m_dev, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Range:%d", data.RangeMilliMeter);
}

static void gpio_enable(void)
{
    nrfx_gpiote_in_event_enable(PIN_GPIO1, true);
}

static void gpio_init(void)
{
    uint32_t err_code;
    nrfx_gpiote_out_config_t cfg_out = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
    nrfx_gpiote_in_config_t cfg_in = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);

    err_code = nrfx_gpiote_init();
    APP_ERROR_CHECK(err_code);
    err_code = nrfx_gpiote_out_init(PIN_XSHUT, &cfg_out);
    APP_ERROR_CHECK(err_code);
    err_code = nrfx_gpiote_in_init(PIN_GPIO1, &cfg_in, gpio_handler);
    APP_ERROR_CHECK(err_code);
}
/*************************************************************/
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!! *
 * Call this function in Main() *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!! */
void VL53L0X_init(void) {
    uint32_t err_code;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    VL53L0X_Version_t version;
    VL53L0X_DeviceInfo_t devinfo;

    twim_init();
    gpio_init();

    err_code = VL53L0X_GetVersion(&version);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Revision:%d", version.revision);
    NRF_LOG_INFO("Major:%d", version.major);
    NRF_LOG_INFO("Minor:%d", version.minor);
    NRF_LOG_INFO("Build:%d", version.build);

    err_code = VL53L0X_DataInit(&m_dev);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Data Init");

    err_code = VL53L0X_GetDeviceInfo(&m_dev, &devinfo);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Name:%s", devinfo.Name);
    NRF_LOG_INFO("Type:%s", devinfo.Type);
    NRF_LOG_INFO("ProductId:%s", devinfo.ProductId);
    NRF_LOG_INFO("ProductType:%d", devinfo.ProductType);
    NRF_LOG_INFO("RevisionMajor:%d", devinfo.ProductRevisionMajor);
    NRF_LOG_INFO("RevisionMinor:%d", devinfo.ProductRevisionMinor);

    err_code = VL53L0X_StaticInit(&m_dev);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Static Init");

    err_code = VL53L0X_PerformRefCalibration(&m_dev,
                &VhvSettings, &PhaseCal);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("SPAD Calibration");

    err_code = VL53L0X_PerformRefSpadManagement(&m_dev, &refSpadCount, &isApertureSpads);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("SPAD Management");

    err_code = VL53L0X_SetDeviceMode(&m_dev, VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Set Device Mode to Continuous Timed Ranging");

    err_code = VL53L0X_SetGpioConfig(&m_dev, 0, VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING,
        VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY,
        VL53L0X_INTERRUPTPOLARITY_LOW);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Set GPIO Config to Interrupt");

    err_code = VL53L0X_SetInterMeasurementPeriodMilliSeconds(&m_dev, 50);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Set Interval to 50ms");

    err_code = VL53L0X_StartMeasurement(&m_dev);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Start Measurement");

    gpio_enable();
}
/*************************************************************/
VL53L0X_Error VL53L0X_LockSequenceAccess(VL53L0X_DEV Dev)
{
    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_UnlockSequenceAccess(VL53L0X_DEV Dev)
{
    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_WriteMulti(VL53L0X_DEV Dev, uint8_t index, uint8_t *pdata, uint32_t count)
{
    uint32_t err_code;
    uint8_t buffer[count+1];

    buffer[0] = index;
    memcpy(&buffer[1], pdata, count);
    err_code = nrfx_twim_tx(&m_twim, Dev->I2cDevAddr, buffer, count+1, false);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_ReadMulti(VL53L0X_DEV Dev, uint8_t index, uint8_t *pdata, uint32_t count)
{
    uint32_t err_code;

    err_code = nrfx_twim_tx(&m_twim, Dev->I2cDevAddr, &index, 1, false);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    err_code = nrfx_twim_rx(&m_twim, Dev->I2cDevAddr, pdata, count);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_WrByte(VL53L0X_DEV Dev, uint8_t index, uint8_t data)
{
    uint32_t err_code;
    uint8_t buffer[2];

    buffer[0] = index;
    buffer[1] = data;
    err_code = nrfx_twim_tx(&m_twim, Dev->I2cDevAddr, buffer, 2, false);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_WrWord(VL53L0X_DEV Dev, uint8_t index, uint16_t data)
{
    uint32_t err_code;
    uint8_t buffer[3];

    buffer[0] = index;
    buffer[1] = (uint8_t)(data>>8);
    buffer[2] = (uint8_t)(data&0x00FF);
    err_code = nrfx_twim_tx(&m_twim, Dev->I2cDevAddr, buffer, 3, false);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_WrDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t data)
{
    uint32_t err_code;
    uint8_t buffer[5];

    buffer[0] = index;
    buffer[1] = (uint8_t) (data>>24);
    buffer[2] = (uint8_t)((data&0x00FF0000)>>16);
    buffer[3] = (uint8_t)((data&0x0000FF00)>>8);
    buffer[4] = (uint8_t) (data&0x000000FF);
    err_code = nrfx_twim_tx(&m_twim, Dev->I2cDevAddr, buffer, 5, false);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_UpdateByte(VL53L0X_DEV Dev, uint8_t index, uint8_t AndData, uint8_t OrData)
{
    uint32_t err_code;
    uint8_t buffer[2];
    uint8_t data;

    err_code = nrfx_twim_tx(&m_twim, Dev->I2cDevAddr, &index, 1, false);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    err_code = nrfx_twim_rx(&m_twim, Dev->I2cDevAddr, &data, 1);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    buffer[0] = index;
    buffer[1] = (data&AndData)|OrData;
    err_code = nrfx_twim_tx(&m_twim, Dev->I2cDevAddr, buffer, 2, false);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_RdByte(VL53L0X_DEV Dev, uint8_t index, uint8_t *data)
{
    uint32_t err_code;

    err_code = nrfx_twim_tx(&m_twim, Dev->I2cDevAddr, &index, 1, false);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    err_code = nrfx_twim_rx(&m_twim, Dev->I2cDevAddr, data, 1);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_RdWord(VL53L0X_DEV Dev, uint8_t index, uint16_t *data)
{
    uint32_t err_code;
    uint8_t buffer[2];

    err_code = nrfx_twim_tx(&m_twim, Dev->I2cDevAddr, &index, 1, false);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    err_code = nrfx_twim_rx(&m_twim, Dev->I2cDevAddr, buffer, 2);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    *data = ((uint16_t)buffer[0]<<8)|(uint16_t)buffer[1];

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_RdDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t *data)
{
    uint32_t err_code;
    uint8_t buffer[4];

    err_code = nrfx_twim_tx(&m_twim, Dev->I2cDevAddr, &index, 1, false);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    err_code = nrfx_twim_rx(&m_twim, Dev->I2cDevAddr, buffer, 4);
    if (err_code != NRF_SUCCESS) {
        return VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    *data = ((uint32_t)buffer[0]<<24)|((uint32_t)buffer[1]<<16)|((uint32_t)buffer[2]<<8)|(uint32_t)buffer[3];

    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_PollingDelay(VL53L0X_DEV Dev)
{
    nrf_delay_ms(1);

    return VL53L0X_ERROR_NONE;
}
