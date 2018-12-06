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
#define CLIENT_CMD_ONLINE                                   "01"    /*GSM上线*/
#define CLIENT_CMD_ASKFOR_NEWLINK                           "31"    /*获取按摩椅通讯地址*/
#define CLIENT_CMD_UPLOAD_HEARTBEAT                         "??"    /*GSM心跳*/
#define CLIENT_CMD_UPLOAD_REMOTE_ORDER_TIME                 "18"    /*GSM微信订单时间上传*/
#define CLIENT_CMD_UPLOAD_NATIVE_ORDER_TIME                 "17"    /*GSM投币订单时间上传*/
#define CLIENT_CMD_UPLOAD_NATIVE_ORDER_MONEY                "19"    /*GSM微信投币当前一笔钱上传*/
#define CLIENT_CMD_UPLOAD_ABNORMAL_ORDER                    "37"    /*上传异常订单信息*/

/*Server*/
#define SERVER_CMD_START_RUNNING                            "05"    /*微信远程启动*/
#define SERVER_CMD_CLOSE_RUNNING                            "09"    /*微信远程关闭*/
#define SERVER_CMD_EXTEND_WORKING_TIME                      "08"    /*微信远程续费*/
#define SERVER_CMD_CHAIR_SET_PARAMETER                      "06"    /*微信APP控制命令*/
#define SERVER_CMD_CHAIR_QUERY_STATUS                       "10"    /*按摩椅状态查询*/

//add by sxk
#define SERVER_CMD_START_CHARGE                             "40"    /*远程启动充电*/
#define SERVER_CMD_CLOSE_CHARGE                             "41"    /*远程关闭充电*/
#define SERVER_CMD_SET_LONG_CHARGE                          "42"    /*设置一直充电*/
#define SERVER_CMD_QUERY_LONG_CHARGE                        "43"    /*查询是否为一直充电*/
//
#define SERVER_CMD_UPGRADING_PROGRAM                        "30"    /*微信远程升级命令*/

#define SERVER_CMD_COIN_BOX_QUERY_ALL                       "11"    /*微信APP查询钱币总额、总识币数*/
#define SERVER_CMD_COIN_BOX_CLEAR_ALL                       "12"    /*微信APP清除钱币总额、总识币数*/
#define SERVER_CMD_COIN_BOX_QUERY_RATE                      "13"    /*微信APP查询费率*/
#define SERVER_CMD_COIN_BOX_SET_RATE                        "14"    /*微信APP设置费率*/
#define SERVER_CMD_COIN_BOX_QUERY_TIME                      "15"    /*微信APP查询投币箱时间*/
#define SERVER_CMD_COIN_BOX_SET_TIME                        "16"    /*微信APP设置投币箱时间*/

#define SERVER_CMD_COIN_BOX_UNLOCKING                       "32"    /*微信APP发送开锁命令*/
#define SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_STATUS          "33"    /*微信查询投币箱门状态*/
#define SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_TIMES           "34"    /*微信查询投币箱累计开锁次数*/
#define SERVER_CMD_COIN_BOX_CLEAR_LEGAL_UNLOCKING_TIMES     "35"    /*微信清除投币箱累计合法开锁次数*/
#define SERVER_CMD_COIN_BOX_CLEAR_ILLEGALUNLOCKING_TIMES    "36"    /*微信清除投币箱累计不合法开锁次数*/

#define SERVER_CMD_COMMUNICATION_TEST                       "50"    /*通讯测试指令*/

/*AT Command*/
#define AT_CMD_AT               "AT"                        /*AT测试指令*/
#define AT_CMD_GSN              "AT+GSN"                    /*获取GSN*/
#define AT_CMD_CCID             "AT+CCID"                   /*获取CCID*/
#define AT_CMD_CIPSHUT          "AT+CIPSHUT"                /*重新START成功后没有CONNECT OK回复*/
#define AT_CMD_CIPCLOSE         "AT+CIPCLOSE"
#define AT_CMD_CSQ              "AT+CSQ"
#define AT_CMD_CSQ_COPS         "AT+CSQ;+COPS?"             /*检查网络信号强度*/
#define AT_CMD_CREG             "AT+CREG?"                  /*检查网络注册状态*/
#define AT_CMD_CGATT            "AT+CGATT?"                 /*检查GPRS附着状态*/
#define AT_CMD_CSTT_10086       "AT+CSTT=CMIoT"             /*中国移动物联网*/
#define AT_CMD_CSTT_10010       "AT+CSTT=UNIM2M.NJM2MAPN"   /*中国联通物联网*/
#define AT_CMD_CSTT_DU          "AT+CSTT=DU"                /*迪拜*/
#define AT_CMD_CSTT_RUSSIA      "AT+CSTT=internet"          /*俄罗斯*/
#define AT_CMD_CIICR            "AT+CIICR"                  /*建立无线链路GPRS或者CSD*/
#define AT_CMD_CIFSR            "AT+CIFSR"                  /*获得本地IP地址*/
#define AT_CMD_CIPSTART         "AT+CIPSTART=TCP,check.51shaoxi.com,5489"   /*新的服务器*/
#define AT_CMD_CIPSEND          "AT+CIPSEND"                /*发送数据到远端服务，CTRL+Z(0x1A)发送*/
#define AT_CMD_CIPSENT_PERMIT   ">"                         /*用户必须等到'>'后才输入数据，然后用CTRL+Z发送*/
#define AT_CMD_END_CR           0x0D                        /*发送命令结束字符*/
#define AT_CMD_END_SUB          0x1A                        /*发送数据结束字符*/

#define AT_CMD_SAPBR_CONTYPE    "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""
#define AT_CMD_SAPBR_APN_10086  "AT+SAPBR=3,1,\"APN\",\"CMIoT\""
#define AT_CMD_SAPBR_APN_10010  "AT+SAPBR=3,1,\"APN\",\"UNIM2M.NJM2MAPN\""
#define AT_CMD_SAPBR_APN_DU     "AT+SAPBR=3,1,\"APN\",\"DU\""
#define AT_CMD_SAPBR_APN_RUSSIA "AT+SAPBR=3,1,\"APN\",\"internet\""
#define AT_CMD_SAPBR_START      "AT+SAPBR=1,1"                          /*激活GPRS上下文*/
#define AT_CMD_SAPBR_QUERY      "AT+SAPBR=2,1"                          /*查询GPRS上下文*/
#define AT_CMD_SAPBR_CLOSE      "AT+SAPBR=0,1"                          /*关闭GPRS上下文*/
#define AT_CMD_FTPMODE          "AT+FTPMODE=0"                          /*FTP模式为Active*/
#define AT_CMD_FTPTYPE          "AT+FTPTYPE=\"I\""                      /*FTP数据传输类型为Binary*/
#define AT_CMD_FTPCID           "AT+FTPCID=1"
#define AT_CMD_FTPSERV          "AT+FTPSERV=\"139.196.138.73\""         /*FTP主机*/
#define AT_CMD_FTPPORT          "AT+FTPPORT=21"                         /*FTP端口*/
#define AT_CMD_FTPUN            "AT+FTPUN=\"shaoxivsftp\""              /*FTP用户名*/
#define AT_CMD_FTPPW            "AT+FTPPW=\"1107@51ShaoXiadjin12%\""    /*FTP密码*/
#define AT_CMD_FTPGETNAME       "AT+FTPGETNAME=\"test.bin\""            /*FTP文件名*/
#define AT_CMD_FTPGETPATH       "AT+FTPGETPATH=\"/update/\""            /*FTP文件目录*/
#define AT_CMD_FTPEXTGET_START  "AT+FTPEXTGET=1"                        /*通过FTPGET开始下载*/
#define AT_CMD_FTPEXTGET_SAVE   "AT+FTPEXTGET=2,\"data.bin\""           /*将下载文件保存到SIM800内部文件系统，文件名为data.bin*/
#define AT_CMD_FTPEXTGET_OUTPUT "AT+FTPEXTGET=3,0,128"                  /*输出下载的文件*/

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


