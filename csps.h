#ifndef __CSPS_H
#define __CSPS_H

#include <stdint.h>
#include "hardware/i2c.h"

typedef void* csps_handle_t;

/**
 * @brief Crate a csps instance
 * 
 * @param i2c should already been inited
 * @param addr csps power supply address select bit 0-7
 * @return csps_handle_t
 */
csps_handle_t csps_new(i2c_inst_t* i2c, int addr);

/**
 * @brief Free a csps instance. The i2c hardware will not be freed.
 */
void csps_free(csps_handle_t handle);

#define CSPS_ROM_SPN_ADDR (0x12)
#define CSPS_ROM_SPN_LEN (0x0A)
#define CSPS_ROM_MFG_ADDR (0x1D)
#define CSPS_ROM_MFG_LEN (0x08)
#define CSPS_ROM_MFR_ADDR (0x2C)
#define CSPS_ROM_MFR_LEN (0x05)
#define CSPS_ROM_NAME_ADDR (0x32)
#define CSPS_ROM_NAME_LEN (0x1A)
#define CSPS_ROM_OKN_ADDR (0x4D)
#define CSPS_ROM_OKN_LEN (0x0A)
#define CSPS_ROM_CT_ADDR (0x5B)
#define CSPS_ROM_CT_LEN (0x0E)

/**
 * @brief Read data from rom
 * 
 * @param handle
 * @param addr Choose from CSPS_ROM_xxx_ADDR
 * @param dst MUST be at least len size
 * @param len MUST be paired with addr
 */
int csps_read_rom(csps_handle_t handle, int addr, uint8_t* dst, int len);

#define CSPS_REG_INPUT_VOLTAGE (0x08)
#define CSPS_REG_UNIT_INPUT_VOLTAGE (32.f)
#define CSPS_REG_INPUT_CURRENT (0x0A)
#define CSPS_REG_UNIT_INPUT_CURRENT (64.f)
#define CSPS_REG_INPUT_POWER (0x0C)
#define CSPS_REG_UNIT_INPUT_POWER (1.f)
#define CSPS_REG_OUTPUT_VOLTAGE (0x0E)
#define CSPS_REG_UNIT_OUTPUT_VOLTAGE (256.f)
#define CSPS_REG_OUTPUT_CURRENT (0x10)
#define CSPS_REG_UNIT_OUTPUT_CURRENT (64.f)
#define CSPS_REG_OUTPUT_POWER (0x12)
#define CSPS_REG_UNIT_OUTPUT_POWER (1.f)
#define CSPS_REG_TEMP1 (0x1A)
#define CSPS_REG_UNIT_TEMP1 (64.f)
#define CSPS_REG_TEMP2 (0x1C)
#define CSPS_REG_UNIT_TEMP2 (64.f)
#define CSPS_REG_READ_FAN_RPM (0x1E)
#define CSPS_REG_UNIT_READ_FAN_RPM (1.f)
#define CSPS_REG_RUN_TIME (0x30)
#define CSPS_REG_UNIT_RUN_TIME (1.f)

/**
 * @brief Read register.
 * 
 * @param handle
 * @param addr Choose from CSPS_REG_xxx
 * @return int The value read or -1 on error
 */
int csps_read_reg(csps_handle_t handle, int addr);

/**
 * @brief Read register by register name
 * 
 * @param handle
 * @param name 
 * @return float The value read or negative on error
 */
#define csps_read_reg_by_name(handle,name) \
    (((float)csps_read_reg(handle, CSPS_REG_##name))/CSPS_REG_UNIT_##name)


#define CSPS_REG_WRITE_FAN_RPM (0x40)

/**
 * @brief Write register
 * 
 * @param handle
 * @param addr
 * @param value
 * @return int 0 on success, -1 on error
 */
int csps_write_reg(csps_handle_t handle, int addr, int value);

#endif
