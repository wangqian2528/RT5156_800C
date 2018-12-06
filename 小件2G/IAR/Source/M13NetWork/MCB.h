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


#define MCB_SOI                     0xF0    /*���ذ�ͨѶ��ʼ��־*/
#define MCB_EOI                     0xF1    /*���ذ�ͨѶ������־*/
#define MCB_CONTROL_CODE            0x85


#define CHAIR_WORK_DISABLE          0
#define CHAIR_WORK_ENABLE           1

#define CHAIR_STATE_IDLE            0       /*����״̬*/
#define CHAIR_STATE_SETTLE          1       /*��λ״̬*/
#define CHAIR_STATE_WAIT_COMMAND    2       /*�ȴ������Ӧ��ָʾ����˸*/
#define CHAIR_STATE_RUN             3       /*����״̬*/
#define CHAIR_STATE_WAIT_MEMORY     4       /*δ֪״̬*/
#define CHAIR_STATE_PROBLEM         5       /*����ģʽ*/
#define CHAIR_STATE_ENGINEERING     6       /*����ģʽ*/
#define CHAIR_STATE_SLEEP           7       /*˯��ģʽ*/

#define H10_KEY_POWER_SWITCH        0x01

#define H10_KEY_CHAIR_AUTO_0        0x10    /*ƣ�ͻָ�*/
#define H10_KEY_CHAIR_AUTO_1        0x11    /*���ʰ�Ħ*/
#define H10_KEY_CHAIR_AUTO_2        0x12    /*���ɰ�Ħ*/
#define H10_KEY_CHAIR_AUTO_3        0x13    /*��ʹ����*/
#define H10_KEY_CHAIR_AUTO_4        0x14    /*�ϰ����Զ�*/
#define H10_KEY_CHAIR_AUTO_5        0x15    /*�°����Զ�*/
#define H10_KEY_CHAIR_AUTO_6        0x16    /*Sleep1*/
#define H10_KEY_CHAIR_AUTO_7        0x19    /*Sleep2*/

#define H10_KEY_AIRBAG_LEG          0x40    /*С��*/
#define H10_KEY_AIRBAG_ARM          0x41    /*�ۼ�*/
#define H10_KEY_AIRBAG_WAIST        0x42    /*����*/
#define H10_KEY_AIRBAG_BUTTOCKS     0x43    /*�β�*/
#define H10_KEY_AIRBAG_AUTO         0x44    /*ȫ���Զ�*/

#define H10_KEY_AIRBAG_STRENGTH_0   0x4A    /*���ҹ�*/
#define H10_KEY_AIRBAG_STRENGTH_1   0x45    /*����������*/
#define H10_KEY_AIRBAG_STRENGTH_2   0x46    /*����������*/
#define H10_KEY_AIRBAG_STRENGTH_3   0x47    /*��������ǿ*/
#define H10_KEY_AIRBAG_STRENGTH_4   0x48
#define H10_KEY_AIRBAG_STRENGTH_5   0x49

#define H10_KEY_WORK_ENABLE         0x72    /*Ͷ�һ���΢��ɨ��ʹ�ܰ�Ħ��*/
#define H10_KEY_WORK_DISABLE        0x73    /*��Ħ�ν�ֹ����*/
#define H10_KEY_TIME_ADD            0x74    /*΢�ż�ʱ����*/
#define H10_KEY_NONE                0x7F    /*��ָ��*/
//add by sxk 2018.3.29//5500B
#define H10_CHARGE_WORK_ENABLE      0x75    /*�򿪳��*/
#define H10_CHARGE_WORK_DISABLE     0x76    /*�رճ��*/
#define H10_LONGCHARGE_WORK_ENABLE  0x77    /*��һֱ���*/
#define H10_LONGCHARGE_WORK_DISABBLE 0x78   /*�ر�һֱ���*/  
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


