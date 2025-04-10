#include "driver_i2c.h"
#include "user_misc.h"


DriverI2C_Struct drv_iic[IIC_MAX] = {
	{IIC_SCL1_PORT, IIC_SCL1_PIN, IIC_SCL1_P_P, IIC_SDA1_PORT, IIC_SDA1_PIN, IIC_SDA1_P_P},
	{IIC_SCL2_PORT, IIC_SCL2_PIN, IIC_SCL2_P_P, IIC_SDA2_PORT, IIC_SDA2_PIN, IIC_SDA2_P_P}
};






void DriverI2C_SCL_HIGH(SoftIICID iic)
{
	if (iic == I2C_1)
	{
		IIC_SCL1_P_P = 1;
	}
	else if (iic == I2C_2)
	{
		IIC_SCL2_P_P = 1;
	}
}
void DriverI2C_SCL_LOW(SoftIICID iic)
{
	if (iic == I2C_1)
	{
		IIC_SCL1_P_P = 0;
	}
	else if (iic == I2C_2)
	{
		IIC_SCL2_P_P = 0;
	}
}
void DriverI2C_SDA_HIGH(SoftIICID iic)
{
	if (iic == I2C_1)
	{
		IIC_SDA1_P_P = 1;
	}
	else if (iic == I2C_2)
	{
		IIC_SDA2_P_P = 1;
	}
}
void DriverI2C_SDA_LOW(SoftIICID iic)
{
	if (iic == I2C_1)
	{
		IIC_SDA1_P_P = 0;
	}
	else if (iic == I2C_2)
	{
		IIC_SDA2_P_P = 0;
	}
}

uint8_t DriverI2C_SDA_READ(SoftIICID iic)
{
	uint8_t BitValue = 0;
	if (iic == I2C_1)
	{
		BitValue = IIC_SDA1_P_P;
	}
	else if (iic == I2C_2)
	{
		BitValue = IIC_SDA2_P_P;
	}
	return BitValue;
}



void DriverI2C_Init(SoftIICID iic)
{

	GPIO_SetMode(drv_iic[iic].scl_port, drv_iic[iic].scl_port, GPIO_PMD_OPEN_DRAIN);
	GPIO_SetMode(drv_iic[iic].sda_port, drv_iic[iic].sda_port, GPIO_PMD_OPEN_DRAIN);

	drv_iic[iic].scl_port_pin = 1;
	drv_iic[iic].sda_port_pin = 1;


	// GPIO_SetMode(IIC_SCL1_PORT, IIC_SCL1_PIN, GPIO_PMD_OPEN_DRAIN);
	// GPIO_SetMode(IIC_SDA1_PORT, IIC_SDA1_PIN, GPIO_PMD_OPEN_DRAIN);
	// GPIO_SetMode(IIC_SCL2_PORT, IIC_SCL2_PIN, GPIO_PMD_OPEN_DRAIN);
	// GPIO_SetMode(IIC_SDA2_PORT, IIC_SDA2_PIN, GPIO_PMD_OPEN_DRAIN);

	// IIC_SCL1_P_P = 1;
	// IIC_SDA1_P_P = 1;
	// IIC_SCL2_P_P = 1;
	// IIC_SDA2_P_P = 1;

}



void DriverI2C_Start(SoftIICID iic)
{
	DriverI2C_SDA_HIGH(iic);
	DriverI2C_SCL_HIGH(iic);
	Delayus(5);
	DriverI2C_SDA_LOW(iic);
	Delayus(5);
	DriverI2C_SCL_LOW(iic);
	Delayus(5);
}

void DriverI2C_Stop(SoftIICID iic)
{
	DriverI2C_SDA_LOW(iic);
	DriverI2C_SCL_HIGH(iic);
	Delayus(5);
	DriverI2C_SDA_HIGH(iic);
	Delayus(5);
}


void DriverI2C_SendByte(SoftIICID iic, uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if (Byte & (0x80 >> i))
		{
			DriverI2C_SDA_HIGH(iic);
		}
		else
		{
			DriverI2C_SDA_LOW(iic);
		}
		Delayus(2);
		DriverI2C_SCL_HIGH(iic);
		Delayus(5);
		DriverI2C_SCL_LOW(iic);
		Delayus(3);
	}
}

uint8_t DriverI2C_ReceiveByte(SoftIICID iic)
{
	uint8_t i, Byte = 0x00;
	DriverI2C_SDA_HIGH(iic);
	Delayus(5);
	for (i = 0; i < 8; i ++)
	{
		DriverI2C_SCL_HIGH(iic);
		Delayus(5);
		if (DriverI2C_SDA_READ(iic) == 1){Byte |= (0x80 >> i);}
		DriverI2C_SCL_LOW(iic);
		Delayus(5);
	}
	return Byte;



}
void DriverI2C_SendAck(SoftIICID iic, uint8_t AckBit)
{
	if (AckBit == 0)
	{
		DriverI2C_SDA_LOW(iic);
	}
	else
	{
		DriverI2C_SDA_HIGH(iic);
	}
	Delayus(1);
	DriverI2C_SCL_HIGH(iic);
	Delayus(5);
	DriverI2C_SCL_LOW(iic);
	Delayus(5);
}

uint8_t DriverI2C_ReceiveAck(SoftIICID iic)
{
	uint8_t AckBit;
	DriverI2C_SDA_HIGH(iic);
	Delayus(5);
	DriverI2C_SCL_HIGH(iic);
	Delayus(5);
	AckBit = DriverI2C_SDA_READ(iic);
	DriverI2C_SCL_LOW(iic);
	Delayus(5);
	return AckBit;
}

