#define __MCB_C__


#include "aP89085.h"
#include "flash.h"
#include "MCB.h"
#include "MCU.h"
#include "money.h"
#include "SIM800.h"


/*ͨѶ����*/
extern QUEUE_Init_TypeDef COM_QUEUE[QUEUE_NUMBER];

extern uint8_t gucExperienceTime;       /*����ʱ������λΪ����*/
extern uint8_t gucMassageManipulation;  /*��Ħ�ַ�*/
extern uint8_t gucAirbagDynamics;       /*��������*/
extern uint8_t gucServerCtrlFlag;       /*Զ�̿�����Ч�ڱ�־*/

extern uint8_t gucMoneyWorkStep;        /*Ͷ����Ĺ�������*/
extern uint8_t gucVoicePlayStep;        /*�����������±��*/
extern uint8_t gucVoicePlayMode;        /*���������Ĵ�����ʽ*/

extern uint8_t gucOrderNumber[14];
extern uint8_t gucFlashPageData[FLASH_PAGE_SIZE];

extern uint8_t gucChargeTime;
extern uint8_t guc_Charging_Flag;

uint8_t gucChairWaitReplyFlag = 0;  /*�ȴ�Ͷ�Ұ�Ļظ�*/
uint8_t gucChairWaitReplyTime = 0;  /*�ȴ�Ͷ�Ұ��ʱ�䣬���ڳ�ʱ����*/
uint8_t gucChairExistFlag     = 1;  /*�Ƿ���ڿ��ư��־��Ĭ���Ǵ���*/

uint8_t gucChairStartRunCheck = 0;

uint8_t gucChairWorkEnable    = 0;  /*��Ħ���Ƿ�ʹ��λ*/
uint8_t gucChairRunStatus     = 0;  /*��Ħ�ε�����״̬*/
uint8_t gucChairRunningFlag   = 0;  /*��Ħ�����ڹ����ı�־*/
uint8_t gucChairMainRunMode   = 0;
uint8_t gucChairSubRunMode    = 0;
uint8_t gucChairSubFunction   = 0;
uint8_t gucChairStopRunCount  = 0;  /*��Ħ��ֹͣ���м�����*/
uint16_t guiChairRunningTime  = 0;  /*��Ħ�������е�ʱ��*/

uint16_t gucCharge_1S_Cuonter  = 0;
uint16_t gucChargeTimeCount  = 0;
uint8_t gucChairChargeStatus  = 0; /*��Ħ�γ��״̬*/  // add by sxk 2018.3.29
uint8_t gucChairLongChargeFlag = 0;/*��Ħ��һֱ����־��Ĭ���ϵ�ر�*/

uint8_t gucChairAirBagStrength = 0; 
TIME_Init_TypeDef  gtChairWorkTime; /*��Ħ�ε�����ʱ��*/


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
    gucChairWaitReplyFlag  = 0; /*�ȴ�Ͷ�Ұ�Ļظ�*/
    gucChairWaitReplyTime  = 0; /*�ȴ�Ͷ�Ұ��ʱ�䣬���ڳ�ʱ����*/
    gucChairExistFlag      = 1; /*�Ƿ���ڿ��ư��־��Ĭ���Ǵ���*/

    /*��ʼ����Ħ�ε�ʹ��λ����ֹ*/
    gucChairWorkEnable     = CHAIR_WORK_DISABLE;

    /*��ʼ����Ħ�εĹ���״̬������״̬*/
    gucChairRunStatus      = CHAIR_STATE_IDLE;

    gucChairRunningFlag    = 0;
    guiChairRunningTime    = 0;

    gucChairMainRunMode    = 0;
    gucChairSubRunMode     = 0;
    gucChairSubFunction    = 0;

    /*��ʼ����Ħ�εĹ���ʱ�䣺0*/
    gtChairWorkTime.hour   = 0;
    gtChairWorkTime.minute = 0;
    gtChairWorkTime.second = 0;

    /*��ʼ����Ħ������*/
    gucChairAirBagStrength = 0;
}


uint8_t MCB_CommandDequeue(uint8_t *buffer)
{
    uint8_t  i    = 0;
    uint8_t  flag = 1;
    uint16_t size = 0;

    /*����Ϣ�����в���ͨѶ����ʼ��־λ*/
    do
    {
        buffer[0] = QUEUE_READ(MBCTRL_QUEUE_TX_IDX);
        
        /*������������Ϣ���ж�������һ�飬��û���ҵ���ʼ��־λ*/
        if(size++ >= QUEUE_SIZE)
        {
            flag = 0; break;
        }
    }while(buffer[0] != MCB_SOI);

    if(flag == 1)
    {
        /*���ҵ���ʼ��־λ������ȡ6������*/
        for(i = 1; i < 7; i++)
        {
            buffer[i] = QUEUE_READ(MBCTRL_QUEUE_TX_IDX);
        }

        /*�ж�ȡ�������һ�������Ƿ��ǽ�����־λ*/
        if(buffer[6] != MCB_EOI) flag = 0;
    }

    return flag;
}


void MCB_CommandEnqueue(uint8_t command, uint16_t time)
{
    uint8_t buffer[4];
    uint8_t ucCRC = 0;
    
    /*����ʱ��ĺϷ���*/
    if(time >= 1024) time = 1023;

    /*����Ҫ����CRC����������ӵ�������*/
    buffer[0] = (MCB_CONTROL_CODE);
    buffer[1] = (command  ) & 0x7F;
    buffer[2] = (time >> 0) & 0x1F;
    buffer[3] = (time >> 5) & 0x1F;

    /*�������ݵļ���ֵ*/
    ucCRC = MCB_CalculateCRC(buffer, 0x04);

    /*����Ҫ���͵�������ӵ�������Ϣ���У��ȴ�����*/
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, MCB_SOI);          /*��ʼλ*/
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, MCB_CONTROL_CODE); /*������*/
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, ((command  ) & 0x7F));
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, ((time >> 0) & 0x1F));
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, ((time >> 5) & 0x1F));
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, ucCRC);
    QUEUE_WRITE(MBCTRL_QUEUE_TX_IDX, MCB_EOI);          /*����λ*/
}


/*
00 start   (1 byte)---0xF0
01 state   (1 byte)---Run State
02 run mode(3 byte)---MainRunMode SubRunMode SubFunction
05 time    (3 byte)---(time>>14)&0x7F (time>>7)&0x7F (time>>0)&0x7F
08 position(3 byte)---(pos>>14)&0x7F (pos>>7)&0x7F (pos>>0)&0x7F
11 stretch (1 byte)
12 roller  (1 byte)---���ֿ��ر�־
13 buzzer  (1 byte)
14 air bag (1 byte)---��������
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

        /*��⵽ͨѶ����ʼ��־λ*/
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
            /*����ȡ�������ݴ洢��������*/
            gucMCBData[gucMCBSize++] = ucData;

            if(ucData == MCB_EOI)       /*�ж��Ƿ���ͨѶ�Ľ�����־λ*/
            {
                /*�������*/
                gucChairWorkEnable = (gucMCBData[1] >> 0x06) & 0x01;
                gucChairRunStatus  = (gucMCBData[1] >> 0x00) & 0x07;

                gucChairMainRunMode = gucMCBData[2] & 0x7F;
                gucChairSubRunMode  = gucMCBData[3] & 0x7F;
                gucChairSubFunction = gucMCBData[4] & 0x7F;

                /*�������ֻ��ȡ�����ˣ�ʵ�ʳ����в�û���õ�*/
                gtChairWorkTime.hour   = gucMCBData[5] & 0x7F;
                gtChairWorkTime.minute = gucMCBData[6] & 0x7F;
                gtChairWorkTime.second = gucMCBData[7] & 0x7F;

                gucChairChargeStatus = gucMCBData[13] & 0x01;//���״̬λ
                gucChairLongChargeFlag = (gucMCBData[13] & 0x02) >> 1;//��һֱ����־λ
                /*�ϴ����Ҳ�λ������壿��*/
                gucChairAirBagStrength = gucMCBData[14] & 0x7F;

                if((gucChairWorkEnable == 1) && (gucChairRunStatus == CHAIR_STATE_RUN))
                {
                    if(gucChairRunningFlag == 0)
                    {
                        /*�����ϴ�����*/
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
                **��Ħ���ڹ�����״̬�£�����ʱ��0�ˣ����Ϳ��ư�������
                **�����оͲ�����Ϣʱ���������ˣ�
                **�����Ͷ�һ��м�����ŵģ�������߲���Ҫ���жϴ���ֻ��Ҫ��SERVER������ͺ��ˣ�
                **��һ�ΰ�Ħ�ε�״̬�����˸ı�ʱ��ִ�У�
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
                    /*�˴ν�������Զ�̿�����������ģ���һ�ΰ�Ħ�ε�״̬�����˸ı�ʱ��ִ��*/

                    if((gucChairRunStatus == CHAIR_STATE_IDLE) || (gucChairRunStatus == CHAIR_STATE_SLEEP))
                    {
                        if(gucChairStopRunCount++ == 9)
                        {
                            /*�������ʱ��*/
                            gucExperienceTime      = 0;

                            /*�����Ħ�ַ�*/
                            gucMassageManipulation = 0;

                            /*�����������*/
                            gucAirbagDynamics      = 0;

                            /*���Զ�̿�����Ч�ڱ�־λ*/
                            gucServerCtrlFlag      = 0;
                            
                            gucChairRunningFlag    = 0;

                            /*�����ϴ�����*/
                            GSM_HeartbeatHandler(1);

                            /*����Ͷ����Ͷ�ҹ���*/
                            MONEY_Demo(0x09);
#if 1
                            /*����ĦĦ�ո�л��������*/
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
                    /*Ͷ�������*/
                    if((gucChairRunStatus == CHAIR_STATE_IDLE) || (gucChairRunStatus == CHAIR_STATE_SLEEP))
                    {
                        if(gucChairStopRunCount++ == 9)
                        {
                            gucMoneyWorkStep    = 0;
                            
                            gucChairRunningFlag = 0;

                            /*�����ϴ�����*/
                            GSM_HeartbeatHandler(1);

                            /*����Ͷ����Ͷ�ҹ���*/
                            MONEY_Demo(0x09);

#if 1
                            /*����ĦĦ�ո�л��������*/
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

                /*���ư�ظ���־λ��0*/
                gucChairWaitReplyFlag = 0;
            }
            else if(gucMCBSize >= 32)   /*������յ��������ݳ��ȳ���32���ֽڻ�û�н���������*/
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

    /*������巢������5���ڼ���Ƿ��п��ư�Ļظ����������Ƿ�����*/
    if(gucChairWaitReplyFlag == 1)
    {
        /*������ư�5���ڶ�û�лظ���˵�����ư��쳣*/
        if(gucChairWaitReplyTime++ == 10)
        {
            gucChairWaitReplyFlag = 0;
            gucChairWaitReplyTime = 0;
            gucChairExistFlag     = 0;

            /*���ư��쳣ʱ�����һ�µ�ǰԶ�̿��Ƶ�״̬���������ϱ�*/
            if(gucServerCtrlFlag == 1)
            {
                /*�������ʱ��*/
                gucExperienceTime      = 0;

                /*�����Ħ�ַ�*/
                gucMassageManipulation = 0;

                /*�����������*/
                gucAirbagDynamics      = 0;

                /*���Զ�̿�����Ч�ڱ�־λ*/
                gucServerCtrlFlag      = 0;

                /*����Ͷ����Ͷ�ҹ���*/
                MONEY_Demo(0x09);

                /*�����ϴ�����*/
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
    �����͵����ذ����Ϣ����Ϊ��ʱ ���� ��ǰ��Ħ�������ڻ�λ��״̬��
    ����巢��һ������Ϣ�����ذ壻��������½���Ϣ�����е��������η��͸����ذ壻
    */
    if((QUEUE_EMPTY(MBCTRL_QUEUE_TX_IDX) == 1) || (gucChairRunStatus  == CHAIR_STATE_SETTLE))
    {
        flag = 0;
    }
    else
    {
        /*��ȡ����������������ȷ�Ա�־*/
        flag = MCB_CommandDequeue(ucTxBuffer);
    }

    if(flag == 0)   /*������ ���� ��ѯ�������ݰ�*/
    {
        ucTxBuffer[0] = MCB_SOI;
        ucTxBuffer[1] = MCB_CONTROL_CODE;
        ucTxBuffer[2] = H10_KEY_NONE;
        ucTxBuffer[3] = 0x1F;
        ucTxBuffer[4] = 0x1F;
        ucTxBuffer[5] = 0x3D;
        ucTxBuffer[6] = MCB_EOI;
    }

    /*ͨ��LEUART1(���ذ�ͨѶ����)������ͳ�ȥ*/
    for(i = 0; i < 7; i++)
    {
        LEUART_Tx(LEUART1, ucTxBuffer[i]);
    }

    /*���ư�ظ���־λ��1*/
    gucChairWaitReplyFlag = 1;
}


void MCB_Demo(uint8_t index)
{
    /*����:6����*/
    switch(index)
    {
        case 0:     /*�ղ���*/
            break;

        case 1:     /*Ͷ�һ���΢��ɨ��ʹ�ܰ�Ħ��*/
            MCB_CommandEnqueue(H10_KEY_WORK_ENABLE,       6);
            MCB_CommandEnqueue(H10_KEY_CHAIR_AUTO_0,      6);
            MCB_CommandEnqueue(H10_KEY_AIRBAG_STRENGTH_0, 6);
            break;

        case 2:     /*��Ħ�ν�ֹ����*/
            MCB_CommandEnqueue(H10_KEY_WORK_DISABLE,      6);
            break;

        case 3:     /*΢�ż�ʱ����*/
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
        if(gucServerCtrlFlag == 1)  /*ֻ��΢�Ŷ����ϵ��쳣���˿��Ͷ����Ļ����˹�����*/
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
    /*��΢��������������������Ħ�κ�ÿ���3���Ӽ�ⰴĦ���Ƿ�����*/
    if(gucServerCtrlFlag == 1)
    {
        if(gucChairStartRunCheck == 0)
        {
            /*ʹ�ܹ������ư�*/
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


