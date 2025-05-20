#ifndef ___HAL_IFLASH_H_
#define ___HAL_IFLASH_H_


#include <stdint.h>


#define CONFIG0_DFEN                0x01
#define CONFIG0_DFVSEN              0x04

#define DATA_FLASH_BASE        (0xF000)
#define DATA_FLASH_SIZE        (0x1000)  // 4k bytes


#define FLASH_ERASE_SIZE         (0x200)  // 512 bytes



#define DATA_PAGE0_ADDRESS  (0xF000)
#define DATA_PAGE1_ADDRESS  (0xF200)
#define DATA_PAGE2_ADDRESS  (0xF400)
#define DATA_PAGE3_ADDRESS  (0xF600)
#define DATA_PAGE4_ADDRESS  (0xF800)
#define DATA_PAGE5_ADDRESS  (0xFA00)
#define DATA_PAGE6_ADDRESS  (0xFC00)
#define DATA_PAGE7_ADDRESS  (0xFE00)

#define DATA_PAGE_END_ADDRESS  (0xFFFF)


typedef enum
{
    MB_BAUD_1200 = 1200,
    MB_BAUD_2400 = 2400,
    MB_BAUD_4800 = 4800,
    MB_BAUD_9600 = 9600,
    MB_BAUD_19200 = 19200,
    MB_BAUD_38400 = 38400,
    MB_BAUD_57600 = 57600,
    MB_BAUD_115200 = 115200,
    MB_BAUD_230400 = 230400,
    MB_BAUD_460800 = 460800,
    MB_BAUD_921600 = 921600,
}eMbBaudrate;




typedef struct 
{
    eMbBaudrate baudrate;
    uint8_t slave_address;
}MtModbus_t;



typedef struct 
{
   uint32_t nvdata_ver;
   MtModbus_t nvdata_mb; 
}DeviceNVData_t;


void HalIflashInit(void);
int8_t HalIflashWriteBufInOnePage(uint32_t u32Addr, uint32_t* u32Data, uint16_t u32Count);
int8_t HalIflashReadInOnePage(uint32_t addr, uint32_t* data, uint16_t len);
void HalIflashSaveData10ms(void);
extern DeviceNVData_t g_nv_data;
extern uint8_t flag_iflash_update;
#endif
