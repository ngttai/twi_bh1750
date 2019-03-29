/**
 * ESP-IDF driver for BH1750 light sensor
 *
 * Ported from esp-open-rtos
 * Copyright (C) 2017 Andrej Krutak <dev@andree.sk>
 *               2018 Ruslan V. Uss <unclerus@gmail.com>
 * BSD Licensed as described in the file LICENSE
 *
 * ROHM Semiconductor bh1750fvi-e.pdf
 */
#include "bh1750.h"
#include <stdio.h>
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define OPCODE_HIGH 0x0
#define OPCODE_HIGH2 0x1
#define OPCODE_LOW 0x3

#define OPCODE_CONT 0x10
#define OPCODE_OT 0x20

static uint8_t dev_adrr;

void bh1750_init(nrf_drv_twi_t dev, uint8_t addr)
{
    ret_code_t err_code;
    if (addr != BH1750_ADDR_LO && addr != BH1750_ADDR_HI)
    {
        NRF_LOG_INFO("BH1750 Invalid I2C address");
    }
    dev_adrr = addr;
    const nrf_drv_twi_config_t twi_lm75b_config = {
        .scl = 27,
        .sda = 26,
        .frequency = NRF_DRV_TWI_FREQ_100K,
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
        .clear_bus_init = false};
    err_code = nrf_drv_twi_init(&dev, &twi_lm75b_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&dev);
}

void bh1750_setup(nrf_drv_twi_t dev, bh1750_mode_t mode, bh1750_resolution_t resolution)
{
    ret_code_t err_code;
    uint8_t opcode = mode == BH1750_MODE_CONTINIOUS ? OPCODE_CONT : OPCODE_OT;
    switch (resolution)
    {
    case BH1750_RES_LOW:
        opcode |= OPCODE_LOW;
        break;
    case BH1750_RES_HIGH:
        opcode |= OPCODE_HIGH;
        break;
    default:
        opcode |= OPCODE_HIGH2;
        break;
    }

    err_code = nrf_drv_twi_tx(&dev, dev_adrr, &opcode, 1, false);
    APP_ERROR_CHECK(err_code);
}

void bh1750_read(nrf_drv_twi_t dev, uint16_t *level)
{
    ret_code_t err_code;
    uint8_t buf[2];

    err_code = nrf_drv_twi_rx(&dev, dev_adrr, buf, 2);
    APP_ERROR_CHECK(err_code);

    *level = buf[0] << 8 | buf[1];
    *level = (*level * 10) / 12; // convert to LUX
}