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


void HalIflashInit(void);
int8_t HalIflashWriteBufInOnePage(uint32_t u32Addr, uint32_t* u32Data, uint16_t u32Count);
int8_t HalIflashReadInOnePage(uint32_t addr, uint32_t* data, uint16_t len);


#endif
