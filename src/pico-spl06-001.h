#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "spl06-001-reg.h"

typedef enum {
    STANDBY, // idle
    CMD_PRS, CMD_TEMP, // command mode, single shot measure by command
    BGD_PRS, BGD_TEMP, BGD_PRS_TEMP // background mode, continuous measure
} spl06_mode_t;

typedef struct spl06_config {
    uint8_t addr; // 0x77 or 0x76
    i2c_inst_t *i2c; // i2c0 or i2c1
    spl06_mode_t mode;
} spl06_config_t;

typedef struct spl06_coef {
    // pressure
    int32_t c00, c10;
    int16_t c20, c30, c01, c11, c21;
    // temp
    int16_t c0, c1;
} spl06_coef_t;

int8_t spl06_init(spl06_config_t *config);
void spl06_read_press(spl06_config_t *config, int32_t *prs);
