#define __SIM800_C__


#include "AES.h"
#include "flash.h"
#include "MCB.h"
#include "MCU.h"
#include "money.h"
#include "SIM800.h"


/*通讯队列*/
extern QUEUE_Init_TypeDef COM_QUEUE[QUEUE_NUMBER];

/*升级缓存*/
extern uint8_t  gucFlashPageData[FLASH_PAGE_SIZE];

/*投币部分的扩展变量*/
extern uint8_t gucMoneyExistFlag;                   /*是否存在投币板标志，默认是存在*/
extern uint8_t gucMoneyAmount[5];                   /*钱币总金额*/
extern uint8_t gucMoneyNumber[5];                   /*钱币总张数*/
extern uint8_t gucMoneyWorkTime[6];                 /*总工作时间*/
extern uint8_t gucMoneyRateBase[2];                 /*费率基数金额*/
extern uint8_t gucMoneyRateTime[2];                 /*费率工作时长*/
extern uint8_t gucMoneyDateTime[12];                /*投币箱的时间*/
extern uint8_t gucMoneyUnlockingState;              /*投币箱锁的状态:关0、开1*/
extern uint8_t gucMoneyLegalUnlockingTimes[5];      /*合法开锁次数*/
extern uint8_t gucMoneyIllegaUnlockinglTimes[5];    /*非法开锁次数*/
extern uint8_t gucMoneyStartWorkTime[12];           /*投币箱开始工作时间*/
extern uint8_t gucMoneyEndOfWorkTime[12];           /*投币箱工作结束时间*/
extern uint8_t gucMoneyWorkStep;                    /*投币箱的工作步骤*/

/*控制部分的扩展变量*/
extern uint8_t gucChairRunStatus;                   /*按摩椅的运行状态*/
extern uint8_t gucChairExistFlag;                   /*是否存在控制板标志，默认是存在*/

//add by sxk
extern uint8_t gucChairChargeStatus;                /*按摩椅充电状态*/
extern uint8_t gucChairLongChargeFlag;             /*按摩椅一直充电标志位，默认关闭*/
extern uint16_t gucChargeTimeCount;         /*充电时间计数器*/
const  uint8_t gucFTPEXTGETCompare[19] =
{
    '+','F','T','P','E','X','T','G','E','T',':',' ','3',',','2','5','6', 0x0D, 0x0A
};

/*SIM800应用变量*/
uint8_t gucSIM800TxData[100];           /*SIM800发给SERVER端的数据缓冲区*/
uint8_t gucSIM800TxSize         = 0;    /*SIM800发给SERVER端的数据包长度*/
uint8_t gucSIM800TxStep         = 0;    /*SIM800发送步骤*/

uint8_t gucSIM800ReplyFlag      = 1;    /*SIM800回复标志*/
uint8_t gucSIM800ReplyTime      = 0;    /*SIM800回复超时*/

uint8_t gucServerReplyFlag      = 1;    /*SERVER回复标志*/
uint8_t gucServerReplyTime      = 0;    /*SERVER回复超时*/
uint8_t gucServerReplyCount     = 0;    /*SERVER回复超时计数*/

uint8_t gucSIM800ConnectFlag    = 0;    /*SIM800连网标志*/
uint8_t gucSIM800OnlineState    = 0;    /*SIM800上线标志*/
uint8_t gucSIM800StartupTime    = 0;    /*SIM800启动计时*/

uint8_t gucSIM800SendEnableFlag = 0;    /*SIM800发送数据使能标志*/

uint8_t gucSIM800CID[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   /*长度固定为20，CCID编号，也作为按摩椅编号*/
uint8_t gucSIM800CSQ[2]  = {0,0};                                       /*长度固定为02，CSQ信号强度*/
uint8_t gucSIM800GSN[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};             /*长度固定为15，GSN编号*/


/*系统应用变量*/
char gcSIM800NewLink[31] = "------------------------------";/*长度最大为30，SIM800新的通讯链接*/
char gcSIM800NewPort[6]  = "-----";                         /*固定长度为05，SIM800新的通讯端口*/

uint8_t gucUniversalTime[12];       /*固定长度为12，通用时间*/
uint8_t gucOrderNumber[14];         /*订单编号*/
uint8_t gucExperienceTime      = 0; /*体验时长，单位为分钟*/
uint8_t gucMassageManipulation = 0; /*按摩手法*/
uint8_t gucAirbagDynamics      = 0; /*气囊力度*/
uint8_t gucServerCtrlFlag      = 0; /*远程控制有效期标志*/

uint8_t gucChargeTime          = 0; /*充电时长，单位为分钟*/  // add by sxk
uint8_t guc_Charging_Flag = 0;

char     gcGSMBusiness[3]    = "??";    /*通讯的交易类型*/
char     gcGSMResponse[3]    = "55";    /*通讯的响应码*/
char     gcCorrespondent[20] = "-------------------";   /*通讯商*/

uint8_t  gucHeartbeatCount  = 0;    /*心跳计时计数器，每250ms增加1*/
uint8_t  gucUpdateCSQCount  = 0;    /*更新信号计数器，每1秒增加1*/

uint8_t  gucFTPUpgradeFlag  = 0;    /*是否要进行FTP升级标志*/
uint8_t  gucFTPUpgradeState = 0;    /*FTP当前升级的状态标志*/
uint8_t  gucFTPExtGetReady  = 0;    /*FTP获取文件已经准备好*/
uint32_t gulFTPExtGetIndex  = 0;    /*FTP当前获取数据的下标*/
uint32_t gulFTPExtGetLength = 0;    /*FTP获取到数据的总长度*/

uint8_t  gucFTPServer[50];          /*FTP主机*/
uint8_t  gucFTPServerLength   = 0;  /*FTP主机数据长度*/
uint8_t  gucFTPPort[4];             /*FTP端口号*/
uint8_t  gucFTPUserName[6];         /*FTP用户名*/
uint8_t  gucFTPPassword[50];        /*FTP密码密文*/
uint8_t  gucFTPPasswordLength = 0;  /*FTP密码密文数据长度*/
uint8_t  gucFTPGetName[20];         /*FTP文件名*/
uint8_t  gucFTPGetPath[50];         /*FTP文件路径*/
uint8_t  gucFTPGetPathLength  = 0;  /*FTP文件路径数据长度*/




uint8_t SIM800_GetModuleSTATUS(void)
{
    /*
    当SIM800模块处于正常工作状态后，STATU状态指示灯引脚会输出高电平，
    用户可以通过该引脚的电平来判断模块是否处于开机工作状态；
    */
    if(GPIO_PinInGet(SIM800_STATUS_PORT, SIM800_STATUS_PIN) == 0)
    {
        return 1;   /*查看相应的电路，三极管后状态反相了*/
    }
    else
    {
        return 0;
    }
}


/*
SIM800模块在回复CCID的时候，字母可能是大写，可能是小写；
我们在比较的时候忽略大小写的比较；
在CLIENT回复SERVER时，回复大小写都可以，SERVER会自行处理；
*/
uint8_t SIM800_CompareCCID(uint8_t *buffer)
{
    uint8_t i     = 0;
    uint8_t value = 0;
    uint8_t state = 1;

    for(i = 0; i < 20; i++)
    {
        if(buffer[i] != gucSIM800CID[i])
        {
            if(buffer[i] > gucSIM800CID[i])
            {
                value = buffer[i] - gucSIM800CID[i];
            }
            else
            {
                value = gucSIM800CID[i] - buffer[i];
            }

            if(('a' - 'A') != value) state = 0;
        }
    }

    return state;
}


void SIM800_PowerUp(void)
{
    /*MIC29302 EN控制引脚，给SIM800模块提供电源*/
    GPIO_PinOutSet(GSM_POWER_CTRL_PORT, GSM_POWER_CTRL_PIN);

    /*拉低PWRKEY引脚超过1秒然后释放，可使模块开机、关机*/
    GPIO_PinOutClear(SIM800_PWR_PORT, SIM800_PWR_PIN);

    delay_ms(2000);

    GPIO_PinOutSet(SIM800_PWR_PORT, SIM800_PWR_PIN);
}


void SIM800_PowerDown(void)
{
    /*MIC29302 EN控制引脚，断开SIM800模块电源*/
    GPIO_PinOutClear(GSM_POWER_CTRL_PORT, GSM_POWER_CTRL_PIN);

    delay_ms(500);
}


void SIM800_ItselfCommandParsing(uint8_t type, uint8_t *buffer)
{
    char CommandLine[10][150];

    uint8_t  i = 0,  j = 0,  k = 0;

    //uint16_t index = 0, length = 0;


    /*将buffer中的数据提取成一行一行的指令*/
    while(buffer[i] != '\0')
    {
        /*去除回车换行符*/
        while((buffer[i] == '\r') || (buffer[i] == '\n'))
        {
            i++;

            if(j)
            {
                /*置结束标志位，并切换到新行*/
                CommandLine[k++][j++] = '\0';   j = 0;
            }
        }

        /*提取有效的数据存储到CommandLine中*/
        CommandLine[k][j++] = buffer[i++];
    }

    /*置结束标志位*/
    CommandLine[k++][j] = '\0';


    switch(type)
    {
        case 0: /*以AT开头，OK结尾的命令解析*/
            if((strcmp(CommandLine[0], AT_CMD_AT)               == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_GSN)         == 0) && (strcmp(CommandLine[2], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;

                /*获取GSN编码*/
                for(i = 0; i < 15; i++) gucSIM800GSN[i] = CommandLine[1][i];
            }
            else if((strcmp(CommandLine[0], AT_CMD_CCID)        == 0) && (strcmp(CommandLine[2], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;

                /*获取CCID编码*/
                for(i = 0; i < 20; i++) gucSIM800CID[i] = CommandLine[1][i];
            }
            else if((strcmp(CommandLine[0], AT_CMD_CIPSHUT)     == 0) && (strcmp(CommandLine[1], "SHUT OK") == 0))
            {
                gucSIM800ConnectFlag = 0;   /*连接标志清零*/
                gucSIM800OnlineState = 0;
                gucSIM800ReplyFlag   = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CIPCLOSE)    == 0) && (strcmp(CommandLine[1], "CLOSE OK") == 0))
            {
                gucSIM800ConnectFlag = 0;   /*连接标志清零*/
                gucSIM800OnlineState = 0;
                gucSIM800ReplyFlag   = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CSQ_COPS)    == 0) && (strcmp(CommandLine[3], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;

                /*获取CSQ信号强度*/
                gucSIM800CSQ[0] = CommandLine[1][6];
                gucSIM800CSQ[1] = CommandLine[1][7];

                /*当信号强度为个位数时的判断处理*/
                if(gucSIM800CSQ[1] == ',')
                {
                    gucSIM800CSQ[1] = gucSIM800CSQ[0];
                    gucSIM800CSQ[0] = '0';
                }

                /*如果信号强度为0，或者获取失败；则重新获取CSQ数据*/
                if((gucSIM800CSQ[0] == '0') && (gucSIM800CSQ[1] == '0'))
                {
                    gucSIM800TxStep-=2; break;
                }

                if(PLATFORM_CHINA != 0) /*中国注册的SIM卡自动识别联通卡、移动卡*/
                {
                    if(sizeof(CommandLine[2]) > 12)
                    {
                        i = 0;

                        while(CommandLine[2][11+i] != '\0')
                        {
                            gcCorrespondent[i] = CommandLine[2][11+i];

                            i++;
                        }

                        gcCorrespondent[i] = '\0';

                        if((strcmp(gcCorrespondent, AT_RET_COPS_10010) != 0) && (strcmp(gcCorrespondent, AT_RET_COPS_10086) != 0))
                        {
                            gucSIM800TxStep-=2; break;
                        }
                    }
                    else
                    {
                        gucSIM800TxStep-=2; break;
                    }
                }
            }
            else if((strcmp(CommandLine[0], AT_CMD_CSQ)         == 0) && (strcmp(CommandLine[2], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;

                /*获取CSQ信号强度*/
                gucSIM800CSQ[0] = CommandLine[1][6];
                gucSIM800CSQ[1] = CommandLine[1][7];

                /*当信号强度为个位数时的判断处理*/
                if(gucSIM800CSQ[1] == ',')
                {
                    gucSIM800CSQ[1] = gucSIM800CSQ[0];
                    gucSIM800CSQ[0] = '0';
                }
            }
            else if((strcmp(CommandLine[0], AT_CMD_CREG)        == 0) && (strcmp(CommandLine[2], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CGATT)       == 0) && (strcmp(CommandLine[2], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CSTT_10010)  == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CSTT_10086)  == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CSTT_DU)     == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CSTT_RUSSIA) == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CIICR)       == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CIPSTART)    == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CIPSEND)     == 0) && (strcmp(CommandLine[2], "SEND OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_SAPBR_CONTYPE)     == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_SAPBR_APN_10010)   == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_SAPBR_APN_10086)   == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_SAPBR_APN_DU)    == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_SAPBR_START)     == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_SAPBR_QUERY)     == 0) && (strcmp(CommandLine[2], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_SAPBR_CLOSE)     == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucFTPExtGetReady  = 0;
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPMODE)     == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPTYPE)     == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPCID)      == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPSERV)     == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPPORT)     == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPUN)       == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPPW)       == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPGETNAME)  == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPGETPATH)  == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPEXTGET_START)  == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_FTPEXTGET_SAVE)  == 0) && (strcmp(CommandLine[2], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;

                gulFTPExtGetIndex  = 0;
                gulFTPExtGetLength = 0;

                i = 14;

                while(CommandLine[1][i] != '\0')
                {
                    gulFTPExtGetLength = (gulFTPExtGetLength * 10) + (CommandLine[1][i++] - 0x30);
                }
            }
            else
            {
                /*
                **只要是AT开头，OK结尾的都回复已响应:在重新连接CIPSTART时，会运行到这边；
                **因为此时的TCP的连接和端口号都是查询服务器反馈的，在程序中无法定义；
                */
                if(gucSIM800TxStep != 232)
                {
                    gucSIM800ReplyFlag = 1;
                }
            }
            break;


        case 1: /*没有以AT开头，但以OK结尾的命令解析*/
            if(strcmp(CommandLine[0], "CONNECT OK") == 0)
            {
                gucSIM800ReplyFlag   = 1;

                /*在TCP连接成功的时候，SIM800会自动上传CONNECT OK*/
                gucSIM800ConnectFlag = 1;
            }
            break;


        case 2: /*以AT开头，ERROR结尾的命令解析*/
            if((strcmp(CommandLine[0], AT_CMD_SAPBR_START) == 0) && (strcmp(CommandLine[1], "ERROR") == 0))
            {
                gucSIM800ReplyFlag = 1;
                gucSIM800TxStep    = 233;
            }
            else
            {
                SIM800_PowerDown(); InitSIM800();
            }
            break;


        case 3: /*没有以AT开头，但以ERROR结尾的命令解析*/
            SIM800_PowerDown(); InitSIM800();
            break;


        case 4: /*以CLOSED结尾的命令解析*/
            if(strcmp(CommandLine[0], "CLOSED") == 0)
            {
                if((gucSIM800TxStep == 231) || (gucSIM800TxStep == 232))
                {
                    return; /*升级的时候不处理这个异常*/
                }

                /*TCP连接自动断开的时候，SIM800会自动上传CLOSED*/
                gucSIM800ConnectFlag = 0;
                gucSIM800OnlineState = 0;
                gucSIM800ReplyFlag   = 1;

                /*重新连接*/
                gucSIM800TxStep      = 7;
            }
            else if(strcmp(CommandLine[0], "STATE: TCP CLOSED") == 0)
            {
                /*在TCP连接失败的时候，SIM800会自动上传STATE: TCP CLOSED*/
            }
            break;


        case 5: /*以FAIL结尾的命令解析*/
            /*在TCP连接失败的时候，SIM800会自动上传CONNECT FAIL*/
            SIM800_PowerDown(); InitSIM800();
            break;

        case 6: /*以Ready结尾的命令解析*/
            if(strcmp(CommandLine[0], "SMS Ready") == 0)
            {
                gucSIM800TxStep = 2;
            }
            break;

        default:
            break;
    }
}


void SIM800_ServerCommandParsing(uint8_t *buffer)
{
    uint8_t i = 0, length = 0;

    /*(*,01,TT,170414114645,!)*/
    gcGSMBusiness[0] = buffer[2];
    gcGSMBusiness[1] = buffer[3];
    gcGSMBusiness[2] = '\0';

    gcGSMResponse[0] = buffer[5];
    gcGSMResponse[1] = buffer[6];
    gcGSMResponse[2] = '\0';

    if(strcmp(gcGSMBusiness, CLIENT_CMD_ONLINE) == 0)               /*接收到网络板发送的CLIENT_CMD_ONLINE的回复*/
    {
        /*(*,01,TT,170414145610,!)*/

        if(strcmp(gcGSMResponse, "TT") == 0)
        {
            for(i = 0; i < 12; i++) gucUniversalTime[i] = buffer[i+8];

            /*上线后同步SERVER端与投币箱的时间*/
            MONEY_CommandEnqueue(MONEY_CMD_SET_DATE_TIME, gucUniversalTime, 12);

            /*这个SERVER对CLIENT的回复，不需要处理回复机制*/
            gcGSMBusiness[0] = '?';
            gcGSMBusiness[1] = '?';
            gcGSMBusiness[2] = '\0';

            if(gucSIM800TxStep < 200)
            {
                /*SERVER端回复ONLINE指令后，即进入到发送等待状态*/
                gucSIM800TxStep = 100;
            }

            if(gucSIM800OnlineState == 0)
            {
                GSM_HeartbeatHandler(1);
            }

            /*ONLINE状态置位*/
            gucSIM800OnlineState = 1;

            /*SERVER回复了ONLINE的指令*/
            gucServerReplyFlag = 1;
        }
        else
        {
            
            GSM_ReplyServerHandler(0);           
        }
    }
    else if(strcmp(gcGSMBusiness, CLIENT_CMD_ASKFOR_NEWLINK) == 0)  /*接收到网络板发送的CLIENT_CMD_ASKFOR_NEWLINK的回复*/
    {
        /*(*,31,AA,898602b7091700545108,16,tx2.51shaoxi.com,05488,!)*/

        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*获取新链接信息的数据长度，长度是不固定的*/
                length  = 0;
                length += (buffer[29] - 0x30) * 0x0A;
                length += (buffer[30] - 0x30) * 0x01;

                /*获取新的链接信息*/
                for(i = 0; i < length; i++)
                {
                    gcSIM800NewLink[i] = buffer[i+32];
                }
                gcSIM800NewLink[i] = '\0';


                /*获取新的端口号，长度固定为5个字节*/
                for(i = 0; i < 5; i++)
                {
                    gcSIM800NewPort[i] = buffer[i+33+length];
                }
                gcSIM800NewPort[i] = '\0';


                /*数据格式处理:将端口号最前面无用的0去掉*/
                while(gcSIM800NewPort[0] == '0')
                {
                    for(i = 0; i < 4; i++)
                    {
                        gcSIM800NewPort[i] = gcSIM800NewPort[i+1];
                    }

                    gcSIM800NewPort[i] = '\0';
                }

                /*这个SERVER对CLIENT的回复，不需要处理回复机制*/
                gcGSMBusiness[0] = '?';
                gcGSMBusiness[1] = '?';
                gcGSMBusiness[2] = '\0';

                /*当SERVER回复新的链接和端口后，进行重新连接*/
                gucSIM800TxStep    = 59;
            }

            gucServerReplyFlag = 1;
        }
        else
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*查询服务器返回失败*/
            }
        }
    }
    else if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_HEARTBEAT) == 0)    /*接收到网络板发送的CLIENT_CMD_UPLOAD_HEARTBEAT的回复*/
    {
        /*这个SERVER对CLIENT的回复，不需要处理回复机制*/
        gcGSMBusiness[0] = '?';
        gcGSMBusiness[1] = '?';
        gcGSMBusiness[2] = '\0';

        gucServerReplyFlag = 1;
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_START_RUNNING) == 0)       /*微信远程启动*/
    {
        /*(*,05,AA,12345678901234567890,12345678901234,1234,12,12,!)*/
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*记录订单编号*/
                for(i = 0; i < 14; i++)
                {
                    gucOrderNumber[i] = buffer[i+29];
                }

                if(MONEY_GetWorkState() == 1)
                {
                    /*在按摩椅处于投币运行的状态下，如果收到SERVER端的启动命令则直接回复失败*/
                    
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*获取体验时长*/
                    gucExperienceTime       = 0;
                    gucExperienceTime      += (buffer[44] - '0') * 1000;
                    gucExperienceTime      += (buffer[45] - '0') * 100;
                    gucExperienceTime      += (buffer[46] - '0') * 10;
                    gucExperienceTime      += (buffer[47] - '0') * 1;
                    
                    gucChargeTimeCount = 0;//add by sxk
                    gucChargeTimeCount = (gucExperienceTime  * 60) + 20 ;//add by sxk                    

                    if(buffer[49] != '!')   /*兼容老平台的通讯协议，老的通讯协议没有按摩手法和气囊力度的参数传递*/
                    {
                        /*获取按摩手法*/
                        gucMassageManipulation  = 0;
                        gucMassageManipulation += (buffer[49] - '0') * 10;
                        gucMassageManipulation += (buffer[50] - '0') * 1;

                        /*获取气囊力度*/
                        gucAirbagDynamics       = 0;
                        gucAirbagDynamics      += (buffer[52] - '0') * 10;
                        gucAirbagDynamics      += (buffer[53] - '0') * 1;
                    }
                    else
                    {
                        gucMassageManipulation  = 1;
                        gucAirbagDynamics       = 3;
                    }
                    
                    MCB_CommandEnqueue(H10_CHARGE_WORK_ENABLE, gucExperienceTime);
                    guc_Charging_Flag = 1;                   

                    MCB_CommandEnqueue(H10_KEY_WORK_ENABLE, gucExperienceTime);

                    /*根据不同的按摩手法进行相应的手法选择*/
                    switch(gucMassageManipulation)
                    {
                        case 1:/*MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_1, gucExperienceTime); break;*/
                        case 2:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_0, gucExperienceTime); break;
                        case 3:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_1, gucExperienceTime); break;
                        case 4:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_2, gucExperienceTime); break;
                        case 5:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_3, gucExperienceTime); break;
                        case 6:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_4, gucExperienceTime); break;
                        case 7:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_5, gucExperienceTime); break;
                        default: MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_0, gucExperienceTime); break;
                    }

                    /*根据不同的气囊力度进行相应的力度选择*/
                    switch(gucAirbagDynamics)
                    {
                        case 0:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_0, gucExperienceTime); break;
                        case 1:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_1, gucExperienceTime); break;
                        case 2:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_2, gucExperienceTime); break;
                        case 3:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_3, gucExperienceTime); break;
                        case 4:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_4, gucExperienceTime); break;
                        case 5:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_5, gucExperienceTime); break;
                        default: MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_3, gucExperienceTime); break;
                    }

                    /*置位远程控制有效期标志位*/
                    gucServerCtrlFlag = 1;

                    /*禁止投币箱投币功能*/
                    MONEY_Demo(0x08);
                    
                    
                    GSM_ReplyServerHandler(1);
                }
            }
            else
            {
                
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_CLOSE_RUNNING) == 0)       /*微信远程关闭*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*对比订单编号*/
                for(i = 0; i < 14; i++)
                {
                    if(buffer[i+29] != gucOrderNumber[i]) break;
                }

                if(i == 14)
                {
                    if(MONEY_GetWorkState() == 1)
                    {
                        /*在按摩椅处于投币运行的状态下，如果收到SERVER端的关闭命令则直接回复失败*/                        
                        GSM_ReplyServerHandler(0);
                    }
                    else
                    {


                        /*让控制板停止工作*/
                        MCB_CommandEnqueue(H10_KEY_WORK_DISABLE, gucExperienceTime);
                        
                        MCB_CommandEnqueue(H10_CHARGE_WORK_DISABLE, gucExperienceTime);
                        guc_Charging_Flag = 0;
                        gucChargeTimeCount = 0;                       
                       
                        GSM_ReplyServerHandler(1);
                    }
                }
                else
                {
                    GSM_ReplyServerHandler(0);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_EXTEND_WORKING_TIME) == 0) /*微信远程续费*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*对比订单编号*/
                for(i = 0; i < 14; i++)
                {
                    if(buffer[i+29] != gucOrderNumber[i]) break;
                }

                if(i == 14)
                {
                    if(MONEY_GetWorkState() == 1)
                    {
                        /*在按摩椅处于投币运行的状态下，如果收到SERVER端的续费命令则直接回复失败*/
                        GSM_ReplyServerHandler(0);
                    }
                    else
                    {
                        // to do...
                       
                        GSM_ReplyServerHandler(1);
                    }
                }
                else
                {
                    
                    GSM_ReplyServerHandler(0);
                }
            }
            else
            {
                
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_CHAIR_SET_PARAMETER) == 0) /*微信APP控制命令*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*对比订单编号*/
                for(i = 0; i < 14; i++)
                {
                    if(buffer[i+29] != gucOrderNumber[i]) break;
                }
                
                //i = 14; for debug

                if(i == 14)
                {
                    if(MONEY_GetWorkState() == 1)
                    {
                        /*在按摩椅处于投币运行的状态下，如果收到SERVER端的控制命令则直接回复失败*/
                        GSM_ReplyServerHandler(0);
                    }
                    else
                    {
                        // to do...2017.11.12

                        /*获取按摩手法*/
                        gucMassageManipulation  = 0;
                        gucMassageManipulation += (buffer[44] - '0') * 10;
                        gucMassageManipulation += (buffer[45] - '0') * 1;

                        /*获取气囊力度*/
                        gucAirbagDynamics       = 0;
                        gucAirbagDynamics      += (buffer[47] - '0') * 10;
                        gucAirbagDynamics      += (buffer[48] - '0') * 1;

                        /*根据不同的按摩手法进行相应的手法选择*/
                        switch(gucMassageManipulation)
                        {
                            case 1:/*MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_1, gucExperienceTime); break;*/
                            case 2:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_0, gucExperienceTime); break;
                            case 3:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_1, gucExperienceTime); break;
                            case 4:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_2, gucExperienceTime); break;
                            case 5:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_3, gucExperienceTime); break;
                            case 6:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_4, gucExperienceTime); break;
                            case 7:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_5, gucExperienceTime); break;
                            default: MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_0, gucExperienceTime); break;
                        }

                        /*根据不同的气囊力度进行相应的力度选择*/
                        switch(gucAirbagDynamics)
                        {
                            case 0:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_0, gucExperienceTime); break;
                            case 1:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_1, gucExperienceTime); break;
                            case 2:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_2, gucExperienceTime); break;
                            case 3:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_3, gucExperienceTime); break;
                            case 4:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_4, gucExperienceTime); break;
                            case 5:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_5, gucExperienceTime); break;
                            default: MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_3, gucExperienceTime); break;
                        }

                        GSM_ReplyServerHandler(1);
                    }
                }
                else
                {
                    GSM_ReplyServerHandler(0);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
        else
        {
            GSM_ReplyServerHandler(0);
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_CHAIR_QUERY_STATUS) == 0)  /*按摩椅状态查询*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*Query Chair Status*/
            
                GSM_ReplyServerHandler(1);
            }
            else
            {
                
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_UPGRADING_PROGRAM) == 0)   /*微信远程升级命令*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*(*,30,AA,898602b7091700544075,14,139.196.138.73,0021,sxftpc,44,oQDDAu5rqccvdCHcUuuCqQWOkqZkwCEujmDPN7unWw0=,M13NET_SX2G_0200.BIN,07,update/,!)*/

                gucFTPServerLength = (buffer[29] - 0x30) * 10 + (buffer[30] - 0x30);

                for(i = 0; i < gucFTPServerLength; i++) gucFTPServer[i] = buffer[i+32];

                for(i = 0; i < 4; i++) gucFTPPort[i] = buffer[i+33+gucFTPServerLength];

                for(i = 0; i < 6; i++) gucFTPUserName[i] = buffer[i+38+gucFTPServerLength];

                gucFTPPasswordLength = (buffer[45+gucFTPServerLength] - 0x30) * 10 + (buffer[46+gucFTPServerLength] - 0x30);

                for(i = 0; i < gucFTPPasswordLength; i++) gucFTPPassword[i] = buffer[i+48+gucFTPServerLength];

                for(i = 0; i < 20; i++) gucFTPGetName[i] = buffer[i+49+gucFTPServerLength+gucFTPPasswordLength];

                gucFTPGetPathLength  = (buffer[70+gucFTPServerLength+gucFTPPasswordLength] - 0x30) * 10;
                gucFTPGetPathLength += (buffer[71+gucFTPServerLength+gucFTPPasswordLength] - 0x30);

                for(i = 0; i < gucFTPGetPathLength; i++) gucFTPGetPath[i] = buffer[i+73+gucFTPServerLength+gucFTPPasswordLength];

                gucFTPUpgradeFlag = 1;
                
                GSM_ReplyServerHandler(1);
            }
            else
            {
                
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_ALL) == 0)  /*微信APP查询钱币总额、总识币数*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*Query All*/
                MONEY_CommandEnqueue(MONEY_CMD_QUERY_ALL, buffer, 0x00);
            }
            else
            {
                
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ALL) == 0)  /*微信APP清除钱币总额、总识币数*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if((MONEY_GetWorkState() == 1) || (gucMoneyExistFlag == 0))
                {
                    /*在按摩椅处于投币运行的状态下，如果收到SERVER端的清除命令则直接回复失败*/
                    
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*Clear All*/
                    MONEY_CommandEnqueue(MONEY_CMD_CLEAR_ALL, buffer, 0x00);
                }
            }
            else
            {               
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_RATE) == 0) /*微信APP查询费率*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if(gucMoneyExistFlag == 0)
                {                   
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    MONEY_CommandEnqueue(MONEY_CMD_QUERY_RATE, buffer, 0x00);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_RATE) == 0)   /*微信APP设置费率*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if((MONEY_GetWorkState() == 1) || (gucMoneyExistFlag == 0))
                {
                    /*在按摩椅处于投币运行的状态下，如果收到SERVER端的设置费率命令则直接回复失败*/
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    buffer[0] = buffer[29];     /*Money*/
                    buffer[1] = buffer[30];
                    buffer[2] = buffer[32];     /*Time*/
                    buffer[3] = buffer[33];

                    MONEY_CommandEnqueue(MONEY_CMD_SET_RATE, buffer, 0x04);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_TIME) == 0) /*微信APP查询投币箱时间*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if(gucMoneyExistFlag == 0)
                {
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*Query Coin Box Time*/
                    MONEY_CommandEnqueue(MONEY_CMD_QUERY_DATE_TIME, buffer, 0x00);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_TIME) == 0)   /*微信APP设置投币箱时间*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*(*,16,AA,11112222333344445555,112233445566,!)*/
                if((MONEY_GetWorkState() == 1) || (gucMoneyExistFlag == 0))
                {
                    /*在按摩椅处于投币运行的状态下，如果收到SERVER端的设置时间命令则直接回复失败*/
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    for(i = 0; i < 12; i++) buffer[i] = buffer[i+29];

                    MONEY_CommandEnqueue(MONEY_CMD_SET_DATE_TIME, buffer, 0x0C);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_UNLOCKING) == 0)  /*微信APP发送开锁命令*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if(gucMoneyExistFlag == 0)
                {
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*Unlocking*/
                    MONEY_CommandEnqueue(MONEY_CMD_UNLOCKING, buffer, 0x00);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_STATUS) == 0) /*微信查询投币箱门状态*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if(gucMoneyExistFlag == 0)
                {
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*Query Coin Box Unlocking Status*/
                    MONEY_CommandEnqueue(MONEY_CMD_QUERY_UNLOCKING_STATUS, buffer, 0x00);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_TIMES ) == 0) /*微信查询投币箱累计开锁次数*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if(gucMoneyExistFlag == 0)
                {
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*Query Unlocking Times*/
                    MONEY_CommandEnqueue(MONEY_CMD_QUERY_UNLOCKING_TIMES, buffer, 0x00);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_LEGAL_UNLOCKING_TIMES ) == 0)   /*微信清除投币箱累计合法开锁次数*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if(gucMoneyExistFlag == 0)
                {
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*Clear Legal Unlocking Times*/
                    MONEY_CommandEnqueue(MONEY_CMD_CLEAR_LEGAL_UNLOCKING_TIMES, buffer, 0x00);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ILLEGALUNLOCKING_TIMES) == 0)   /*微信清除投币箱累计不合法开锁次数*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if(gucMoneyExistFlag == 0)
                {
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*Clear Illegal Unlocking Times*/
                    MONEY_CommandEnqueue(MONEY_CMD_CLEAR_ILLEGAL_UNLOCKING_TIMES, buffer, 0x00);
                }
                
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_START_CHARGE) == 0)       /*微信远程启动带充电功能*/
    {
        /*(*,05,AA,12345678901234567890,12345678901234,1234,12,12,!)*/
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*记录订单编号*/
                for(i = 0; i < 14; i++)
                {
                    gucOrderNumber[i] = buffer[i+29];
                }

                if(MONEY_GetWorkState() == 1)
                {
                    /*在按摩椅处于投币运行的状态下，如果收到SERVER端的启动命令则直接回复失败*/
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*获取体验时长*/
                    gucExperienceTime       = 0;
                    gucExperienceTime      += (buffer[44] - '0') * 1000;
                    gucExperienceTime      += (buffer[45] - '0') * 100;
                    gucExperienceTime      += (buffer[46] - '0') * 10;
                    gucExperienceTime      += (buffer[47] - '0') * 1;
                    /*获取充电时间*/
                    gucChargeTime           = 0;
                    gucChargeTime          += (buffer[55] - '0') * 1000;
                    gucChargeTime          += (buffer[56] - '0') * 100;
                    gucChargeTime          += (buffer[57] - '0') * 10;
                    gucChargeTime          += (buffer[58] - '0') * 1;
                    gucChargeTimeCount = 0;//add by sxk
                    gucChargeTimeCount = (gucChargeTime * 60) + 20 ;//add by sxk

                    if(buffer[49] != '!')   /*兼容老平台的通讯协议，老的通讯协议没有按摩手法和气囊力度的参数传递*/
                    {
                        /*获取按摩手法*/
                        gucMassageManipulation  = 0;
                        gucMassageManipulation += (buffer[49] - '0') * 10;
                        gucMassageManipulation += (buffer[50] - '0') * 1;

                        /*获取气囊力度*/
                        gucAirbagDynamics       = 0;
                        gucAirbagDynamics      += (buffer[52] - '0') * 10;
                        gucAirbagDynamics      += (buffer[53] - '0') * 1;
                    }
                    else
                    {
                        gucMassageManipulation  = 1;
                        gucAirbagDynamics       = 3;
                    }
                    
                           /*使能充电*/
                    if(gucChargeTime != 0)
                    MCB_CommandEnqueue(H10_CHARGE_WORK_ENABLE, gucChargeTime);
                    guc_Charging_Flag = 1;
                    if(gucExperienceTime != 0)
                    {
 
                    /*使能工作控制板*/
                        MCB_CommandEnqueue(H10_KEY_WORK_ENABLE, gucExperienceTime);
                                            /*置位远程控制有效期标志位*/
                        gucServerCtrlFlag = 1;

                    /*禁止投币箱投币功能*/
                        MONEY_Demo(0x08);
                    }

                    /*根据不同的按摩手法进行相应的手法选择*/
                    switch(gucMassageManipulation)
                    {
                        case 1:/*MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_1, gucExperienceTime); break;*/
                        case 2:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_0, gucExperienceTime); break;
                        case 3:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_1, gucExperienceTime); break;
                        case 4:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_2, gucExperienceTime); break;
                        case 5:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_3, gucExperienceTime); break;
                        case 6:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_4, gucExperienceTime); break;
                        case 7:  MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_5, gucExperienceTime); break;
                        default: MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_0, gucExperienceTime); break;
                    }

                    /*根据不同的气囊力度进行相应的力度选择*/
                    switch(gucAirbagDynamics)
                    {
                        case 0:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_0, gucExperienceTime); break;
                        case 1:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_1, gucExperienceTime); break;
                        case 2:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_2, gucExperienceTime); break;
                        case 3:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_3, gucExperienceTime); break;
                        case 4:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_4, gucExperienceTime); break;
                        case 5:  MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_5, gucExperienceTime); break;
                        default: MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_3, gucExperienceTime); break;
                    }
                    
//#endif  

                    
                    GSM_ReplyServerHandler(1);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
        else
        {
            GSM_ReplyServerHandler(0);
        }
    }
    
    else if(strcmp(gcGSMBusiness, SERVER_CMD_CLOSE_CHARGE) == 0)       /*微信远程关闭充电*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*对比订单编号*/
                for(i = 0; i < 14; i++)
                {
                    if(buffer[i+29] != gucOrderNumber[i]) break;
                }

                if(i == 14)
                {
                    if(MONEY_GetWorkState() == 1)
                    {
                        /*在按摩椅处于投币运行的状态下，如果收到SERVER端的关闭命令则直接回复失败*/
                        GSM_ReplyServerHandler(0);
                    }
                    else
                    {

  

                        /*让控制板停止工作*/
                        MCB_CommandEnqueue(H10_KEY_WORK_DISABLE, gucExperienceTime);
                        /*停止充电*/
                        MCB_CommandEnqueue(H10_CHARGE_WORK_DISABLE, gucChargeTime);
                        guc_Charging_Flag = 0;
                        gucChargeTimeCount = 0;

                        GSM_ReplyServerHandler(1);
                    }
                }
                else
                {
                    GSM_ReplyServerHandler(0);
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
        else
        {
            GSM_ReplyServerHandler(0);
        }
    }
 
     else if(strcmp(gcGSMBusiness, SERVER_CMD_SET_LONG_CHARGE) == 0)  /*设置按摩椅通电后一直提供充电功能*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                gucChairLongChargeFlag = buffer[29];
                if(gucChairLongChargeFlag == '1')
                {
                    MCB_CommandEnqueue(H10_LONGCHARGE_WORK_ENABLE, gucExperienceTime);    
                    GSM_ReplyServerHandler(1); 
                }
                else if(gucChairLongChargeFlag == '0')
                {
                    MCB_CommandEnqueue(H10_LONGCHARGE_WORK_DISABBLE, gucExperienceTime); 
                    GSM_ReplyServerHandler(1);                    
                }
                else
                {
                    GSM_ReplyServerHandler(0); 
                }
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
        else
        {
            GSM_ReplyServerHandler(0);
        }
    }
    
     else if(strcmp(gcGSMBusiness, SERVER_CMD_QUERY_LONG_CHARGE) == 0)  /*查询按摩椅通电后一直提供充电功能*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                GSM_ReplyServerHandler(1);                    
            }
            else
            {
                GSM_ReplyServerHandler(0);
            }
        }
        else
        {
            GSM_ReplyServerHandler(0);
        }
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COMMUNICATION_TEST) == 0)
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*比较SERVER端发送的编号是否与当前椅子的编号一致*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {                
                GSM_ReplyServerHandler(1);                    
            }
            else
            {               
                GSM_ReplyServerHandler(0);
            }
        }
        else
        {
            GSM_ReplyServerHandler(0);
        }     
    }
  
    else
    {
        gcGSMBusiness[0] = '?';
        gcGSMBusiness[1] = '?';
        gcGSMBusiness[2] = '\0';

        gcGSMResponse[0] = '5';
        gcGSMResponse[1] = '5';
        gcGSMResponse[2] = '\0';
    }
}


void SIM800_RX_ExtractDatHandler(uint8_t value)
{
    static uint8_t ucExtractData[19];

    uint16_t i = 0, index = 0;


    for(i = 0; i < 18; i++) ucExtractData[i] = ucExtractData[i+1];

    ucExtractData[i] = value;

    for(i = 0; i < 19; i++)
    {
        if(ucExtractData[i] != gucFTPEXTGETCompare[i]) break;
    }

    if(i == 19)
    {
        if(gulFTPExtGetIndex < gulFTPExtGetLength)
        {
            for(i = 0; i < 256; i++)
            {
                index = (gulFTPExtGetIndex + i) % FLASH_PAGE_SIZE; WDOG_Feed();

                gucFlashPageData[index] = QUEUE_READ(SIM800_QUEUE_RX_IDX);
            }

            if(index == (FLASH_PAGE_SIZE - 1))
            {
#if 1
                FLASH_UpgradeProgram(gulFTPExtGetIndex / FLASH_PAGE_SIZE);
#else
                for(i = 0; i < FLASH_PAGE_SIZE; i++)
                {
                    LEUART_Tx(LEUART0, gucFlashPageData[i]); WDOG_Feed();
                }
#endif
            }

            gulFTPExtGetIndex += 256;
        }

        gucSIM800ReplyFlag = 1;
    }
}


/*
**SIM800有几种接收的方式:
**1.以AT开头，OK结尾
**2.以OK结尾
**3.以AT开头，ERROR结尾
**4.SERVER端的消息
**5.以FAIL结尾
*/
void SIM800_RX_Handler(void)
{
    uint8_t i = 0, ucData = 0;

    static uint8_t gucServerFlag  = 0;  /*SERVER接收数据起始标志位*/
    static uint8_t gucServerSize  = 0;  /*SERVER接收数据长度*/
    static uint8_t gucServerData[200];  /*SERVER接收数据缓冲区*/

    static uint8_t gucSIM800Flag = 0;   /*SIM800起始AT标志位*/
    static uint8_t gucSIM800Size = 0;   /*SIM800接收数据长度*/
    static uint8_t gucSIM800Data[200];  /*SIM800接收数据缓冲区*/
    static uint8_t gucSIM800Count= 0;   /*SIM800接收累加器*/

    static char    gcSIM800RxSymbol[2]   = "-";                 /*符号> 关键字缓冲区*/
    static char    gcSIM800RxOK[3]       = "--";                /*OK    关键字缓冲区*/
    static char    gcSIM800RxAT[4]       = "---";               /*AT    关键字缓冲区*/
    static char    gcSIM800RxFAIL[5]     = "----";              /*FAIL  关键字缓冲区*/
    static char    gcSIM800RxERROR[6]    = "-----";             /*ERROR 关键字缓冲区*/
    static char    gcSIM800RxReady[6]    = "-----";             /*Ready 关键字缓冲区*/
    static char    gcSIM800RxCLOSED[7]   = "------";            /*CLOSED关键字缓冲区*/
    static char    gcSIM800FTPEXTGET[16] = "---------------";   /*FTPEXTGET FINISHED*/
    static char    gcSIM800FTPERROR[17]  = "----------------";  /*FTPEXTGET ERROR*/

    /*在收到>后，不允许命令解析打断输入*/
    if(
        (gucSIM800TxStep == 53) ||  /*上线指令数据*/
        (gucSIM800TxStep == 57) ||  /*请求新链接的指令数据*/
        (gucSIM800TxStep == 103))   /*回复数据*/
    {
        return;
    }

    /*判断SIM800的接收消息队列是否为空*/
    if(QUEUE_EMPTY(SIM800_QUEUE_RX_IDX) == 0)
    {
        ucData = QUEUE_READ(SIM800_QUEUE_RX_IDX);

        /*在升级的时候接收到的命令和数据特殊处理，加快处理速度*/
        if((gucSIM800TxStep == 231) || (gucSIM800TxStep == 232))
        {
            SIM800_RX_ExtractDatHandler(ucData); return;
        }


        /*将读取到的数据存放在命令预解析数组中*/
        gucSIM800Data[gucSIM800Size++] = ucData;

        /*检测关键字:AT*/
        gcSIM800RxAT[0] = gcSIM800RxAT[1];
        gcSIM800RxAT[1] = gcSIM800RxAT[2];
        gcSIM800RxAT[2] = ucData;
        gcSIM800RxAT[3] = '\0';

        if(strcmp(gcSIM800RxAT, "AT+") == 0)
        {
            gucSIM800Flag = 1;
            gucSIM800Size = 3;  /*AT后面的数据从下标3开始存放*/

            gucSIM800Data[0] = 'A';
            gucSIM800Data[1] = 'T';
            gucSIM800Data[2] = '+';

            /*清除关键字比较缓冲数组*/
            strncpy(gcSIM800RxAT, "---", sizeof("---"));
        }
        else if(strcmp(gcSIM800RxAT, "AT\r") == 0)
        {
            gucSIM800Flag = 1;
            gucSIM800Size = 3;  /*AT后面的数据从下标3开始存放*/

            gucSIM800Data[0] = 'A';
            gucSIM800Data[1] = 'T';
            gucSIM800Data[2] = '\r';

            /*清除关键字比较缓冲数组*/
            strncpy(gcSIM800RxAT, "---", sizeof("---"));
        }
        else
        {
            // to do...nothing
        }

        gcSIM800RxSymbol[0] = ucData;

        if(strcmp(gcSIM800RxSymbol, ">") == 0)
        {
            if(gucSIM800Flag == 1)
            {
                gucSIM800SendEnableFlag = 1;
            }
        }
        else if(strcmp(gcSIM800RxSymbol, ".") == 0)
        {
            gucSIM800Count++;

            if(gucSIM800Count == 3)
            {
                gucSIM800Count = 0;

                if(gucSIM800Flag == 1)
                {
                    /*在获取IP地址时，以AT开头，连接出现3个点说明得到了SIM800的回复*/
                    if(gucSIM800TxStep == 20) gucSIM800ReplyFlag = 1;
                }
            }
        }
        else
        {
            // to do...nothing
        }

        /*检测关键字:OK*/
        gcSIM800RxOK[0] = gcSIM800RxOK[1];
        gcSIM800RxOK[1] = ucData;
        gcSIM800RxOK[2] = '\0';

        if(strcmp(gcSIM800RxOK, "OK") == 0)
        {
            /*设置结束标志位*/
            gucSIM800Data[gucSIM800Size] = '\0';

            if(gucSIM800Flag == 1)
            {
                /*以AT开头，OK结尾的命令解析*/
                SIM800_ItselfCommandParsing(0, gucSIM800Data);
            }
            else
            {
                /*没有以AT开头，但以OK结尾的命令解析*/
                SIM800_ItselfCommandParsing(1, gucSIM800Data);
            }

            gucSIM800Flag = 0;
            gucSIM800Size = 0;

            /*清除关键字比较缓冲数组*/
            strncpy(gcSIM800RxOK, "--", sizeof("--"));
        }

        /*检测关键字:ERROR*/
        gcSIM800RxERROR[0] = gcSIM800RxERROR[1];
        gcSIM800RxERROR[1] = gcSIM800RxERROR[2];
        gcSIM800RxERROR[2] = gcSIM800RxERROR[3];
        gcSIM800RxERROR[3] = gcSIM800RxERROR[4];
        gcSIM800RxERROR[4] = ucData;
        gcSIM800RxERROR[5] = '\0';

        if(strcmp(gcSIM800RxERROR, "ERROR") == 0)
        {
            /*设置结束标志位*/
            gucSIM800Data[gucSIM800Size] = '\0';

            if(gucSIM800Flag == 1)
            {
                /*以AT开头，ERROR结尾的命令解析*/
                SIM800_ItselfCommandParsing(2, gucSIM800Data);
            }
            else
            {
                /*没有以AT开头，但以ERROR结尾的命令解析*/
                SIM800_ItselfCommandParsing(3, gucSIM800Data);
            }

            gucSIM800Flag = 0;
            gucSIM800Size = 0;

            /*清除关键字比较缓冲数组*/
            strncpy(gcSIM800RxERROR, "-----", sizeof("-----"));
        }


        gcSIM800RxReady[0] = gcSIM800RxReady[1];
        gcSIM800RxReady[1] = gcSIM800RxReady[2];
        gcSIM800RxReady[2] = gcSIM800RxReady[3];
        gcSIM800RxReady[3] = gcSIM800RxReady[4];
        gcSIM800RxReady[4] = ucData;
        gcSIM800RxReady[5] = '\0';

        if(strcmp(gcSIM800RxReady, "Ready") == 0)
        {
            /*设置结束标志位*/
            gucSIM800Data[gucSIM800Size] = '\0';

            if(gucSIM800Flag == 1)
            {
                /*以AT开头，ERROR结尾的命令解析*/
            }
            else
            {
                /*没有以AT开头，但以Ready结尾的命令解析*/
                SIM800_ItselfCommandParsing(6, gucSIM800Data);
            }

            gucSIM800Flag = 0;
            gucSIM800Size = 0;

            /*清除关键字比较缓冲数组*/
            strncpy(gcSIM800RxReady, "-----", sizeof("-----"));
        }


        /*检测关键字:CLOSED*/
        gcSIM800RxCLOSED[0] = gcSIM800RxCLOSED[1];
        gcSIM800RxCLOSED[1] = gcSIM800RxCLOSED[2];
        gcSIM800RxCLOSED[2] = gcSIM800RxCLOSED[3];
        gcSIM800RxCLOSED[3] = gcSIM800RxCLOSED[4];
        gcSIM800RxCLOSED[4] = gcSIM800RxCLOSED[5];
        gcSIM800RxCLOSED[5] = ucData;
        gcSIM800RxCLOSED[6] = '\0';

        if(strcmp(gcSIM800RxCLOSED, "CLOSED") == 0)
        {
            /*设置结束标志位*/
            gucSIM800Data[gucSIM800Size] = '\0';

            /*以CLOSED结尾的命令解析*/
            SIM800_ItselfCommandParsing(4, gucSIM800Data);

            gucSIM800Flag = 0;
            gucSIM800Size = 0;

            /*清除关键字比较缓冲数组*/
            strncpy(gcSIM800RxCLOSED, "------", sizeof("------"));
        }

        /*检测关键字:FAIL*/
        gcSIM800RxFAIL[0] = gcSIM800RxFAIL[1];
        gcSIM800RxFAIL[1] = gcSIM800RxFAIL[2];
        gcSIM800RxFAIL[2] = gcSIM800RxFAIL[3];
        gcSIM800RxFAIL[3] = ucData;
        gcSIM800RxFAIL[4] = '\0';

        if(strcmp(gcSIM800RxFAIL, "FAIL") == 0)
        {
            /*设置结束标志位*/
            gucSIM800Data[gucSIM800Size] = '\0';

            /*以FAIL结尾的命令解析*/
            SIM800_ItselfCommandParsing(5, gucSIM800Data);

            gucSIM800Flag = 0;
            gucSIM800Size = 0;

            /*清除关键字比较缓冲数组*/
            strncpy(gcSIM800RxFAIL, "----", sizeof("----"));
        }


        gcSIM800FTPEXTGET[0]  = gcSIM800FTPEXTGET[1];
        gcSIM800FTPEXTGET[1]  = gcSIM800FTPEXTGET[2];
        gcSIM800FTPEXTGET[2]  = gcSIM800FTPEXTGET[3];
        gcSIM800FTPEXTGET[3]  = gcSIM800FTPEXTGET[4];
        gcSIM800FTPEXTGET[4]  = gcSIM800FTPEXTGET[5];
        gcSIM800FTPEXTGET[5]  = gcSIM800FTPEXTGET[6];
        gcSIM800FTPEXTGET[6]  = gcSIM800FTPEXTGET[7];
        gcSIM800FTPEXTGET[7]  = gcSIM800FTPEXTGET[8];
        gcSIM800FTPEXTGET[8]  = gcSIM800FTPEXTGET[9];
        gcSIM800FTPEXTGET[9]  = gcSIM800FTPEXTGET[10];
        gcSIM800FTPEXTGET[10] = gcSIM800FTPEXTGET[11];
        gcSIM800FTPEXTGET[11] = gcSIM800FTPEXTGET[12];
        gcSIM800FTPEXTGET[12] = gcSIM800FTPEXTGET[13];
        gcSIM800FTPEXTGET[13] = gcSIM800FTPEXTGET[14];
        gcSIM800FTPEXTGET[14] = ucData;
        gcSIM800FTPEXTGET[15] = '\0';

        if(strcmp(gcSIM800FTPEXTGET, "+FTPEXTGET: 1,0") == 0)
        {
            gucFTPExtGetReady  = 1;
            gucFTPUpgradeState = 1;
        }


        gcSIM800FTPERROR[0]  = gcSIM800FTPERROR[1];
        gcSIM800FTPERROR[1]  = gcSIM800FTPERROR[2];
        gcSIM800FTPERROR[2]  = gcSIM800FTPERROR[3];
        gcSIM800FTPERROR[3]  = gcSIM800FTPERROR[4];
        gcSIM800FTPERROR[4]  = gcSIM800FTPERROR[5];
        gcSIM800FTPERROR[5]  = gcSIM800FTPERROR[6];
        gcSIM800FTPERROR[6]  = gcSIM800FTPERROR[7];
        gcSIM800FTPERROR[7]  = gcSIM800FTPERROR[8];
        gcSIM800FTPERROR[8]  = gcSIM800FTPERROR[9];
        gcSIM800FTPERROR[9]  = gcSIM800FTPERROR[10];
        gcSIM800FTPERROR[10] = gcSIM800FTPERROR[11];
        gcSIM800FTPERROR[11] = gcSIM800FTPERROR[12];
        gcSIM800FTPERROR[12] = gcSIM800FTPERROR[13];
        gcSIM800FTPERROR[13] = gcSIM800FTPERROR[14];
        gcSIM800FTPERROR[14] = gcSIM800FTPERROR[15];
        gcSIM800FTPERROR[15] = ucData;
        gcSIM800FTPERROR[16] = '\0';

        if(
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,61") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,62") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,63") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,64") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,65") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,66") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,70") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,71") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,72") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,73") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,74") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,75") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,76") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,77") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,78") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,79") == 0) ||
            (strcmp(gcSIM800FTPERROR, "+FTPEXTGET: 1,86") == 0)
        )
        {
            gucFTPExtGetReady  = 1;
            gucFTPUpgradeState = (gcSIM800FTPERROR[14] - '0') * 10 + (gcSIM800FTPERROR[15] - '0');
        }


        /*接收到的命令是SERVER端传递给SIM800，与EFM32之间的数据命令解析*/
        if(ucData == SIM800_RX_SOI)
        {
            gucServerFlag = 1;
            gucServerSize = 0;

            for(i = 0; i < 200; i++) gucServerData[i] = 0;

            gucServerData[gucServerSize++] = ucData;
        }
        else if(gucServerFlag == 1)
        {
            gucServerData[gucServerSize++] = ucData;

            if(ucData == SIM800_RX_EOI)
            {
                /*服务器端的数据命令解析*/
                SIM800_ServerCommandParsing(gucServerData);
#if 1
                for(i = 0; i < gucServerSize; i++)
                {
                    LEUART_Tx(LEUART0, gucServerData[i]); WDOG_Feed();
                }
#endif
                gucServerFlag = 0;
                gucServerSize = 0;

                for(i = 0; i < 200; i++) gucServerData[i] = 0;
            }
        }
        else if(gucServerSize >= 200)
        {
            gucServerFlag = 0;
            gucServerSize = 0;

            for(i = 0; i < 200; i++) gucServerData[i] = 0;
        }
    }
}


void SIM800_SendPacket(uint8_t *pData, uint16_t length)
{
    uint16_t i = 0;

    /*判断SIM800的发送消息队列是否为空*/
    if(QUEUE_EMPTY(BT_QUEUE_TX_IDX) == 0)
    {
        return;
    }

    /*将即将要发送的数据或命令复制到SIM800的发送消息队列中*/
    for(i = 0; i < length; i++)
    {
        QUEUE_WRITE(SIM800_QUEUE_TX_IDX, pData[i]);
    }

    /*启动发送*/
    USART_IntEnable(USART0, UART_IF_TXBL);
}


void SIM800_TX_Process(uint8_t index, uint8_t next)
{
    uint8_t i = 0, buffer[200];

    switch(index)
    {
        case AT_IDX_AT      : for(i = 0; i < sizeof(AT_CMD_AT           ) -1; i++) buffer[i] = AT_CMD_AT        [i]; break;
        case AT_IDX_GSN     : for(i = 0; i < sizeof(AT_CMD_GSN          ) -1; i++) buffer[i] = AT_CMD_GSN       [i]; break;
        case AT_IDX_CCID    : for(i = 0; i < sizeof(AT_CMD_CCID         ) -1; i++) buffer[i] = AT_CMD_CCID      [i]; break;
        case AT_IDX_CIPSHUT : for(i = 0; i < sizeof(AT_CMD_CIPSHUT      ) -1; i++) buffer[i] = AT_CMD_CIPSHUT   [i]; break;
        case AT_IDX_CIPCLOSE: for(i = 0; i < sizeof(AT_CMD_CIPCLOSE     ) -1; i++) buffer[i] = AT_CMD_CIPCLOSE  [i]; break;
        case AT_IDX_CSQ     : for(i = 0; i < sizeof(AT_CMD_CSQ          ) -1; i++) buffer[i] = AT_CMD_CSQ       [i]; break;
        case AT_IDX_CSQ_COPS: for(i = 0; i < sizeof(AT_CMD_CSQ_COPS     ) -1; i++) buffer[i] = AT_CMD_CSQ_COPS  [i]; break;
        case AT_IDX_CREG    : for(i = 0; i < sizeof(AT_CMD_CREG         ) -1; i++) buffer[i] = AT_CMD_CREG      [i]; break;
        case AT_IDX_CGATT   : for(i = 0; i < sizeof(AT_CMD_CGATT        ) -1; i++) buffer[i] = AT_CMD_CGATT     [i]; break;
        case AT_IDX_CSTT    :
            if(PLATFORM_CHINA != 0)
            {
                if(strcmp(gcCorrespondent, AT_RET_COPS_10010) == 0)
                {
                    for(i = 0; i < sizeof(AT_CMD_CSTT_10010) -1; i++) buffer[i] = AT_CMD_CSTT_10010[i];
                }
                else
                {
                    for(i = 0; i < sizeof(AT_CMD_CSTT_10086) -1; i++) buffer[i] = AT_CMD_CSTT_10086[i];
                }
            }

            if(PLATFORM_DUBAI == 1)
            {
                for(i = 0; i < sizeof(AT_CMD_CSTT_DU) -1; i++) buffer[i] = AT_CMD_CSTT_DU[i];
            }

            if(PLATFROM_RUSSIA == 1)
            {
                for(i = 0; i < sizeof(AT_CMD_CSTT_RUSSIA) -1; i++) buffer[i] = AT_CMD_CSTT_RUSSIA[i];
            }
            break;
        case AT_IDX_CIICR   : for(i = 0; i < sizeof(AT_CMD_CIICR        ) -1; i++) buffer[i] = AT_CMD_CIICR     [i]; break;
        case AT_IDX_CIFSR   : for(i = 0; i < sizeof(AT_CMD_CIFSR        ) -1; i++) buffer[i] = AT_CMD_CIFSR     [i]; break;
        case AT_IDX_CIPSTART: for(i = 0; i < sizeof(AT_CMD_CIPSTART     ) -1; i++) buffer[i] = AT_CMD_CIPSTART  [i]; break;
        case AT_IDX_CIPSEND : for(i = 0; i < sizeof(AT_CMD_CIPSEND      ) -1; i++) buffer[i] = AT_CMD_CIPSEND   [i]; break;

        case AT_IDX_SAPBR_CONTYPE   : for(i = 0; i < sizeof(AT_CMD_SAPBR_CONTYPE    ) - 1; i++) buffer[i] = AT_CMD_SAPBR_CONTYPE    [i]; break;
        case AT_IDX_SAPBR_APN       :
            if(PLATFORM_CHINA != 0)
            {
                if(strcmp(gcCorrespondent, AT_RET_COPS_10010) == 0)
                {
                    for(i = 0; i < sizeof(AT_CMD_SAPBR_APN_10010) - 1; i++) buffer[i] = AT_CMD_SAPBR_APN_10010[i];
                }
                else
                {
                    for(i = 0; i < sizeof(AT_CMD_SAPBR_APN_10086) - 1; i++) buffer[i] = AT_CMD_SAPBR_APN_10086[i];
                }
            }


            if(PLATFORM_DUBAI == 1)
            {
                for(i = 0; i < sizeof(AT_CMD_SAPBR_APN_DU) - 1; i++) buffer[i] = AT_CMD_SAPBR_APN_DU[i];
            }

            if(PLATFROM_RUSSIA == 1)
            {
                for(i = 0; i < sizeof(AT_CMD_SAPBR_APN_RUSSIA) - 1; i++) buffer[i] = AT_CMD_SAPBR_APN_RUSSIA[i];
            }
            break;
        case AT_IDX_SAPBR_START     : for(i = 0; i < sizeof(AT_CMD_SAPBR_START      ) - 1; i++) buffer[i] = AT_CMD_SAPBR_START      [i]; break;
        case AT_IDX_SAPBR_QUERY     : for(i = 0; i < sizeof(AT_CMD_SAPBR_QUERY      ) - 1; i++) buffer[i] = AT_CMD_SAPBR_QUERY      [i]; break;
        case AT_IDX_SAPBR_CLOSE     : for(i = 0; i < sizeof(AT_CMD_SAPBR_CLOSE      ) - 1; i++) buffer[i] = AT_CMD_SAPBR_CLOSE      [i]; break;
        case AT_IDX_FTPMODE         : for(i = 0; i < sizeof(AT_CMD_FTPMODE          ) - 1; i++) buffer[i] = AT_CMD_FTPMODE          [i]; break;
        case AT_IDX_FTPTYPE         : for(i = 0; i < sizeof(AT_CMD_FTPTYPE          ) - 1; i++) buffer[i] = AT_CMD_FTPTYPE          [i]; break;
        case AT_IDX_FTPCID          : for(i = 0; i < sizeof(AT_CMD_FTPCID           ) - 1; i++) buffer[i] = AT_CMD_FTPCID           [i]; break;
        case AT_IDX_FTPSERV         : for(i = 0; i < sizeof(AT_CMD_FTPSERV          ) - 1; i++) buffer[i] = AT_CMD_FTPSERV          [i]; break;
        case AT_IDX_FTPPORT         : for(i = 0; i < sizeof(AT_CMD_FTPPORT          ) - 1; i++) buffer[i] = AT_CMD_FTPPORT          [i]; break;
        case AT_IDX_FTPUN           : for(i = 0; i < sizeof(AT_CMD_FTPUN            ) - 1; i++) buffer[i] = AT_CMD_FTPUN            [i]; break;
        case AT_IDX_FTPPW           : for(i = 0; i < sizeof(AT_CMD_FTPPW            ) - 1; i++) buffer[i] = AT_CMD_FTPPW            [i]; break;
        case AT_IDX_FTPGETNAME      : for(i = 0; i < sizeof(AT_CMD_FTPGETNAME       ) - 1; i++) buffer[i] = AT_CMD_FTPGETNAME       [i]; break;
        case AT_IDX_FTPGETPATH      : for(i = 0; i < sizeof(AT_CMD_FTPGETPATH       ) - 1; i++) buffer[i] = AT_CMD_FTPGETPATH       [i]; break;
        case AT_IDX_FTPEXTGET_START : for(i = 0; i < sizeof(AT_CMD_FTPEXTGET_START  ) - 1; i++) buffer[i] = AT_CMD_FTPEXTGET_START  [i]; break;
        case AT_IDX_FTPEXTGET_SAVE  : for(i = 0; i < sizeof(AT_CMD_FTPEXTGET_SAVE   ) - 1; i++) buffer[i] = AT_CMD_FTPEXTGET_SAVE   [i]; break;
        case AT_IDX_FTPEXTGET_OUTPUT: for(i = 0; i < sizeof(AT_CMD_FTPEXTGET_OUTPUT ) - 1; i++) buffer[i] = AT_CMD_FTPEXTGET_OUTPUT [i]; break;

        default: break;
    }

    buffer[i++] = AT_CMD_END_CR;

    SIM800_SendPacket(buffer, i);

    gucSIM800TxStep    = next;
    gucSIM800ReplyFlag = 0;
}


void SIM800_TX_Handler(void)
{
    uint8_t i = 0;//, buffer[200];

    /*在SIM800模块发送数据之前先检测一下工作状态*/
    if(SIM800_GetModuleSTATUS() == 0) return;

    /*SIM800回复超时处理:累加计时单位为250毫秒*/
    if(gucSIM800ReplyFlag == 0)
    {
        if(gucSIM800ReplyTime++ == 60)  /*60.15s  80.20s 100.25s*/
        {
            SIM800_PowerDown(); InitSIM800(); return;
        }
    }
    else
    {
        gucSIM800ReplyTime = 0;

        /*
        **在CLIENT端发送的数据需要SERVER回复时，进行超时处理；
        **先进行5秒的重发处理，如果重复6次后仍没有回复，重启；
        */
        if(gucServerReplyFlag == 0)
        {
            if(gucServerReplyTime++ == 20) /*20.5s 40.10s*/
            {
                gucServerReplyTime = 0;
                gucSIM800TxStep   -= 3;

                if(gucServerReplyCount++ == 5)
                {
                    SIM800_PowerDown(); InitSIM800(); return;
                }
            }
        }
        else
        {
            gucServerReplyTime  = 0;
            gucServerReplyCount = 0;
        }
    }

    switch(gucSIM800TxStep)
    {
        case 0:  break;  /*空操作，不作任何处理*/


/********************************************************************************/
/*1~22:SIM800模块启动************************************************************/
/********************************************************************************/
        case 1:  SIM800_TX_Process(AT_IDX_AT,               0); break;
        case 2:  if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 3:  SIM800_TX_Process(AT_IDX_GSN,              4); break;
        case 4:  if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 5:  SIM800_TX_Process(AT_IDX_CCID,             6); break;
        case 6:  if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 7:  SIM800_TX_Process(AT_IDX_CIPSHUT,          8); break;
        case 8:  if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 9:  SIM800_TX_Process(AT_IDX_CSQ_COPS,        10); break;
        case 10: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 11: SIM800_TX_Process(AT_IDX_CREG,            12); break;
        case 12: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 13: SIM800_TX_Process(AT_IDX_CGATT,           14); break;
        case 14: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 15: SIM800_TX_Process(AT_IDX_CSTT,            16); break;
        case 16: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 17: SIM800_TX_Process(AT_IDX_CIICR,           18); break;
        case 18: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 19: SIM800_TX_Process(AT_IDX_CIFSR,           20); break;
        case 20: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 21: SIM800_TX_Process(AT_IDX_CIPSTART,        22); break;
        case 22: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep=55;break;


        case 50: break; /*空操作，不作任何处理*/


/********************************************************************************/
/*51~54:网络板上传服务器上线指令*************************************************/
/********************************************************************************/
        case 51:
            if(gucSIM800ConnectFlag == 1)
            {
                gucSIM800SendEnableFlag = 0;
                SIM800_TX_Process(AT_IDX_CIPSEND, 52);
            }
            break;

        case 52:
            if(gucSIM800SendEnableFlag == 1)
            {
                gucSIM800TxStep++;
                gucServerReplyFlag      = 0;    /*01上线指令需要SERVER的回复*/
                gucSIM800SendEnableFlag = 0;
            }
            break;

        case 53:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = '%';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            gucSIM800TxData[gucSIM800TxSize++] = '0';
            gucSIM800TxData[gucSIM800TxSize++] = '1';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            for(i = 0; i < 20; i++)
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
            }
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            for(i = 0; i < 15; i++)
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucSIM800GSN[i];
            }
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            /*投币箱版本号*/
            gucSIM800TxData[gucSIM800TxSize++] = '0';
            gucSIM800TxData[gucSIM800TxSize++] = '2';
            gucSIM800TxData[gucSIM800TxSize++] = '0';
            gucSIM800TxData[gucSIM800TxSize++] = '0';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            /*检测是否存在投币箱*/
            if(gucMoneyExistFlag == 0)
            {
                gucSIM800TxData[gucSIM800TxSize++] = '0';
            }
            else
            {
                gucSIM800TxData[gucSIM800TxSize++] = '1';
            }
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            /*网络板软件版本号*/
            gucSIM800TxData[gucSIM800TxSize++] = FW_VERSION[0];
            gucSIM800TxData[gucSIM800TxSize++] = FW_VERSION[1];
            gucSIM800TxData[gucSIM800TxSize++] = FW_VERSION[2];
            gucSIM800TxData[gucSIM800TxSize++] = FW_VERSION[3];
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            gucSIM800TxData[gucSIM800TxSize++] = '-';

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_SUB;

            //for(i = 0; i < gucSIM800TxSize; i++) buffer[i] = gucSIM800TxData[i];

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize);  gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;

        case 54: 
            if((gucSIM800ReplyFlag == 1) && (gucServerReplyFlag == 1))
            {
                gucSIM800TxStep = 50;
            }
            break;

/********************************************************************************/
/*55~58:网络板向服务器寻求新服务器地址*******************************************/
/********************************************************************************/
        case 55:
            if(gucSIM800ConnectFlag == 1)
            {
                gucSIM800SendEnableFlag = 0;
                SIM800_TX_Process(AT_IDX_CIPSEND, 56);
            }
            break;

        case 56:
            if(gucSIM800SendEnableFlag == 1)
            {
                gucSIM800TxStep++;
                gucServerReplyFlag      = 0;    /*03询问新连接地址指令需要SERVER的回复*/
                gucSIM800SendEnableFlag = 0;
            }
            break;

        case 57:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = '%';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            gucSIM800TxData[gucSIM800TxSize++] = '3';
            gucSIM800TxData[gucSIM800TxSize++] = '1';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            for(i = 0; i < 20; i++)
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
            }
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            gucSIM800TxData[gucSIM800TxSize++] = '-';

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_SUB;

            //for(i = 0; i < gucSIM800TxSize; i++) buffer[i] = gucSIM800TxData[i];

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize);  gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;

        case 58:
            if((gucSIM800ReplyFlag == 1) && (gucServerReplyFlag == 1))
            {
                gucSIM800TxStep = 50;
            }
            break;

/********************************************************************************/
/*59~60:网络重启*****************************************************************/
/********************************************************************************/
        case 59: SIM800_TX_Process(AT_IDX_CIPSHUT,         60); break;
        case 60: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;


/********************************************************************************/
/*61~63:重新连接新的网络链接*****************************************************/
/********************************************************************************/
        case 61:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '+';
            gucSIM800TxData[gucSIM800TxSize++] = 'C';
            gucSIM800TxData[gucSIM800TxSize++] = 'I';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'S';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'R';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '=';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'C';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            i =  0;

            while(gcSIM800NewLink[i] != '\0')
            {
                gucSIM800TxData[gucSIM800TxSize++] = gcSIM800NewLink[i++];
            }
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            i = 0;

            while(gcSIM800NewPort[i] != '\0')
            {
                gucSIM800TxData[gucSIM800TxSize++] = gcSIM800NewPort[i++];
            }

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_CR;

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize); gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;

        case 62: if(gucSIM800ReplyFlag   == 1) gucSIM800TxStep++;    break;
        case 63: if(gucSIM800ConnectFlag == 1) gucSIM800TxStep = 51; break;


        case 100: break; /*空操作，不作任何处理*/


/********************************************************************************/
/*101~104:回复SERVER端的命令请求*************************************************/
/********************************************************************************/
        case 101:
            if(gucSIM800ConnectFlag == 1)
            {
                gucSIM800SendEnableFlag = 0;
                SIM800_TX_Process(AT_IDX_CIPSEND, 102);
            }
            break;

        case 102:
            if(gucSIM800SendEnableFlag == 1)
            {
                gucSIM800TxStep++;
               // gucServerReplyFlag      = 1;    /*这些指令是回复SERVER的*/
                gucSIM800SendEnableFlag = 0;
            }
            break;

        case 103:
            gucSIM800TxSize = 0;
            gucSIM800TxData[gucSIM800TxSize++] = '%';

            gucSIM800TxData[gucSIM800TxSize++] = ',';

            gucSIM800TxData[gucSIM800TxSize++] = gcGSMBusiness[0];
            gucSIM800TxData[gucSIM800TxSize++] = gcGSMBusiness[1];

            gucSIM800TxData[gucSIM800TxSize++] = ',';

            gucSIM800TxData[gucSIM800TxSize++] = gcGSMResponse[0];
            gucSIM800TxData[gucSIM800TxSize++] = gcGSMResponse[1];

            gucSIM800TxData[gucSIM800TxSize++] = ',';

            if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_HEARTBEAT) == 0)     /*GSM心跳*/
            {
                /*如果存在控制板，则返回相应的状态，否则返回090，按摩椅出错*/
                if(gucChairExistFlag == 1)
                {
                    /*高字节:微信控制有效期标志*/
                    if(gucServerCtrlFlag == 0)
                    {
                        gucSIM800TxData[gucSIM800TxSize++] = '0';
                    }
                    else
                    {
                        gucSIM800TxData[gucSIM800TxSize++] = '1';
                    }

                    /*中字节:按摩椅运行状态*/
                    switch(gucChairRunStatus)
                    {
                        case CHAIR_STATE_IDLE:
                            gucSIM800TxData[gucSIM800TxSize++] = '0';
                            break;

                        case CHAIR_STATE_RUN:
                            gucSIM800TxData[gucSIM800TxSize++] = '1';
                            break;

                        case CHAIR_STATE_SETTLE:
                            gucSIM800TxData[gucSIM800TxSize++] = '2';
                            break;

                        default:
                            gucSIM800TxData[gucSIM800TxSize++] = '0';
                            break;
                    }

                    /*低字节:按摩手法*/
                    gucSIM800TxData[gucSIM800TxSize++] = '0' + gucMassageManipulation;
                }
                else
                {
                    gucSIM800TxData[gucSIM800TxSize++] = '0';
                    gucSIM800TxData[gucSIM800TxSize++] = '9';
                    gucSIM800TxData[gucSIM800TxSize++] = '0';
                }

                gucSIM800TxData[gucSIM800TxSize++] = ',';

                /*如果存在投币板，则返回相应的状态，否则返回00*/
                if(gucMoneyExistFlag == 1)
                {
                    /*高字节:未用*/
                    gucSIM800TxData[gucSIM800TxSize++] = '0';

                    /*低字节:当前投币状态*/
                    if(MONEY_GetWorkState() == 0)
                    {
                        gucSIM800TxData[gucSIM800TxSize++] = '0';   /*未投币*/
                    }
                    else
                    {
                        gucSIM800TxData[gucSIM800TxSize++] = '1';   /*已投币*/
                    }
                }
                else
                {
                    gucSIM800TxData[gucSIM800TxSize++] = '0';
                    gucSIM800TxData[gucSIM800TxSize++] = '0';
                }

                gucSIM800TxData[gucSIM800TxSize++] = ',';

                /*当前CSQ信号强度*/
                gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CSQ[0];
                gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CSQ[1];

                gucSIM800TxData[gucSIM800TxSize++] = ',';
                
                /*当前充电状态*/
                gucSIM800TxData[gucSIM800TxSize++] = '0' + gucChairChargeStatus;

                gucSIM800TxData[gucSIM800TxSize++] = ',';                

                gucServerReplyFlag = 0;
            }
            else if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_REMOTE_ORDER_TIME) == 0)        /*GSM微信订单时间上传*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_NATIVE_ORDER_TIME) == 0)        /*GSM投币订单时间上传*/
            {
                for(i = 0; i < 12; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyStartWorkTime[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                for(i = 0; i < 12; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyEndOfWorkTime[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_NATIVE_ORDER_MONEY) == 0)       /*GSM微信投币当前一笔钱上传*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_ABNORMAL_ORDER) == 0)           /*上传异常订单信息*/
            {
                /*按摩椅编号*/
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                /*返回订单号*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = *((uint8_t *)(M13NetWork_DATA_START + i));
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                /*按摩椅已运行时长*/
                for(i = 0; i <  4; i++) gucSIM800TxData[gucSIM800TxSize++] = *((uint8_t *)(M13NetWork_DATA_START + 14 + i));
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_START_RUNNING) == 0)                   /*微信远程启动*/
            {
                /*返回订单号*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_CLOSE_RUNNING) == 0)                   /*微信远程关闭*/
            {
                /*返回订单号*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_EXTEND_WORKING_TIME) == 0)             /*微信远程续费*/
            {
                /*返回订单号*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_CHAIR_SET_PARAMETER) == 0)             /*微信APP控制命令*/
            {
                /*返回订单号*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_CHAIR_QUERY_STATUS) == 0)              /*按摩椅状态查询*/
            {
                gucSIM800TxData[gucSIM800TxSize++] = '0';
                gucSIM800TxData[gucSIM800TxSize++] = '0';
                gucSIM800TxData[gucSIM800TxSize++] = '0';
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                gucSIM800TxData[gucSIM800TxSize++] = '0';
                gucSIM800TxData[gucSIM800TxSize++] = '0';
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_UPGRADING_PROGRAM) == 0)               /*微信远程升级命令*/
            {
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                gucSIM800TxData[gucSIM800TxSize++] = '0';
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_ALL) == 0)              /*微信APP查询钱币总额、总识币数*/
            {
                for(i = 0; i < 5; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyAmount[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                for(i = 0; i < 6; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyWorkTime[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                for(i = 0; i < 5; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ALL) == 0)              /*微信APP清除钱币总额、总识币数*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_RATE) == 0)             /*微信APP查询费率*/
            {
                for(i = 0; i < 2; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyRateBase[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                for(i = 0; i < 2; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyRateTime[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_RATE) == 0)               /*微信APP设置费率*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_TIME) == 0)             /*微信APP查询投币箱时间*/
            {
                for(i = 0; i < 12; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyDateTime[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_TIME) == 0)               /*微信APP设置投币箱时间*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_UNLOCKING) == 0)              /*微信APP发送开锁命令*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_STATUS) == 0) /*微信查询投币箱门状态*/
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucMoneyUnlockingState;
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_TIMES) == 0)  /*微信查询投币箱累计开锁次数*/
            {
                for(i = 0; i < 5; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyLegalUnlockingTimes[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                for(i = 0; i < 5; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyIllegaUnlockinglTimes[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_LEGAL_UNLOCKING_TIMES) == 0)    /*微信清除投币箱累计合法开锁次数*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ILLEGALUNLOCKING_TIMES) == 0)   /*微信清除投币箱累计不合法开锁次数*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COMMUNICATION_TEST) == 0)   /*test*/
            {
                 /*返回按摩椅编号*/
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';  
                gucServerReplyFlag      = 1;  
                gucServerReplyFlag      = 1; 
                
            } 
            else if(strcmp(gcGSMBusiness, SERVER_CMD_START_CHARGE) == 0)   /*远程开启充电功能*/
            {
                 /*返回按摩椅编号*/
                //for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                //gucSIM800TxData[gucSIM800TxSize++] = ',';
                 /*返回订单号*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';           
                gucServerReplyFlag      = 1; 
            }    
            else if(strcmp(gcGSMBusiness, SERVER_CMD_CLOSE_CHARGE) == 0)   /*远程关闭充电功能*/
            {
                 /*返回按摩椅编号*/
                //for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                //gucSIM800TxData[gucSIM800TxSize++] = ',';
                 /*返回订单号*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';      
                gucServerReplyFlag      = 1; 
            }  
            else if(strcmp(gcGSMBusiness, SERVER_CMD_SET_LONG_CHARGE) == 0)   /*设置一直充电*/
            {
                 /*返回按摩椅编号*/
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';    
                gucServerReplyFlag      = 1; 
            } 
            else if(strcmp(gcGSMBusiness, SERVER_CMD_QUERY_LONG_CHARGE) == 0)   /*查询一直充电*/
            {
                 /*返回按摩椅编号*/
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';  
                gucSIM800TxData[gucSIM800TxSize++] = gucChairLongChargeFlag + '0';
                gucSIM800TxData[gucSIM800TxSize++] = ','; 
                gucServerReplyFlag      = 1; 
            } 
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COMMUNICATION_TEST) == 0)   /*test*/
            {
                 /*返回按摩椅编号*/
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';    
                gucServerReplyFlag      = 1; 
            }
            else 
            {
                gucServerReplyFlag      = 1; 
            }

            gucSIM800TxData[gucSIM800TxSize++] = '-';

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_SUB;

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize); gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;

            gcGSMBusiness[0] = '?';
            gcGSMBusiness[1] = '?';
            gcGSMBusiness[2] = '\0';

            gcGSMResponse[0] = '5';
            gcGSMResponse[1] = '5';
            gcGSMResponse[2] = '\0';
            break;

        case 104:
            if((gucSIM800ReplyFlag == 1) && (gucServerReplyFlag == 1))
            {
                gucSIM800TxStep = 100;
            }
            break;


/********************************************************************************/
/*110~111:更新信号强度***********************************************************/
/********************************************************************************/
        case 110: SIM800_TX_Process(AT_IDX_CSQ,              111); break;
        case 111: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep=100; break;



        case 200: break;


/********************************************************************************/
/*201~234:远程程序升级***********************************************************/
/********************************************************************************/
        case 201: SIM800_TX_Process(AT_IDX_SAPBR_CONTYPE,  202); break;
        case 202: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 203: SIM800_TX_Process(AT_IDX_SAPBR_APN,      204); break;
        case 204: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 205: SIM800_TX_Process(AT_IDX_SAPBR_START,    206); break;
        case 206: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 207: SIM800_TX_Process(AT_IDX_SAPBR_QUERY,    208); break;
        case 208: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 209: SIM800_TX_Process(AT_IDX_FTPMODE,        210); break;
        case 210: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 211: SIM800_TX_Process(AT_IDX_FTPTYPE,        212); break;
        case 212: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 213: SIM800_TX_Process(AT_IDX_FTPCID,         214); break;
        case 214: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 215:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '+';
            gucSIM800TxData[gucSIM800TxSize++] = 'F';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'S';
            gucSIM800TxData[gucSIM800TxSize++] = 'E';
            gucSIM800TxData[gucSIM800TxSize++] = 'R';
            gucSIM800TxData[gucSIM800TxSize++] = 'V';
            gucSIM800TxData[gucSIM800TxSize++] = '=';
            gucSIM800TxData[gucSIM800TxSize++] = '\"';

            for(i = 0; i < gucFTPServerLength; i++)
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPServer[i];
            }

            gucSIM800TxData[gucSIM800TxSize++] = '\"';

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_CR;

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize); gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;
        case 216: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 217:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '+';
            gucSIM800TxData[gucSIM800TxSize++] = 'F';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'O';
            gucSIM800TxData[gucSIM800TxSize++] = 'R';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '=';

            if((gucFTPPort[0] == '0') && (gucFTPPort[1] == '0') && (gucFTPPort[2] == '0'))
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPort[3];
            }
            else if((gucFTPPort[0] == '0') && (gucFTPPort[1] == '0') )
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPort[2];
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPort[3];
            }
            else if(gucFTPPort[0] == '0')
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPort[1];
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPort[2];
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPort[3];
            }
            else
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPort[0];
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPort[1];
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPort[2];
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPort[3];
            }

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_CR;

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize); gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;
        case 218: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 219:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '+';
            gucSIM800TxData[gucSIM800TxSize++] = 'F';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'U';
            gucSIM800TxData[gucSIM800TxSize++] = 'N';
            gucSIM800TxData[gucSIM800TxSize++] = '=';
            gucSIM800TxData[gucSIM800TxSize++] = '\"';

            for(i = 0; i < 6; i++)
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPUserName[i];
            }

            gucSIM800TxData[gucSIM800TxSize++] = '\"';

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_CR;

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize); gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;
        case 220: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 221:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '+';
            gucSIM800TxData[gucSIM800TxSize++] = 'F';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'W';
            gucSIM800TxData[gucSIM800TxSize++] = '=';
            gucSIM800TxData[gucSIM800TxSize++] = '\"';

            /*AES128解密*/
            AES_Decode(gucFTPPassword, gucFTPPassword, gucFTPPasswordLength);

            for(i = 0; i < 16; i++)
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPPassword[i];
            }

            gucSIM800TxData[gucSIM800TxSize++] = '\"';

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_CR;

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize); gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;
        case 222: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 223:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '+';
            gucSIM800TxData[gucSIM800TxSize++] = 'F';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'G';
            gucSIM800TxData[gucSIM800TxSize++] = 'E';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'N';
            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'M';
            gucSIM800TxData[gucSIM800TxSize++] = 'E';
            gucSIM800TxData[gucSIM800TxSize++] = '=';
            gucSIM800TxData[gucSIM800TxSize++] = '\"';

            for(i = 0; i < 20; i++)
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPGetName[i];
            }

            gucSIM800TxData[gucSIM800TxSize++] = '\"';

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_CR;

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize); gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;
        case 224: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 225:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '+';
            gucSIM800TxData[gucSIM800TxSize++] = 'F';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'G';
            gucSIM800TxData[gucSIM800TxSize++] = 'E';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'H';
            gucSIM800TxData[gucSIM800TxSize++] = '=';
            gucSIM800TxData[gucSIM800TxSize++] = '\"';

            for(i = 0; i < gucFTPGetPathLength; i++)
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucFTPGetPath[i];
            }

            gucSIM800TxData[gucSIM800TxSize++] = '\"';

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_CR;

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize); gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;
        case 226: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 227: SIM800_TX_Process(AT_IDX_FTPEXTGET_START,  228); break;
        case 228: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep=100; break;

        case 229: SIM800_TX_Process(AT_IDX_FTPEXTGET_SAVE, 230); break;
        case 230: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;

        case 231:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '+';
            gucSIM800TxData[gucSIM800TxSize++] = 'F';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'P';
            gucSIM800TxData[gucSIM800TxSize++] = 'E';
            gucSIM800TxData[gucSIM800TxSize++] = 'X';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = 'G';
            gucSIM800TxData[gucSIM800TxSize++] = 'E';
            gucSIM800TxData[gucSIM800TxSize++] = 'T';
            gucSIM800TxData[gucSIM800TxSize++] = '=';
            gucSIM800TxData[gucSIM800TxSize++] = '3';
            gucSIM800TxData[gucSIM800TxSize++] = ',';


            if(gulFTPExtGetIndex >= 10000)
            {
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 10000) % 10;
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 1000 ) % 10;
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 100  ) % 10;
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 10   ) % 10;
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 1    ) % 10;
            }
            else if(gulFTPExtGetIndex >= 1000)
            {
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 1000 ) % 10;
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 100  ) % 10;
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 10   ) % 10;
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 1    ) % 10;
            }
            else if(gulFTPExtGetIndex >= 100)
            {
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 100  ) % 10;
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 10   ) % 10;
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 1    ) % 10;
            }
            else if(gulFTPExtGetIndex >= 10)
            {
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 10   ) % 10;
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 1    ) % 10;
            }
            else
            {
                gucSIM800TxData[gucSIM800TxSize++] = '0' + (gulFTPExtGetIndex / 1    ) % 10;
            }


            gucSIM800TxData[gucSIM800TxSize++] = ',';
            gucSIM800TxData[gucSIM800TxSize++] = '2';
            gucSIM800TxData[gucSIM800TxSize++] = '5';
            gucSIM800TxData[gucSIM800TxSize++] = '6';
            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_CR;


            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize); gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;

        /*232:判断FTP的数据是否读取结束，如果没有结束则继续读取，读取结束等待复位升级*/
        case 232:
            if(gucSIM800ReplyFlag == 1)
            { 
                if(gulFTPExtGetIndex < gulFTPExtGetLength)
                {
                    gucSIM800TxStep = 231;
                }
                else
                {
                    while(1)
                    {
                        /*等待看门狗复位*/
                    }
                }
            }
            break;


        case 233: SIM800_TX_Process(AT_IDX_SAPBR_CLOSE,      234); break;
        case 234: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep=205; break;


/********************************************************************************/
/*235~238:发送FTP的升级状态，1正在升级，9升级异常********************************/
/********************************************************************************/
        case 235:
            if(gucSIM800ConnectFlag == 1)
            {
                gucSIM800SendEnableFlag = 0;
                SIM800_TX_Process(AT_IDX_CIPSEND, 236);
            }
            break;

        case 236:
            if(gucSIM800SendEnableFlag == 1)
            {
                gucSIM800TxStep++;
                gucServerReplyFlag      = 0;    /*30主动上报升级状态需要SERVER的回复*/
                gucSIM800SendEnableFlag = 0;
            }
            break;

        case 237:
            gucSIM800TxSize = 0;

            gucSIM800TxData[gucSIM800TxSize++] = '%';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            gucSIM800TxData[gucSIM800TxSize++] = '3';
            gucSIM800TxData[gucSIM800TxSize++] = '0';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = 'A';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            if(gucFTPUpgradeState == 1)
            {
                gucSIM800TxData[gucSIM800TxSize++] = '1';
            }
            else
            {
                gucSIM800TxData[gucSIM800TxSize++] = '9';
            }
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            gucSIM800TxData[gucSIM800TxSize++] = '-';

            gucSIM800TxData[gucSIM800TxSize++] = AT_CMD_END_SUB;

            SIM800_SendPacket(gucSIM800TxData, gucSIM800TxSize); gucSIM800ReplyFlag = 0;

            gucSIM800TxStep++;
            break;

        case 238:
            if((gucSIM800ReplyFlag == 1) && (gucServerReplyFlag == 1))
            {
                if(gucFTPUpgradeState == 1)
                {
                    MONEY_Demo(8);  /*开始升级，禁止投币*/

                    gucSIM800TxStep = 229;
                }
                else
                {
                    gucHeartbeatCount = 1;
                    gucUpdateCSQCount = 1;
                    gucSIM800TxStep   = 100;
                }
            }
            break;


        default:
          if(gucSIM800ConnectFlag)  gucSIM800TxStep=100;
          else                      gucSIM800TxStep=1;         
            break;
    }
}


void InitSIM800(void)
{
    uint8_t i = 0;

    /*SIM800模块的电源供电使能引脚*/
    GPIO_PinModeSet(GSM_POWER_CTRL_PORT, GSM_POWER_CTRL_PIN, GSM_POWER_CTRL_MODE, 0);

    /*SIM800模块的STATUS、LIGHT、RESET、PWRKEY引脚初始化配置*/
    GPIO_PinModeSet(SIM800_STATUS_PORT,  SIM800_STATUS_PIN,  SIM800_STATUS_MODE,  1);
    GPIO_PinModeSet(SIM800_LIGHT_PORT,   SIM800_LIGHT_PIN,   SIM800_LIGHT_MODE,   1);
    GPIO_PinModeSet(SIM800_RST_PORT,     SIM800_RST_PIN,     SIM800_RST_MODE,     0);
    GPIO_PinModeSet(SIM800_PWR_PORT,     SIM800_PWR_PIN,     SIM800_PWR_MODE,     0);

    delay_ms(200);

    /*SIM800模块上电初始化操作*/
    SIM800_PowerUp();


    /*SIM800发给SERVER端的数据缓冲区*/
    for(i = 0; i < 100; i++) gucSIM800TxData[i] = 0;

    gucSIM800TxSize         = 0;    /*SIM800发给SERVER端的数据包长度*/
    gucSIM800TxStep         = 0;    /*SIM800发送步骤*/

    gucSIM800ReplyFlag      = 1;    /*SIM800回复标志*/
    gucSIM800ReplyTime      = 0;    /*SIM800回复超时*/

    gucServerReplyFlag      = 1;    /*SERVER回复标志*/
    gucServerReplyTime      = 0;    /*SERVER回复超时*/
    gucServerReplyCount     = 0;    /*SERVER回复超时计数*/

    gucSIM800ConnectFlag    = 0;    /*SIM800连网标志*/
    gucSIM800OnlineState    = 0;    /*SIM800上线标志*/
    gucSIM800StartupTime    = 0;

    gucSIM800SendEnableFlag = 0;    /*SIM800发送数据使能标志*/
    


    /*长度固定为20，CCID编号，也作为按摩椅编号*/
    for(i = 0; i < 20; i++) gucSIM800CID[i] = 0;

    /*长度固定为02，CSQ信号强度*/
    for(i = 0; i < 2;  i++) gucSIM800CSQ[i] = 0;

    /*长度固定为15，GSN编号*/
    for(i = 0; i < 15; i++) gucSIM800GSN[i] = 0;

    gucHeartbeatCount = 0;  /*心跳计时计数器，每250ms增加1*/

    gucSIM800TxStep = 1;
}


void GSM_HeartbeatHandler(uint8_t mode)
{
    /*在TCP没有连接成功 或者 ONLINE指令没有成功 的时候不上传心跳*/
    if((gucSIM800ConnectFlag == 0) || (gucSIM800OnlineState == 0))
    {
        gucHeartbeatCount = 0;
    }


    /*即刻上传心跳*/
    if(mode == 1) gucHeartbeatCount = 240;


    /*每250ms调用一次，经过240累加后为60s*/
    if(gucHeartbeatCount++ == 240)
    {
        gucHeartbeatCount = 240;

        /*判断当前SIM800的发送步骤是否处于空闲步骤，如果是则切换到发送心跳的步骤*/
        if(GSM_IsTxStepIDLE() == 1)
        {
            /*在没有其它命令发送的情况下，再发送心跳命令，心跳的优先级最低*/
            if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_HEARTBEAT) == 0)
            {
                gucSIM800TxStep  = 101;

                gcGSMResponse[0] = 'A';
                gcGSMResponse[1] = 'A';
                gcGSMResponse[2] = '\0';

                gucHeartbeatCount = 0;
            }
        }
    }
}


void GSM_UpdateCSQHandler(uint8_t mode)
{
    /*在TCP没有连接成功 或者 ONLINE指令没有成功 的时候不上传心跳*/
    if((gucSIM800ConnectFlag == 0) || (gucSIM800OnlineState == 0))
    {
        gucUpdateCSQCount = 0;
    }


    /*即刻上传心跳*/
    if(mode == 1) gucUpdateCSQCount = 60;


    /*每250ms调用一次，经过240累加后为60s*/
    if(gucUpdateCSQCount++ == 60)
    {
        gucUpdateCSQCount = 60;

        /*判断当前SIM800的发送步骤是否处于空闲步骤，如果是则切换到发送心跳的步骤*/
        if(GSM_IsTxStepIDLE() == 1)
        {
            /*在没有其它命令发送的情况下，再发送心跳命令，心跳的优先级最低*/
            if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_HEARTBEAT) == 0)
            {
                gucSIM800TxStep   = 110;
                gucUpdateCSQCount = 0;
            }
        }
    }
}


void GSM_UploadOrderHandler(uint8_t mode)
{
    static uint8_t ucOrderType = 0;

    if(mode != 0) ucOrderType = mode;

    if(GSM_IsTxStepIDLE() == 1)
    {
        if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_HEARTBEAT) == 0)
        {
            switch(ucOrderType)
            {
                case 17:
                    gucSIM800TxStep  = 101;

                    gcGSMBusiness[0] = '1';
                    gcGSMBusiness[1] = '7';
                    gcGSMBusiness[2] = '\0';

                    gcGSMResponse[0] = 'A';
                    gcGSMResponse[1] = 'A';
                    gcGSMResponse[2] = '\0';

                    ucOrderType      = 0;

                    gucMoneyWorkStep = 3;
                    break;

                case 18:
                    gucSIM800TxStep  = 101;

                    gcGSMBusiness[0] = '1';
                    gcGSMBusiness[1] = '8';
                    gcGSMBusiness[2] = '\0';

                    gcGSMResponse[0] = 'A';
                    gcGSMResponse[1] = 'A';
                    gcGSMResponse[2] = '\0';

                    ucOrderType      = 0;
                    break;

                case 19:
                    gucSIM800TxStep  = 101;

                    gcGSMBusiness[0] = '1';
                    gcGSMBusiness[1] = '9';
                    gcGSMBusiness[2] = '\0';

                    gcGSMResponse[0] = 'A';
                    gcGSMResponse[1] = 'A';
                    gcGSMResponse[2] = '\0';

                    ucOrderType      = 0;
                    break;

                case 37:
                    gucSIM800TxStep  = 101;

                    gcGSMBusiness[0] = '3';
                    gcGSMBusiness[1] = '7';
                    gcGSMBusiness[2] = '\0';

                    gcGSMResponse[0] = 'A';
                    gcGSMResponse[1] = 'A';
                    gcGSMResponse[2] = '\0';

                    ucOrderType      = 0;
                    break;

                default:
                    break;
            }
        }
    }
}


void GSM_ReplyServerHandler(uint8_t flag)
{
  if(GSM_IsTxStepIDLE() == 1)
  {
    if(flag)
    {
        gcGSMResponse[0] = 'A';
        gcGSMResponse[1] = 'A';
        gcGSMResponse[2] = '\0';
    }
    else
    {
        gcGSMResponse[0] = '5';
        gcGSMResponse[1] = '5';
        gcGSMResponse[2] = '\0';
    }

    gucSIM800TxStep = 101;
  }
}


uint8_t GSM_IsTxStepIDLE(void)
{
    if(gucSIM800TxStep == 100) return 1;
    else                       return 0;
}


void GSM_Demo(uint8_t index)
{
    uint8_t i       = 0;
    uint8_t length  = 0;
    uint8_t buffer[200];

    uint8_t ucFTPSERV[15]    = "139.196.138.73";
    uint8_t ucFTPPORT[5]     = "0021";
    uint8_t ucFTPUN[7]       = "sxftpc";
    uint8_t ucFTPPW[45]      = "tQVwW9UbBnrr3r+d7banQTnvGRjCJRxXN+MrWrYE70I=";
    uint8_t ucFTPGETNAME[21] = "M13NET_SX2G_0201.BIN";
    uint8_t ucFTPGETPATH[8]  = "update/";

    switch(index)
    {
        case 0:     /*空操作*/
            break;

        case 1:     /*微信远程启动，05*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '5';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            for(i = 0; i < 14; i++) buffer[length++] = 0x30 + (i % 10);
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '0';
            buffer[length++] = '0';
            buffer[length++] = '5';
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '2';
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '2';
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 2:     /*微信远程关闭，09*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '9';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            for(i = 0; i < 14; i++) buffer[length++] = 0x30 + (i % 10);
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 3:     /*微信远程续费，08*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '8';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            for(i = 0; i < 14; i++) buffer[length++] = 0x30 + (i % 10);
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '0';
            buffer[length++] = '0';
            buffer[length++] = '5';
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 4:     /*微信APP控制命令，06*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '6';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            for(i = 0; i < 14; i++) buffer[length++] = 0x30 + (i % 10);
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '2';
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '2';
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '0';
            buffer[length++] = '0';
            buffer[length++] = '5';
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 5:     /*按摩椅状态查询，10*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '1';
            buffer[length++] = '0';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 6:     /*微信远程升级命令，30*/
            break;

        case 7:     /*微信APP查询钱币总额、总识币数，11*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '1';
            buffer[length++] = '1';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 8:     /*微信APP清除钱币总额、总识币数，12*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '1';
            buffer[length++] = '2';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 9:     /*微信APP查询费率，13*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '1';
            buffer[length++] = '3';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 10:    /*微信APP设置费率，14*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '1';
            buffer[length++] = '4';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '0'; 
            buffer[length++] = '5';
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '2';
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 11:    /*微信APP查询投币箱时间，15*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '1';
            buffer[length++] = '5';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 12:    /*微信APP设置投币箱时间，16*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '1';
            buffer[length++] = '6';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            for(i = 0; i < 12; i++) buffer[length++] = gucUniversalTime[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 13:    /*微信APP发送开锁命令，32*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '3';
            buffer[length++] = '2';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 14:    /*微信查询投币箱门状态，33*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '3';
            buffer[length++] = '3';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 15:    /*微信查询投币箱累计开锁次数，34*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '3';
            buffer[length++] = '4';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 16:    /*微信清除投币箱累计合法开锁次数，35*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '3';
            buffer[length++] = '5';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 17:    /*微信清除投币箱累计不合法开锁次数，36*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '3';
            buffer[length++] = '6';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        case 18:    /*远程升级*/
            buffer[length++] = '*';
            buffer[length++] = ',';

            buffer[length++] = '3';
            buffer[length++] = '0';
            buffer[length++] = ',';

            buffer[length++] = 'A';
            buffer[length++] = 'A';
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = gucSIM800CID[i];
            buffer[length++] = ',';

            buffer[length++] = '1';
            buffer[length++] = '4';
            buffer[length++] = ',';

            for(i = 0; i < 14; i++) buffer[length++] = ucFTPSERV[i];
            buffer[length++] = ',';

            for(i = 0; i <  4; i++) buffer[length++] = ucFTPPORT[i];
            buffer[length++] = ',';

            for(i = 0; i <  6; i++) buffer[length++] = ucFTPUN[i];
            buffer[length++] = ',';

            buffer[length++] = '4';
            buffer[length++] = '4';
            buffer[length++] = ',';

            for(i = 0; i < 44; i++) buffer[length++] = ucFTPPW[i];
            buffer[length++] = ',';

            for(i = 0; i < 20; i++) buffer[length++] = ucFTPGETNAME[i];
            buffer[length++] = ',';

            buffer[length++] = '0';
            buffer[length++] = '7';
            buffer[length++] = ',';

            for(i = 0; i <  7; i++) buffer[length++] = ucFTPGETPATH[i];
            buffer[length++] = ',';

            buffer[length++] = '!';

            for(i = 0; i < length; i++)
            {
                QUEUE_WRITE(SIM800_QUEUE_RX_IDX, buffer[i]);
            }
            break;

        default:
            break;
    }
}


void GSM_StartupTimeoutHandler(void)
{
    /*如果在150秒内，SIM800没有启动成功，连接到SERVER端，则认为启动失败*/
    if((gucSIM800ConnectFlag == 0) || (gucSIM800OnlineState == 0))
    {
        if(gucSIM800StartupTime++ == 150)
        {
            gucSIM800StartupTime = 0;

            SIM800_PowerDown(); InitSIM800();
        }
    }
    else
    {
        gucSIM800StartupTime = 0;
    }
}


void GSM_UpgradeProgramHandler(void)
{
    if(gucFTPUpgradeFlag == 1)
    {
        if(GSM_IsTxStepIDLE() == 1)
        {
            gucFTPUpgradeFlag  = 0;
            gucFTPUpgradeState = 0;
            gucSIM800TxStep    = 201;
        }
    }

    if(gucFTPExtGetReady == 1)
    {
        if(GSM_IsTxStepIDLE() == 1)
        {
            if(MCB_GetChairRunningFlag() == 0)   /*按摩椅不处于运行的状态*/
            {
                gucFTPExtGetReady = 0;
                gucSIM800TxStep   = 235;
            }
        }
    }
}






