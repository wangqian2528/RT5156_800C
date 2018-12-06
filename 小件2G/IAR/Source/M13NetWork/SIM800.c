#define __SIM800_C__


#include "AES.h"
#include "flash.h"
#include "MCB.h"
#include "MCU.h"
#include "money.h"
#include "SIM800.h"


/*ͨѶ����*/
extern QUEUE_Init_TypeDef COM_QUEUE[QUEUE_NUMBER];

/*��������*/
extern uint8_t  gucFlashPageData[FLASH_PAGE_SIZE];

/*Ͷ�Ҳ��ֵ���չ����*/
extern uint8_t gucMoneyExistFlag;                   /*�Ƿ����Ͷ�Ұ��־��Ĭ���Ǵ���*/
extern uint8_t gucMoneyAmount[5];                   /*Ǯ���ܽ��*/
extern uint8_t gucMoneyNumber[5];                   /*Ǯ��������*/
extern uint8_t gucMoneyWorkTime[6];                 /*�ܹ���ʱ��*/
extern uint8_t gucMoneyRateBase[2];                 /*���ʻ������*/
extern uint8_t gucMoneyRateTime[2];                 /*���ʹ���ʱ��*/
extern uint8_t gucMoneyDateTime[12];                /*Ͷ�����ʱ��*/
extern uint8_t gucMoneyUnlockingState;              /*Ͷ��������״̬:��0����1*/
extern uint8_t gucMoneyLegalUnlockingTimes[5];      /*�Ϸ���������*/
extern uint8_t gucMoneyIllegaUnlockinglTimes[5];    /*�Ƿ���������*/
extern uint8_t gucMoneyStartWorkTime[12];           /*Ͷ���俪ʼ����ʱ��*/
extern uint8_t gucMoneyEndOfWorkTime[12];           /*Ͷ���乤������ʱ��*/
extern uint8_t gucMoneyWorkStep;                    /*Ͷ����Ĺ�������*/

/*���Ʋ��ֵ���չ����*/
extern uint8_t gucChairRunStatus;                   /*��Ħ�ε�����״̬*/
extern uint8_t gucChairExistFlag;                   /*�Ƿ���ڿ��ư��־��Ĭ���Ǵ���*/

//add by sxk
extern uint8_t gucChairChargeStatus;                /*��Ħ�γ��״̬*/
extern uint8_t gucChairLongChargeFlag;             /*��Ħ��һֱ����־λ��Ĭ�Ϲر�*/
extern uint16_t gucChargeTimeCount;         /*���ʱ�������*/
const  uint8_t gucFTPEXTGETCompare[19] =
{
    '+','F','T','P','E','X','T','G','E','T',':',' ','3',',','2','5','6', 0x0D, 0x0A
};

/*SIM800Ӧ�ñ���*/
uint8_t gucSIM800TxData[100];           /*SIM800����SERVER�˵����ݻ�����*/
uint8_t gucSIM800TxSize         = 0;    /*SIM800����SERVER�˵����ݰ�����*/
uint8_t gucSIM800TxStep         = 0;    /*SIM800���Ͳ���*/

uint8_t gucSIM800ReplyFlag      = 1;    /*SIM800�ظ���־*/
uint8_t gucSIM800ReplyTime      = 0;    /*SIM800�ظ���ʱ*/

uint8_t gucServerReplyFlag      = 1;    /*SERVER�ظ���־*/
uint8_t gucServerReplyTime      = 0;    /*SERVER�ظ���ʱ*/
uint8_t gucServerReplyCount     = 0;    /*SERVER�ظ���ʱ����*/

uint8_t gucSIM800ConnectFlag    = 0;    /*SIM800������־*/
uint8_t gucSIM800OnlineState    = 0;    /*SIM800���߱�־*/
uint8_t gucSIM800StartupTime    = 0;    /*SIM800������ʱ*/

uint8_t gucSIM800SendEnableFlag = 0;    /*SIM800��������ʹ�ܱ�־*/

uint8_t gucSIM800CID[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   /*���ȹ̶�Ϊ20��CCID��ţ�Ҳ��Ϊ��Ħ�α��*/
uint8_t gucSIM800CSQ[2]  = {0,0};                                       /*���ȹ̶�Ϊ02��CSQ�ź�ǿ��*/
uint8_t gucSIM800GSN[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};             /*���ȹ̶�Ϊ15��GSN���*/


/*ϵͳӦ�ñ���*/
char gcSIM800NewLink[31] = "------------------------------";/*�������Ϊ30��SIM800�µ�ͨѶ����*/
char gcSIM800NewPort[6]  = "-----";                         /*�̶�����Ϊ05��SIM800�µ�ͨѶ�˿�*/

uint8_t gucUniversalTime[12];       /*�̶�����Ϊ12��ͨ��ʱ��*/
uint8_t gucOrderNumber[14];         /*�������*/
uint8_t gucExperienceTime      = 0; /*����ʱ������λΪ����*/
uint8_t gucMassageManipulation = 0; /*��Ħ�ַ�*/
uint8_t gucAirbagDynamics      = 0; /*��������*/
uint8_t gucServerCtrlFlag      = 0; /*Զ�̿�����Ч�ڱ�־*/

uint8_t gucChargeTime          = 0; /*���ʱ������λΪ����*/  // add by sxk
uint8_t guc_Charging_Flag = 0;

char     gcGSMBusiness[3]    = "??";    /*ͨѶ�Ľ�������*/
char     gcGSMResponse[3]    = "55";    /*ͨѶ����Ӧ��*/
char     gcCorrespondent[20] = "-------------------";   /*ͨѶ��*/

uint8_t  gucHeartbeatCount  = 0;    /*������ʱ��������ÿ250ms����1*/
uint8_t  gucUpdateCSQCount  = 0;    /*�����źż�������ÿ1������1*/

uint8_t  gucFTPUpgradeFlag  = 0;    /*�Ƿ�Ҫ����FTP������־*/
uint8_t  gucFTPUpgradeState = 0;    /*FTP��ǰ������״̬��־*/
uint8_t  gucFTPExtGetReady  = 0;    /*FTP��ȡ�ļ��Ѿ�׼����*/
uint32_t gulFTPExtGetIndex  = 0;    /*FTP��ǰ��ȡ���ݵ��±�*/
uint32_t gulFTPExtGetLength = 0;    /*FTP��ȡ�����ݵ��ܳ���*/

uint8_t  gucFTPServer[50];          /*FTP����*/
uint8_t  gucFTPServerLength   = 0;  /*FTP�������ݳ���*/
uint8_t  gucFTPPort[4];             /*FTP�˿ں�*/
uint8_t  gucFTPUserName[6];         /*FTP�û���*/
uint8_t  gucFTPPassword[50];        /*FTP��������*/
uint8_t  gucFTPPasswordLength = 0;  /*FTP�����������ݳ���*/
uint8_t  gucFTPGetName[20];         /*FTP�ļ���*/
uint8_t  gucFTPGetPath[50];         /*FTP�ļ�·��*/
uint8_t  gucFTPGetPathLength  = 0;  /*FTP�ļ�·�����ݳ���*/




uint8_t SIM800_GetModuleSTATUS(void)
{
    /*
    ��SIM800ģ�鴦����������״̬��STATU״ָ̬ʾ�����Ż�����ߵ�ƽ��
    �û�����ͨ�������ŵĵ�ƽ���ж�ģ���Ƿ��ڿ�������״̬��
    */
    if(GPIO_PinInGet(SIM800_STATUS_PORT, SIM800_STATUS_PIN) == 0)
    {
        return 1;   /*�鿴��Ӧ�ĵ�·�������ܺ�״̬������*/
    }
    else
    {
        return 0;
    }
}


/*
SIM800ģ���ڻظ�CCID��ʱ����ĸ�����Ǵ�д��������Сд��
�����ڱȽϵ�ʱ����Դ�Сд�ıȽϣ�
��CLIENT�ظ�SERVERʱ���ظ���Сд�����ԣ�SERVER�����д���
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
    /*MIC29302 EN�������ţ���SIM800ģ���ṩ��Դ*/
    GPIO_PinOutSet(GSM_POWER_CTRL_PORT, GSM_POWER_CTRL_PIN);

    /*����PWRKEY���ų���1��Ȼ���ͷţ���ʹģ�鿪�����ػ�*/
    GPIO_PinOutClear(SIM800_PWR_PORT, SIM800_PWR_PIN);

    delay_ms(2000);

    GPIO_PinOutSet(SIM800_PWR_PORT, SIM800_PWR_PIN);
}


void SIM800_PowerDown(void)
{
    /*MIC29302 EN�������ţ��Ͽ�SIM800ģ���Դ*/
    GPIO_PinOutClear(GSM_POWER_CTRL_PORT, GSM_POWER_CTRL_PIN);

    delay_ms(500);
}


void SIM800_ItselfCommandParsing(uint8_t type, uint8_t *buffer)
{
    char CommandLine[10][150];

    uint8_t  i = 0,  j = 0,  k = 0;

    //uint16_t index = 0, length = 0;


    /*��buffer�е�������ȡ��һ��һ�е�ָ��*/
    while(buffer[i] != '\0')
    {
        /*ȥ���س����з�*/
        while((buffer[i] == '\r') || (buffer[i] == '\n'))
        {
            i++;

            if(j)
            {
                /*�ý�����־λ�����л�������*/
                CommandLine[k++][j++] = '\0';   j = 0;
            }
        }

        /*��ȡ��Ч�����ݴ洢��CommandLine��*/
        CommandLine[k][j++] = buffer[i++];
    }

    /*�ý�����־λ*/
    CommandLine[k++][j] = '\0';


    switch(type)
    {
        case 0: /*��AT��ͷ��OK��β���������*/
            if((strcmp(CommandLine[0], AT_CMD_AT)               == 0) && (strcmp(CommandLine[1], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_GSN)         == 0) && (strcmp(CommandLine[2], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;

                /*��ȡGSN����*/
                for(i = 0; i < 15; i++) gucSIM800GSN[i] = CommandLine[1][i];
            }
            else if((strcmp(CommandLine[0], AT_CMD_CCID)        == 0) && (strcmp(CommandLine[2], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;

                /*��ȡCCID����*/
                for(i = 0; i < 20; i++) gucSIM800CID[i] = CommandLine[1][i];
            }
            else if((strcmp(CommandLine[0], AT_CMD_CIPSHUT)     == 0) && (strcmp(CommandLine[1], "SHUT OK") == 0))
            {
                gucSIM800ConnectFlag = 0;   /*���ӱ�־����*/
                gucSIM800OnlineState = 0;
                gucSIM800ReplyFlag   = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CIPCLOSE)    == 0) && (strcmp(CommandLine[1], "CLOSE OK") == 0))
            {
                gucSIM800ConnectFlag = 0;   /*���ӱ�־����*/
                gucSIM800OnlineState = 0;
                gucSIM800ReplyFlag   = 1;
            }
            else if((strcmp(CommandLine[0], AT_CMD_CSQ_COPS)    == 0) && (strcmp(CommandLine[3], "OK") == 0))
            {
                gucSIM800ReplyFlag = 1;

                /*��ȡCSQ�ź�ǿ��*/
                gucSIM800CSQ[0] = CommandLine[1][6];
                gucSIM800CSQ[1] = CommandLine[1][7];

                /*���ź�ǿ��Ϊ��λ��ʱ���жϴ���*/
                if(gucSIM800CSQ[1] == ',')
                {
                    gucSIM800CSQ[1] = gucSIM800CSQ[0];
                    gucSIM800CSQ[0] = '0';
                }

                /*����ź�ǿ��Ϊ0�����߻�ȡʧ�ܣ������»�ȡCSQ����*/
                if((gucSIM800CSQ[0] == '0') && (gucSIM800CSQ[1] == '0'))
                {
                    gucSIM800TxStep-=2; break;
                }

                if(PLATFORM_CHINA != 0) /*�й�ע���SIM���Զ�ʶ����ͨ�����ƶ���*/
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

                /*��ȡCSQ�ź�ǿ��*/
                gucSIM800CSQ[0] = CommandLine[1][6];
                gucSIM800CSQ[1] = CommandLine[1][7];

                /*���ź�ǿ��Ϊ��λ��ʱ���жϴ���*/
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
                **ֻҪ��AT��ͷ��OK��β�Ķ��ظ�����Ӧ:����������CIPSTARTʱ�������е���ߣ�
                **��Ϊ��ʱ��TCP�����ӺͶ˿ںŶ��ǲ�ѯ�����������ģ��ڳ������޷����壻
                */
                if(gucSIM800TxStep != 232)
                {
                    gucSIM800ReplyFlag = 1;
                }
            }
            break;


        case 1: /*û����AT��ͷ������OK��β���������*/
            if(strcmp(CommandLine[0], "CONNECT OK") == 0)
            {
                gucSIM800ReplyFlag   = 1;

                /*��TCP���ӳɹ���ʱ��SIM800���Զ��ϴ�CONNECT OK*/
                gucSIM800ConnectFlag = 1;
            }
            break;


        case 2: /*��AT��ͷ��ERROR��β���������*/
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


        case 3: /*û����AT��ͷ������ERROR��β���������*/
            SIM800_PowerDown(); InitSIM800();
            break;


        case 4: /*��CLOSED��β���������*/
            if(strcmp(CommandLine[0], "CLOSED") == 0)
            {
                if((gucSIM800TxStep == 231) || (gucSIM800TxStep == 232))
                {
                    return; /*������ʱ�򲻴�������쳣*/
                }

                /*TCP�����Զ��Ͽ���ʱ��SIM800���Զ��ϴ�CLOSED*/
                gucSIM800ConnectFlag = 0;
                gucSIM800OnlineState = 0;
                gucSIM800ReplyFlag   = 1;

                /*��������*/
                gucSIM800TxStep      = 7;
            }
            else if(strcmp(CommandLine[0], "STATE: TCP CLOSED") == 0)
            {
                /*��TCP����ʧ�ܵ�ʱ��SIM800���Զ��ϴ�STATE: TCP CLOSED*/
            }
            break;


        case 5: /*��FAIL��β���������*/
            /*��TCP����ʧ�ܵ�ʱ��SIM800���Զ��ϴ�CONNECT FAIL*/
            SIM800_PowerDown(); InitSIM800();
            break;

        case 6: /*��Ready��β���������*/
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

    if(strcmp(gcGSMBusiness, CLIENT_CMD_ONLINE) == 0)               /*���յ�����巢�͵�CLIENT_CMD_ONLINE�Ļظ�*/
    {
        /*(*,01,TT,170414145610,!)*/

        if(strcmp(gcGSMResponse, "TT") == 0)
        {
            for(i = 0; i < 12; i++) gucUniversalTime[i] = buffer[i+8];

            /*���ߺ�ͬ��SERVER����Ͷ�����ʱ��*/
            MONEY_CommandEnqueue(MONEY_CMD_SET_DATE_TIME, gucUniversalTime, 12);

            /*���SERVER��CLIENT�Ļظ�������Ҫ����ظ�����*/
            gcGSMBusiness[0] = '?';
            gcGSMBusiness[1] = '?';
            gcGSMBusiness[2] = '\0';

            if(gucSIM800TxStep < 200)
            {
                /*SERVER�˻ظ�ONLINEָ��󣬼����뵽���͵ȴ�״̬*/
                gucSIM800TxStep = 100;
            }

            if(gucSIM800OnlineState == 0)
            {
                GSM_HeartbeatHandler(1);
            }

            /*ONLINE״̬��λ*/
            gucSIM800OnlineState = 1;

            /*SERVER�ظ���ONLINE��ָ��*/
            gucServerReplyFlag = 1;
        }
        else
        {
            
            GSM_ReplyServerHandler(0);           
        }
    }
    else if(strcmp(gcGSMBusiness, CLIENT_CMD_ASKFOR_NEWLINK) == 0)  /*���յ�����巢�͵�CLIENT_CMD_ASKFOR_NEWLINK�Ļظ�*/
    {
        /*(*,31,AA,898602b7091700545108,16,tx2.51shaoxi.com,05488,!)*/

        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*��ȡ��������Ϣ�����ݳ��ȣ������ǲ��̶���*/
                length  = 0;
                length += (buffer[29] - 0x30) * 0x0A;
                length += (buffer[30] - 0x30) * 0x01;

                /*��ȡ�µ�������Ϣ*/
                for(i = 0; i < length; i++)
                {
                    gcSIM800NewLink[i] = buffer[i+32];
                }
                gcSIM800NewLink[i] = '\0';


                /*��ȡ�µĶ˿ںţ����ȹ̶�Ϊ5���ֽ�*/
                for(i = 0; i < 5; i++)
                {
                    gcSIM800NewPort[i] = buffer[i+33+length];
                }
                gcSIM800NewPort[i] = '\0';


                /*���ݸ�ʽ����:���˿ں���ǰ�����õ�0ȥ��*/
                while(gcSIM800NewPort[0] == '0')
                {
                    for(i = 0; i < 4; i++)
                    {
                        gcSIM800NewPort[i] = gcSIM800NewPort[i+1];
                    }

                    gcSIM800NewPort[i] = '\0';
                }

                /*���SERVER��CLIENT�Ļظ�������Ҫ����ظ�����*/
                gcGSMBusiness[0] = '?';
                gcGSMBusiness[1] = '?';
                gcGSMBusiness[2] = '\0';

                /*��SERVER�ظ��µ����ӺͶ˿ں󣬽�����������*/
                gucSIM800TxStep    = 59;
            }

            gucServerReplyFlag = 1;
        }
        else
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*��ѯ����������ʧ��*/
            }
        }
    }
    else if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_HEARTBEAT) == 0)    /*���յ�����巢�͵�CLIENT_CMD_UPLOAD_HEARTBEAT�Ļظ�*/
    {
        /*���SERVER��CLIENT�Ļظ�������Ҫ����ظ�����*/
        gcGSMBusiness[0] = '?';
        gcGSMBusiness[1] = '?';
        gcGSMBusiness[2] = '\0';

        gucServerReplyFlag = 1;
    }
    else if(strcmp(gcGSMBusiness, SERVER_CMD_START_RUNNING) == 0)       /*΢��Զ������*/
    {
        /*(*,05,AA,12345678901234567890,12345678901234,1234,12,12,!)*/
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*��¼�������*/
                for(i = 0; i < 14; i++)
                {
                    gucOrderNumber[i] = buffer[i+29];
                }

                if(MONEY_GetWorkState() == 1)
                {
                    /*�ڰ�Ħ�δ���Ͷ�����е�״̬�£�����յ�SERVER�˵�����������ֱ�ӻظ�ʧ��*/
                    
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*��ȡ����ʱ��*/
                    gucExperienceTime       = 0;
                    gucExperienceTime      += (buffer[44] - '0') * 1000;
                    gucExperienceTime      += (buffer[45] - '0') * 100;
                    gucExperienceTime      += (buffer[46] - '0') * 10;
                    gucExperienceTime      += (buffer[47] - '0') * 1;
                    
                    gucChargeTimeCount = 0;//add by sxk
                    gucChargeTimeCount = (gucExperienceTime  * 60) + 20 ;//add by sxk                    

                    if(buffer[49] != '!')   /*������ƽ̨��ͨѶЭ�飬�ϵ�ͨѶЭ��û�а�Ħ�ַ����������ȵĲ�������*/
                    {
                        /*��ȡ��Ħ�ַ�*/
                        gucMassageManipulation  = 0;
                        gucMassageManipulation += (buffer[49] - '0') * 10;
                        gucMassageManipulation += (buffer[50] - '0') * 1;

                        /*��ȡ��������*/
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

                    /*���ݲ�ͬ�İ�Ħ�ַ�������Ӧ���ַ�ѡ��*/
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

                    /*���ݲ�ͬ���������Ƚ�����Ӧ������ѡ��*/
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

                    /*��λԶ�̿�����Ч�ڱ�־λ*/
                    gucServerCtrlFlag = 1;

                    /*��ֹͶ����Ͷ�ҹ���*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_CLOSE_RUNNING) == 0)       /*΢��Զ�̹ر�*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*�Աȶ������*/
                for(i = 0; i < 14; i++)
                {
                    if(buffer[i+29] != gucOrderNumber[i]) break;
                }

                if(i == 14)
                {
                    if(MONEY_GetWorkState() == 1)
                    {
                        /*�ڰ�Ħ�δ���Ͷ�����е�״̬�£�����յ�SERVER�˵Ĺر�������ֱ�ӻظ�ʧ��*/                        
                        GSM_ReplyServerHandler(0);
                    }
                    else
                    {


                        /*�ÿ��ư�ֹͣ����*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_EXTEND_WORKING_TIME) == 0) /*΢��Զ������*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*�Աȶ������*/
                for(i = 0; i < 14; i++)
                {
                    if(buffer[i+29] != gucOrderNumber[i]) break;
                }

                if(i == 14)
                {
                    if(MONEY_GetWorkState() == 1)
                    {
                        /*�ڰ�Ħ�δ���Ͷ�����е�״̬�£�����յ�SERVER�˵�����������ֱ�ӻظ�ʧ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_CHAIR_SET_PARAMETER) == 0) /*΢��APP��������*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*�Աȶ������*/
                for(i = 0; i < 14; i++)
                {
                    if(buffer[i+29] != gucOrderNumber[i]) break;
                }
                
                //i = 14; for debug

                if(i == 14)
                {
                    if(MONEY_GetWorkState() == 1)
                    {
                        /*�ڰ�Ħ�δ���Ͷ�����е�״̬�£�����յ�SERVER�˵Ŀ���������ֱ�ӻظ�ʧ��*/
                        GSM_ReplyServerHandler(0);
                    }
                    else
                    {
                        // to do...2017.11.12

                        /*��ȡ��Ħ�ַ�*/
                        gucMassageManipulation  = 0;
                        gucMassageManipulation += (buffer[44] - '0') * 10;
                        gucMassageManipulation += (buffer[45] - '0') * 1;

                        /*��ȡ��������*/
                        gucAirbagDynamics       = 0;
                        gucAirbagDynamics      += (buffer[47] - '0') * 10;
                        gucAirbagDynamics      += (buffer[48] - '0') * 1;

                        /*���ݲ�ͬ�İ�Ħ�ַ�������Ӧ���ַ�ѡ��*/
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

                        /*���ݲ�ͬ���������Ƚ�����Ӧ������ѡ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_CHAIR_QUERY_STATUS) == 0)  /*��Ħ��״̬��ѯ*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_UPGRADING_PROGRAM) == 0)   /*΢��Զ����������*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_ALL) == 0)  /*΢��APP��ѯǮ���ܶ��ʶ����*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ALL) == 0)  /*΢��APP���Ǯ���ܶ��ʶ����*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if((MONEY_GetWorkState() == 1) || (gucMoneyExistFlag == 0))
                {
                    /*�ڰ�Ħ�δ���Ͷ�����е�״̬�£�����յ�SERVER�˵����������ֱ�ӻظ�ʧ��*/
                    
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_RATE) == 0) /*΢��APP��ѯ����*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_RATE) == 0)   /*΢��APP���÷���*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                if((MONEY_GetWorkState() == 1) || (gucMoneyExistFlag == 0))
                {
                    /*�ڰ�Ħ�δ���Ͷ�����е�״̬�£�����յ�SERVER�˵����÷���������ֱ�ӻظ�ʧ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_TIME) == 0) /*΢��APP��ѯͶ����ʱ��*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_TIME) == 0)   /*΢��APP����Ͷ����ʱ��*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*(*,16,AA,11112222333344445555,112233445566,!)*/
                if((MONEY_GetWorkState() == 1) || (gucMoneyExistFlag == 0))
                {
                    /*�ڰ�Ħ�δ���Ͷ�����е�״̬�£�����յ�SERVER�˵�����ʱ��������ֱ�ӻظ�ʧ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_UNLOCKING) == 0)  /*΢��APP���Ϳ�������*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_STATUS) == 0) /*΢�Ų�ѯͶ������״̬*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_TIMES ) == 0) /*΢�Ų�ѯͶ�����ۼƿ�������*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_LEGAL_UNLOCKING_TIMES ) == 0)   /*΢�����Ͷ�����ۼƺϷ���������*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ILLEGALUNLOCKING_TIMES) == 0)   /*΢�����Ͷ�����ۼƲ��Ϸ���������*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    else if(strcmp(gcGSMBusiness, SERVER_CMD_START_CHARGE) == 0)       /*΢��Զ����������繦��*/
    {
        /*(*,05,AA,12345678901234567890,12345678901234,1234,12,12,!)*/
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*��¼�������*/
                for(i = 0; i < 14; i++)
                {
                    gucOrderNumber[i] = buffer[i+29];
                }

                if(MONEY_GetWorkState() == 1)
                {
                    /*�ڰ�Ħ�δ���Ͷ�����е�״̬�£�����յ�SERVER�˵�����������ֱ�ӻظ�ʧ��*/
                    GSM_ReplyServerHandler(0);
                }
                else
                {
                    /*��ȡ����ʱ��*/
                    gucExperienceTime       = 0;
                    gucExperienceTime      += (buffer[44] - '0') * 1000;
                    gucExperienceTime      += (buffer[45] - '0') * 100;
                    gucExperienceTime      += (buffer[46] - '0') * 10;
                    gucExperienceTime      += (buffer[47] - '0') * 1;
                    /*��ȡ���ʱ��*/
                    gucChargeTime           = 0;
                    gucChargeTime          += (buffer[55] - '0') * 1000;
                    gucChargeTime          += (buffer[56] - '0') * 100;
                    gucChargeTime          += (buffer[57] - '0') * 10;
                    gucChargeTime          += (buffer[58] - '0') * 1;
                    gucChargeTimeCount = 0;//add by sxk
                    gucChargeTimeCount = (gucChargeTime * 60) + 20 ;//add by sxk

                    if(buffer[49] != '!')   /*������ƽ̨��ͨѶЭ�飬�ϵ�ͨѶЭ��û�а�Ħ�ַ����������ȵĲ�������*/
                    {
                        /*��ȡ��Ħ�ַ�*/
                        gucMassageManipulation  = 0;
                        gucMassageManipulation += (buffer[49] - '0') * 10;
                        gucMassageManipulation += (buffer[50] - '0') * 1;

                        /*��ȡ��������*/
                        gucAirbagDynamics       = 0;
                        gucAirbagDynamics      += (buffer[52] - '0') * 10;
                        gucAirbagDynamics      += (buffer[53] - '0') * 1;
                    }
                    else
                    {
                        gucMassageManipulation  = 1;
                        gucAirbagDynamics       = 3;
                    }
                    
                           /*ʹ�ܳ��*/
                    if(gucChargeTime != 0)
                    MCB_CommandEnqueue(H10_CHARGE_WORK_ENABLE, gucChargeTime);
                    guc_Charging_Flag = 1;
                    if(gucExperienceTime != 0)
                    {
 
                    /*ʹ�ܹ������ư�*/
                        MCB_CommandEnqueue(H10_KEY_WORK_ENABLE, gucExperienceTime);
                                            /*��λԶ�̿�����Ч�ڱ�־λ*/
                        gucServerCtrlFlag = 1;

                    /*��ֹͶ����Ͷ�ҹ���*/
                        MONEY_Demo(0x08);
                    }

                    /*���ݲ�ͬ�İ�Ħ�ַ�������Ӧ���ַ�ѡ��*/
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

                    /*���ݲ�ͬ���������Ƚ�����Ӧ������ѡ��*/
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
    
    else if(strcmp(gcGSMBusiness, SERVER_CMD_CLOSE_CHARGE) == 0)       /*΢��Զ�̹رճ��*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
            if(SIM800_CompareCCID(&buffer[8]) == 1)
            {
                /*�Աȶ������*/
                for(i = 0; i < 14; i++)
                {
                    if(buffer[i+29] != gucOrderNumber[i]) break;
                }

                if(i == 14)
                {
                    if(MONEY_GetWorkState() == 1)
                    {
                        /*�ڰ�Ħ�δ���Ͷ�����е�״̬�£�����յ�SERVER�˵Ĺر�������ֱ�ӻظ�ʧ��*/
                        GSM_ReplyServerHandler(0);
                    }
                    else
                    {

  

                        /*�ÿ��ư�ֹͣ����*/
                        MCB_CommandEnqueue(H10_KEY_WORK_DISABLE, gucExperienceTime);
                        /*ֹͣ���*/
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
 
     else if(strcmp(gcGSMBusiness, SERVER_CMD_SET_LONG_CHARGE) == 0)  /*���ð�Ħ��ͨ���һֱ�ṩ��繦��*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
    
     else if(strcmp(gcGSMBusiness, SERVER_CMD_QUERY_LONG_CHARGE) == 0)  /*��ѯ��Ħ��ͨ���һֱ�ṩ��繦��*/
    {
        if(strcmp(gcGSMResponse, "AA") == 0)
        {
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
            /*�Ƚ�SERVER�˷��͵ı���Ƿ��뵱ǰ���ӵı��һ��*/
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
**SIM800�м��ֽ��յķ�ʽ:
**1.��AT��ͷ��OK��β
**2.��OK��β
**3.��AT��ͷ��ERROR��β
**4.SERVER�˵���Ϣ
**5.��FAIL��β
*/
void SIM800_RX_Handler(void)
{
    uint8_t i = 0, ucData = 0;

    static uint8_t gucServerFlag  = 0;  /*SERVER����������ʼ��־λ*/
    static uint8_t gucServerSize  = 0;  /*SERVER�������ݳ���*/
    static uint8_t gucServerData[200];  /*SERVER�������ݻ�����*/

    static uint8_t gucSIM800Flag = 0;   /*SIM800��ʼAT��־λ*/
    static uint8_t gucSIM800Size = 0;   /*SIM800�������ݳ���*/
    static uint8_t gucSIM800Data[200];  /*SIM800�������ݻ�����*/
    static uint8_t gucSIM800Count= 0;   /*SIM800�����ۼ���*/

    static char    gcSIM800RxSymbol[2]   = "-";                 /*����> �ؼ��ֻ�����*/
    static char    gcSIM800RxOK[3]       = "--";                /*OK    �ؼ��ֻ�����*/
    static char    gcSIM800RxAT[4]       = "---";               /*AT    �ؼ��ֻ�����*/
    static char    gcSIM800RxFAIL[5]     = "----";              /*FAIL  �ؼ��ֻ�����*/
    static char    gcSIM800RxERROR[6]    = "-----";             /*ERROR �ؼ��ֻ�����*/
    static char    gcSIM800RxReady[6]    = "-----";             /*Ready �ؼ��ֻ�����*/
    static char    gcSIM800RxCLOSED[7]   = "------";            /*CLOSED�ؼ��ֻ�����*/
    static char    gcSIM800FTPEXTGET[16] = "---------------";   /*FTPEXTGET FINISHED*/
    static char    gcSIM800FTPERROR[17]  = "----------------";  /*FTPEXTGET ERROR*/

    /*���յ�>�󣬲�������������������*/
    if(
        (gucSIM800TxStep == 53) ||  /*����ָ������*/
        (gucSIM800TxStep == 57) ||  /*���������ӵ�ָ������*/
        (gucSIM800TxStep == 103))   /*�ظ�����*/
    {
        return;
    }

    /*�ж�SIM800�Ľ�����Ϣ�����Ƿ�Ϊ��*/
    if(QUEUE_EMPTY(SIM800_QUEUE_RX_IDX) == 0)
    {
        ucData = QUEUE_READ(SIM800_QUEUE_RX_IDX);

        /*��������ʱ����յ���������������⴦���ӿ촦���ٶ�*/
        if((gucSIM800TxStep == 231) || (gucSIM800TxStep == 232))
        {
            SIM800_RX_ExtractDatHandler(ucData); return;
        }


        /*����ȡ�������ݴ��������Ԥ����������*/
        gucSIM800Data[gucSIM800Size++] = ucData;

        /*���ؼ���:AT*/
        gcSIM800RxAT[0] = gcSIM800RxAT[1];
        gcSIM800RxAT[1] = gcSIM800RxAT[2];
        gcSIM800RxAT[2] = ucData;
        gcSIM800RxAT[3] = '\0';

        if(strcmp(gcSIM800RxAT, "AT+") == 0)
        {
            gucSIM800Flag = 1;
            gucSIM800Size = 3;  /*AT��������ݴ��±�3��ʼ���*/

            gucSIM800Data[0] = 'A';
            gucSIM800Data[1] = 'T';
            gucSIM800Data[2] = '+';

            /*����ؼ��ֱȽϻ�������*/
            strncpy(gcSIM800RxAT, "---", sizeof("---"));
        }
        else if(strcmp(gcSIM800RxAT, "AT\r") == 0)
        {
            gucSIM800Flag = 1;
            gucSIM800Size = 3;  /*AT��������ݴ��±�3��ʼ���*/

            gucSIM800Data[0] = 'A';
            gucSIM800Data[1] = 'T';
            gucSIM800Data[2] = '\r';

            /*����ؼ��ֱȽϻ�������*/
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
                    /*�ڻ�ȡIP��ַʱ����AT��ͷ�����ӳ���3����˵���õ���SIM800�Ļظ�*/
                    if(gucSIM800TxStep == 20) gucSIM800ReplyFlag = 1;
                }
            }
        }
        else
        {
            // to do...nothing
        }

        /*���ؼ���:OK*/
        gcSIM800RxOK[0] = gcSIM800RxOK[1];
        gcSIM800RxOK[1] = ucData;
        gcSIM800RxOK[2] = '\0';

        if(strcmp(gcSIM800RxOK, "OK") == 0)
        {
            /*���ý�����־λ*/
            gucSIM800Data[gucSIM800Size] = '\0';

            if(gucSIM800Flag == 1)
            {
                /*��AT��ͷ��OK��β���������*/
                SIM800_ItselfCommandParsing(0, gucSIM800Data);
            }
            else
            {
                /*û����AT��ͷ������OK��β���������*/
                SIM800_ItselfCommandParsing(1, gucSIM800Data);
            }

            gucSIM800Flag = 0;
            gucSIM800Size = 0;

            /*����ؼ��ֱȽϻ�������*/
            strncpy(gcSIM800RxOK, "--", sizeof("--"));
        }

        /*���ؼ���:ERROR*/
        gcSIM800RxERROR[0] = gcSIM800RxERROR[1];
        gcSIM800RxERROR[1] = gcSIM800RxERROR[2];
        gcSIM800RxERROR[2] = gcSIM800RxERROR[3];
        gcSIM800RxERROR[3] = gcSIM800RxERROR[4];
        gcSIM800RxERROR[4] = ucData;
        gcSIM800RxERROR[5] = '\0';

        if(strcmp(gcSIM800RxERROR, "ERROR") == 0)
        {
            /*���ý�����־λ*/
            gucSIM800Data[gucSIM800Size] = '\0';

            if(gucSIM800Flag == 1)
            {
                /*��AT��ͷ��ERROR��β���������*/
                SIM800_ItselfCommandParsing(2, gucSIM800Data);
            }
            else
            {
                /*û����AT��ͷ������ERROR��β���������*/
                SIM800_ItselfCommandParsing(3, gucSIM800Data);
            }

            gucSIM800Flag = 0;
            gucSIM800Size = 0;

            /*����ؼ��ֱȽϻ�������*/
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
            /*���ý�����־λ*/
            gucSIM800Data[gucSIM800Size] = '\0';

            if(gucSIM800Flag == 1)
            {
                /*��AT��ͷ��ERROR��β���������*/
            }
            else
            {
                /*û����AT��ͷ������Ready��β���������*/
                SIM800_ItselfCommandParsing(6, gucSIM800Data);
            }

            gucSIM800Flag = 0;
            gucSIM800Size = 0;

            /*����ؼ��ֱȽϻ�������*/
            strncpy(gcSIM800RxReady, "-----", sizeof("-----"));
        }


        /*���ؼ���:CLOSED*/
        gcSIM800RxCLOSED[0] = gcSIM800RxCLOSED[1];
        gcSIM800RxCLOSED[1] = gcSIM800RxCLOSED[2];
        gcSIM800RxCLOSED[2] = gcSIM800RxCLOSED[3];
        gcSIM800RxCLOSED[3] = gcSIM800RxCLOSED[4];
        gcSIM800RxCLOSED[4] = gcSIM800RxCLOSED[5];
        gcSIM800RxCLOSED[5] = ucData;
        gcSIM800RxCLOSED[6] = '\0';

        if(strcmp(gcSIM800RxCLOSED, "CLOSED") == 0)
        {
            /*���ý�����־λ*/
            gucSIM800Data[gucSIM800Size] = '\0';

            /*��CLOSED��β���������*/
            SIM800_ItselfCommandParsing(4, gucSIM800Data);

            gucSIM800Flag = 0;
            gucSIM800Size = 0;

            /*����ؼ��ֱȽϻ�������*/
            strncpy(gcSIM800RxCLOSED, "------", sizeof("------"));
        }

        /*���ؼ���:FAIL*/
        gcSIM800RxFAIL[0] = gcSIM800RxFAIL[1];
        gcSIM800RxFAIL[1] = gcSIM800RxFAIL[2];
        gcSIM800RxFAIL[2] = gcSIM800RxFAIL[3];
        gcSIM800RxFAIL[3] = ucData;
        gcSIM800RxFAIL[4] = '\0';

        if(strcmp(gcSIM800RxFAIL, "FAIL") == 0)
        {
            /*���ý�����־λ*/
            gucSIM800Data[gucSIM800Size] = '\0';

            /*��FAIL��β���������*/
            SIM800_ItselfCommandParsing(5, gucSIM800Data);

            gucSIM800Flag = 0;
            gucSIM800Size = 0;

            /*����ؼ��ֱȽϻ�������*/
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


        /*���յ���������SERVER�˴��ݸ�SIM800����EFM32֮��������������*/
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
                /*�������˵������������*/
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

    /*�ж�SIM800�ķ�����Ϣ�����Ƿ�Ϊ��*/
    if(QUEUE_EMPTY(BT_QUEUE_TX_IDX) == 0)
    {
        return;
    }

    /*������Ҫ���͵����ݻ�����Ƶ�SIM800�ķ�����Ϣ������*/
    for(i = 0; i < length; i++)
    {
        QUEUE_WRITE(SIM800_QUEUE_TX_IDX, pData[i]);
    }

    /*��������*/
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

    /*��SIM800ģ�鷢������֮ǰ�ȼ��һ�¹���״̬*/
    if(SIM800_GetModuleSTATUS() == 0) return;

    /*SIM800�ظ���ʱ����:�ۼӼ�ʱ��λΪ250����*/
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
        **��CLIENT�˷��͵�������ҪSERVER�ظ�ʱ�����г�ʱ����
        **�Ƚ���5����ط���������ظ�6�κ���û�лظ���������
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
        case 0:  break;  /*�ղ����������κδ���*/


/********************************************************************************/
/*1~22:SIM800ģ������************************************************************/
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


        case 50: break; /*�ղ����������κδ���*/


/********************************************************************************/
/*51~54:������ϴ�����������ָ��*************************************************/
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
                gucServerReplyFlag      = 0;    /*01����ָ����ҪSERVER�Ļظ�*/
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

            /*Ͷ����汾��*/
            gucSIM800TxData[gucSIM800TxSize++] = '0';
            gucSIM800TxData[gucSIM800TxSize++] = '2';
            gucSIM800TxData[gucSIM800TxSize++] = '0';
            gucSIM800TxData[gucSIM800TxSize++] = '0';
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            /*����Ƿ����Ͷ����*/
            if(gucMoneyExistFlag == 0)
            {
                gucSIM800TxData[gucSIM800TxSize++] = '0';
            }
            else
            {
                gucSIM800TxData[gucSIM800TxSize++] = '1';
            }
            gucSIM800TxData[gucSIM800TxSize++] = ',';

            /*���������汾��*/
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
/*55~58:������������Ѱ���·�������ַ*******************************************/
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
                gucServerReplyFlag      = 0;    /*03ѯ�������ӵ�ַָ����ҪSERVER�Ļظ�*/
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
/*59~60:��������*****************************************************************/
/********************************************************************************/
        case 59: SIM800_TX_Process(AT_IDX_CIPSHUT,         60); break;
        case 60: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep++; break;


/********************************************************************************/
/*61~63:���������µ���������*****************************************************/
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


        case 100: break; /*�ղ����������κδ���*/


/********************************************************************************/
/*101~104:�ظ�SERVER�˵���������*************************************************/
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
               // gucServerReplyFlag      = 1;    /*��Щָ���ǻظ�SERVER��*/
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

            if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_HEARTBEAT) == 0)     /*GSM����*/
            {
                /*������ڿ��ư壬�򷵻���Ӧ��״̬�����򷵻�090����Ħ�γ���*/
                if(gucChairExistFlag == 1)
                {
                    /*���ֽ�:΢�ſ�����Ч�ڱ�־*/
                    if(gucServerCtrlFlag == 0)
                    {
                        gucSIM800TxData[gucSIM800TxSize++] = '0';
                    }
                    else
                    {
                        gucSIM800TxData[gucSIM800TxSize++] = '1';
                    }

                    /*���ֽ�:��Ħ������״̬*/
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

                    /*���ֽ�:��Ħ�ַ�*/
                    gucSIM800TxData[gucSIM800TxSize++] = '0' + gucMassageManipulation;
                }
                else
                {
                    gucSIM800TxData[gucSIM800TxSize++] = '0';
                    gucSIM800TxData[gucSIM800TxSize++] = '9';
                    gucSIM800TxData[gucSIM800TxSize++] = '0';
                }

                gucSIM800TxData[gucSIM800TxSize++] = ',';

                /*�������Ͷ�Ұ壬�򷵻���Ӧ��״̬�����򷵻�00*/
                if(gucMoneyExistFlag == 1)
                {
                    /*���ֽ�:δ��*/
                    gucSIM800TxData[gucSIM800TxSize++] = '0';

                    /*���ֽ�:��ǰͶ��״̬*/
                    if(MONEY_GetWorkState() == 0)
                    {
                        gucSIM800TxData[gucSIM800TxSize++] = '0';   /*δͶ��*/
                    }
                    else
                    {
                        gucSIM800TxData[gucSIM800TxSize++] = '1';   /*��Ͷ��*/
                    }
                }
                else
                {
                    gucSIM800TxData[gucSIM800TxSize++] = '0';
                    gucSIM800TxData[gucSIM800TxSize++] = '0';
                }

                gucSIM800TxData[gucSIM800TxSize++] = ',';

                /*��ǰCSQ�ź�ǿ��*/
                gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CSQ[0];
                gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CSQ[1];

                gucSIM800TxData[gucSIM800TxSize++] = ',';
                
                /*��ǰ���״̬*/
                gucSIM800TxData[gucSIM800TxSize++] = '0' + gucChairChargeStatus;

                gucSIM800TxData[gucSIM800TxSize++] = ',';                

                gucServerReplyFlag = 0;
            }
            else if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_REMOTE_ORDER_TIME) == 0)        /*GSM΢�Ŷ���ʱ���ϴ�*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_NATIVE_ORDER_TIME) == 0)        /*GSMͶ�Ҷ���ʱ���ϴ�*/
            {
                for(i = 0; i < 12; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyStartWorkTime[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                for(i = 0; i < 12; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyEndOfWorkTime[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_NATIVE_ORDER_MONEY) == 0)       /*GSM΢��Ͷ�ҵ�ǰһ��Ǯ�ϴ�*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, CLIENT_CMD_UPLOAD_ABNORMAL_ORDER) == 0)           /*�ϴ��쳣������Ϣ*/
            {
                /*��Ħ�α��*/
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                /*���ض�����*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = *((uint8_t *)(M13NetWork_DATA_START + i));
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                /*��Ħ��������ʱ��*/
                for(i = 0; i <  4; i++) gucSIM800TxData[gucSIM800TxSize++] = *((uint8_t *)(M13NetWork_DATA_START + 14 + i));
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_START_RUNNING) == 0)                   /*΢��Զ������*/
            {
                /*���ض�����*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_CLOSE_RUNNING) == 0)                   /*΢��Զ�̹ر�*/
            {
                /*���ض�����*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_EXTEND_WORKING_TIME) == 0)             /*΢��Զ������*/
            {
                /*���ض�����*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_CHAIR_SET_PARAMETER) == 0)             /*΢��APP��������*/
            {
                /*���ض�����*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_CHAIR_QUERY_STATUS) == 0)              /*��Ħ��״̬��ѯ*/
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
            else if(strcmp(gcGSMBusiness, SERVER_CMD_UPGRADING_PROGRAM) == 0)               /*΢��Զ����������*/
            {
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                gucSIM800TxData[gucSIM800TxSize++] = '0';
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_ALL) == 0)              /*΢��APP��ѯǮ���ܶ��ʶ����*/
            {
                for(i = 0; i < 5; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyAmount[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                for(i = 0; i < 6; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyWorkTime[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                for(i = 0; i < 5; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ALL) == 0)              /*΢��APP���Ǯ���ܶ��ʶ����*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_RATE) == 0)             /*΢��APP��ѯ����*/
            {
                for(i = 0; i < 2; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyRateBase[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                for(i = 0; i < 2; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyRateTime[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_RATE) == 0)               /*΢��APP���÷���*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_TIME) == 0)             /*΢��APP��ѯͶ����ʱ��*/
            {
                for(i = 0; i < 12; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyDateTime[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_TIME) == 0)               /*΢��APP����Ͷ����ʱ��*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_UNLOCKING) == 0)              /*΢��APP���Ϳ�������*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_STATUS) == 0) /*΢�Ų�ѯͶ������״̬*/
            {
                gucSIM800TxData[gucSIM800TxSize++] = gucMoneyUnlockingState;
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_TIMES) == 0)  /*΢�Ų�ѯͶ�����ۼƿ�������*/
            {
                for(i = 0; i < 5; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyLegalUnlockingTimes[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';

                for(i = 0; i < 5; i++) gucSIM800TxData[gucSIM800TxSize++] = gucMoneyIllegaUnlockinglTimes[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_LEGAL_UNLOCKING_TIMES) == 0)    /*΢�����Ͷ�����ۼƺϷ���������*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ILLEGALUNLOCKING_TIMES) == 0)   /*΢�����Ͷ�����ۼƲ��Ϸ���������*/
            {
                gucServerReplyFlag      = 1; 
            }
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COMMUNICATION_TEST) == 0)   /*test*/
            {
                 /*���ذ�Ħ�α��*/
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';  
                gucServerReplyFlag      = 1;  
                gucServerReplyFlag      = 1; 
                
            } 
            else if(strcmp(gcGSMBusiness, SERVER_CMD_START_CHARGE) == 0)   /*Զ�̿�����繦��*/
            {
                 /*���ذ�Ħ�α��*/
                //for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                //gucSIM800TxData[gucSIM800TxSize++] = ',';
                 /*���ض�����*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';           
                gucServerReplyFlag      = 1; 
            }    
            else if(strcmp(gcGSMBusiness, SERVER_CMD_CLOSE_CHARGE) == 0)   /*Զ�̹رճ�繦��*/
            {
                 /*���ذ�Ħ�α��*/
                //for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                //gucSIM800TxData[gucSIM800TxSize++] = ',';
                 /*���ض�����*/
                for(i = 0; i < 14; i++) gucSIM800TxData[gucSIM800TxSize++] = gucOrderNumber[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';      
                gucServerReplyFlag      = 1; 
            }  
            else if(strcmp(gcGSMBusiness, SERVER_CMD_SET_LONG_CHARGE) == 0)   /*����һֱ���*/
            {
                 /*���ذ�Ħ�α��*/
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';    
                gucServerReplyFlag      = 1; 
            } 
            else if(strcmp(gcGSMBusiness, SERVER_CMD_QUERY_LONG_CHARGE) == 0)   /*��ѯһֱ���*/
            {
                 /*���ذ�Ħ�α��*/
                for(i = 0; i < 20; i++) gucSIM800TxData[gucSIM800TxSize++] = gucSIM800CID[i];
                gucSIM800TxData[gucSIM800TxSize++] = ',';  
                gucSIM800TxData[gucSIM800TxSize++] = gucChairLongChargeFlag + '0';
                gucSIM800TxData[gucSIM800TxSize++] = ','; 
                gucServerReplyFlag      = 1; 
            } 
            else if(strcmp(gcGSMBusiness, SERVER_CMD_COMMUNICATION_TEST) == 0)   /*test*/
            {
                 /*���ذ�Ħ�α��*/
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
/*110~111:�����ź�ǿ��***********************************************************/
/********************************************************************************/
        case 110: SIM800_TX_Process(AT_IDX_CSQ,              111); break;
        case 111: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep=100; break;



        case 200: break;


/********************************************************************************/
/*201~234:Զ�̳�������***********************************************************/
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

            /*AES128����*/
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

        /*232:�ж�FTP�������Ƿ��ȡ���������û�н����������ȡ����ȡ�����ȴ���λ����*/
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
                        /*�ȴ����Ź���λ*/
                    }
                }
            }
            break;


        case 233: SIM800_TX_Process(AT_IDX_SAPBR_CLOSE,      234); break;
        case 234: if(gucSIM800ReplyFlag == 1) gucSIM800TxStep=205; break;


/********************************************************************************/
/*235~238:����FTP������״̬��1����������9�����쳣********************************/
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
                gucServerReplyFlag      = 0;    /*30�����ϱ�����״̬��ҪSERVER�Ļظ�*/
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
                    MONEY_Demo(8);  /*��ʼ��������ֹͶ��*/

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

    /*SIM800ģ��ĵ�Դ����ʹ������*/
    GPIO_PinModeSet(GSM_POWER_CTRL_PORT, GSM_POWER_CTRL_PIN, GSM_POWER_CTRL_MODE, 0);

    /*SIM800ģ���STATUS��LIGHT��RESET��PWRKEY���ų�ʼ������*/
    GPIO_PinModeSet(SIM800_STATUS_PORT,  SIM800_STATUS_PIN,  SIM800_STATUS_MODE,  1);
    GPIO_PinModeSet(SIM800_LIGHT_PORT,   SIM800_LIGHT_PIN,   SIM800_LIGHT_MODE,   1);
    GPIO_PinModeSet(SIM800_RST_PORT,     SIM800_RST_PIN,     SIM800_RST_MODE,     0);
    GPIO_PinModeSet(SIM800_PWR_PORT,     SIM800_PWR_PIN,     SIM800_PWR_MODE,     0);

    delay_ms(200);

    /*SIM800ģ���ϵ��ʼ������*/
    SIM800_PowerUp();


    /*SIM800����SERVER�˵����ݻ�����*/
    for(i = 0; i < 100; i++) gucSIM800TxData[i] = 0;

    gucSIM800TxSize         = 0;    /*SIM800����SERVER�˵����ݰ�����*/
    gucSIM800TxStep         = 0;    /*SIM800���Ͳ���*/

    gucSIM800ReplyFlag      = 1;    /*SIM800�ظ���־*/
    gucSIM800ReplyTime      = 0;    /*SIM800�ظ���ʱ*/

    gucServerReplyFlag      = 1;    /*SERVER�ظ���־*/
    gucServerReplyTime      = 0;    /*SERVER�ظ���ʱ*/
    gucServerReplyCount     = 0;    /*SERVER�ظ���ʱ����*/

    gucSIM800ConnectFlag    = 0;    /*SIM800������־*/
    gucSIM800OnlineState    = 0;    /*SIM800���߱�־*/
    gucSIM800StartupTime    = 0;

    gucSIM800SendEnableFlag = 0;    /*SIM800��������ʹ�ܱ�־*/
    


    /*���ȹ̶�Ϊ20��CCID��ţ�Ҳ��Ϊ��Ħ�α��*/
    for(i = 0; i < 20; i++) gucSIM800CID[i] = 0;

    /*���ȹ̶�Ϊ02��CSQ�ź�ǿ��*/
    for(i = 0; i < 2;  i++) gucSIM800CSQ[i] = 0;

    /*���ȹ̶�Ϊ15��GSN���*/
    for(i = 0; i < 15; i++) gucSIM800GSN[i] = 0;

    gucHeartbeatCount = 0;  /*������ʱ��������ÿ250ms����1*/

    gucSIM800TxStep = 1;
}


void GSM_HeartbeatHandler(uint8_t mode)
{
    /*��TCPû�����ӳɹ� ���� ONLINEָ��û�гɹ� ��ʱ���ϴ�����*/
    if((gucSIM800ConnectFlag == 0) || (gucSIM800OnlineState == 0))
    {
        gucHeartbeatCount = 0;
    }


    /*�����ϴ�����*/
    if(mode == 1) gucHeartbeatCount = 240;


    /*ÿ250ms����һ�Σ�����240�ۼӺ�Ϊ60s*/
    if(gucHeartbeatCount++ == 240)
    {
        gucHeartbeatCount = 240;

        /*�жϵ�ǰSIM800�ķ��Ͳ����Ƿ��ڿ��в��裬��������л������������Ĳ���*/
        if(GSM_IsTxStepIDLE() == 1)
        {
            /*��û����������͵�����£��ٷ�������������������ȼ����*/
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
    /*��TCPû�����ӳɹ� ���� ONLINEָ��û�гɹ� ��ʱ���ϴ�����*/
    if((gucSIM800ConnectFlag == 0) || (gucSIM800OnlineState == 0))
    {
        gucUpdateCSQCount = 0;
    }


    /*�����ϴ�����*/
    if(mode == 1) gucUpdateCSQCount = 60;


    /*ÿ250ms����һ�Σ�����240�ۼӺ�Ϊ60s*/
    if(gucUpdateCSQCount++ == 60)
    {
        gucUpdateCSQCount = 60;

        /*�жϵ�ǰSIM800�ķ��Ͳ����Ƿ��ڿ��в��裬��������л������������Ĳ���*/
        if(GSM_IsTxStepIDLE() == 1)
        {
            /*��û����������͵�����£��ٷ�������������������ȼ����*/
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
        case 0:     /*�ղ���*/
            break;

        case 1:     /*΢��Զ��������05*/
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

        case 2:     /*΢��Զ�̹رգ�09*/
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

        case 3:     /*΢��Զ�����ѣ�08*/
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

        case 4:     /*΢��APP�������06*/
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

        case 5:     /*��Ħ��״̬��ѯ��10*/
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

        case 6:     /*΢��Զ���������30*/
            break;

        case 7:     /*΢��APP��ѯǮ���ܶ��ʶ������11*/
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

        case 8:     /*΢��APP���Ǯ���ܶ��ʶ������12*/
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

        case 9:     /*΢��APP��ѯ���ʣ�13*/
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

        case 10:    /*΢��APP���÷��ʣ�14*/
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

        case 11:    /*΢��APP��ѯͶ����ʱ�䣬15*/
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

        case 12:    /*΢��APP����Ͷ����ʱ�䣬16*/
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

        case 13:    /*΢��APP���Ϳ������32*/
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

        case 14:    /*΢�Ų�ѯͶ������״̬��33*/
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

        case 15:    /*΢�Ų�ѯͶ�����ۼƿ���������34*/
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

        case 16:    /*΢�����Ͷ�����ۼƺϷ�����������35*/
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

        case 17:    /*΢�����Ͷ�����ۼƲ��Ϸ�����������36*/
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

        case 18:    /*Զ������*/
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
    /*�����150���ڣ�SIM800û�������ɹ������ӵ�SERVER�ˣ�����Ϊ����ʧ��*/
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
            if(MCB_GetChairRunningFlag() == 0)   /*��Ħ�β��������е�״̬*/
            {
                gucFTPExtGetReady = 0;
                gucSIM800TxStep   = 235;
            }
        }
    }
}






