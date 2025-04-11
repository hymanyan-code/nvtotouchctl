#include "M0518.h"
#include "hal_iflash.h"
#include <stdio.h>


static int  SetDataFlashBase(uint32_t u32DFBA);
static int32_t  FlashTest(uint32_t u32StartAddr, uint32_t u32EndAddr, uint32_t u32Pattern);
#define TEST_PATTERN                0x5A5A5A5A

void HalIflashInit(void)
{
    uint32_t i, u32Data;
    SYS_UnlockReg();
    FMC_Open();

    if(SetDataFlashBase(DATA_FLASH_BASE) < 0)
    {
        printf("Failed to set Data Flash base address!\n");
    }

    /* Read BS */
    printf("  Boot Mode ............................. ");
    if(FMC_GetBootSource() == 0)
        printf("[APROM]\n");
    else
    {
        printf("[LDROM]\n");
        printf("  WARNING: The driver sample code must execute in AP mode!\n");

    }

    u32Data = FMC_ReadCID();
    printf("  Company ID ............................ [0x%08x]\n", u32Data);

    u32Data = FMC_ReadDID();
    printf("  Device ID ............................. [0x%08x]\n", u32Data);

    u32Data = FMC_ReadPID();
    printf("  Product ID ............................ [0x%08x]\n", u32Data);

    for(i = 0; i < 3; i++)
    {
        u32Data = FMC_ReadUID(i);
        printf("  Unique ID %d ........................... [0x%08x]\n", i, u32Data);
    }

    for(i = 0; i < 4; i++)
    {
        u32Data = FMC_ReadUCID(i);
        printf("  Unique Customer ID %d .................. [0x%08x]\n", i, u32Data);
    }

    /* Read User Configuration */
    printf("  User Config 0 ......................... [0x%08x]\n", FMC_Read(FMC_CONFIG_BASE));
    printf("  User Config 1 ......................... [0x%08x]\n", FMC_Read(FMC_CONFIG_BASE + 4));

    /* Read Data Flash base address */
    u32Data = FMC_ReadDataFlashBaseAddr();
    printf("  Data Flash Base Address ............... [0x%08x]\n", u32Data);
    
    printf("start flash test!\r\n");
    FlashTest(DATA_PAGE0_ADDRESS, DATA_PAGE_END_ADDRESS, TEST_PATTERN);





    /* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();

}


int8_t HalIflashWriteBufInOnePage(uint32_t u32Addr, uint32_t* u32Data, uint16_t u32Count)
{
    uint32_t i, u32DataTemp;
    int8_t ret = 0;


    if(u32Addr < DATA_PAGE0_ADDRESS || u32Addr > DATA_PAGE7_ADDRESS)
    {
        printf("Error: Address out of range!\n");
        return -1;
    }
    if(u32Addr % 4 != 0)
    {
        printf("Error: Address must be word alignment!\n");
        return -1;
    }
    if(u32Count > FMC_FLASH_PAGE_SIZE / 4)
    {
        printf("Error: Data count exceeds page size!\n");
        return -1;
    }
    if(u32Count == 0)
    {
        printf("Error: Data count must be greater than 0!\n");
        return -1;
    }


    /* Unlock protected registers */
    SYS_UnlockReg();
    FMC_Open();

    /* Erase page */
    FMC_Erase(u32Addr); /* Set Target ROM Address. The address must be page alignment. */

    for(i = 0; i < u32Count; i++)
    {
        u32DataTemp = u32Data[i];
        ret = FMC_Write(u32Addr + (i * 4), u32DataTemp);
        // if(FMC_Read(u32Addr + (i * 4)) != u32DataTemp)
        // {
        //     printf("Failed to write data!\n");
        //     return -1;
        // }
    }

    /* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();

    return ret;
}


int8_t HalIflashReadInOnePage(uint32_t addr, uint32_t* data, uint16_t len)
{
    uint32_t i;
    int8_t ret = 0;

    if(addr < DATA_PAGE0_ADDRESS || addr > DATA_PAGE7_ADDRESS)
    {
        printf("Error: Address out of range!\n");
        return -1;
    }
    if(addr % 4 != 0)
    {
        printf("Error: Address must be word alignment!\n");
        return -1;
    }
    if(len > FMC_FLASH_PAGE_SIZE / 4)
    {
        printf("Error: Data count exceeds page size!\n");
        return -1;
    }
    if(len == 0)
    {
        printf("Error: Data count must be greater than 0!\n");
        return -1;
    }

    /* Read data from flash */
    for(i = 0; i < len; i++)
    {
        data[i] = FMC_Read(addr + (i * 4));
    }

    return ret;
}



static int  SetDataFlashBase(uint32_t u32DFBA)
{
    uint32_t au32Config[2];

    /* Read current User Configuration */
    FMC_ReadConfig(au32Config, 1);

    /* Just return when Data Flash has been enabled */
    if(!(au32Config[0] & 0x1))
        return 0;

    /* Enable User Configuration Update */
    FMC_EnableConfigUpdate();

    /* Erase User Configuration */
    FMC_Erase(FMC_CONFIG_BASE);

    /* Write User Configuration to Enable Data Flash */
    /* Note: DFVSEN = 1, DATA Flash Size is 4K bytes
             DFVSEN = 0, DATA Flash Size is based on CONFIG1 */
    au32Config[0] &= ~(CONFIG0_DFEN | CONFIG0_DFVSEN);
    au32Config[1] = u32DFBA;

    if(FMC_WriteConfig(au32Config, 2))
        return -1;

    printf("\nSet Data Flash base as 0x%x.\n", FMC_ReadDataFlashBaseAddr());

    /* Perform chip reset to make new User Config take effect */
    SYS->IPRSTC1 |= SYS_IPRSTC1_CHIP_RST_Msk;

    return 0;
}





int32_t FillDataPattern(uint32_t u32StartAddr, uint32_t u32EndAddr, uint32_t u32Pattern)
{
    uint32_t u32Addr;

    for(u32Addr = u32StartAddr; u32Addr < u32EndAddr; u32Addr += 4)
    {
        FMC_Write(u32Addr, u32Pattern);
    }
    return 0;
}


int32_t  VerifyData(uint32_t u32StartAddr, uint32_t u32EndAddr, uint32_t u32Pattern)
{
    uint32_t    u32Addr;
    uint32_t    u32Data;

    for(u32Addr = u32StartAddr; u32Addr < u32EndAddr; u32Addr += 4)
    {
        u32Data = FMC_Read(u32Addr);
        if(u32Data != u32Pattern)
        {
            printf("\nFMC_Read data verify failed at address 0x%x, read=0x%x, expect=0x%x\n", u32Addr, u32Data, u32Pattern);
            return -1;
        }
    }
    return 0;
}




int32_t  FlashTest(uint32_t u32StartAddr, uint32_t u32EndAddr, uint32_t u32Pattern)
{
    uint32_t    u32Addr;

    for(u32Addr = u32StartAddr; u32Addr < u32EndAddr; u32Addr += FMC_FLASH_PAGE_SIZE)
    {
        printf("    Flash test address: 0x%x    \r", u32Addr);

        // Erase page
        FMC_Erase(u32Addr);/* Set Target ROM Address. The address must be page alignment. */

        // Verify if page contents are all 0xFFFFFFFF
        if(VerifyData(u32Addr, u32Addr + FMC_FLASH_PAGE_SIZE, 0xFFFFFFFF) < 0)
        {
            printf("\nPage 0x%x erase verify failed!\n", u32Addr);
            return -1;
        }

        // Write test pattern to fill the whole page
        if(FillDataPattern(u32Addr, u32Addr + FMC_FLASH_PAGE_SIZE, u32Pattern) < 0)
        {
            printf("Failed to write page 0x%x!\n", u32Addr);
            return -1;
        }

        // Verify if page contents are all equal to test pattern
        if(VerifyData(u32Addr, u32Addr + FMC_FLASH_PAGE_SIZE, u32Pattern) < 0)
        {
            printf("\nData verify failed!\n ");
            return -1;
        }

        FMC_Erase(u32Addr);

        // Verify if page contents are all 0xFFFFFFFF
        if(VerifyData(u32Addr, u32Addr + FMC_FLASH_PAGE_SIZE, 0xFFFFFFFF) < 0)
        {
            printf("\nPage 0x%x erase verify failed!\n", u32Addr);
            return -1;
        }
    }
    printf("\r    Flash Test Passed.          \n");
    return 0;
}
