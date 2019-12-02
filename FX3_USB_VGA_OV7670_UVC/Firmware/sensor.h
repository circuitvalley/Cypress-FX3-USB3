/*
 ## Cypress FX3 Camera Kit header file (sensor.h)
 ## ===========================
 ##
 ##  Copyright Cypress Semiconductor Corporation, 2010-2012,
 ##  All Rights Reserved
 ##  UNPUBLISHED, LICENSED SOFTWARE.
 ##
 ##  CONFIDENTIAL AND PROPRIETARY INFORMATION
 ##  WHICH IS THE PROPERTY OF CYPRESS.
 ##
 ##  Use of this file is governed
 ##  by the license agreement included in the file
 ##
 ##     <install>/license/license.txt
 ##
 ##  where <install> is the Cypress software
 ##  installation root directory path.
 ##
 ## ===========================
*/

/* This file defines the parameters and the interface for the MT9M114 image
   sensor driver.
 */

#ifndef _INCLUDED_SENSOR_H_
#define _INCLUDED_SENSOR_H_

#include <cyu3types.h>

/* The SADDR line allows MT9M114 image sensor to select between two different I2C slave address.
   If the SADDR line is high, enable this #define to allow access to the correct I2C address for the sensor.
 */
/* #define SADDR_HIGH */

/* I2C Slave address for the image sensor. */
#ifdef SADDR_HIGH
#define SENSOR_ADDR_WR 0xBA             /* Slave address used to write sensor registers. */
#define SENSOR_ADDR_RD 0xBB             /* Slave address used to read from sensor registers. */
#else
#define SENSOR_ADDR_WR 0x42             /* Slave address used to write sensor registers. */
#define SENSOR_ADDR_RD 0x43             /* Slave address used to read from sensor registers. */
#endif

#define I2C_SLAVEADDR_MASK 0xFE         /* Mask to get actual I2C slave address value without direction bit. */

#define I2C_MEMORY_ADDR_WR 0xA0         /* I2C slave address used to write to an EEPROM. */
#define I2C_MEMORY_ADDR_RD 0xA1         /* I2C slave address used to read from an EEPROM. */

/* GPIO 22 on FX3 is used to reset the Image sensor. */
#define DEBUG1_GPIO 	26
#define DEBUG_GPIO 	27
#define SENSOR_RESET_GPIO 	22
#define SENSOR_PWDN_GPIO 	17

/* Function    : SensorWrite2B
   Description : Write two bytes of data to image sensor over I2C interface.
   Parameters  :
                 slaveAddr - I2C slave address for the sensor.
                 highAddr  - High byte of memory address being written to.
                 lowAddr   - Low byte of memory address being written to.
                 highData  - High byte of data to be written.
                 lowData   - Low byte of data to be written.
 */
extern CyU3PReturnStatus_t
SensorWrite2B (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t highData,
        uint8_t lowData);

/* Function    : SensorWrite
   Description : Write arbitrary amount of data to image sensor over I2C interface.
   Parameters  :
                 slaveAddr - I2C slave address for the sensor.
                 highAddr  - High byte of memory address being written to.
                 lowAddr   - Low byte of memory address being written to.
                 count     - Size of write data in bytes. Limited to a maximum of 64 bytes.
                 buf       - Pointer to buffer containing data.
 */
extern CyU3PReturnStatus_t
SensorWrite (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t count,
        uint8_t *buf);

/* Function    : SensorRead2B
   Description : Read 2 bytes of data from image sensor over I2C interface.
   Parameters  :
                 slaveAddr - I2C slave address for the sensor.
                 highAddr  - High byte of memory address being written to.
                 lowAddr   - Low byte of memory address being written to.
                 buf       - Buffer to be filled with data. MSB goes in byte 0.
 */
extern CyU3PReturnStatus_t
SensorRead2B (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t *buf);

/* Function    : SensorRead
   Description : Read arbitrary amount of data from image sensor over I2C interface.
   Parameters  :
                 slaveAddr - I2C slave address for the sensor.
                 highAddr  - High byte of memory address being written to.
                 lowAddr   - Low byte of memory address being written to.
                 count     = Size of data to be read in bytes. Limited to a max of 64.
                 buf       - Buffer to be filled with data.
 */
extern CyU3PReturnStatus_t
SensorRead (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t count,
        uint8_t *buf);

/* Function    : SensorInit
   Description : Initialize the MT9M114 sensor.
   Parameters  : None
 */
extern void
SensorInit (
        void);

/* Function    : SensorReset
   Description : Reset the MT9M114 image sensor using FX3 GPIO.
   Parameters  : None
 */
extern void
SensorReset (
        void);




/* Function    : SensorI2cBusTest
   Description : Test whether the MT9M114 sensor is connected on the I2C bus.
   Parameters  : None
 */
extern uint8_t
SensorI2cBusTest (
        void);

/* Function    : SensorGetBrightness
   Description : Get the current brightness setting from the MT9M114 sensor.
   Parameters  : None
 */
extern uint8_t
SensorGetBrightness (
        void);

/* Function    : SensorSetBrightness
   Description : Set the desired brightness setting on the MT9M114 sensor.
   Parameters  :
                 brightness - Desired brightness level.
 */
extern void
SensorSetBrightness (
        uint8_t input);

/* TODO #2-4 Add gain control function definitions */
/* Copy the SensorGetBrightness and SensorSetBrightness function definitions from above
 * and paste them below this comment.
 * Rename the functions to SensorGetGain and SensorSetGain respectively.
 */

static const uint8_t OV7670_reg_vga[][2] = {
    {0x3a, 0x04},
    {0x40, 0xC0},
    {0x12, 0x0},
    {0x32, 0x80},
    {0x17, 0x16},
    {0x18, 0x04},
    {0x19, 0x02},
    {0x1a, 0x7b},
    {0x03, 0x06},
    {0x0c, 0x00},
    {0x3e, 0x00},
    {0x70, 0x3A},
    {0x71, 0x35},
    {0x72, 0x11},
    {0x73, 0xF0},
    {0xa2, 0x02},
    {0x11, 0x01},  //CLKRC Prescaler settings
    {0x7a, 0x20},
    {0x7b, 0x1c},
    {0x7c, 0x28},
    {0x7d, 0x3c},
    {0x7e, 0x55},
    {0x7f, 0x68},
    {0x80, 0x76},
    {0x81, 0x80},
    {0x82, 0x88},
    {0x83, 0x8f},
    {0x84, 0x96},
    {0x85, 0xa3},
    {0x86, 0xaf},
    {0x87, 0xc4},
    {0x88, 0xd7},
    {0x89, 0xe8},
    {0x13, 0xe0},
    {0x00, 0x00},
    {0x10, 0x00},
    {0x0d, 0x00},
    {0x14, 0x28},
    {0xa5, 0x05},
    {0xab, 0x07},
    {0x24, 0x75},
    {0x25, 0x63},
    {0x26, 0xA5},
    {0x9f, 0x78},
    {0xa0, 0x68},
    {0xa1, 0x03},
    {0xa6, 0xdf},
    {0xa7, 0xdf},
    {0xa8, 0xf0},
    {0xa9, 0x90},
    {0xaa, 0x94},
    {0x13, 0xe5},
    {0x0e, 0x61},
    {0x0f, 0x4b},
    {0x16, 0x02},
    {0x1e, 0x17},
    {0x21, 0x02},
    {0x22, 0x91},
    {0x29, 0x07},
    {0x33, 0x0b},
    {0x35, 0x0b},
    {0x37, 0x1d},
    {0x38, 0x71},
    {0x39, 0x2a},
    {0x3c, 0x78},
    {0x4d, 0x40},
    {0x4e, 0x20},
    {0x69, 0x00},
    {0x6b, 0x40}, //DBLV PLL Control Bit[7:6] 0x40 --> x4 ,0x80 --> x6 , 0xC0 --> 0x8
    {0x74, 0x19},
    {0x8d, 0x4f},
    {0x8e, 0x00},
    {0x8f, 0x00},
    {0x90, 0x00},
    {0x91, 0x00},
    {0x92, 0x00},
    {0x96, 0x00},
    {0x9a, 0x80},
    {0xb0, 0x84},
    {0xb1, 0x0c},
    {0xb2, 0x0e},
    {0xb3, 0x82},
    {0xb8, 0x0a},
    {0x43, 0x14},
    {0x44, 0xf0},
    {0x45, 0x34},
    {0x46, 0x58},
    {0x47, 0x28},
    {0x48, 0x3a},
    {0x59, 0x88},
    {0x5a, 0x88},
    {0x5b, 0x44},
    {0x5c, 0x67},
    {0x5d, 0x49},
    {0x5e, 0x0e},
    {0x64, 0x04},
    {0x65, 0x20},
    {0x66, 0x05},
    {0x94, 0x04},
    {0x95, 0x08},
    {0x6c, 0x0a},
    {0x6d, 0x55},
    {0x6e, 0x11},
    {0x6f, 0x9f},
    {0x6a, 0x40},
    {0x01, 0x40},
    {0x02, 0x40},
    {0x13, 0xe7},
    {0x15, 0x02},
    {0x4f, 0x40},
    {0x50, 0x34},
    {0x51, 0x0C},
    {0x52, 0x17},
    {0x53, 0x29},
    {0x54, 0x40},
    {0x58, 0x1E},
    {0x41, 0x08},
    {0x3f, 0x00},
    {0x75, 0x05},
    {0x76, 0xe1},
    {0x4c, 0x00},
    {0x77, 0x01},
    {0x3d, 0xc2},
    {0x4b, 0x09},
    {0xc9, 0x60},
    {0x41, 0x38},
    {0x56, 0x40},
    {0x34, 0x11},
    {0x3b, 0x02},
    {0xa4, 0x89},
    {0x96, 0x00},
    {0x97, 0x30},
    {0x98, 0x20},
    {0x99, 0x30},
    {0x9a, 0x84},
    {0x9b, 0x29},
    {0x9c, 0x03},
    {0x9d, 0x4c},
    {0x9e, 0x3f},
    {0x78, 0x04},
    {0x79, 0x01},
    {0xc8, 0xf0},
    {0x79, 0x0f},
    {0xc8, 0x00},
    {0x79, 0x10},
    {0xc8, 0x7e},
    {0x79, 0x0a},
    {0xc8, 0x80},
    {0x79, 0x0b},
    {0xc8, 0x01},
    {0x79, 0x0c},
    {0xc8, 0x0f},
    {0x79, 0x0d},
    {0xc8, 0x20},
    {0x79, 0x09},
    {0xc8, 0x80},
    {0x79, 0x02},
    {0xc8, 0xc0},
    {0x79, 0x03},
    {0xc8, 0x40},
    {0x79, 0x05},
    {0xc8, 0x30},
    {0x79, 0x26},
    {0x09, 0x03},
    {0x3b, 0x42},
	{0xff, 0xff}
};


#endif /* _INCLUDED_SENSOR_H_ */

/*[]*/

