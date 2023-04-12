#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../src/pico-spl06-001.h"

spl06_config_t config = {0x77, i2c0, BGD_PRS_TEMP, {4, 64}, {4, 8}};
spl06_coef_t coef;
float prs;
float temp;

int main(void) {
    stdio_init_all();
    sleep_ms(2000);
    printf("\nGoertek SPL06-001 \n");

    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(12, GPIO_FUNC_I2C);
    gpio_set_function(13, GPIO_FUNC_I2C);

    spl06_init(&config, &coef);

    while (1) {
        spl06_read_press_cal(&config, &coef, &prs);
        spl06_read_temp_cal(&config, &coef, &temp);
        printf("prs: %4.2f Pa, temp: %2.2f degC\n", prs, temp);
        sleep_ms(150);
    }
}