#ifndef __FLASH_H__
#define __FLASH_H__


#undef  EXTERN

#ifdef  __FLASH_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


#if 1

#define M13NetWork_BOOT_START   0x00000000  /*0x00000000 ~ 0x00001FFF : 8  * 1024*/ /*  0K~  8K*/
#define M13NetWork_CODE_START   0x00002000  /*0x00002000 ~ 0x0000E7FF : 50 * 1024*/ /*  8K~ 58K*/
#define M13NetWork_SWAP_START   0x0000E800  /*0x0000E800 ~ 0x0001AFFF : 50 * 1024*/ /* 58K~108K*/
#define M13NetWork_DATA_START   0x0001B000  /*0x0001B000 ~ 0x0001D7FF : 10 * 1024*/ /*108K~128K*/

#define M13NetWork_BOOT_SIZE    (8  * 1024) /*8K*/
#define M13NetWork_CODE_SIZE    (50 * 1024) /*50K*/
#define M13NetWork_SWAP_SIZE    (50 * 1024) /*50K*/
#define M13NetWork_DATA_SIZE    (10 * 1024) /*10K*/

#else

#define M13NetWork_BOOT_START   0x00000000  /*0x00000000 ~ 0x00001FFF : 8  * 1024*/
#define M13NetWork_APP1_START   0x00002000  /*0x00002000 ~ 0x0000E7FF : 50 * 1024*/
#define M13NetWork_APP2_START   0x0000E800  /*0x0000E800 ~ 0x0001AFFF : 50 * 1024*/
#define M13NetWork_NVM1_START   0x0001B000  /*0x0001B000 ~ 0x0001D7FF : 10 * 1024*/
#define M13NetWork_NVM2_START   0x0001D800  /*0x0001D800 ~ 0x0001FFFF : 10 * 1024*/

#define M13NetWork_BOOT_SIZE    (8  * 1024) /*8K*/
#define M13NetWork_APP1_SIZE    (50 * 1024) /*50K*/
#define M13NetWork_APP2_SIZE    (50 * 1024) /*50K*/
#define M13NetWork_NVM1_SIZE    (10 * 1024) /*10K*/
#define M13NetWork_NVM2_SIZE    (10 * 1024) /*10K*/

#endif


EXTERN void InitFLASH(void);
EXTERN void FLASH_Demo(void);
EXTERN MSC_Status_TypeDef FLASH_WritePage(uint32_t address);
EXTERN MSC_Status_TypeDef FLASH_UpgradeProgram(uint8_t index);


#endif


