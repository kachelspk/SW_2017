/*
 * 
 * Driver MPU6050 for nRF51xxx
 * Author: Kachel
 * Data: Mar-26-2017
 */

#include "mpu6050.h"

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(0);

bool twi_init(){
    ret_code_t err_code;
    const nrf_drv_twi_config_t twi_config = {
       .scl                = SCL_PIN,
       .sda                = SDA_PIN,
       .frequency          = NRF_TWI_FREQ_100K,
    };
    
    err_code = nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_twi_enable(&m_twi);
    return true;
}

bool twi_readByte(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data)
{
    uint8_t len = 1;
    nrf_drv_twi_tx(&m_twi, slaveAddr, &regAddr, 1, true);
    ret_code_t  err = nrf_drv_twi_rx(&m_twi, slaveAddr, data, len, false);
    return err == NRF_SUCCESS : true ? false;
}

bool twi_readBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t len, uint8_t *data)
{
    nrf_drv_twi_tx(&m_twi, slaveAddr, &regAddr, 1, true);
    ret_code_t  err = nrf_drv_twi_rx(&m_twi, slaveAddr, data, len, false);
    return err == NRF_SUCCESS : true ? false;
}

uint8_t twi_readBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum)
{
    uint8_t raw;
    uint8_t result;
    twi_readByte(slaveAddr, regAddr, &raw);
    result = (raw >> bitNum) & 0x01;
    return  result;
}

uint8_t twi_readBits(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitStart, uint8_t lenght)
{
    uint8_t i;
    uint8_t raw;
    uint8_t result;
    uint8_t bitsMark;

    bitsMark = ((1 << lenght) - 1) <<  (bitStart - lenght + 1);
    
    twi_readByte(slaveAddr, regAddr, &raw);
    result = (raw && bitsMark) >> bitStart;
    return  result;
}

bool twi_writeByte(uint8_t slaveAddr, uint8_t regAddr, uint8_t data)
{
    uint8_t buff[2];
    ret_code_t  err;
    
    buff[0] = regAddr;
    buff[1] = data;
    nrf_drv_twi_tx(&m_twi, slaveAddr, buff, 2, false);
    return err == NRF_SUCCESS : true ? false;
}

bool twi_writeBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t lenght)
{
    ret_code_t  err;
    nrf_drv_twi_tx(&m_twi, slaveAddr, data, lenght, false);
    return err == NRF_SUCCESS : true ? false;
}

bool twi_writeBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t value)
{
    uint8_t raw;
    twi_readByte(slaveAddr, regAddr, &raw);
    if (value == 1){
        raw = raw | (1 << bitNum);
    } else if (value == 0) {
        raw &= ~(1 << bitNum);
    }
    else 
        return false;
    return twi_writeByte(slaveAddr, regAddr, raw);
}

bool twi_writeBits()
{
    return true;
}

bool mpu6050_init()
{
    return true;
}