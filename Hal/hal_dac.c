#include "hal_dac.h"
#include "driver_i2c.h"
#include "dev_dac5571.h"
#include "user_misc.h"
#include "stdio.h"

uint16_t dac_voltage[2];

uint8_t HalDacInit(void)
{
    DriverI2C_Init();
    
    return RETURN_OK;
}




uint8_t HalDac1SetVoltage(float vol)
{
    if(vol < 0 || vol > 5) // Check if voltage is within range
    {
        printf("Error: Voltage out of range (0-5V)\n");
        return RETURN_FAIL;
    }
    uint8_t digital_value = 0;
    uint8_t i2c_data[3] = {0};
    digital_value = (uint8_t)(vol * 255 / 5); // Convert voltage to digital value (0-255)

    i2c_data[0] = (U1_DAC5571_ADDR<<1) & 0xFE; // Command to write data
    i2c_data[1] = digital_value>>4 & 0x0F; // DAC channel 1
    i2c_data[2] = digital_value<<4; // Digital value for DAC channel 1

    DriverI2C_Start(I2C_1); // Start I2C communication
    DriverI2C_SendByte(I2C_1, i2c_data[0]); // Send I2C address
    DriverI2C_ReceiveAck(I2C_1); // Wait for ACK
    DriverI2C_SendByte(I2C_1, i2c_data[1]); // Send data high byte
    DriverI2C_ReceiveAck(I2C_1); // Wait for ACK
    DriverI2C_SendByte(I2C_1, i2c_data[2]); // Send data low byte
    DriverI2C_ReceiveAck(I2C_1); // Wait for ACK
    DriverI2C_Stop(I2C_1); // Stop I2C communication
    printf("DAC1 voltage set to: %f V\n", vol); // Debug print statement
    //dac_voltage [0] =(uint16_t) (vol*1000); // Store the voltage value in the array
    return RETURN_OK;
}

uint8_t HalDac2SetVoltage(float vol)
{
    if(vol < 0 || vol > 5) // Check if voltage is within range
    {
        printf("Error: Voltage out of range (0-5V)\n");
        return RETURN_FAIL;
    }
    uint8_t digital_value = 0;
    uint8_t i2c_data[3] = {0};
    digital_value = (uint8_t)(vol * 255 / 5); // Convert voltage to digital value (0-255)

    i2c_data[0] = (U2_DAC5571_ADDR<<1) & 0xFE; // Command to write data
    i2c_data[1] = digital_value>>4 & 0x0F; // DAC channel 2
    i2c_data[2] = digital_value<<4; // Digital value for DAC channel 2

    DriverI2C_Start(I2C_2); // Start I2C communication
    DriverI2C_SendByte(I2C_2, i2c_data[0]); // Send I2C address
    DriverI2C_ReceiveAck(I2C_2); // Wait for ACK
    DriverI2C_SendByte(I2C_2, i2c_data[1]); // Send data high byte
    DriverI2C_ReceiveAck(I2C_2); // Wait for ACK
    DriverI2C_SendByte(I2C_2, i2c_data[2]); // Send data low byte
    DriverI2C_ReceiveAck(I2C_2); // Wait for ACK
    DriverI2C_Stop(I2C_2); // Stop I2C communication
    printf("DAC2 voltage set to: %f V\n", vol); // Debug print statement
    //dac_voltage [1] = (uint16_t) (vol*1000);// Store the voltage value in the array

    return RETURN_OK;
}
