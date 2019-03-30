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

/* TWI instance. */
static const nrf_drv_twi_t *p_twi = NULL;

static uint8_t dev_adrr;

void bh1750_init(const nrf_drv_twi_t *dev, uint8_t addr)
{
    if (addr != BH1750_ADDR_LO && addr != BH1750_ADDR_HI)
    {
        NRF_LOG_INFO("BH1750 Invalid I2C address");
    }
    dev_adrr = addr;
    p_twi = dev;
}

void bh1750_setup(bh1750_mode_t mode, bh1750_resolution_t resolution)
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

    err_code = nrf_drv_twi_tx(p_twi, dev_adrr, &opcode, 1, false);
    APP_ERROR_CHECK(err_code);
}

void bh1750_read(uint16_t *level)
{
    ret_code_t err_code;
    uint8_t buf[2];

    err_code = nrf_drv_twi_rx(p_twi, dev_adrr, buf, 2);
    APP_ERROR_CHECK(err_code);

    *level = buf[0] << 8 | buf[1];
    *level = (*level * 10) / 12; // convert to LUX
}