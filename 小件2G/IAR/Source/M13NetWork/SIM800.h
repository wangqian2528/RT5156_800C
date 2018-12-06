#ifndef __SIM800_H__
#define __SIM800_H__


#undef  EXTERN

#ifdef  __SIM800_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


#define SIM800_TX_SOI   '%'
#define SIM800_TX_EOI   '-'

#define SIM800_RX_SOI   '*'
#define SIM800_RX_EOI   '!'

/*Client*/
#define CLIENT_CMD_ONLINE                                   "01"    /*GSM����*/
#define CLIENT_CMD_ASKFOR_NEWLINK                           "31"    /*��ȡ��Ħ��ͨѶ��ַ*/
#define CLIENT_CMD_UPLOAD_HEARTBEAT                         "??"    /*GSM����*/
#define CLIENT_CMD_UPLOAD_REMOTE_ORDER_TIME                 "18"    /*GSM΢�Ŷ���ʱ���ϴ�*/
#define CLIENT_CMD_UPLOAD_NATIVE_ORDER_TIME                 "17"    /*GSMͶ�Ҷ���ʱ���ϴ�*/
#define CLIENT_CMD_UPLOAD_NATIVE_ORDER_MONEY                "19"    /*GSM΢��Ͷ�ҵ�ǰһ��Ǯ�ϴ�*/
#define CLIENT_CMD_UPLOAD_ABNORMAL_ORDER                    "37"    /*�ϴ��쳣������Ϣ*/

/*Server*/
#define SERVER_CMD_START_RUNNING                            "05"    /*΢��Զ������*/
#define SERVER_CMD_CLOSE_RUNNING                            "09"    /*΢��Զ�̹ر�*/
#define SERVER_CMD_EXTEND_WORKING_TIME                      "08"    /*΢��Զ������*/
#define SERVER_CMD_CHAIR_SET_PARAMETER                      "06"    /*΢��APP��������*/
#define SERVER_CMD_CHAIR_QUERY_STATUS                       "10"    /*��Ħ��״̬��ѯ*/

//add by sxk
#define SERVER_CMD_START_CHARGE                             "40"    /*Զ���������*/
#define SERVER_CMD_CLOSE_CHARGE                             "41"    /*Զ�̹رճ��*/
#define SERVER_CMD_SET_LONG_CHARGE                          "42"    /*����һֱ���*/
#define SERVER_CMD_QUERY_LONG_CHARGE                        "43"    /*��ѯ�Ƿ�Ϊһֱ���*/
//
#define SERVER_CMD_UPGRADING_PROGRAM                        "30"    /*΢��Զ����������*/

#define SERVER_CMD_COIN_BOX_QUERY_ALL                       "11"    /*΢��APP��ѯǮ���ܶ��ʶ����*/
#define SERVER_CMD_COIN_BOX_CLEAR_ALL                       "12"    /*΢��APP���Ǯ���ܶ��ʶ����*/
#define SERVER_CMD_COIN_BOX_QUERY_RATE                      "13"    /*΢��APP��ѯ����*/
#define SERVER_CMD_COIN_BOX_SET_RATE                        "14"    /*΢��APP���÷���*/
#define SERVER_CMD_COIN_BOX_QUERY_TIME                      "15"    /*΢��APP��ѯͶ����ʱ��*/
#define SERVER_CMD_COIN_BOX_SET_TIME                        "16"    /*΢��APP����Ͷ����ʱ��*/

#define SERVER_CMD_COIN_BOX_UNLOCKING                       "32"    /*΢��APP���Ϳ�������*/
#define SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_STATUS          "33"    /*΢�Ų�ѯͶ������״̬*/
#define SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_TIMES           "34"    /*΢�Ų�ѯͶ�����ۼƿ�������*/
#define SERVER_CMD_COIN_BOX_CLEAR_LEGAL_UNLOCKING_TIMES     "35"    /*΢�����Ͷ�����ۼƺϷ���������*/
#define SERVER_CMD_COIN_BOX_CLEAR_ILLEGALUNLOCKING_TIMES    "36"    /*΢�����Ͷ�����ۼƲ��Ϸ���������*/

#define SERVER_CMD_COMMUNICATION_TEST                       "50"    /*ͨѶ����ָ��*/

/*AT Command*/
#define AT_CMD_AT               "AT"                        /*AT����ָ��*/
#define AT_CMD_GSN              "AT+GSN"                    /*��ȡGSN*/
#define AT_CMD_CCID             "AT+CCID"                   /*��ȡCCID*/
#define AT_CMD_CIPSHUT          "AT+CIPSHUT"                /*����START�ɹ���û��CONNECT OK�ظ�*/
#define AT_CMD_CIPCLOSE         "AT+CIPCLOSE"
#define AT_CMD_CSQ              "AT+CSQ"
#define AT_CMD_CSQ_COPS         "AT+CSQ;+COPS?"             /*��������ź�ǿ��*/
#define AT_CMD_CREG             "AT+CREG?"                  /*�������ע��״̬*/
#define AT_CMD_CGATT            "AT+CGATT?"                 /*���GPRS����״̬*/
#define AT_CMD_CSTT_10086       "AT+CSTT=CMIoT"             /*�й��ƶ�������*/
#define AT_CMD_CSTT_10010       "AT+CSTT=UNIM2M.NJM2MAPN"   /*�й���ͨ������*/
#define AT_CMD_CSTT_DU          "AT+CSTT=DU"                /*�ϰ�*/
#define AT_CMD_CSTT_RUSSIA      "AT+CSTT=internet"          /*����˹*/
#define AT_CMD_CIICR            "AT+CIICR"                  /*����������·GPRS����CSD*/
#define AT_CMD_CIFSR            "AT+CIFSR"                  /*��ñ���IP��ַ*/
#define AT_CMD_CIPSTART         "AT+CIPSTART=TCP,check.51shaoxi.com,5489"   /*�µķ�����*/
#define AT_CMD_CIPSEND          "AT+CIPSEND"                /*�������ݵ�Զ�˷���CTRL+Z(0x1A)����*/
#define AT_CMD_CIPSENT_PERMIT   ">"                         /*�û�����ȵ�'>'����������ݣ�Ȼ����CTRL+Z����*/
#define AT_CMD_END_CR           0x0D                        /*������������ַ�*/
#define AT_CMD_END_SUB          0x1A                        /*�������ݽ����ַ�*/

#define AT_CMD_SAPBR_CONTYPE    "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""
#define AT_CMD_SAPBR_APN_10086  "AT+SAPBR=3,1,\"APN\",\"CMIoT\""
#define AT_CMD_SAPBR_APN_10010  "AT+SAPBR=3,1,\"APN\",\"UNIM2M.NJM2MAPN\""
#define AT_CMD_SAPBR_APN_DU     "AT+SAPBR=3,1,\"APN\",\"DU\""
#define AT_CMD_SAPBR_APN_RUSSIA "AT+SAPBR=3,1,\"APN\",\"internet\""
#define AT_CMD_SAPBR_START      "AT+SAPBR=1,1"                          /*����GPRS������*/
#define AT_CMD_SAPBR_QUERY      "AT+SAPBR=2,1"                          /*��ѯGPRS������*/
#define AT_CMD_SAPBR_CLOSE      "AT+SAPBR=0,1"                          /*�ر�GPRS������*/
#define AT_CMD_FTPMODE          "AT+FTPMODE=0"                          /*FTPģʽΪActive*/
#define AT_CMD_FTPTYPE          "AT+FTPTYPE=\"I\""                      /*FTP���ݴ�������ΪBinary*/
#define AT_CMD_FTPCID           "AT+FTPCID=1"
#define AT_CMD_FTPSERV          "AT+FTPSERV=\"139.196.138.73\""         /*FTP����*/
#define AT_CMD_FTPPORT          "AT+FTPPORT=21"                         /*FTP�˿�*/
#define AT_CMD_FTPUN            "AT+FTPUN=\"shaoxivsftp\""              /*FTP�û���*/
#define AT_CMD_FTPPW            "AT+FTPPW=\"1107@51ShaoXiadjin12%\""    /*FTP����*/
#define AT_CMD_FTPGETNAME       "AT+FTPGETNAME=\"test.bin\""            /*FTP�ļ���*/
#define AT_CMD_FTPGETPATH       "AT+FTPGETPATH=\"/update/\""            /*FTP�ļ�Ŀ¼*/
#define AT_CMD_FTPEXTGET_START  "AT+FTPEXTGET=1"                        /*ͨ��FTPGET��ʼ����*/
#define AT_CMD_FTPEXTGET_SAVE   "AT+FTPEXTGET=2,\"data.bin\""           /*�������ļ����浽SIM800�ڲ��ļ�ϵͳ���ļ���Ϊdata.bin*/
#define AT_CMD_FTPEXTGET_OUTPUT "AT+FTPEXTGET=3,0,128"                  /*������ص��ļ�*/

/*AT Command Index*/
#define AT_IDX_AT               0x01
#define AT_IDX_GSN              0x02
#define AT_IDX_CCID             0x03
#define AT_IDX_CIPSHUT          0x04
#define AT_IDX_CIPCLOSE         0x05
#define AT_IDX_CSQ              0x06
#define AT_IDX_CSQ_COPS         0x07
#define AT_IDX_CREG             0x08
#define AT_IDX_CGATT            0x09
#define AT_IDX_CSTT             0x0A
#define AT_IDX_CIICR            0x0B
#define AT_IDX_CIFSR            0x0C
#define AT_IDX_CIPSTART         0x0D
#define AT_IDX_CIPSEND          0x0E

#define AT_IDX_SAPBR_CONTYPE    0x20
#define AT_IDX_SAPBR_APN        0x21
#define AT_IDX_SAPBR_START      0x22
#define AT_IDX_SAPBR_QUERY      0x23
#define AT_IDX_SAPBR_CLOSE      0x24
#define AT_IDX_FTPMODE          0x25
#define AT_IDX_FTPTYPE          0x26
#define AT_IDX_FTPCID           0x27
#define AT_IDX_FTPSERV          0x28
#define AT_IDX_FTPPORT          0x29
#define AT_IDX_FTPUN            0x2A
#define AT_IDX_FTPPW            0x2B
#define AT_IDX_FTPGETNAME       0x2C
#define AT_IDX_FTPGETPATH       0x2D
#define AT_IDX_FTPEXTGET_START  0x2E
#define AT_IDX_FTPEXTGET_SAVE   0x2F
#define AT_IDX_FTPEXTGET_OUTPUT 0x30

/*AT Command Return*/
#define AT_RET_COPS_10010       "\"CHN-UNICOM\""
#define AT_RET_COPS_10086       "\"CHINA MOBILE\""


EXTERN void InitSIM800(void);
EXTERN void SIM800_RX_Handler(void);
EXTERN void SIM800_TX_Handler(void);
EXTERN void GSM_HeartbeatHandler(uint8_t mode);
EXTERN void GSM_UpdateCSQHandler(uint8_t mode);
EXTERN void GSM_UploadOrderHandler(uint8_t mode);
EXTERN void GSM_ReplyServerHandler(uint8_t flag);
EXTERN uint8_t GSM_IsTxStepIDLE(void);
EXTERN void GSM_Demo(uint8_t index);
EXTERN void GSM_StartupTimeoutHandler(void);
EXTERN void GSM_UpgradeProgramHandler(void);

EXTERN void GSM_APP_START_CHAIR(void);
EXTERN void GSM_APP_CLOSE_CHAIR(void);

#endif


