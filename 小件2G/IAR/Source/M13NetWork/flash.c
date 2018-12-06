#define __FLASH_C__


#include "flash.h"


uint8_t gucFlashPageData[FLASH_PAGE_SIZE];


void InitFLASH(void)
{
#if 1
    uint32_t i = 0;
#else
    uint32_t address  = 0;
    uint32_t i = 0, j = 0;
#endif


    /*������MSC�ķ��ʺ�����MSC��Flash������ʱ���׼*/
    MSC_Init();

    /*�����ں�����ʱ��Ƶ�ʸ���16MHz����32MHz�������Ҫ�����ȴ���������ΪWS1*/
    MSC->READCTRL |= MSC_READCTRL_MODE_WS1SCBTP;

    /*�������������󣬸�ʽ��SWAP��*/
    if(*((uint32_t *)M13NetWork_SWAP_START) != 0xFFFFFFFF)
    {
        for(i = 0; i < (M13NetWork_SWAP_SIZE / FLASH_PAGE_SIZE); i++)
        {
#if 1
            __disable_irq();

            MSC_ErasePage((uint32_t *)(M13NetWork_SWAP_START + FLASH_PAGE_SIZE * i));

            __enable_irq(); WDOG_Feed();
#else
            address = M13NetWork_SWAP_START + FLASH_PAGE_SIZE * i;

            for(j = 0; j < FLASH_PAGE_SIZE; j++)
            {
                LEUART_Tx(LEUART0, *((uint8_t *)(address + j))); WDOG_Feed();
            }
#endif
        }
    }
}


MSC_Status_TypeDef FLASH_WritePage(uint32_t address)
{
    MSC_Status_TypeDef retval = mscReturnOk;

    /*Flashд���߲��������У����������жϣ������Ӳ�����ϻ��ں�����*/
    __disable_irq();

    MSC_ErasePage((uint32_t *)address);

    retval = MSC_WriteWord((uint32_t *)address, gucFlashPageData, FLASH_PAGE_SIZE);

    /*������д������ɺ�ʹ���ж�*/
    __enable_irq();

    return retval;
}


MSC_Status_TypeDef FLASH_UpgradeProgram(uint8_t index)
{
    uint16_t i = 0, j = 0;
    uint32_t address  = 0;

    MSC_Status_TypeDef retval = mscReturnOk;

    address = M13NetWork_SWAP_START + index * FLASH_PAGE_SIZE;

    do
    {
        retval = FLASH_WritePage(address);

        if(j++ == 10) return mscReturnUnaligned;
        
        if(retval == mscReturnOk)
        {
            /*��д��������������ȶԣ�����в�һ�µ����ô����־λ*/
            for(i = 0; i < FLASH_PAGE_SIZE; i++)
            {
                if(gucFlashPageData[i] != *((uint8_t *)(address + i)))
                {
                    retval = mscReturnTimeOut;
                }
            }
        }

        WDOG_Feed();
    }while(retval != mscReturnOk);

    for(i = 0; i < FLASH_PAGE_SIZE; i++) gucFlashPageData[i] = 0xFF;

    return mscReturnOk;
}


void FLASH_Demo(void)
{
    uint16_t i = 0;

    for(i = 0; i < FLASH_PAGE_SIZE; i++)
    {
        gucFlashPageData[i] = i % 256;
    }

    FLASH_WritePage(M13NetWork_DATA_START);

#if 0
    for(i = 0; i < FLASH_PAGE_SIZE; i++)
    {
        WDOG_Feed();

        LEUART_Tx(LEUART0, *((uint8_t *)(M13NetWork_DATA_START + i)));
    }
#endif

    for(i = 0; i < FLASH_PAGE_SIZE; i++)
    {
        gucFlashPageData[i] = 0xFF;
    }
}


#if 0
uint32_t APPLICATION_START  = *((uint32_t *)0x00000000);
uint32_t MEM_INFO_PAGE_SIZE = *((uint32_t *)0x0FE081E7);
uint32_t UNIQUE_0           = *((uint32_t *)0x0FE081F0);
uint32_t UNIQUE_1           = *((uint32_t *)0x0FE081F4);
uint32_t MEM_INFO_FLASH     = *((uint32_t *)0x0FE081F8);
uint32_t MEM_INFO_RAM       = *((uint32_t *)0x0FE081FA);
#endif


