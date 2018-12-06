#ifndef __MCU_H__
#define __MCU_H__


#undef  EXTERN

#ifdef  __MCU_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


#define DELAY_LEVEL_MS          1
#define DELAY_LEVEL_S           1000

#define MONIES_QUEUE_RX_IDX     0
#define MONIES_QUEUE_TX_IDX     1
#define MBCTRL_QUEUE_RX_IDX     2
#define MBCTRL_QUEUE_TX_IDX     3
#define SIM800_QUEUE_RX_IDX     4
#define SIM800_QUEUE_TX_IDX     5
#define BT_QUEUE_RX_IDX         6
#define BT_QUEUE_TX_IDX         7
#define QUEUE_NUMBER            8


EXTERN void    InitUSART2(uint8_t ucMode);
EXTERN void    InitMCU(void);
EXTERN uint8_t GetSystemRunFlag(void);
EXTERN void    delay_ms(uint32_t count);
EXTERN void    QUEUE_INIT(uint8_t index);
EXTERN uint8_t QUEUE_EMPTY(uint8_t index);
EXTERN uint8_t QUEUE_READ(uint8_t index);
EXTERN void    QUEUE_WRITE(uint8_t index, uint8_t value);


#endif


