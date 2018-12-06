#define __MONEY_C__


#include "MCB.h"
#include "MCU.h"
#include "money.h"
#include "SIM800.h"


/*ͨѶ����*/
extern QUEUE_Init_TypeDef COM_QUEUE[QUEUE_NUMBER];
extern char    gcGSMBusiness[3];
extern uint8_t gucSIM800ConnectFlag;    /*SIM800������־*/
extern uint8_t gucSIM800OnlineState;    /*SIM800���߱�־*/


/*Ͷ�Ұ�Ӧ�ñ���*/
uint8_t gucMoneyWaitReplyFlag = 0;  /*�ȴ�Ͷ�Ұ�Ļظ�*/
uint8_t gucMoneyWaitReplyTime = 0;  /*�ȴ�Ͷ�Ұ��ʱ�䣬���ڳ�ʱ����*/
uint8_t gucMoneyExistFlag     = 1;  /*�Ƿ����Ͷ�Ұ��־��Ĭ���Ǵ���*/
uint8_t gucMoneyCheckCount    = 0;  /*�Ƿ����Ͷ�Ұ��ѯ���Ƽ�����*/
uint8_t gucMoneyRunningFlag   = 0;  /*Ͷ�Ұ��һ������*/

uint8_t gucMoneyWorkStep      = 0;  /*Ͷ����Ĺ�������*/
uint8_t gucMoneyWorkState     = 0;  /*Ͷ����Ĺ���״̬*/
uint8_t gucMoneyStartWorkTime[12];  /*Ͷ���俪ʼ����ʱ��*/
uint8_t gucMoneyEndOfWorkTime[12];  /*Ͷ���乤������ʱ��*/

uint8_t gucMoneyAmount[5];                  /*Ǯ���ܽ��*/
uint8_t gucMoneyNumber[5];                  /*Ǯ��������*/
uint8_t gucMoneyWorkTime[6];                /*�ܹ���ʱ��*/
uint8_t gucMoneyRateBase[2];                /*���ʻ������*/
uint8_t gucMoneyRateTime[2];                /*���ʹ���ʱ��*/
uint8_t gucMoneyDateTime[12];               /*Ͷ�����ʱ��*/
uint8_t gucMoneyUnlockingState = 0;         /*Ͷ��������״̬:��0����1*/
uint8_t gucMoneyLegalUnlockingTimes[5];     /*�Ϸ���������*/
uint8_t gucMoneyIllegaUnlockinglTimes[5];   /*�Ƿ���������*/


void InitMoney(void)
{
    uint8_t i = 0;

    GPIO_PinModeSet(CHK_PUT_COIN_PORT,  CHK_PUT_COIN_PIN,  CHK_PUT_COIN_MODE,  1);

    gucMoneyWaitReplyFlag  = 0;     /*�ȴ�Ͷ�Ұ�Ļظ�*/
    gucMoneyWaitReplyTime  = 0;     /*�ȴ�Ͷ�Ұ��ʱ�䣬���ڳ�ʱ����*/
    gucMoneyExistFlag      = 1;     /*�Ƿ����Ͷ�Ұ��־��Ĭ���Ǵ���*/

    gucMoneyWorkStep       = 0;     /*Ͷ����Ĺ�������*/
    gucMoneyWorkState      = 0;     /*Ͷ����Ĺ���״̬*/

    gucMoneyUnlockingState = 0;     /*Ͷ��������״̬:��0����1*/

    for(i = 0; i < 12; i++) gucMoneyStartWorkTime[i] = 0;   /*Ͷ���俪ʼ����ʱ��*/
    for(i = 0; i < 12; i++) gucMoneyEndOfWorkTime[i] = 0;   /*Ͷ���乤������ʱ��*/
    for(i = 0; i < 5;  i++) gucMoneyAmount[i]   = 0;        /*Ǯ���ܽ��*/
    for(i = 0; i < 5;  i++) gucMoneyNumber[i]   = 0;        /*Ǯ��������*/
    for(i = 0; i < 6;  i++) gucMoneyWorkTime[i] = 0x30;     /*�ܹ���ʱ��*/
    for(i = 0; i < 2;  i++) gucMoneyRateBase[i] = 0;        /*���ʻ������*/
    for(i = 0; i < 2;  i++) gucMoneyRateTime[i] = 0;        /*���ʹ���ʱ��*/
    for(i = 0; i < 12; i++) gucMoneyDateTime[i] = 0;        /*Ͷ�����ʱ��*/
    for(i = 0; i < 5;  i++) gucMoneyLegalUnlockingTimes[i]   = 0;   /*�Ϸ���������*/
    for(i = 0; i < 5;  i++) gucMoneyIllegaUnlockinglTimes[i] = 0;   /*�Ƿ���������*/
}


uint8_t MONEY_GetWorkState(void)
{
    /*���ص�ǰͶ����Ĺ���״̬�����ڹ���ʱ����0�����ڵ���ʱ����״̬ʱ����1*/
    if(GPIO_PinInGet(CHK_PUT_COIN_PORT, CHK_PUT_COIN_PIN) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


uint8_t MONEY_CalculateCRC(uint8_t *buffer, uint8_t length)
{
    uint8_t i = 0, ucCRC = 0;
    
    for(i = 0; i < length; i++)
    {
        ucCRC += buffer[i];
    }
    
    ucCRC ^= 0xFF;
    ucCRC += 0x01;
    ucCRC &= 0x7F;
    
    return (ucCRC);
}


uint8_t MONEY_CommandDequeue(uint8_t *buffer)
{
    uint8_t  i    = 0;
    uint8_t  flag = 1;
    uint16_t size = 0;

    /*����Ϣ�����в���ͨѶ����ʼ��־λ*/
    do
    {
        WDOG_Feed();

        buffer[0] = QUEUE_READ(MONIES_QUEUE_TX_IDX);        /*��ʼ��־λ*/

        /*������������Ϣ���ж�������һ�飬��û���ҵ���ʼ��־λ*/
        if(size++ >= QUEUE_SIZE)
        {
            flag = 0; break;
        }
    }while(buffer[0] != MONEY_SOI);

    if(flag == 1)
    {
        /*���ҵ���ʼ��־λ������ȡN������*/
        buffer[1] = QUEUE_READ(MONIES_QUEUE_TX_IDX);        /*Command�ֶ�*/
        buffer[2] = QUEUE_READ(MONIES_QUEUE_TX_IDX);        /*Length�ֶ�*/

        for(i = 0; i < buffer[2]; i++)                      /*Parameter�ֶ�*/
        {
            buffer[i+3] = QUEUE_READ(MONIES_QUEUE_TX_IDX);
        }

        buffer[i+3] = QUEUE_READ(MONIES_QUEUE_TX_IDX);    /*�����ֶ�*/
        buffer[i+4] = QUEUE_READ(MONIES_QUEUE_TX_IDX);    /*������־λ*/

        /*�ж�ȡ�������һ�������Ƿ��ǽ�����־λ*/
        if(buffer[i+4] != MONEY_EOI)
        {
            flag = 0;
        }
    }

    return flag;
}


void MONEY_CommandEnqueue(uint8_t command, uint8_t *parameter, uint8_t length)
{
    uint8_t i = 0;
    uint8_t buffer[32];
    uint8_t ucCRC = 0;

    /*�Բ����ĳ��Ƚ������ƣ���ֹ����Խ��*/
    if(length >= 30) return;

    /*���������CRC�ĸ������������δ�ŵ�������*/
    buffer[0] = command;
    buffer[1] = length;

    for(i = 0; i < length; i++)
    {
        buffer[i + 2] = parameter[i];
    }

    /*�������ݵļ���ֵ*/
    ucCRC = MONEY_CalculateCRC(buffer, (length + 0x02));

    /*����Ҫ���͵�������ӵ����Ͷ����У��ȴ�����*/
    QUEUE_WRITE(MONIES_QUEUE_TX_IDX, MONEY_SOI);          /*��ʼλ*/
    QUEUE_WRITE(MONIES_QUEUE_TX_IDX, command);
    QUEUE_WRITE(MONIES_QUEUE_TX_IDX, length);

    for(i = 0; i < length; i++)
    {
        QUEUE_WRITE(MONIES_QUEUE_TX_IDX, parameter[i]);
    }

    QUEUE_WRITE(MONIES_QUEUE_TX_IDX, ucCRC);
    QUEUE_WRITE(MONIES_QUEUE_TX_IDX, MONEY_EOI);          /*����λ*/
}


void MONEY_CommandParsing(uint8_t *buffer)
{
    uint8_t i = 0;
    uint8_t command = 0;
    uint8_t length  = 0;
    uint8_t ucCRC   = 0;

    /*ȡ�����֡��������ݳ���*/
    command = buffer[1];
    length  = buffer[2];

    /*������0xF1 0x01 0x00 CRC 0xF0����֤���ݰ�����ȷ��*/
    ucCRC = MONEY_CalculateCRC(&buffer[1], (length + 0x02));

    /*������鲻��ȷֱ�ӷ���*/
    if(ucCRC != buffer[length + 0x03])
    {
        return;
    }

    /*Ͷ�����һ�γɹ����յ�һ��ָ���ʹ������Ͷ�ҵĹ��ܣ���Ҫ�ڳ�����������MCU���ִ�е�*/
    if(gucMoneyRunningFlag == 0)
    {
        MONEY_Demo(9);  /*����Ͷ�ҹ���*/

        gucMoneyRunningFlag = 1;
    }


    switch(command)
    {
        case MONEY_CMD_QUERY_CURRENT:                   /*������ѯ��ǰһ�ν��յ���Ǯ����Ŀ(MAX:99999)*/
            break;

        case MONEY_CMD_QUERY_ALL:                       /*��ѯ���յ�����Ǯ���ۼ���Ŀ����ʱ��������ʶ������(MAX:99999)*/
            for(i = 0; i < 5; i++) gucMoneyAmount[i]     = buffer[i+3];
            for(i = 0; i < 5; i++) gucMoneyWorkTime[i+1] = buffer[i+8];
            for(i = 0; i < 5; i++) gucMoneyNumber[i]     = buffer[i+13];

            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_ALL) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_CLEAR_ALL:                       /*������Ǯ���ۼ���Ŀ����ʱ��������ʶ������*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ALL) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_SET_RATE:                        /*���÷���*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_RATE) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_QUERY_RATE:                      /*��ѯ����*/
            gucMoneyRateBase[0] = buffer[3];
            gucMoneyRateBase[1] = buffer[4];

            gucMoneyRateTime[0] = buffer[5];
            gucMoneyRateTime[1] = buffer[6];

            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_RATE) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_SET_DATE_TIME:                   /*����������ʱ����*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_TIME) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_QUERY_DATE_TIME:                 /*��ѯ������ʱ����*/
            /*
            **��¼Ͷ�����ѯ����ʱ��
            **�����ǰͶ���������ڿ�ʼ��������������Ĺ����У�SERVER�����ղ������أ�
            **���԰�Ħ���ڹ�����ʱ��Ҫ�������úͲ�ѯͶ�����ʱ��Ĳ�����
            */
            if((gucMoneyWorkState == 0) && (gucMoneyWorkStep == 0))
            {
                for(i = 0; i < 12; i++) gucMoneyDateTime[i] = buffer[i+3];

                if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_TIME) == 0)
                {
                    GSM_ReplyServerHandler(1);
                }
            }

            /*��¼Ͷ���乤������ʱ��*/
            if((gucMoneyWorkState == 0) && (gucMoneyWorkStep == 1))
            {
                for(i = 0; i < 12; i++) gucMoneyEndOfWorkTime[i] = buffer[i+3];

                gucMoneyWorkStep = 2;
            }

            /*��¼Ͷ�ҿ�ʼ����ʱ��*/
            if((gucMoneyWorkState == 1) && (gucMoneyWorkStep == 1))
            {
                for(i = 0; i < 12; i++) gucMoneyStartWorkTime[i] = buffer[i+3];

                gucMoneyWorkStep = 2;
            }
            break;

        case MONEY_CMD_COIN_DISABLE:                    /*��ֹͶ�ҹ��ܣ��ϵ��Ĭ��Ϊ����Ͷ��*/
            break;

        case MONEY_CMD_COIN_ENABLE:                     /*����Ͷ�ҹ���*/
            break;

        case MONEY_CMD_MEMORY_ENABLE:                   /*����ϵ籣�湦��*/
            break;

        case MONEY_CMD_MEMORY_DISABLE:                  /*��ֹ�ϵ籣�湦�ܣ��ϵ��Ĭ��Ϊ��ֹ�ϵ籣��*/
            break;

        case MONEY_CMD_UNLOCKING:                       /*΢��ɨ�뿪��*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_UNLOCKING) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_QUERY_UNLOCKING_STATUS:          /*������ѯͶ����������״̬*/
            gucMoneyUnlockingState = buffer[3];

            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_STATUS) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_QUERY_UNLOCKING_TIMES:           /*������ѯͶ�����ۼƿ�������*/
            for(i = 0; i < 5; i++) gucMoneyLegalUnlockingTimes[i]   = buffer[i+3];
            for(i = 0; i < 5; i++) gucMoneyIllegaUnlockinglTimes[i] = buffer[i+8];

            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_TIMES) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_CLEAR_LEGAL_UNLOCKING_TIMES:     /*��������Ͷ�����ۼƺϷ���������*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_LEGAL_UNLOCKING_TIMES) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_CLEAR_ILLEGAL_UNLOCKING_TIMES:   /*��������Ͷ�����ۼ��쳣��������*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ILLEGALUNLOCKING_TIMES) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        default:
            break;
    }

    /*���յ�Ͷ�Ұ�����������������ɺ󣬿��Խ�����һ�������ͨѶ*/
    gucMoneyWaitReplyFlag = 0;
}


void MONEY_RX_Handler(void)
{
    uint8_t i = 0, ucData = 0;

    static uint8_t gucMoneyFlag = 0;
    static uint8_t gucMoneySize = 0;
    static uint8_t gucMoneyData[32];

    if(QUEUE_EMPTY(MONIES_QUEUE_RX_IDX) == 0)
    {
        ucData = QUEUE_READ(MONIES_QUEUE_RX_IDX);

        /*��⵽ͨѶ����ʼ��־λ*/
        if(ucData == MONEY_SOI)
        {
            gucMoneyFlag = 1;
            gucMoneySize = 0;

            for(i = 0; i < 32; i++) gucMoneyData[i] = 0;

            gucMoneyData[gucMoneySize++] = ucData;
        }
        else if(gucMoneyFlag == 1)
        {
            /*����ȡ�������ݴ洢��������*/
            gucMoneyData[gucMoneySize++] = ucData;

            if(ucData == MONEY_EOI) /*�ж��Ƿ���ͨѶ�Ľ�����־λ*/
            {
                /*�������*/
                MONEY_CommandParsing(gucMoneyData);

                gucMoneyFlag = 0;
                gucMoneySize = 0;

                for(i = 0; i < 32; i++) gucMoneyData[i] = 0;
            }
        }
        else if(gucMoneySize >= 32) /*������յ��������ݳ��ȳ���32���ֽڻ�û�н���������*/
        {
            gucMoneyFlag = 0;
            gucMoneySize = 0;

            for(i = 0; i < 32; i++) gucMoneyData[i] = 0;
        }
    }
}


void MONEY_TX_Handler(void)
{
    uint8_t i    = 0;
    uint8_t flag = 0;
    uint8_t ucTxBuffer[32];


    /*�����ǰ����Ͷ�Ұ������û�лظ�������г�ʱ����*/
    if(gucMoneyWaitReplyFlag == 1)
    {
        if(gucMoneyWaitReplyTime++ == 10)   /*�ȴ�5�볬ʱ������Ӧ�Ĵ���*/
        {
            gucMoneyWaitReplyTime = 0;
            gucMoneyExistFlag     = 0;      /*Ͷ�Ұ岻���ڵı�־λ*/

            /*���Ͷ��������ظ���ʱ�����͵���Ӧ��Ϊ55*/
            if(
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_ALL)  == 0)                   ||
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ALL)  == 0)                   ||
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_RATE) == 0)                   ||
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_RATE)   == 0)                   ||
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_TIME) == 0)                   ||
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_TIME)   == 0)                   ||
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_UNLOCKING)  == 0)                   ||
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_STATUS) == 0)       ||
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_TIMES)  == 0)       ||
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_LEGAL_UNLOCKING_TIMES)  == 0) ||
                (strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ILLEGALUNLOCKING_TIMES) == 0)
            )
            {
                GSM_ReplyServerHandler(0);
            }

            /*�����ǰͶ���䴦�ڵ���ʱ������״̬������Ҫ��ֹͣ��Ħ�����У�Ȼ���ϴ���������ʱ�Ķ�������ʱ����ô��*/
            // to do...
        }
    }
    else
    {
        gucMoneyWaitReplyTime = 0;
        gucMoneyExistFlag     = 1;
    }

    /*
    �����͵�Ͷ�Ұ����Ϣ����Ϊ��ʱ
    ����巢��һ����ѯ��Ϣ��Ͷ�Ұ壻��������½���Ϣ�����е��������η��͸����ذ壻
    */
    if(QUEUE_EMPTY(MONIES_QUEUE_TX_IDX) == 1)
    {
        flag = 0;

        if(gucMoneyCheckCount++ == 10)  /*�ȴ�5�볬ʱ�󽫲�ѯ������ӵ���Ϣ������*/
        {
            gucMoneyCheckCount = 0; MONEY_Demo(1);
        }
    }
    else
    {
        gucMoneyCheckCount = 0;

        /*��ȡ����������������ȷ�Ա�־*/
        flag = MONEY_CommandDequeue(ucTxBuffer);
    }

    /*��ǰ������Ϣ����Ϊ�գ�ֱ�ӷ��أ���������������������ѯ����:MONEY_Demo(1)*/
    if(flag == 0) return;

    /*ͨ��LEUART0(Ͷ�Ұ�ͨѶ����)������ͳ�ȥ*/
    for(i = 0; i < ucTxBuffer[2] + 0x05; i++)
    {
        LEUART_Tx(LEUART0, ucTxBuffer[i]);
    }

    /*����ͽ������õȴ��ظ���־λ*/
    gucMoneyWaitReplyFlag = 1;
    gucMoneyWaitReplyTime = 0;
}


void MONEY_Demo(uint8_t index)
{
    uint8_t i = 0, buffer[32];

    for(i = 0; i < 32; i++) buffer[i] = 0;

    switch(index)
    {
        case 0:     /*�ղ���*/
            break;

        case 1:     /*������ѯ��ǰһ�ν��յ���Ǯ����Ŀ(MAX:99999)*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_CURRENT, buffer, 0x00);
            break;

        case 2:     /*��ѯ���յ�����Ǯ���ۼ���Ŀ����ʱ��������ʶ������(MAX:99999)*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_ALL, buffer, 0x00);
            break;

        case 3:     /*������Ǯ���ۼ���Ŀ����ʱ��������ʶ������*/
            MONEY_CommandEnqueue(MONEY_CMD_CLEAR_ALL, buffer, 0x00);
            break;

        case 4:     /*���÷���*/
            buffer[0] = 0x30;
            buffer[1] = 0x35;
            buffer[2] = 0x30;
            buffer[3] = 0x35;
            MONEY_CommandEnqueue(MONEY_CMD_SET_RATE, buffer, 0x04);
            break;

        case 5:     /*��ѯ����*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_RATE, buffer, 0x00);
            break;

        case 6:     /*����������ʱ����*/
            buffer[0]  = 0x31;
            buffer[1]  = 0x37;
            buffer[2]  = 0x30;
            buffer[3]  = 0x34;
            buffer[4]  = 0x31;
            buffer[5]  = 0x32;
            buffer[6]  = 0x31;
            buffer[7]  = 0x33;
            buffer[8]  = 0x32;
            buffer[9]  = 0x30;
            buffer[10] = 0x30;
            buffer[11] = 0x30;
            MONEY_CommandEnqueue(MONEY_CMD_SET_DATE_TIME, buffer, 0x0C);
            break;

        case 7:     /*��ѯ������ʱ����*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_DATE_TIME, buffer, 0x00);
            break;

        case 8:     /*��ֹͶ�ҹ��ܣ��ϵ��Ĭ��Ϊ����Ͷ��*/
            MONEY_CommandEnqueue(MONEY_CMD_COIN_DISABLE, buffer, 0x00);
            break;

        case 9:     /*����Ͷ�ҹ���*/
            MONEY_CommandEnqueue(MONEY_CMD_COIN_ENABLE, buffer, 0x00);
            break;

        case 10:    /*����ϵ籣�湦��*/
            MONEY_CommandEnqueue(MONEY_CMD_MEMORY_ENABLE, buffer, 0x00);
            break;

        case 11:    /*��ֹ�ϵ籣�湦�ܣ��ϵ��Ĭ��Ϊ��ֹ�ϵ籣��*/
            MONEY_CommandEnqueue(MONEY_CMD_MEMORY_DISABLE, buffer, 0x00);
            break;

        case 12:    /*΢��ɨ�뿪��*/
            MONEY_CommandEnqueue(MONEY_CMD_UNLOCKING, buffer, 0x00);
            break;

        case 13:    /*������ѯͶ����������״̬*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_UNLOCKING_STATUS, buffer, 0x00);
            break;

        case 14:    /*������ѯͶ�����ۼƿ�������*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_UNLOCKING_TIMES, buffer, 0x00);
            break;

        case 15:    /*��������Ͷ�����ۼƺϷ���������*/
            MONEY_CommandEnqueue(MONEY_CMD_CLEAR_LEGAL_UNLOCKING_TIMES, buffer, 0x00);
            break;

        case 16:    /*��������Ͷ�����ۼ��쳣��������*/
            MONEY_CommandEnqueue(MONEY_CMD_CLEAR_ILLEGAL_UNLOCKING_TIMES, buffer, 0x00);
            break;

        default:
            break;
    }
}


void MONEY_Handler(void)
{
    uint8_t i = 0, buffer[32];

    for(i = 0; i < 32; i++) buffer[i] = 0;

#if 0
	/*�ڰ�Ħ��Ͷ�����е�ʱ��Ͷ���䱻�����ˣ���Ħ��ֹͣ����*/
	if((gucMoneyExistFlag == 0) && (gucMoneyWorkState == 1))
	{
        gucMoneyWorkState = 0;
        gucMoneyWorkStep  = 0;

		MCB_CommandEnqueue(H10_KEY_WORK_DISABLE, 0xFFFF);
	}
#endif

    if(MONEY_GetWorkState() == 1)
    {
        if(gucMoneyWorkState == 0)
        {
            /*Ͷ���俪ʼ�����Ĵ���*/
            gucMoneyWorkState = 1;
            gucMoneyWorkStep  = 1;

            /*��ѯͶ����ʱ�䣬����¼��ʼʱ��*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_DATE_TIME, buffer, 0x00); 
        }
        else
        {
            switch(gucMoneyWorkStep)
            {
                case 0: break;  /*ʲô������*/
                case 1: break;  /*�ȴ�Ͷ�Ұ�ظ���ǰ��ʼʱ��*/
                case 2:
                    /*ʹ�ܹ������ư塢��Ħ�ַ�Ĭ��ѡ����������Ĭ��ѡ��*/
                    MCB_CommandEnqueue(H10_KEY_WORK_ENABLE,       0xFFFF);
                    MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_0,      0xFFFF);
                    MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_3, 0xFFFF);
                    gucMoneyWorkStep = 0;
                    break;

                default:
                    break;
            }
        }
    }
    else
    {
        if(gucMoneyWorkState == 1)
        {
            /*Ͷ����ֹͣ�����Ĵ���*/
            gucMoneyWorkState = 0;
            gucMoneyWorkStep  = 1;

            /*�ϴ���ǰͶ������ܽ���ʶ��������Ϣ*/
            GSM_Demo(7);

            /*��ѯͶ����ʱ�䣬����¼����ʱ��*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_DATE_TIME, buffer, 0x00);
        }
        else
        {
            switch(gucMoneyWorkStep)
            {
                case 0: break;  /*ʲô������*/

                case 1: break;  /*�ȴ�Ͷ�Ұ�ظ���ǰ����ʱ��*/

                case 2:         /*�ϴ�Ͷ���䶩��*/
                    if((gucSIM800ConnectFlag == 1) && (gucSIM800OnlineState == 1))
                    {
                        GSM_UploadOrderHandler(17);

                        // to do...���涩����¼���Ա����������ϴ�����
                    }
                    else
                    {
                        gucMoneyWorkStep = 3;
                    }

                    MONEY_CommandEnqueue(MONEY_CMD_COIN_DISABLE, buffer, 0x00);
                    break;

                case 3:     /*�ȴ����ư����*/
                    MCB_CommandEnqueue(H10_KEY_WORK_DISABLE, 0xFFFF);
                    gucMoneyWorkStep = 4;
                    break;

                case 4: break;

                default:
                    break;
            }
        }
    }
}


