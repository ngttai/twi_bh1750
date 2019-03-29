/**
 * NRF5 SDK driver for BH1750 light sensor
 *
 * Ported from esp-open-rtos
 * Copyright (C) 2017 Andrej Krutak <dev@andree.sk>
 * Copyright (C) 2018 Ruslan V. Uss <unclerus@gmail.com>
 * BSD Licensed as described in the file LICENSE
 *
 * ROHM Semiconductor bh1750fvi-e.pdf
 */
#ifndef __BH1750_H__
#define __BH1750_H__

#include <stdint.h>
#include "nrf_drv_twi.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Possible chip addresses
 */
#define BH1750_ADDR_LO 0x23 //!< ADDR pin floating/low
#define BH1750_ADDR_HI 0x5c //!< ADDR pin high

/**
 * Measurement mode
 */
typedef enum
{
    BH1750_MODE_ONE_TIME = 0, //!< One time measurement
    BH1750_MODE_CONTINIOUS    //!< Continious measurement
} bh1750_mode_t;

/**
 * Measurement resolution
 */
typedef enum
{
    BH1750_RES_LOW = 0,  //!< 4 lx resolution, measurement time is usually 16 ms
    BH1750_RES_HIGH,     //!< 1 lx resolution, measurement time is usually 120 ms
    BH1750_RES_HIGHEST     //!< 0.5 lx resolution, measurement time is usually 120 ms
} bh1750_resolution_t;

/**
 * @brief Initialize device descriptior
 * @param[out] dev Pointer to device descriptor
 * @param[in] addr I2C address, BH1750_ADDR_LO or BH1750_ADDR_HI
 */
void bh1750_init(nrf_drv_twi_t dev, uint8_t addr);

/**
 * @brief Setup device parameters
 * @param dev Pointer to device descriptor
 * @param mode Measurement mode
 * @param resolution Measurement resolution
 */
void bh1750_setup(nrf_drv_twi_t dev, bh1750_mode_t mode, bh1750_resolution_t resolution);


/**
 * @brief Read LUX value from the device.
 * @param dev Pointer to I2C
 * @param[out] level read value in lux units

 */
void bh1750_read(nrf_drv_twi_t dev, uint16_t *level);

#ifdef __cplusplus
}
#endif

#endif /* __BH1750_H__ */
