#define __MCB_C__


#include "aP89085.h"
#include "flash.h"
#include "MCB.h"
#include "MCU.h"
#include "money.h"
#include "SIM800.h"


/*通讯队列*/
extern QUEUE_Init_TypeDef COM_QUEUE[QUEUE_NUMBER];

extern uint8_t gucExperienceTime;       /*体验时长，单位为分钟*/
extern uint8_t gucMassageManipulation;  /*按摩手法*/
extern uint8_t gucAirbagDynamics;       /*气囊力度*/
extern uint8_t gucServerCtrlFlag;       /*远程控制有效期标志*/

extern uint8_t gucMoneyWorkStep;        /*投币箱的工作步骤*/
extern uint8_t gucVoicePlayStep;        /*播放语音的下标号*/
extern uint8_t gucVoicePlayMode;        /*播放语音的触发方式*/

extern uint8_t gucOrderNumber[14];
extern uint8_t gucFlashPageData[FLASH_PAGE_SIZE];

extern uint8_t gucChargeTime;
extern uint8_t guc_Charging_Flag;

uint8_t gucChairWaitReplyFlag = 0;  /*等待投币板的回复*/
uint8_t gucChairWaitReplyTime = 0;  /*等待投币板的时间，用于超时处理*/
uint8_t gucChairExistFlag     = 1;  /*是否存在控制板标志，默认是存在*/

uint8_t gucChairStartRunCheck = 0;

uint8_t gucChairWorkEnable    = 0;  /*按摩椅是否使能位*/
uint8_t gucChairRunStatus     = 0;  /*按摩椅的运行状态*/
uint8_t gucChairRunningFlag   = 0;  /*按摩椅正在工作的标志*/
uint8_t gucChairMainRunMode   = 0;
uint8_t gucChairSubRunMode    = 0;
uint8_t gucChairSubFunction   = 0;
uint8_t gucChairStopRunCount  = 0;  /*按摩椅停止运行计数器*/
uint16_t guiChairRunningTime  = 0;  /*按摩椅已运行的时长*/

uint16_t gucCharge_1S_Cuonter  = 0;
uint16_t gucChargeTimeCount  = 0;
uint8_t gucChairChargeStatus  = 0; /*按摩椅充电状态*/  // add by sxk 2018.3.29
uint8_t gucChairLongChargeFlag = 0;/*按摩椅一直充电标志，默认上电关闭*/

uint8_t gucChairAirBagStrength = 0; 
TIME_Init_TypeDef  gtChairWorkTime; /*按摩椅的运行时间*/


uint8_t MCB_CalculateCRC(uint8_t *buffer, uint8_t length)
{
    uint8_t i = 0, ucCRC = 0;

    for(i = 0; i < length; i++)
    {
        ucCRC += buffer[i];
    }

    ucCRC ^= 0xFF; ucCRC &= 0x7F;

    return (ucCRC);
}


void InitMCB(void)
{
    gucChairWaitReplyFlag  = 0; /*等待投币板的回复*/
    gucChairWaitReplyTime  = 0; /*等待投币板的时间，用于超时处理*/
    gucChairExistFlag      = 1; /*是否存在控制板标志，默认是存在*/

    /*初始化按摩椅的使能位：禁止*/
    gucChairWorkEnable     = CHAIR_WORK_DISABLE;

    /*初始化按摩椅的工作状态：待机状态*/
    gucChairRunStatus      = CHAIR_STATE_IDLE;

    gucChairRunningFlag    = 0;
    guiChairRunningTime    = 0;

    gucChairMainRunMode    = 0;
    gucChairSubRunMode     = 0;
    gucChairSubFunction    = 0;

    /*初始化按摩椅的工作时间：0*/
    gtChairWorkTime.hour   = 0;
    gtChairWorkTime.minute = 0;
    gtChairWorkTime.second = 0;

    /*初始化按摩椅气囊*/
    gucChairAirBagStrength = 0;
}


uint8_t MCB_CommandDequeue(uint8_t *buffer)
{
    uint8_t  i    = 0;
    uint8_t  flag = 1;
    uint16_t size = 0;

    /*在消息队列中查找通讯的起始标志位*/
    do
    {
        buffer[0] = QUEUE_READ(MBCTRL_QUEUE_TX_IDX);
        
        /*将整个发送消息队列都查找了一遍，都没有找到起始标志位*/
        if(size++ >= QUEUE_SIZE)
        {
            flag = 0; break;
        }
    }while(buffer[0] != MCB_SOI);

    if(flag == 1)
    {
        /*在找到起始标志位后，连续取6个数据*/
        for(i = 1; i < 7; i++)
        {
            buffer[i] = QUEUE_READ(MBCTRL_QUEUE_TX_IDX);
        }

        /*判断取出的最后一个数据是否是结束标志位*/
        if(buffer[6] != MCB_EOI) flag = 0;
    }

    return flag;
}


void MCB_CommandEnqueue(uint8_t command, uint16_t time)
{
    uint8_t buffer[4];
    uint8_t ucCRC = 0;
    
    /*纠正时间的合法性*/
    if(time >= 1024) time = 1023;

    /*将需要计算CRC的数据项添加到数组中*/
    buffer[0] = (MCB_CONTROL_CODE);
    buffer[1] = (command  ) & 0x7F;
    buffer[2] = (time >> 0) & 0x1F;
    buffer[3] = (time >> 5) & 0x1F;

    /*计算数据的检验值*/
    ucCRC = MCB_CalculateCRC(buffer, 0x04);

    /*将需要发送的数据添加到发送消息队列，等待发送*/
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, MCB_SOI);          /*起始位*/
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, MCB_CONTROL_CODE); /*控制码*/
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, ((command  ) & 0x7F));
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, ((time >> 0) & 0x1F));
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, ((time >> 5) & 0x1F));
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, ucCRC);
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, MCB_EOI);          /*结束位*/
}


/*
00 start   (1 byte)---0xF0
01 state   (1 byte)---Run State
02 run mode(3 byte)---MainRunMode SubRunMode SubFunction
05 time    (3 byte)---(time>>14)&0x7F (time>>7)&0x7F (time>>0)&0x7F
08 position(3 byte)---(pos>>14)&0x7F (pos>>7)&0x7F (pos>>0)&0x7F
11 stretch (1 byte)
12 roller  (1 byte)---滚轮开关标志
13 buzzer  (1 byte)
14 air bag (1 byte)---气囊力度
15 crc     (1 byte)
16 end     (1 byte)---0xF1
*/
void MCB_RX_Handler(void)
{
    uint8_t i = 0, ucData = 0;

    static uint8_t gucMCBFlag = 0;
    static uint8_t gucMCBSize = 0;
    static uint8_t gucMCBData[32];

    if(QUEUE_EMPTY(MBCTRL_QUEUE_RX_IDX) == 0)
    {
        ucData = QUEUE_READ(MBCTRL_QUEUE_RX_IDX);

        /*检测到通讯的起始标志位*/
        if(ucData == MCB_SOI)
        {
            gucMCBFlag = 1;
            gucMCBSize = 0;

            for(i = 0; i < 32; i++)
            {
                gucMCBData[i] = 0x00;
            }

            gucMCBData[gucMCBSize++] = ucData;
        }
        else if(gucMCBFlag == 1)
        {
            /*将读取到的数据存储在数组中*/
            gucMCBData[gucMCBSize++] = ucData;

            if(ucData == MCB_EOI)       /*判断是否是通讯的结束标志位*/
            {
                /*命令解析*/
                gucChairWorkEnable = (gucMCBData[1] >> 0x06) & 0x01;
                gucChairRunStatus  = (gucMCBData[1] >> 0x00) & 0x07;

                gucChairMainRunMode = gucMCBData[2] & 0x7F;
                gucChairSubRunMode  = gucMCBData[3] & 0x7F;
                gucChairSubFunction = gucMCBData[4] & 0x7F;

                /*这个数据只是取出来了，实际程序中并没有用到*/
                gtChairWorkTime.hour   = gucMCBData[5] & 0x7F;
                gtChairWorkTime.minute = gucMCBData[6] & 0x7F;
                gtChairWorkTime.second = gucMCBData[7] & 0x7F;

                gucChairChargeStatus = gucMCBData[13] & 0x01;//充电状态位
                gucChairLongChargeFlag = (gucMCBData[13] & 0x02) >> 1;//打开一直充电标志位
                /*上传气囊部位给网络板？？*/
                gucChairAirBagStrength = gucMCBData[14] & 0x7F;

                if((gucChairWorkEnable == 1) && (gucChairRunStatus == CHAIR_STATE_RUN))
                {
                    if(gucChairRunningFlag == 0)
                    {
                        /*即刻上传心跳*/
                        GSM_HeartbeatHandler(1);
                    }

                    gucChairRunningFlag  = 1;
                    gucChairStartRunCheck = 1;
                }


                if(gucChairWorkEnable == 1)
                {
                    gucChairStopRunCount = 0;
                }


                /*
                **按摩椅在工作的状态下，倒计时清0了，发送控制板结束命令；
                **程序中就不对信息时间作处理了；
                **如果是投币会有检测引脚的，所以这边不需要做判断处理，只需要对SERVER做处理就好了；
                **第一次按摩椅的状态发生了改变时，执行；
                */
                if((gucServerCtrlFlag == 1) && (gucChairRunningFlag == 1))
                {
                    if(gucChairRunStatus == CHAIR_STATE_SETTLE)
                    {
                        if(
                            (gtChairWorkTime.hour   == 0) &&
                            (gtChairWorkTime.minute == 0) &&
                            (gtChairWorkTime.second == 0))
                        {
                            gucChairRunningFlag = 0;

                            MCB_CommandEnqueue(H10_KEY_WORK_DISABLE, gucExperienceTime);
                        }
                    }
                }


                if((gucServerCtrlFlag == 1) && (gucChairWorkEnable == 0))
                {
                    /*此次结束是由远程控制启动引起的，第一次按摩椅的状态发生了改变时，执行*/

                    if((gucChairRunStatus == CHAIR_STATE_IDLE) || (gucChairRunStatus == CHAIR_STATE_SLEEP))
                    {
                        if(gucChairStopRunCount++ == 9)
                        {
                            /*清除体验时长*/
                            gucExperienceTime      = 0;

                            /*清除按摩手法*/
                            gucMassageManipulation = 0;

                            /*清除气囊力度*/
                            gucAirbagDynamics      = 0;

                            /*清除远程控制有效期标志位*/
                            gucServerCtrlFlag      = 0;
                            
                            gucChairRunningFlag    = 0;

                            /*即刻上传心跳*/
                            GSM_HeartbeatHandler(1);

                            /*允许投币箱投币功能*/
                            MONEY_Demo(0x09);
#if 1
                            /*播放摩摩哒感谢您的体验*/
                            if(PLATFORM_CHINA == 1)
                            {
                                gucVoicePlayMode = 1;
                                gucVoicePlayStep = aP89085_STEP_WELCOME;
                            }
#endif
                        }
                    }
                }


                if((gucMoneyWorkStep == 4) && (gucChairWorkEnable == 0))
                {
                    /*投币箱结束*/
                    if((gucChairRunStatus == CHAIR_STATE_IDLE) || (gucChairRunStatus == CHAIR_STATE_SLEEP))
                    {
                        if(gucChairStopRunCount++ == 9)
                        {
                            gucMoneyWorkStep    = 0;
                            
                            gucChairRunningFlag = 0;

                            /*即刻上传心跳*/
                            GSM_HeartbeatHandler(1);

                            /*允许投币箱投币功能*/
                            MONEY_Demo(0x09);

#if 1
                            /*播放摩摩哒感谢您的体验*/
                            if(PLATFORM_CHINA == 1)
                            {
                                gucVoicePlayMode = 1;
                                gucVoicePlayStep = aP89085_STEP_WELCOME;
                            }
#endif
                        }
                    }
                } 


                gucMCBFlag = 0;
                gucMCBSize = 0;

                for(i = 0; i < 32; i++)
                {
                    gucMCBData[i] = 0x00;
                }

                /*控制板回复标志位清0*/
                gucChairWaitReplyFlag = 0;
            }
            else if(gucMCBSize >= 32)   /*如果接收的命令数据长度超过32个字节还没有结束，则丢弃*/
            {
                gucMCBFlag = 0;
                gucMCBSize = 0;

                for(i = 0; i < 32; i++)
                {
                    gucMCBData[i] = 0x00;
                }
            }
        }
    }
}


void MCB_TX_Handler(void)
{
    uint8_t i    = 0;
    uint8_t flag = 0;
    uint8_t ucTxBuffer[7];

    /*在网络板发送数据5秒内检测是否有控制板的回复，检测控制是否正常*/
    if(gucChairWaitReplyFlag == 1)
    {
        /*如果控制板5秒内都没有回复，说明控制板异常*/
        if(gucChairWaitReplyTime++ == 10)
        {
            gucChairWaitReplyFlag = 0;
            gucChairWaitReplyTime = 0;
            gucChairExistFlag     = 0;

            /*控制板异常时，检测一下当前远程控制的状态，并进行上报*/
            if(gucServerCtrlFlag == 1)
            {
                /*清除体验时长*/
                gucExperienceTime      = 0;

                /*清除按摩手法*/
                gucMassageManipulation = 0;

                /*清除气囊力度*/
                gucAirbagDynamics      = 0;

                /*清除远程控制有效期标志位*/
                gucServerCtrlFlag      = 0;

                /*允许投币箱投币功能*/
                MONEY_Demo(0x09);

                /*即刻上传心跳*/
                GSM_HeartbeatHandler(1);
            }
        }
    }
    else
    {
        gucChairWaitReplyTime = 0;
        gucChairExistFlag     = 1;
    }

    /*
    当发送到主控板的消息队列为空时 或者 当前按摩椅正处于回位的状态，
    网络板发送一条空消息给主控板；其它情况下将消息队列中的数据依次发送给主控板；
    */
    if((QUEUE_EMPTY(MBCTRL_QUEUE_TX_IDX) == 1) || (gucChairRunStatus  == CHAIR_STATE_SETTLE))
    {
        flag = 0;
    }
    else
    {
        /*获取命令，并返回命令的正确性标志*/
        flag = MCB_CommandDequeue(ucTxBuffer);
    }

    if(flag == 0)   /*空命令 或者 轮询命令数据包*/
    {
        ucTxBuffer[0] = MCB_SOI;
        ucTxBuffer[1] = MCB_CONTROL_CODE;
        ucTxBuffer[2] = H10_KEY_NONE;
        ucTxBuffer[3] = 0x1F;
        ucTxBuffer[4] = 0x1F;
        ucTxBuffer[5] = 0x3D;
        ucTxBuffer[6] = MCB_EOI;
    }

    /*通过LEUART1(主控板通讯串口)将命令发送出去*/
    for(i = 0; i < 7; i++)
    {
        LEUART_Tx(LEUART1, ucTxBuffer[i]);
    }

    /*控制板回复标志位置1*/
    gucChairWaitReplyFlag = 1;
}


void MCB_Demo(uint8_t index)
{
    /*举例:6分钟*/
    switch(index)
    {
        case 0:     /*空操作*/
            break;

        case 1:     /*投币或者微信扫码使能按摩椅*/
            MCB_CommandEnqueue(H10_KEY_WORK_ENABLE,       6);
            MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_0,      6);
            MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_0, 6);
            break;

        case 2:     /*按摩椅禁止工作*/
            MCB_CommandEnqueue(H10_KEY_WORK_DISABLE,      6);
            break;

        case 3:     /*微信加时命令*/
            MCB_CommandEnqueue(H10_KEY_TIME_ADD,          6);
            break;

        case 4:
            break;

        case 5:
            break;

        default:
            break;
    }
}


uint8_t MCB_GetChairRunningFlag(void)
{
    return gucChairRunningFlag;
}


void MCB_SysPowerDownHandler(void)
{
    if(gucChairRunningFlag == 0)
    {
        guiChairRunningTime = 0;
    }
    else
    {
        if(gucServerCtrlFlag == 1)  /*只对微信订单断电异常做退款处理，投币箱的还是人工处理*/
        {
            guiChairRunningTime++;
        }
        else
        {
            guiChairRunningTime = 0;
        }
    }
}

void MCB_StartRunCheckHandler(void)
{
    /*在微信启动或者蓝牙启动按摩椅后，每间隔3秒钟检测按摩椅是否启动*/
    if(gucServerCtrlFlag == 1)
    {
        if(gucChairStartRunCheck == 0)
        {
            /*使能工作控制板*/
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
        }
    }
    else
    {
        gucChairStartRunCheck = 0;
    }
}

void ChargeHandler(void)
{
    if(guc_Charging_Flag == 1)
    {
          if(gucCharge_1S_Cuonter++ == gucChargeTimeCount)
          {
              MCB_CommandEnqueue(H10_CHARGE_WORK_DISABLE, gucChargeTime); 
              gucChargeTimeCount = 0;
              guc_Charging_Flag = 0;
          }
    }
    else
    {
          gucChargeTimeCount = 0;
          gucCharge_1S_Cuonter = 0;
          if((gucChairLongChargeFlag == 0) &&(gucChairChargeStatus == 1))
          {
              MCB_CommandEnqueue(H10_CHARGE_WORK_DISABLE, gucChargeTime); 
          }
    }
} 


