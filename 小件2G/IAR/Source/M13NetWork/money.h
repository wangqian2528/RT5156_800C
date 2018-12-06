#ifndef __MONEY_H__
#define __MONEY_H__


#undef  EXTERN

#ifdef  __MONEY_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


#define MONEY_SOI                       0xF1    /*与投币板通讯的起始标志位*/
#define MONEY_EOI                       0xF0    /*与投币板通讯的结束标志位*/

#define MONEY_CMD_QUERY_CURRENT         0x01    /*主机查询当前一次接收到的钱币数目(MAX:99999)*/
#define MONEY_CMD_QUERY_ALL             0x02    /*查询接收到的总钱币累计数目、总时间数、总识币张数(MAX:99999)*/
#define MONEY_CMD_CLEAR_ALL             0x03    /*清零总钱币累计数目、总时间数、总识币张数*/
#define MONEY_CMD_SET_RATE              0x04    /*设置费率*/
#define MONEY_CMD_QUERY_RATE            0x05    /*查询费率*/
#define MONEY_CMD_SET_DATE_TIME         0x06    /*设置年月日时分秒*/
#define MONEY_CMD_QUERY_DATE_TIME       0x07    /*查询年月日时分秒*/
#define MONEY_CMD_COIN_DISABLE          0x08    /*禁止投币功能，上电后默认为允许投币*/
#define MONEY_CMD_COIN_ENABLE           0x09    /*允许投币功能*/
#define MONEY_CMD_MEMORY_ENABLE         0x10    /*允许断电保存功能*/
#define MONEY_CMD_MEMORY_DISABLE        0x11    /*禁止断电保存功能，上电后默认为禁止断电保存*/

#define MONEY_CMD_UNLOCKING                     0x12    /*微信扫码开锁*/
#define MONEY_CMD_QUERY_UNLOCKING_STATUS        0x13    /*主机查询投币箱门锁的状态*/
#define MONEY_CMD_QUERY_UNLOCKING_TIMES         0x14    /*主机查询投币箱累计开锁次数*/
#define MONEY_CMD_CLEAR_LEGAL_UNLOCKING_TIMES   0x15    /*主机清零投币箱累计合法开锁次数*/
#define MONEY_CMD_CLEAR_ILLEGAL_UNLOCKING_TIMES 0x16    /*主机清零投币箱累计异常开锁次数*/


EXTERN void    InitMoney(void);
EXTERN uint8_t MONEY_GetWorkState(void);
EXTERN uint8_t MONEY_CalculateCRC(uint8_t *buffer, uint8_t length);
EXTERN uint8_t MONEY_CommandDequeue(uint8_t *buffer);
EXTERN void    MONEY_CommandEnqueue(uint8_t command, uint8_t *parameter, uint8_t length);
EXTERN void    MONEY_CommandParsing(uint8_t *buffer);
EXTERN void    MONEY_RX_Handler(void);
EXTERN void    MONEY_TX_Handler(void);
EXTERN void    MONEY_Demo(uint8_t index);
EXTERN void    MONEY_Handler(void);


#endif

