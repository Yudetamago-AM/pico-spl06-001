#pragma once

// Pressure Value
// 24bit(3bytes) 2's complement value
#define SPL06_PRS_B2 0x00 // MSB
#define SPL06_PRS_B1 0x01 // LSB
#define SPL06_PRS_B0 0x02 // XLSB

// Pressure Config
// 7   X
// 6:4 PM_RATE(2bytes)
// 3:0 PM_PRC (3bytes)
#define SPL06_PRS_CFG 0x06 

// Temperature Value
// 24bit(3bytes) 2's complement value
#define SPL06_TMP_B2 0x03 // MSB
#define SPL06_TMP_B1 0x04 // LSB
#define SPL06_TMP_B0 0x05 // XLSB

// Temperature Config
// 7   TMP_EXT(1byte)
// 6:4 TMP_RATE(2bytes)
// 3   X
// 2:0 TMP_PRC(3bytes)
#define SPL06_TMP_CFG 0x07

// Sensor Operating Mode and 

