#ifndef __LED_H__
#define __LED_H__


#undef  EXTERN

#ifdef  __LED_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


#define IndicateLEDON       GPIO_PinOutClear( INDICATE_LED_PORT, INDICATE_LED_PIN)
#define IndicateLEDOFF      GPIO_PinOutSet(   INDICATE_LED_PORT, INDICATE_LED_PIN)
#define IndicateLEDToggle   GPIO_PinOutToggle(INDICATE_LED_PORT, INDICATE_LED_PIN)


EXTERN void InitLED(void);
EXTERN void LED_Handler(void);


#endif


