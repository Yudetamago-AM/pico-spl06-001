#include "pico-spl06-001.h"

int8_t spl06_init(spl06_config_t *config, spl06_mode_t mode) {
    uint8_t tmp[2], buf;

    // check if sensor could be accesed
    tmp[0] = SPL06_ID;
    i2c_write_blocking(config->i2c, config->addr, &tmp[0], 1, true);
    i2c_read_blocking(config->i2c, config->addr, &buf, 1, false);
    if (buf != 0x10) return PICO_ERROR_GENERIC;

    // 104 ms per measure, 5 cm presision
    spl06_config_prs(config, 4, 64);

    spl06_set_mode(config, mode);


    return 0;
}

void spl06_set_mode(spl06_config_t *config, spl06_mode_t mode) {
    uint8_t tmp[2];

    tmp[0] = SPL06_MEAS_CFG;
    switch (mode) {
    case STANDBY:
        tmp[1] = 0x00;
        break;

    case CMD_PRS:
        tmp[1] = 0x01;
        break;
        
    case CMD_TEMP:
        tmp[1] = 0x02;
        break;

    case BGD_PRS:
        tmp[1] = 0x05;
        break;
    
    case BGD_TEMP:
        tmp[1] = 0x06;
        break;
    
    case BGD_PRS_TEMP:
        tmp[1] = 0x07;
    
    default:
        break;
    }
    i2c_write_blocking(config->i2c, config->addr, tmp, 2, false);
}

int8_t spl06_config_prs(spl06_config_t *config, uint8_t rate, uint8_t oversample) {
    uint8_t tmp[2];
    uint16_t time; // mesurement time(ms)

    tmp[0] = SPL06_PRS_CFG;
    switch (oversample) {
    // moversampling times
    case 1:
        tmp[1] = 0x00;
        time = 4;
        break;
    case 2:
        tmp[1] = 0x01;
        time = 5;
        break;
    case 4:
        tmp[1] = 0x02;
        time = 8;
        break;
    case 8:
        tmp[1] = 0x03;
        time = 15;
        break;
    case 16:
        tmp[1] = 0x04;
        time = 28;
        break;
    case 32:
        tmp[1] = 0x05;
        time = 53;
        break;
    case 64:
        tmp[1] = 0x06;
        time = 104;
        break;
    case 128:
        tmp[1] = 0x07;
        time = 207;
        break;
    default:
        break;
    }

    if ((time * rate) > 1000) return PICO_ERROR_GENERIC;

    switch (rate) {
    // mesurements per second
    case 1:
        tmp[1] |= 0x00;
        break;
    case 2:
        tmp[1] |= 0x10;
        break;
    case 4:
        tmp[1] |= 0x20;
        break;
    case 8:
        tmp[1] |= 0x30;
        break;
    case 16:
        tmp[1] |= 0x40;
        break;
    case 32:
        tmp[1] |= 0x50;
        break;
    case 64:
        tmp[1] |= 0x60;
        break;
    case 128:
        tmp[1] |= 0x70;
        break;
    default:
        break;
    }
    
    i2c_write_blocking(config->i2c, config->addr, tmp, 2, false);

    if (oversample > 8) {
        tmp[0] = SPL06_CFG_REG;
        tmp[1] = 0x04;
        i2c_write_blocking(config->i2c, config->addr, tmp, 2, false);
    }

    return 0;
}

void spl06_config_temp(spl06_config_t *config, uint8_t rate) {
    uint8_t tmp[2];

    tmp[0] = SPL06_TMP_CFG;
    switch (rate) {
    // mesurements per second
    case 1:
        tmp[1] = 0x00;
        break;
    case 2:
        tmp[1] = 0x10;
        break;
    case 4:
        tmp[1] = 0x20;
        break;
    case 8:
        tmp[1] = 0x30;
        break;
    case 16:
        tmp[1] = 0x40;
        break;
    case 32:
        tmp[1] = 0x50;
        break;
    case 64:
        tmp[1] = 0x60;
        break;
    case 128:
        tmp[1] = 0x70;
        break;
    default:
        break;
    }
    i2c_write_blocking(config->i2c, config->addr, tmp, 2, false);

}

void spl06_read_coef(spl06_config_t *config, spl06_coef_t *coef) {
    uint8_t reg = SPL06_COEF;
    uint8_t buf[18];

    i2c_write_blocking(config->i2c, config->addr, &reg, 1, true);
    i2c_read_blocking(config->i2c, config->addr, buf, 18, false);

    // temp
    // c0: 12bit
    coef->c0  = (((int16_t)buf[0]) << 4);
    coef->c0 |= ((buf[1] >> 4) & 0x0F);
    if (buf[0] & 0x80) coef->c0 |= 0xF000;

    // c1: 12bit
    coef->c1  = (((int16_t)(buf[1] & 0x0F)) << 8);
    coef->c1 |= buf[2];
    if (buf[0] & 0x08) coef->c1 |= 0xF000;

    // c00: 20bit
    coef->c00  = (((int32_t)buf[3]) << 12);
    coef->c00 |= (((int32_t)buf[4]) << 4);
    coef->c00 |= ((buf[5] >> 4) & 0x0F);
    if (buf[3] & 0x80) coef->c00 |= 0xFFF00000;

    // c10: 20bit
    coef->c10  = (((int32_t)(buf[5] & 0x0F)) << 16);
    coef->c10 |= (((int32_t)buf[6]) << 8);
    coef->c10 |= buf[7];
    if (buf[5] & 0x08) coef->c10 |= 0xFFF00000;

    // c01: 16bit
    coef->c01  = (((int16_t)buf[8]) << 8);
    coef->c01 |= buf[9];

    // c11: 16bit
    coef->c11  = (((int16_t)buf[10]) << 8);
    coef->c11 |= buf[11];

    // c20: 16bit
    coef->c20  = (((int16_t)buf[12]) << 8);
    coef->c20 |= buf[13];

    // c21: 16bit
    coef->c21  = (((int16_t)buf[14]) << 8);
    coef->c21 |= buf[15];

    // c30: 16bit
    coef->c30  = (((int16_t)buf[16]) << 8);
    coef->c30 |= buf[17];
}

void spl06_read_press(spl06_config_t *config, int32_t *prs) {
    uint8_t reg;
    uint8_t buf[3];
    *prs = 0;

    reg = SPL06_PRS_B0;
    i2c_write_blocking(config->i2c, config->addr, &reg, 1, true);
    i2c_read_blocking(config->i2c, config->addr, buf, 3, false);

    *prs |= buf[2];
    *prs |=  (((int32_t)buf[1]) << 8);
    *prs |=  (((int32_t)buf[0]) << 16);
    if (buf[0] & 0x80) *prs |= 0xFF000000;
}

void spl06_read_temp(spl06_config_t *config, int32_t *temp) {
    uint8_t reg;
    uint8_t buf[3];
    *temp = 0;

    reg = SPL06_TMP_B0;
    i2c_write_blocking(config->i2c, config->addr, &reg, 1, true);
    i2c_read_blocking(config->i2c, config->addr, buf, 3, false);

    *temp |= buf[2];
    *temp |=  (((int32_t)buf[1]) << 8);
    *temp |=  (((int32_t)buf[0]) << 16);
    if (buf[0] & 0x80) *temp |= 0xFF000000;
}