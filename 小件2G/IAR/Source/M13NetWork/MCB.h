#ifndef __MCB_H__
#define __MCB_H__


#undef  EXTERN

#ifdef  __MCB_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


#define MCB_SOI                     0xF0    /*主控板通讯起始标志*/
#define MCB_EOI                     0xF1    /*主控板通讯结束标志*/
#define MCB_CONTROL_CODE            0x85


#define CHAIR_WORK_DISABLE          0
#define CHAIR_WORK_ENABLE           1

#define CHAIR_STATE_IDLE            0       /*待机状态*/
#define CHAIR_STATE_SETTLE          1       /*回位状态*/
#define CHAIR_STATE_WAIT_COMMAND    2       /*等待命令，相应的指示灯闪烁*/
#define CHAIR_STATE_RUN             3       /*运行状态*/
#define CHAIR_STATE_WAIT_MEMORY     4       /*未知状态*/
#define CHAIR_STATE_PROBLEM         5       /*故障模式*/
#define CHAIR_STATE_ENGINEERING     6       /*工程模式*/
#define CHAIR_STATE_SLEEP           7       /*睡眠模式*/

#define H10_KEY_POWER_SWITCH        0x01

#define H10_KEY_CHAIR_AUTO_0        0x10    /*疲劳恢复*/
#define H10_KEY_CHAIR_AUTO_1        0x11    /*舒适按摩*/
#define H10_KEY_CHAIR_AUTO_2        0x12    /*轻松按摩*/
#define H10_KEY_CHAIR_AUTO_3        0x13    /*酸痛改善*/
#define H10_KEY_CHAIR_AUTO_4        0x14    /*上半身自动*/
#define H10_KEY_CHAIR_AUTO_5        0x15    /*下半身自动*/
#define H10_KEY_CHAIR_AUTO_6        0x16    /*Sleep1*/
#define H10_KEY_CHAIR_AUTO_7        0x19    /*Sleep2*/

#define H10_KEY_AIRBAG_LEG          0x40    /*小腿*/
#define H10_KEY_AIRBAG_ARM          0x41    /*臂肩*/
#define H10_KEY_AIRBAG_WAIST        0x42    /*背腰*/
#define H10_KEY_AIRBAG_BUTTOCKS     0x43    /*臀部*/
#define H10_KEY_AIRBAG_AUTO         0x44    /*全身自动*/

#define H10_KEY_AIRBAG_STRENGTH_0   0x4A    /*气囊关*/
#define H10_KEY_AIRBAG_STRENGTH_1   0x45    /*气囊力度弱*/
#define H10_KEY_AIRBAG_STRENGTH_2   0x46    /*气囊力度中*/
#define H10_KEY_AIRBAG_STRENGTH_3   0x47    /*气囊力度强*/
#define H10_KEY_AIRBAG_STRENGTH_4   0x48
#define H10_KEY_AIRBAG_STRENGTH_5   0x49

#define H10_KEY_WORK_ENABLE         0x72    /*投币或者微信扫码使能按摩椅*/
#define H10_KEY_WORK_DISABLE        0x73    /*按摩椅禁止工作*/
#define H10_KEY_TIME_ADD            0x74    /*微信加时命令*/
#define H10_KEY_NONE                0x7F    /*空指令*/
//add by sxk 2018.3.29//5500B
#define H10_CHARGE_WORK_ENABLE      0x75    /*打开充电*/
#define H10_CHARGE_WORK_DISABLE     0x76    /*关闭充电*/
#define H10_LONGCHARGE_WORK_ENABLE  0x77    /*打开一直充电*/
#define H10_LONGCHARGE_WORK_DISABBLE 0x78   /*关闭一直充电*/  
//


EXTERN void InitMCB(void);
EXTERN void MCB_CommandEnqueue(uint8_t command, uint16_t time);
EXTERN void MCB_RX_Handler(void);
EXTERN void MCB_TX_Handler(void);
EXTERN void MCB_Demo(uint8_t index);
EXTERN uint8_t MCB_GetChairRunningFlag(void);
EXTERN void MCB_SysPowerDownHandler(void);

EXTERN void MCB_StartRunCheckHandler(void);
EXTERN void ChargeHandler(void);

#endif


