#ifndef __DRIVER_I2C_H
#define __DRIVER_I2C_H

#include "M0518.h"
#include "stdint.h"

#define IIC_SCL1_PORT             PB
#define IIC_SCL1_PIN              BIT1
#define IIC_SCL1_P_P              PB1

#define IIC_SDA1_PORT             PB
#define IIC_SDA1_PIN              BIT0  
#define IIC_SDA1_P_P              PB0





#define IIC_SCL2_PORT             PB
#define IIC_SCL2_PIN              BIT2
#define IIC_SCL2_P_P              PB2

#define IIC_SDA2_PORT             PB
#define IIC_SDA2_PIN              BIT3
#define IIC_SDA2_P_P              PB3




typedef enum
{
    I2C_1 = 0,
    I2C_2,
    IIC_MAX
}SoftIICID;


typedef struct 
{
    GPIO_T* scl_port;
    uint32_t scl_pin;
    volatile uint32_t* scl_port_pin;
    GPIO_T* sda_port;
    uint32_t sda_pin;
    volatile uint32_t* sda_port_pin;
}DriverI2C_Struct;















void DriverI2C_Init(SoftIICID iic);
void DriverI2C_Start(SoftIICID iic);
void DriverI2C_Stop(SoftIICID iic);
void DriverI2C_SendByte(SoftIICID iic, uint8_t Byte);
uint8_t DriverI2C_ReceiveByte(SoftIICID iic);
void DriverI2C_SendAck(SoftIICID iic, uint8_t AckBit);
uint8_t DriverI2C_ReceiveAck(SoftIICID iic);



#endif
