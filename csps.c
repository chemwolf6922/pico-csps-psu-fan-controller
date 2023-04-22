#include <stdlib.h>
#include <string.h>
#include "csps.h"
#include "hardware/i2c.h"

#define CSPS_REG_ADDR_BASE (0x58)
#define CSPS_ROM_ADDR_BASE (0x50)

typedef struct
{
    i2c_inst_t* i2c;
    int reg_addr;
    int rom_addr;
} csps_t;

csps_handle_t csps_new(i2c_inst_t* i2c, int addr)
{
    csps_t* this = malloc(sizeof(csps_t));
    if(!this)   
        goto error;
    memset(this, 0, sizeof(csps_t));
    this->i2c = i2c;
    this->reg_addr = CSPS_REG_ADDR_BASE + addr;
    this->rom_addr = CSPS_ROM_ADDR_BASE + addr;
    return (csps_handle_t)this;
error:
    csps_free((csps_handle_t)this);
    return NULL;
}

void csps_free(csps_handle_t handle)
{
    csps_t* this = (csps_t*)handle;
    if(this)
    {
        free(this);
    }
}

int csps_read_rom(csps_handle_t handle, int addr, uint8_t* dst, int len)
{
    csps_t* this = (csps_t*)handle;
    uint8_t cmd[] = {addr};
    if(i2c_write_blocking(this->i2c, this->rom_addr, cmd, sizeof(cmd), false) == PICO_ERROR_GENERIC)
        return -1;
    if(i2c_read_blocking(this->i2c, this->rom_addr, dst, len, false) == PICO_ERROR_GENERIC)
        return -1;
    return 0;
}

int csps_read_reg(csps_handle_t handle, int addr)
{
    csps_t* this = (csps_t*)handle;
    int check_sum = (this->reg_addr << 1) + addr;
    check_sum = ((0xff-check_sum)+1)&0xff;
    uint8_t cmd[] = {addr, check_sum};
    if(i2c_write_blocking(this->i2c, this->reg_addr, cmd, sizeof(cmd), false) == PICO_ERROR_GENERIC)
        return -1;
    uint8_t dst[3] = {0};
    if(i2c_read_blocking(this->i2c, this->reg_addr, dst, sizeof(dst), false) == PICO_ERROR_GENERIC)
        return -1;
    return (dst[1] << 8) | dst[0];
}

int csps_write_reg(csps_handle_t handle, int addr, int value)
{
    csps_t* this = (csps_t*)handle;
    int low = value & 0xFF;
    int high = (value >> 8) & 0xFF;
    int check_sum = (this->reg_addr << 1 ) + addr + high + low;
    check_sum = ((0xff-check_sum)+1)&0xff;
    uint8_t cmd[] = {addr, low, high, check_sum};
    if(i2c_write_blocking(this->i2c, this->reg_addr, cmd, sizeof(cmd), false) == PICO_ERROR_GENERIC)
        return -1;
    return 0;
}
