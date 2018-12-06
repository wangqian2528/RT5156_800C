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


#define BT_AT_CMD_RSET      "AT+RSET"   /*�ָ���������*/
#define BT_AT_CMD_CONB      "AT+CONB"   /*�Ͽ�����*/
#define BT_AT_CMD_REST      "AT+REST"   /*��λ*/

#define BT_AT_CMD_GCTO      "AT+GCTO"   /*��ѯ-���ӳ�ʱ*/
#define BT_AT_CMD_VRES      "AT+VRES"   /*��ѯ-����汾*/
#define BT_AT_CMD_GADD      "AT+GADD"   /*��ѯ-ģ���ַ*/
#define BT_AT_CMD_GNAM      "AT+GNAM"   /*��ѯ-ģ������*/
#define BT_AT_CMD_GCMA      "AT+GCMA"   /*��ѯ-�����ʱ��*/
#define BT_AT_CMD_GPWR      "AT+GPWR"   /*��ѯ-���书��*/
#define BT_AT_CMD_GSLA      "AT+GSLA"   /*��ѯ-�ӻ�Ǳ��*/
#define BT_AT_CMD_GCMI      "AT+GCMI"   /*��ѯ-��С���Ӽ��*/
#define BT_AT_CMD_GURT      "AT+GURT"   /*��ѯ-������*/
#define BT_AT_CMD_GAVI      "AT+GAVI"   /*��ѯ-�㲥���*/
#define BT_AT_CMD_GPAC      "AT+GPAC"   /*��ѯ-�������*/
#define BT_AT_CMD_GPAE      "AT+GPAE"   /*��ѯ-�������ʹ��*/

#define BT_AT_CMD_UART      "AT+UART+"  /*����-���ڲ�����*/
#define BT_AT_CMD_SNAM      "AT+SNAM+"  /*����-ģ������*/
#define BT_AT_CMD_SCMA      "AT+SCMA+"  /*����-�����ʱ��*/
#define BT_AT_CMD_SPWR      "AT+SPWR+"  /*����-���书��*/
#define BT_AT_CMD_SCMI      "AT+GCMI+"  /*����-��С���Ӽ��*/
#define BT_AT_CMD_SPAC      "AT+GPAC+"  /*����-�������*/
#define BT_AT_CMD_SPAE      "AT+SPAE+"  /*����-�������ʹ��*/
#define BT_AT_CMD_SSLA      "AT+SSLA+"  /*����-�ӻ�Ǳ��ֵ*/
#define BT_AT_CMD_SCTO      "AT+SCTO+"  /*����-���ӳ�ʱ*/
#define BT_AT_CMD_SAVI      "AT+SAVI+"  /*����-�㲥���*/
#define BT_AT_CMD_ENLP      "AT+ENLP"   /*����-�͹��Ŀ���ʹ��*/
#define BT_AT_CMD_NOLP      "AT+NOLP"   /*����-�͹��Ŀ��ƽ�ֹ*/
#define BT_AT_CMD_ELED      "AT+ELED"   /*����-����LED ��ʹ��*/
#define BT_AT_CMD_DLED      "AT+DLED"   /*����-����LED �ƽ�ֹ*/
#define BT_AT_CMD_ESLP      "AT+ESLP"   /*����-ģ�鿪�ؿ���ʹ��*/
#define BT_AT_CMD_DSLP      "AT+DSLP"   /*����-ģ�鿪�ؿ��ƽ�ֹ*/

#define BT_AT_CMD_END_CR    0x0D
#define BT_AT_CMD_END_LF    0x0A

#define BT_AT_IDX_RSET      0x01        /*�ָ���������*/
#define BT_AT_IDX_CONB      0x02        /*�Ͽ�����*/
#define BT_AT_IDX_REST      0x03        /*��λ*/

#define BT_AT_IDX_GCTO      0x04        /*��ѯ-���ӳ�ʱ*/
#define BT_AT_IDX_VRES      0x05        /*��ѯ-����汾*/
#define BT_AT_IDX_GADD      0x06        /*��ѯ-ģ���ַ*/
#define BT_AT_IDX_GNAM      0x07        /*��ѯ-ģ������*/
#define BT_AT_IDX_GCMA      0x08        /*��ѯ-�����ʱ��*/
#define BT_AT_IDX_GPWR      0x09        /*��ѯ-���书��*/
#define BT_AT_IDX_GSLA      0x0A        /*��ѯ-�ӻ�Ǳ��*/
#define BT_AT_IDX_GCMI      0x0B        /*��ѯ-��С���Ӽ��*/
#define BT_AT_IDX_GURT      0x0C        /*��ѯ-������*/
#define BT_AT_IDX_GAVI      0x0D        /*��ѯ-�㲥���*/
#define BT_AT_IDX_GPAC      0x0E        /*��ѯ-�������*/
#define BT_AT_IDX_GPAE      0x0F        /*��ѯ-�������ʹ��*/

#define BT_AT_IDX_UART      0x10        /*����-���ڲ�����*/
#define BT_AT_IDX_SNAM      0x11        /*����-ģ������*/
#define BT_AT_IDX_SCMA      0x12        /*����-�����ʱ��*/
#define BT_AT_IDX_SPWR      0x13        /*����-���书��*/
#define BT_AT_IDX_SCMI      0x14        /*����-��С���Ӽ��*/
#define BT_AT_IDX_SPAC      0x15        /*����-�������*/
#define BT_AT_IDX_SPAE      0x16        /*����-�������ʹ��*/
#define BT_AT_IDX_SSLA      0x17        /*����-�ӻ�Ǳ��ֵ*/
#define BT_AT_IDX_SCTO      0x18        /*����-���ӳ�ʱ*/
#define BT_AT_IDX_SAVI      0x19        /*����-�㲥���*/
#define BT_AT_IDX_ENLP      0x1A        /*����-�͹��Ŀ���ʹ��*/
#define BT_AT_IDX_NOLP      0x1B        /*����-�͹��Ŀ��ƽ�ֹ*/
#define BT_AT_IDX_ELED      0x1C        /*����-����LED ��ʹ��*/
#define BT_AT_IDX_DLED      0x1D        /*����-����LED �ƽ�ֹ*/
#define BT_AT_IDX_ESLP      0x1E        /*����-ģ�鿪�ؿ���ʹ��*/
#define BT_AT_IDX_DSLP      0x1F        /*����-ģ�鿪�ؿ��ƽ�ֹ*/


EXTERN void BT_RX_Handler(void);
EXTERN void BT_TX_Handler(void);
EXTERN void BT_Init(void);
EXTERN uint8_t BT_CheckConnectionState(void);


#endif


