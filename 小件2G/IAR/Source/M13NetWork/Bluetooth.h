#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__


#undef  EXTERN

#ifdef  __BLUETOOTH_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


#define BT_AT_CMD_RSET      "AT+RSET"   /*恢复出厂设置*/
#define BT_AT_CMD_CONB      "AT+CONB"   /*断开连接*/
#define BT_AT_CMD_REST      "AT+REST"   /*复位*/

#define BT_AT_CMD_GCTO      "AT+GCTO"   /*查询-连接超时*/
#define BT_AT_CMD_VRES      "AT+VRES"   /*查询-软件版本*/
#define BT_AT_CMD_GADD      "AT+GADD"   /*查询-模块地址*/
#define BT_AT_CMD_GNAM      "AT+GNAM"   /*查询-模块名称*/
#define BT_AT_CMD_GCMA      "AT+GCMA"   /*查询-最大间隔时间*/
#define BT_AT_CMD_GPWR      "AT+GPWR"   /*查询-发射功率*/
#define BT_AT_CMD_GSLA      "AT+GSLA"   /*查询-从机潜伏*/
#define BT_AT_CMD_GCMI      "AT+GCMI"   /*查询-最小连接间隔*/
#define BT_AT_CMD_GURT      "AT+GURT"   /*查询-波特率*/
#define BT_AT_CMD_GAVI      "AT+GAVI"   /*查询-广播间隔*/
#define BT_AT_CMD_GPAC      "AT+GPAC"   /*查询-配对密码*/
#define BT_AT_CMD_GPAE      "AT+GPAE"   /*查询-配对密码使能*/

#define BT_AT_CMD_UART      "AT+UART+"  /*设置-串口波特率*/
#define BT_AT_CMD_SNAM      "AT+SNAM+"  /*设置-模块名称*/
#define BT_AT_CMD_SCMA      "AT+SCMA+"  /*设置-最大间隔时间*/
#define BT_AT_CMD_SPWR      "AT+SPWR+"  /*设置-发射功率*/
#define BT_AT_CMD_SCMI      "AT+GCMI+"  /*设置-最小连接间隔*/
#define BT_AT_CMD_SPAC      "AT+GPAC+"  /*设置-配对密码*/
#define BT_AT_CMD_SPAE      "AT+SPAE+"  /*设置-配对密码使能*/
#define BT_AT_CMD_SSLA      "AT+SSLA+"  /*设置-从机潜伏值*/
#define BT_AT_CMD_SCTO      "AT+SCTO+"  /*设置-连接超时*/
#define BT_AT_CMD_SAVI      "AT+SAVI+"  /*设置-广播间隔*/
#define BT_AT_CMD_ENLP      "AT+ENLP"   /*设置-低功耗控制使能*/
#define BT_AT_CMD_NOLP      "AT+NOLP"   /*设置-低功耗控制禁止*/
#define BT_AT_CMD_ELED      "AT+ELED"   /*设置-蓝牙LED 灯使能*/
#define BT_AT_CMD_DLED      "AT+DLED"   /*设置-蓝牙LED 灯禁止*/
#define BT_AT_CMD_ESLP      "AT+ESLP"   /*设置-模块开关控制使能*/
#define BT_AT_CMD_DSLP      "AT+DSLP"   /*设置-模块开关控制禁止*/

#define BT_AT_CMD_END_CR    0x0D
#define BT_AT_CMD_END_LF    0x0A

#define BT_AT_IDX_RSET      0x01        /*恢复出厂设置*/
#define BT_AT_IDX_CONB      0x02        /*断开连接*/
#define BT_AT_IDX_REST      0x03        /*复位*/

#define BT_AT_IDX_GCTO      0x04        /*查询-连接超时*/
#define BT_AT_IDX_VRES      0x05        /*查询-软件版本*/
#define BT_AT_IDX_GADD      0x06        /*查询-模块地址*/
#define BT_AT_IDX_GNAM      0x07        /*查询-模块名称*/
#define BT_AT_IDX_GCMA      0x08        /*查询-最大间隔时间*/
#define BT_AT_IDX_GPWR      0x09        /*查询-发射功率*/
#define BT_AT_IDX_GSLA      0x0A        /*查询-从机潜伏*/
#define BT_AT_IDX_GCMI      0x0B        /*查询-最小连接间隔*/
#define BT_AT_IDX_GURT      0x0C        /*查询-波特率*/
#define BT_AT_IDX_GAVI      0x0D        /*查询-广播间隔*/
#define BT_AT_IDX_GPAC      0x0E        /*查询-配对密码*/
#define BT_AT_IDX_GPAE      0x0F        /*查询-配对密码使能*/

#define BT_AT_IDX_UART      0x10        /*设置-串口波特率*/
#define BT_AT_IDX_SNAM      0x11        /*设置-模块名称*/
#define BT_AT_IDX_SCMA      0x12        /*设置-最大间隔时间*/
#define BT_AT_IDX_SPWR      0x13        /*设置-发射功率*/
#define BT_AT_IDX_SCMI      0x14        /*设置-最小连接间隔*/
#define BT_AT_IDX_SPAC      0x15        /*设置-配对密码*/
#define BT_AT_IDX_SPAE      0x16        /*设置-配对密码使能*/
#define BT_AT_IDX_SSLA      0x17        /*设置-从机潜伏值*/
#define BT_AT_IDX_SCTO      0x18        /*设置-连接超时*/
#define BT_AT_IDX_SAVI      0x19        /*设置-广播间隔*/
#define BT_AT_IDX_ENLP      0x1A        /*设置-低功耗控制使能*/
#define BT_AT_IDX_NOLP      0x1B        /*设置-低功耗控制禁止*/
#define BT_AT_IDX_ELED      0x1C        /*设置-蓝牙LED 灯使能*/
#define BT_AT_IDX_DLED      0x1D        /*设置-蓝牙LED 灯禁止*/
#define BT_AT_IDX_ESLP      0x1E        /*设置-模块开关控制使能*/
#define BT_AT_IDX_DSLP      0x1F        /*设置-模块开关控制禁止*/


EXTERN void BT_RX_Handler(void);
EXTERN void BT_TX_Handler(void);
EXTERN void BT_Init(void);
EXTERN uint8_t BT_CheckConnectionState(void);


#endif


