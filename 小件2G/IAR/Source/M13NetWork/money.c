#define __MONEY_C__


#include "MCB.h"
#include "MCU.h"
#include "money.h"
#include "SIM800.h"


/*通讯队列*/
extern QUEUE_Init_TypeDef COM_QUEUE[QUEUE_NUMBER];
extern char    gcGSMBusiness[3];
extern uint8_t gucSIM800ConnectFlag;    /*SIM800连网标志*/
extern uint8_t gucSIM800OnlineState;    /*SIM800上线标志*/


/*投币板应用变量*/
uint8_t gucMoneyWaitReplyFlag = 0;  /*等待投币板的回复*/
uint8_t gucMoneyWaitReplyTime = 0;  /*等待投币板的时间，用于超时处理*/
uint8_t gucMoneyExistFlag     = 1;  /*是否存在投币板标志，默认是存在*/
uint8_t gucMoneyCheckCount    = 0;  /*是否存在投币板查询机制计数器*/
uint8_t gucMoneyRunningFlag   = 0;  /*投币板第一次启动*/

uint8_t gucMoneyWorkStep      = 0;  /*投币箱的工作步骤*/
uint8_t gucMoneyWorkState     = 0;  /*投币箱的工作状态*/
uint8_t gucMoneyStartWorkTime[12];  /*投币箱开始工作时间*/
uint8_t gucMoneyEndOfWorkTime[12];  /*投币箱工作结束时间*/

uint8_t gucMoneyAmount[5];                  /*钱币总金额*/
uint8_t gucMoneyNumber[5];                  /*钱币总张数*/
uint8_t gucMoneyWorkTime[6];                /*总工作时间*/
uint8_t gucMoneyRateBase[2];                /*费率基数金额*/
uint8_t gucMoneyRateTime[2];                /*费率工作时长*/
uint8_t gucMoneyDateTime[12];               /*投币箱的时间*/
uint8_t gucMoneyUnlockingState = 0;         /*投币箱锁的状态:关0、开1*/
uint8_t gucMoneyLegalUnlockingTimes[5];     /*合法开锁次数*/
uint8_t gucMoneyIllegaUnlockinglTimes[5];   /*非法开锁次数*/


void InitMoney(void)
{
    uint8_t i = 0;

    GPIO_PinModeSet(CHK_PUT_COIN_PORT,  CHK_PUT_COIN_PIN,  CHK_PUT_COIN_MODE,  1);

    gucMoneyWaitReplyFlag  = 0;     /*等待投币板的回复*/
    gucMoneyWaitReplyTime  = 0;     /*等待投币板的时间，用于超时处理*/
    gucMoneyExistFlag      = 1;     /*是否存在投币板标志，默认是存在*/

    gucMoneyWorkStep       = 0;     /*投币箱的工作步骤*/
    gucMoneyWorkState      = 0;     /*投币箱的工作状态*/

    gucMoneyUnlockingState = 0;     /*投币箱锁的状态:关0、开1*/

    for(i = 0; i < 12; i++) gucMoneyStartWorkTime[i] = 0;   /*投币箱开始工作时间*/
    for(i = 0; i < 12; i++) gucMoneyEndOfWorkTime[i] = 0;   /*投币箱工作结束时间*/
    for(i = 0; i < 5;  i++) gucMoneyAmount[i]   = 0;        /*钱币总金额*/
    for(i = 0; i < 5;  i++) gucMoneyNumber[i]   = 0;        /*钱币总张数*/
    for(i = 0; i < 6;  i++) gucMoneyWorkTime[i] = 0x30;     /*总工作时间*/
    for(i = 0; i < 2;  i++) gucMoneyRateBase[i] = 0;        /*费率基数金额*/
    for(i = 0; i < 2;  i++) gucMoneyRateTime[i] = 0;        /*费率工作时长*/
    for(i = 0; i < 12; i++) gucMoneyDateTime[i] = 0;        /*投币箱的时间*/
    for(i = 0; i < 5;  i++) gucMoneyLegalUnlockingTimes[i]   = 0;   /*合法开锁次数*/
    for(i = 0; i < 5;  i++) gucMoneyIllegaUnlockinglTimes[i] = 0;   /*非法开锁次数*/
}


uint8_t MONEY_GetWorkState(void)
{
    /*返回当前投币箱的工作状态，不在工作时返回0，处于倒计时工作状态时返回1*/
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

    /*在消息队列中查找通讯的起始标志位*/
    do
    {
        WDOG_Feed();

        buffer[0] = QUEUE_READ(MONIES_QUEUE_TX_IDX);        /*起始标志位*/

        /*将整个发送消息队列都查找了一遍，都没有找到起始标志位*/
        if(size++ >= QUEUE_SIZE)
        {
            flag = 0; break;
        }
    }while(buffer[0] != MONEY_SOI);

    if(flag == 1)
    {
        /*在找到起始标志位后，连续取N个数据*/
        buffer[1] = QUEUE_READ(MONIES_QUEUE_TX_IDX);        /*Command字段*/
        buffer[2] = QUEUE_READ(MONIES_QUEUE_TX_IDX);        /*Length字段*/

        for(i = 0; i < buffer[2]; i++)                      /*Parameter字段*/
        {
            buffer[i+3] = QUEUE_READ(MONIES_QUEUE_TX_IDX);
        }

        buffer[i+3] = QUEUE_READ(MONIES_QUEUE_TX_IDX);    /*检验字段*/
        buffer[i+4] = QUEUE_READ(MONIES_QUEUE_TX_IDX);    /*结束标志位*/

        /*判断取出的最后一个数据是否是结束标志位*/
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

    /*对参数的长度进行限制，防止数组越界*/
    if(length >= 30) return;

    /*将参与计算CRC的各个数据项依次存放到数组中*/
    buffer[0] = command;
    buffer[1] = length;

    for(i = 0; i < length; i++)
    {
        buffer[i + 2] = parameter[i];
    }

    /*计算数据的检验值*/
    ucCRC = MONEY_CalculateCRC(buffer, (length + 0x02));

    /*将需要发送的数据添加到发送队列中，等待发送*/
    QUEUE_WRITE(MONIES_QUEUE_TX_IDX, MONEY_SOI);          /*起始位*/
    QUEUE_WRITE(MONIES_QUEUE_TX_IDX, command);
    QUEUE_WRITE(MONIES_QUEUE_TX_IDX, length);

    for(i = 0; i < length; i++)
    {
        QUEUE_WRITE(MONIES_QUEUE_TX_IDX, parameter[i]);
    }

    QUEUE_WRITE(MONIES_QUEUE_TX_IDX, ucCRC);
    QUEUE_WRITE(MONIES_QUEUE_TX_IDX, MONEY_EOI);          /*结束位*/
}


void MONEY_CommandParsing(uint8_t *buffer)
{
    uint8_t i = 0;
    uint8_t command = 0;
    uint8_t length  = 0;
    uint8_t ucCRC   = 0;

    /*取命令字、参数数据长度*/
    command = buffer[1];
    length  = buffer[2];

    /*举例：0xF1 0x01 0x00 CRC 0xF0，验证数据包的正确性*/
    ucCRC = MONEY_CalculateCRC(&buffer[1], (length + 0x02));

    /*如果检验不正确直接返回*/
    if(ucCRC != buffer[length + 0x03])
    {
        return;
    }

    /*投币箱第一次成功接收到一条指令后，使能允许投币的功能；主要在程序升级重启MCU后会执行到*/
    if(gucMoneyRunningFlag == 0)
    {
        MONEY_Demo(9);  /*允许投币功能*/

        gucMoneyRunningFlag = 1;
    }


    switch(command)
    {
        case MONEY_CMD_QUERY_CURRENT:                   /*主机查询当前一次接收到的钱币数目(MAX:99999)*/
            break;

        case MONEY_CMD_QUERY_ALL:                       /*查询接收到的总钱币累计数目、总时间数、总识币张数(MAX:99999)*/
            for(i = 0; i < 5; i++) gucMoneyAmount[i]     = buffer[i+3];
            for(i = 0; i < 5; i++) gucMoneyWorkTime[i+1] = buffer[i+8];
            for(i = 0; i < 5; i++) gucMoneyNumber[i]     = buffer[i+13];

            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_ALL) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_CLEAR_ALL:                       /*清零总钱币累计数目、总时间数、总识币张数*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ALL) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_SET_RATE:                        /*设置费率*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_RATE) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_QUERY_RATE:                      /*查询费率*/
            gucMoneyRateBase[0] = buffer[3];
            gucMoneyRateBase[1] = buffer[4];

            gucMoneyRateTime[0] = buffer[5];
            gucMoneyRateTime[1] = buffer[6];

            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_RATE) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_SET_DATE_TIME:                   /*设置年月日时分秒*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_SET_TIME) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_QUERY_DATE_TIME:                 /*查询年月日时分秒*/
            /*
            **记录投币箱查询到的时间
            **如果当前投身箱正处于开始工作或结束工作的过程中，SERVER端则收不到返回；
            **所以按摩椅在工作的时候不要进行设置和查询投币箱的时间的操作；
            */
            if((gucMoneyWorkState == 0) && (gucMoneyWorkStep == 0))
            {
                for(i = 0; i < 12; i++) gucMoneyDateTime[i] = buffer[i+3];

                if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_TIME) == 0)
                {
                    GSM_ReplyServerHandler(1);
                }
            }

            /*记录投币箱工作结束时间*/
            if((gucMoneyWorkState == 0) && (gucMoneyWorkStep == 1))
            {
                for(i = 0; i < 12; i++) gucMoneyEndOfWorkTime[i] = buffer[i+3];

                gucMoneyWorkStep = 2;
            }

            /*记录投币开始工作时间*/
            if((gucMoneyWorkState == 1) && (gucMoneyWorkStep == 1))
            {
                for(i = 0; i < 12; i++) gucMoneyStartWorkTime[i] = buffer[i+3];

                gucMoneyWorkStep = 2;
            }
            break;

        case MONEY_CMD_COIN_DISABLE:                    /*禁止投币功能，上电后默认为允许投币*/
            break;

        case MONEY_CMD_COIN_ENABLE:                     /*允许投币功能*/
            break;

        case MONEY_CMD_MEMORY_ENABLE:                   /*允许断电保存功能*/
            break;

        case MONEY_CMD_MEMORY_DISABLE:                  /*禁止断电保存功能，上电后默认为禁止断电保存*/
            break;

        case MONEY_CMD_UNLOCKING:                       /*微信扫码开锁*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_UNLOCKING) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_QUERY_UNLOCKING_STATUS:          /*主机查询投币箱门锁的状态*/
            gucMoneyUnlockingState = buffer[3];

            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_STATUS) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_QUERY_UNLOCKING_TIMES:           /*主机查询投币箱累计开锁次数*/
            for(i = 0; i < 5; i++) gucMoneyLegalUnlockingTimes[i]   = buffer[i+3];
            for(i = 0; i < 5; i++) gucMoneyIllegaUnlockinglTimes[i] = buffer[i+8];

            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_QUERY_UNLOCKING_TIMES) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_CLEAR_LEGAL_UNLOCKING_TIMES:     /*主机清零投币箱累计合法开锁次数*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_LEGAL_UNLOCKING_TIMES) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        case MONEY_CMD_CLEAR_ILLEGAL_UNLOCKING_TIMES:   /*主机清零投币箱累计异常开锁次数*/
            if(strcmp(gcGSMBusiness, SERVER_CMD_COIN_BOX_CLEAR_ILLEGALUNLOCKING_TIMES) == 0)
            {
                GSM_ReplyServerHandler(1);
            }
            break;

        default:
            break;
    }

    /*接收到投币板的命令，解析并处理完成后，可以进行下一组命令的通讯*/
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

        /*检测到通讯的起始标志位*/
        if(ucData == MONEY_SOI)
        {
            gucMoneyFlag = 1;
            gucMoneySize = 0;

            for(i = 0; i < 32; i++) gucMoneyData[i] = 0;

            gucMoneyData[gucMoneySize++] = ucData;
        }
        else if(gucMoneyFlag == 1)
        {
            /*将读取到的数据存储在数组中*/
            gucMoneyData[gucMoneySize++] = ucData;

            if(ucData == MONEY_EOI) /*判断是否是通讯的结束标志位*/
            {
                /*命令解析*/
                MONEY_CommandParsing(gucMoneyData);

                gucMoneyFlag = 0;
                gucMoneySize = 0;

                for(i = 0; i < 32; i++) gucMoneyData[i] = 0;
            }
        }
        else if(gucMoneySize >= 32) /*如果接收的命令数据长度超过32个字节还没有结束，则丢弃*/
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


    /*如果当前发给投币板的命令没有回复，则进行超时处理*/
    if(gucMoneyWaitReplyFlag == 1)
    {
        if(gucMoneyWaitReplyTime++ == 10)   /*等待5秒超时后做相应的处理*/
        {
            gucMoneyWaitReplyTime = 0;
            gucMoneyExistFlag     = 0;      /*投币板不存在的标志位*/

            /*如果投币箱命令回复超时，则发送的响应码为55*/
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

            /*如果当前投币箱处于倒计时，运行状态，则需要先停止按摩椅运行，然后上传订单，此时的订单结束时间怎么算*/
            // to do...
        }
    }
    else
    {
        gucMoneyWaitReplyTime = 0;
        gucMoneyExistFlag     = 1;
    }

    /*
    当发送到投币板的消息队列为空时
    网络板发送一条查询消息给投币板；其它情况下将消息队列中的数据依次发送给主控板；
    */
    if(QUEUE_EMPTY(MONIES_QUEUE_TX_IDX) == 1)
    {
        flag = 0;

        if(gucMoneyCheckCount++ == 10)  /*等待5秒超时后将查询命令添加到消息队列中*/
        {
            gucMoneyCheckCount = 0; MONEY_Demo(1);
        }
    }
    else
    {
        gucMoneyCheckCount = 0;

        /*获取命令，并返回命令的正确性标志*/
        flag = MONEY_CommandDequeue(ucTxBuffer);
    }

    /*当前发送消息队列为空，直接返回，如果不返回则发送上面的轮询命令:MONEY_Demo(1)*/
    if(flag == 0) return;

    /*通过LEUART0(投币板通讯串口)将命令发送出去*/
    for(i = 0; i < ucTxBuffer[2] + 0x05; i++)
    {
        LEUART_Tx(LEUART0, ucTxBuffer[i]);
    }

    /*命令发送结束后，置等待回复标志位*/
    gucMoneyWaitReplyFlag = 1;
    gucMoneyWaitReplyTime = 0;
}


void MONEY_Demo(uint8_t index)
{
    uint8_t i = 0, buffer[32];

    for(i = 0; i < 32; i++) buffer[i] = 0;

    switch(index)
    {
        case 0:     /*空操作*/
            break;

        case 1:     /*主机查询当前一次接收到的钱币数目(MAX:99999)*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_CURRENT, buffer, 0x00);
            break;

        case 2:     /*查询接收到的总钱币累计数目、总时间数、总识币张数(MAX:99999)*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_ALL, buffer, 0x00);
            break;

        case 3:     /*清零总钱币累计数目、总时间数、总识币张数*/
            MONEY_CommandEnqueue(MONEY_CMD_CLEAR_ALL, buffer, 0x00);
            break;

        case 4:     /*设置费率*/
            buffer[0] = 0x30;
            buffer[1] = 0x35;
            buffer[2] = 0x30;
            buffer[3] = 0x35;
            MONEY_CommandEnqueue(MONEY_CMD_SET_RATE, buffer, 0x04);
            break;

        case 5:     /*查询费率*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_RATE, buffer, 0x00);
            break;

        case 6:     /*设置年月日时分秒*/
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

        case 7:     /*查询年月日时分秒*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_DATE_TIME, buffer, 0x00);
            break;

        case 8:     /*禁止投币功能，上电后默认为允许投币*/
            MONEY_CommandEnqueue(MONEY_CMD_COIN_DISABLE, buffer, 0x00);
            break;

        case 9:     /*允许投币功能*/
            MONEY_CommandEnqueue(MONEY_CMD_COIN_ENABLE, buffer, 0x00);
            break;

        case 10:    /*允许断电保存功能*/
            MONEY_CommandEnqueue(MONEY_CMD_MEMORY_ENABLE, buffer, 0x00);
            break;

        case 11:    /*禁止断电保存功能，上电后默认为禁止断电保存*/
            MONEY_CommandEnqueue(MONEY_CMD_MEMORY_DISABLE, buffer, 0x00);
            break;

        case 12:    /*微信扫码开锁*/
            MONEY_CommandEnqueue(MONEY_CMD_UNLOCKING, buffer, 0x00);
            break;

        case 13:    /*主机查询投币箱门锁的状态*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_UNLOCKING_STATUS, buffer, 0x00);
            break;

        case 14:    /*主机查询投币箱累计开锁次数*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_UNLOCKING_TIMES, buffer, 0x00);
            break;

        case 15:    /*主机清零投币箱累计合法开锁次数*/
            MONEY_CommandEnqueue(MONEY_CMD_CLEAR_LEGAL_UNLOCKING_TIMES, buffer, 0x00);
            break;

        case 16:    /*主机清零投币箱累计异常开锁次数*/
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
	/*在按摩椅投币运行的时候，投币箱被拨掉了，按摩椅停止工作*/
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
            /*投币箱开始工作的处理*/
            gucMoneyWorkState = 1;
            gucMoneyWorkStep  = 1;

            /*查询投币箱时间，并记录开始时间*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_DATE_TIME, buffer, 0x00); 
        }
        else
        {
            switch(gucMoneyWorkStep)
            {
                case 0: break;  /*什么都不做*/
                case 1: break;  /*等待投币板回复当前开始时间*/
                case 2:
                    /*使能工作控制板、按摩手法默认选择、气囊力度默认选择*/
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
            /*投币箱停止工作的处理*/
            gucMoneyWorkState = 0;
            gucMoneyWorkStep  = 1;

            /*上传当前投币箱的总金额、总识币数等信息*/
            GSM_Demo(7);

            /*查询投币箱时间，并记录结束时间*/
            MONEY_CommandEnqueue(MONEY_CMD_QUERY_DATE_TIME, buffer, 0x00);
        }
        else
        {
            switch(gucMoneyWorkStep)
            {
                case 0: break;  /*什么都不做*/

                case 1: break;  /*等待投币板回复当前结束时间*/

                case 2:         /*上传投币箱订单*/
                    if((gucSIM800ConnectFlag == 1) && (gucSIM800OnlineState == 1))
                    {
                        GSM_UploadOrderHandler(17);

                        // to do...保存订单记录，以便下连网后上传订单
                    }
                    else
                    {
                        gucMoneyWorkStep = 3;
                    }

                    MONEY_CommandEnqueue(MONEY_CMD_COIN_DISABLE, buffer, 0x00);
                    break;

                case 3:     /*等待控制板结束*/
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


