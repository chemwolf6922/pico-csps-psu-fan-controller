#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "csps.h"

#define CSPS_I2C (i2c0)
#define CSPS_SDA (4)
#define CSPS_SCL (5)

int main()
{

    stdio_init_all();

    i2c_init(CSPS_I2C, 100000);
    gpio_set_function(CSPS_SDA, GPIO_FUNC_I2C);
    gpio_set_function(CSPS_SCL, GPIO_FUNC_I2C);

    csps_handle_t csps = csps_new(CSPS_I2C, 7);
    if(!csps)
        goto finish;

    for(;;)
    {
        sleep_ms(100);
        int ret = csps_write_reg(csps, CSPS_REG_WRITE_FAN_RPM, 5000);
        // printf("result :%d\n", ret);
        // float fan_speed = csps_read_reg_by_name(csps, READ_FAN_RPM);
        // printf("Fan speed: %f rpm\n", fan_speed);
    }

finish:
    if(csps)
        csps_free(csps);
    gpio_deinit(CSPS_SDA);
    gpio_deinit(CSPS_SCL);
    i2c_deinit(CSPS_I2C);

    return 0;
}