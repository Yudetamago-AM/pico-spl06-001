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
    uint8_t addr; // 0x77(default) or 0x76
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

// user functions
int8_t spl06_init(spl06_config_t *config, spl06_coef_t *coef);
void spl06_read_press_cal(spl06_config_t *config, spl06_coef_t *coef, uint8_t oversample, float *prs);
void spl06_read_temp_cal(spl06_config_t *config, spl06_coef_t *coef, float *temp);

// internal functions, but allowed to call from other files
void spl06_set_mode(spl06_config_t *config, spl06_mode_t mode);

int8_t spl06_config_prs(spl06_config_t *config, uint8_t rate, uint8_t oversample);

int8_t spl06_config_temp(spl06_config_t *config, uint8_t rate, uint8_t oversample);
int8_t spl06_read_coef(spl06_config_t *config, spl06_coef_t *coef);

// internal functions, not allowed to call from other files
static int32_t oversample2k(uint8_t oversample);
static int8_t rate2config(uint8_t rate);
static int8_t oversample2config(uint8_t oversample, int16_t *time);
void spl06_read_press_raw(spl06_config_t *config, int32_t *prs);
void spl06_read_temp_raw(spl06_config_t *config, int32_t *temp);
