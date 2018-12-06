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


#define MONEY_SOI                       0xF1    /*��Ͷ�Ұ�ͨѶ����ʼ��־λ*/
#define MONEY_EOI                       0xF0    /*��Ͷ�Ұ�ͨѶ�Ľ�����־λ*/

#define MONEY_CMD_QUERY_CURRENT         0x01    /*������ѯ��ǰһ�ν��յ���Ǯ����Ŀ(MAX:99999)*/
#define MONEY_CMD_QUERY_ALL             0x02    /*��ѯ���յ�����Ǯ���ۼ���Ŀ����ʱ��������ʶ������(MAX:99999)*/
#define MONEY_CMD_CLEAR_ALL             0x03    /*������Ǯ���ۼ���Ŀ����ʱ��������ʶ������*/
#define MONEY_CMD_SET_RATE              0x04    /*���÷���*/
#define MONEY_CMD_QUERY_RATE            0x05    /*��ѯ����*/
#define MONEY_CMD_SET_DATE_TIME         0x06    /*����������ʱ����*/
#define MONEY_CMD_QUERY_DATE_TIME       0x07    /*��ѯ������ʱ����*/
#define MONEY_CMD_COIN_DISABLE          0x08    /*��ֹͶ�ҹ��ܣ��ϵ��Ĭ��Ϊ����Ͷ��*/
#define MONEY_CMD_COIN_ENABLE           0x09    /*����Ͷ�ҹ���*/
#define MONEY_CMD_MEMORY_ENABLE         0x10    /*����ϵ籣�湦��*/
#define MONEY_CMD_MEMORY_DISABLE        0x11    /*��ֹ�ϵ籣�湦�ܣ��ϵ��Ĭ��Ϊ��ֹ�ϵ籣��*/

#define MONEY_CMD_UNLOCKING                     0x12    /*΢��ɨ�뿪��*/
#define MONEY_CMD_QUERY_UNLOCKING_STATUS        0x13    /*������ѯͶ����������״̬*/
#define MONEY_CMD_QUERY_UNLOCKING_TIMES         0x14    /*������ѯͶ�����ۼƿ�������*/
#define MONEY_CMD_CLEAR_LEGAL_UNLOCKING_TIMES   0x15    /*��������Ͷ�����ۼƺϷ���������*/
#define MONEY_CMD_CLEAR_ILLEGAL_UNLOCKING_TIMES 0x16    /*��������Ͷ�����ۼ��쳣��������*/


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

