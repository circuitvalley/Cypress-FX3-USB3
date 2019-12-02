/*
 ## Cypress FX3 Camera Kit source file (sensor.c)
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

/* This file implements the I2C based driver for the MT9M114 image sensor used
   in the FX3 HD 720p camera kit.

   Please refer to the Aptina MT9M114 sensor datasheet for the details of the
   I2C commands used to configure the sensor.
 */

#include <cyu3system.h>
#include <cyu3os.h>
#include <cyu3dma.h>
#include <cyu3error.h>
#include <cyu3uart.h>
#include <cyu3i2c.h>
#include <cyu3types.h>
#include <cyu3gpio.h>
#include <cyu3utils.h>
#include "sensor.h"


/* This function inserts a delay between successful I2C transfers to prevent
   false errors due to the slave being busy.
 */
static void
SensorI2CAccessDelay (
        CyU3PReturnStatus_t status)
{
    /* Add a 10us delay if the I2C operation that preceded this call was successful. */
   // if (status == CY_U3P_SUCCESS)
        CyU3PBusyWait (10);
}

CyU3PReturnStatus_t sensor_i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t data)
{
	CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
	CyU3PI2cPreamble_t  preamble;
	uint8_t buf[2];

	/* Validate the I2C slave address. */
	if ((slave_addr != SENSOR_ADDR_WR) && (slave_addr != I2C_MEMORY_ADDR_WR))
	{
		CyU3PDebugPrint (4, "I2C Slave address is not valid!\n");
		return 1;
	}

	/* Set the parameters for the I2C API access and then call the write API. */
	preamble.buffer[0] = slave_addr;
	preamble.buffer[1] = reg_addr;
	preamble.length    = 2;             /*  Three byte preamble. */
	preamble.ctrlMask  = 0x0000;        /*  No additional start and stop bits. */
	buf[0] = data;
	apiRetStatus = CyU3PI2cTransmitBytes (&preamble, buf, 1, 0);
	SensorI2CAccessDelay (apiRetStatus);

	return apiRetStatus;
}

CyU3PReturnStatus_t sensor_i2c_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *buff)
{
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t preamble;

    if ((slave_addr != SENSOR_ADDR_RD) && (slave_addr != I2C_MEMORY_ADDR_RD))
    {
        CyU3PDebugPrint (4, "I2C Slave address is not valid!\n");
        return 1;
    }
	preamble.length    = 3;
    preamble.buffer[0] = slave_addr & I2C_SLAVEADDR_MASK;        /*  Mask out the transfer type bit. */
    preamble.buffer[1] = reg_addr;
    preamble.buffer[2] = slave_addr ;
    preamble.ctrlMask  = 1<<1;                                /*  Send start bit after second byte of preamble. */

   // apiRetStatus = CyU3PI2cReceiveBytes (&preamble, &buff, 1, 0);
   // SensorI2CAccessDelay (apiRetStatus);


    return apiRetStatus;

}

/*
 * Reset the MT9M114 sensor using GPIO.
 */
void
SensorReset (
        void)
{
    CyU3PReturnStatus_t apiRetStatus;

    /* Drive the POWE DOWN GPIO low to the sensor. */
     apiRetStatus = CyU3PGpioSetValue (SENSOR_PWDN_GPIO, CyFalse);
     if (apiRetStatus != CY_U3P_SUCCESS)
     {
         CyU3PDebugPrint (4, "GPIO Set Value Error, Error Code = %d\n", apiRetStatus);
         return;
     }

    /* Drive the GPIO low to reset the sensor. */
    apiRetStatus = CyU3PGpioSetValue (SENSOR_RESET_GPIO, CyFalse);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint (4, "GPIO Set Value Error, Error Code = %d\n", apiRetStatus);
        return;
    }

    /* Wait for some time to allow proper reset. */
    CyU3PThreadSleep (10);

    /* Drive the GPIO high to bring the sensor out of reset. */
    apiRetStatus = CyU3PGpioSetValue (SENSOR_RESET_GPIO, CyTrue);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint (4, "GPIO Set Value Error, Error Code = %d\n", apiRetStatus);
        return;
    }

    /* Delay the allow the sensor to power up. */
    CyU3PThreadSleep (10);
    return;
}


#define _countof(array) (sizeof(array) / sizeof(array[0]))


void OV7670_init(void) {
	uint16_t i = 0;

	// Configure camera registers
	for (i = 0; i < _countof(OV7670_reg_vga); i++)
	{
		sensor_i2c_write(SENSOR_ADDR_WR, OV7670_reg_vga[i][0], OV7670_reg_vga[i][1]);

	}

}


/* MT9M114 sensor initialization sequence.
   Adapted from MT9M114-REV2.ini file provided by Aptina.

   Step1_Post_Reset       : Post-Reset Settings
   Step2_PLL_Timing       : PLL and Timing settings
   Step3_Recommended      : Patch, Errata and Sensor optimization Setting
   Step4_APGA             : APGA
   Step5_AWB_CCM          : AWB & CCM
   Step7_PIPE_Preference  : Color Pipe preference settings
   Step8_Features         : Ports, special features etc.
   Select Video Resolution
   SensorChangeConfig     : Update sensor configuration.
*/
void
SensorInit (
        void)
{

    SensorReset();


    if (SensorI2cBusTest () != CY_U3P_SUCCESS)        /* Verify that the sensor is connected. */
    {
        CyU3PDebugPrint (4, "Error: Reading Sensor ID failed!\r\n");
        return;
    }

    OV7670_init();

}

/*
 * Verify that the sensor can be accessed over the I2C bus from FX3.
 */
uint8_t
SensorI2cBusTest (
        void)
{
    /* The sensor ID register can be read here to verify sensor connectivity. */
    uint8_t buf[2];

    /* Reading sensor ID */
    if (sensor_i2c_read (SENSOR_ADDR_RD, 0x00, buf) == CY_U3P_SUCCESS)
    {
        //if ((buf[0] == 0x24))
        {
            return CY_U3P_SUCCESS;
        }
    }
    return 1;
}


/*
   Get the current brightness setting from the MT9M114 sensor.
 */
uint8_t
SensorGetBrightness (
        void)
{
    uint8_t buf[2];

    sensor_i2c_read (SENSOR_ADDR_RD, 0xCC, buf);
    return (uint8_t)buf[1];
}

/* TODO #2-2 sensor function to read gain register */
/* Copy the SensorGetBrightness function (right aobve this comment),
 * paste it below this comment and rename it to SensorGetGain.
 * Change the register address accessed to 0xCC12.
 */

/*
   Update the brightness setting for the MT9M114 sensor.
 */
void
SensorSetBrightness (
        uint8_t input)
{
	sensor_i2c_write (SENSOR_ADDR_WR, 0xCC, input);
}

/* TODO #2-3 sensor function to write gain register */
/* Copy the SensorSetBrightness function (right above),
 * paste it below this comment and rename it to SensorSetGain.
 * Change the register address accessed to 0xCC12.
 */


