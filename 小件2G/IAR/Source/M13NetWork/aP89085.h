#ifndef __AP89085_H__
#define __AP89085_H__


#undef  EXTERN

#ifdef  __AP89085_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


#define aP89085_CS_CLR          GPIO_PinOutClear(aP89085_CS_PORT, aP89085_CS_PIN)
#define aP89085_CS_SET          GPIO_PinOutSet(aP89085_CS_PORT, aP89085_CS_PIN)

#define aP89085_SCK_CLR         GPIO_PinOutClear(aP89085_SCK_PORT, aP89085_SCK_PIN)
#define aP89085_SCK_SET         GPIO_PinOutSet(aP89085_SCK_PORT, aP89085_SCK_PIN)

#define aP89085_DI_CLR          GPIO_PinOutClear(aP89085_DI_PORT, aP89085_DI_PIN)
#define aP89085_DI_SET          GPIO_PinOutSet(aP89085_DI_PORT, aP89085_DI_PIN)

/*
aP89085 Command
PUP1(C5h)     : Power up the chip with NO ramp-up(suitable for VOUT direct drive)
PUP2(8Dh)     : Power up the chip with ramp-up(suitable for COUT transistor drive)
PDN1(E1h)     : Power down the chip with NO ramp-down(suitable for VOUT direct drive)
PDN2(A9h)     : Power down the chip with ramp-down(suitable for COUT transistor drive)
STATUS(E3h)   : Set output status for OUT2 pin
                OUT2 = BUSYB(000),8K(001),4K,2K,1K,16Hz,1M,EMPTY(FULLB)
PAUSE(39h)    : Pause the playback and hold at current COUT value
RESUME(1Dh)   : Resume playback from the previout COUT value
PREFETCH(71h) : Pre-load Voice Group address into buffer for next playback
*/
#define aP89085_CMD_PUP1        0xC5
#define aP89085_CMD_PUP2        0x8D
#define aP89085_CMD_PDN1        0xE1
#define aP89085_CMD_PDN2        0xA9
#define aP89085_CMD_STATUS      0xE3
#define aP89085_CMD_PAUSE       0x39
#define aP89085_CMD_RESUME      0x1D
#define aP89085_CMD_PREFETCH    0x71

/*aP89085 Voice Group Address*/
#define aP89085_VGA_00          0x00    /*Ä¦Ä¦ßÕ¸ÐÐ»ÄúµÄÌåÑé----------2s*/
#define aP89085_VGA_01          0x01    /*ÇëÉ¨ÂëÖ§¸¶£¬Ð»Ð»------------2s*/
#define aP89085_VGA_02          0x02    /*ÇëÍ¶±Ò¼ìÑé£¬Ð»Ð»------------2s*/
#define aP89085_VGA_03          0x03    /*Ó¢ÓïÌáÊ¾-For DUBAI----------4s*/
#define aP89085_VGA_04          0x04    /*ÆÏÌÑÑÀÓï-For BRAZIL---------5s*/
#define aP89085_VGA_05          0x05    /*°¢À­²®Óï-For DUBAI----------9s*/
#define aP89085_VGA_06          0x06    /*»¶Ó­À´µ½Ä¦Ä¦ßÕÎ¢SPA......--14s*/
#define aP89085_VGA_07          0x07    /*Õ¼×ù³¬Ê±£¬ÇëÉ¨ÂëÖ§¸¶£¬Ð»Ð»--4s*/
#define aP89085_VGA_08          0x08    /*ßÙßÙßÙ----------------------3s*/


#define aP89085_STEP_ALARM      10
#define aP89085_STEP_WELCOME    100


EXTERN void InitAP89085(void);
EXTERN void aP89085_Handler(void);
EXTERN void aP89085_Demo(uint8_t index);


#endif


