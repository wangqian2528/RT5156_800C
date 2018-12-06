#ifndef __BOARD_H__
#define __BOARD_H__


#undef  EXTERN

#ifdef  __BOARD_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


EXTERN void InitBoard(void);
EXTERN uint8_t BSP_SitDetected(void);


#endif

