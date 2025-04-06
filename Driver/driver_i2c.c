#include "driver_i2c.h"
#include "user_misc.h"









void DriverI2C_SCL_HIGH(SoftIIC iic)
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
void DriverI2C_SCL_LOW(SoftIIC iic)
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
void DriverI2C_SDA_HIGH(SoftIIC iic)
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
void DriverI2C_SDA_LOW(SoftIIC iic)
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

uint8_t DriverI2C_SDA_READ(SoftIIC iic)
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



void DriverI2C_Init(void)
{
	GPIO_SetMode(IIC_SCL1_PORT, IIC_SCL1_PIN, GPIO_PMD_OPEN_DRAIN);
	GPIO_SetMode(IIC_SDA1_PORT, IIC_SDA1_PIN, GPIO_PMD_OPEN_DRAIN);
	GPIO_SetMode(IIC_SCL2_PORT, IIC_SCL2_PIN, GPIO_PMD_OPEN_DRAIN);
	GPIO_SetMode(IIC_SDA2_PORT, IIC_SDA2_PIN, GPIO_PMD_OPEN_DRAIN);

	IIC_SCL1_P_P = 1;
	IIC_SDA1_P_P = 1;
	IIC_SCL2_P_P = 1;
	IIC_SDA2_P_P = 1;

}



void DriverI2C_Start(SoftIIC iic)
{
	DriverI2C_SDA_HIGH(iic);
	DriverI2C_SCL_HIGH(iic);
	Delayus(5);
	DriverI2C_SDA_LOW(iic);
	Delayus(5);
	DriverI2C_SCL_LOW(iic);
	Delayus(5);
}

void DriverI2C_Stop(SoftIIC iic)
{
	DriverI2C_SDA_LOW(iic);
	DriverI2C_SCL_HIGH(iic);
	Delayus(5);
	DriverI2C_SDA_HIGH(iic);
	Delayus(5);
}


void DriverI2C_SendByte(SoftIIC iic, uint8_t Byte)
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

uint8_t DriverI2C_ReceiveByte(SoftIIC iic)
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
void DriverI2C_SendAck(SoftIIC iic, uint8_t AckBit)
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

uint8_t DriverI2C_ReceiveAck(SoftIIC iic)
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

