#define __MAIN_C__


#include "AES.h"
#include "aP89085.h"
#include "Base64.h"
#include "board.h"
#include "flash.h"
#include "LED.h"
#include "main.h"
#include "MCB.h"
#include "MCU.h"
#include "money.h"
#include "SIM800.h"


extern unsigned short __checksum;
extern unsigned char  gucSIM800OnlineState;


uint32_t gulChipUnique0 = 0;    /*[31,00]Unique Number*/
uint32_t gulChipUnique1 = 0;    /*[64,32]Unique Number*/


/*��ʼ���г�����ת*/
void JumpRun(void)
{
    /*V2.XX & V4.XX֧��Զ����������*/
    if((FW_VERSION_MAJOR == 2) || (FW_VERSION_MAJOR == 4))
    {
    }

    SCB->VTOR = (uint32_t)(M13NetWork_CODE_START);

    if(__checksum == 0)
    {
        __checksum = 1;
    }

    gulChipUnique0 = *((uint32_t *)(0x0FE081F0));
    gulChipUnique1 = *((uint32_t *)(0x0FE081F4));
}


void InitSystem(void)
{
    uint8_t ucFlag = 0;

    JumpRun();

    InitMCU();

    delay_ms(9);

    InitBoard();

    if(ucFlag) BASE64_Demo();

    if(ucFlag) AES_Demo();

    if(ucFlag) FLASH_Demo();
}


void Simulator(void)
{
    static uint8_t ucSimulatorDemo = 0;
    static uint8_t ucSimulatorStep = 0;
    static uint8_t ucSimulatorTime = 0;
    static uint8_t ucMoneyRunState = 0;
    
    if(ucSimulatorDemo == 0)
    {
        ucSimulatorStep = 0;
        ucSimulatorTime = 0;    return;
    }
    else
    {
        ucSimulatorTime++;
    }

    if(ucSimulatorTime == 3)
    {
        switch(ucSimulatorDemo)
        {
            case 1:
                /*
                ** 1:΢��Զ������
                ** 2:΢��Զ�̹ر�
                ** 3:΢��Զ������
                ** 4:΢��APP��������
                ** 5:��Ħ��״̬��ѯ
                ** 6:΢��Զ����������
                ** 7:΢��APP��ѯǮ���ܶ��ʶ����
                ** 8:΢��APP���Ǯ���ܶ��ʶ����
                ** 9:΢��APP��ѯ����
                **10:΢��APP���÷���
                **11:΢��APP��ѯͶ����ʱ��
                **12:΢��APP����Ͷ��7��ʱ��
                **13:΢��APP���Ϳ�������
                **14:΢�Ų�ѯͶ������״̬
                **15:΢�Ų�ѯͶ�����ۼƿ�������
                **16:΢�����Ͷ�����ۼƺϷ���������
                **17:΢�����Ͷ�����ۼƲ��Ϸ���������
                **18:Զ������
                */
                if((GSM_IsTxStepIDLE() == 0) || (gucSIM800OnlineState == 0))
                {
                    ucSimulatorTime = 2;    return;
                }

                GSM_Demo(ucSimulatorStep);
                break;

            case 2:
                /*
                ** 1:Ͷ�һ���΢��ɨ��ʹ�ܰ�Ħ��
                ** 2:��Ħ�ν�ֹ����
                ** 3:΢�ż�ʱ����
                */
                MCB_Demo(ucSimulatorStep);
                break;

            case 3:
                /*
                ** 1:������ѯ��ǰһ�ν��յ���Ǯ����Ŀ(MAX:99999)
                ** 2:��ѯ���յ�����Ǯ���ۼ���Ŀ����ʱ��������ʶ������(MAX:99999)
                ** 3:������Ǯ���ۼ���Ŀ����ʱ��������ʶ������
                ** 4:���÷���
                ** 5:��ѯ����
                ** 6:����������ʱ����
                ** 7:��ѯ������ʱ����
                ** 8:��ֹͶ�ҹ��ܣ��ϵ��Ĭ��Ϊ����Ͷ��
                ** 9:����Ͷ�ҹ���
                **10:����ϵ籣�湦��
                **11:��ֹ�ϵ籣�湦�ܣ��ϵ��Ĭ��Ϊ��ֹ�ϵ籣��
                **12:΢��ɨ�뿪��
                **13:������ѯͶ����������״̬
                **14:������ѯͶ�����ۼƿ�������
                **15:��������Ͷ�����ۼƺϷ���������
                **16:��������Ͷ�����ۼ��쳣��������
                */
                MONEY_Demo(ucSimulatorStep);
                break;
                
            case 4:
                ucMoneyRunState = MONEY_GetWorkState();

                if(ucMoneyRunState == 1)
                {
                    // to do...
                }
                break;

            case 5:
                aP89085_Demo(ucSimulatorStep);
                break;

            default:
                break;
        }

        ucSimulatorStep = 0;
        ucSimulatorTime = 0;
    }
}


void main(void)
{
    uint8_t ucFlag = 0;

    static uint8_t ucMCBRunFlag    = 1;
    static uint8_t ucMoneyRunFlag  = 1;
    static uint8_t ucSIM800RunFlag = 1;
    static uint8_t ucPowerDownFlag = 1;

    InitSystem();

   // if(ucFlag) aP89085_Demo(1);

    while(1)
    {
        ucFlag = GetSystemRunFlag();


        if(ucFlag & 0x01)                                   /*ÿ���10ms�¼������֧*/
        {
            if(ucMCBRunFlag   ) MCB_RX_Handler();           /*���ذ�ͨѶ���մ�����*/

            if(ucMoneyRunFlag ) MONEY_RX_Handler();         /*Ͷ�Ұ�ͨѶ���մ�����*/

            if(ucSIM800RunFlag) SIM800_RX_Handler();        /*SIM800ͨѶ���մ�����*/
            
        }


        if(ucFlag & 0x02)   /*ÿ���100ms�¼������֧*/
        {
            //aP89085_Handler();

            LED_Handler();
        }


        if(ucFlag & 0x04)                                   /*ÿ���250ms�¼������֧*/
        {
            if(ucSIM800RunFlag) GSM_UploadOrderHandler(0);  /*�����ϴ������Ĵ���*/

            if(ucSIM800RunFlag) GSM_HeartbeatHandler(0);    /*�����ϴ������Ĵ���*/

            if(ucSIM800RunFlag) SIM800_TX_Handler();        /*SIM800ͨѶ���ʹ�����*/
                      
        }


        if(ucFlag & 0x08)                                   /*ÿ���0.5s�¼������֧*/
        {
            if(ucMCBRunFlag   ) MCB_TX_Handler();           /*���ذ�ͨѶ���ʹ�����*/

            if(ucMoneyRunFlag ) MONEY_TX_Handler();         /*Ͷ�Ұ�ͨѶ���ʹ�����*/
        }


        if(ucFlag & 0x10)                                   /*ÿ���1s�¼������֧*/
        {
            Simulator();

            if(ucMoneyRunFlag ) MONEY_Handler();            /*Ͷ�������ش���*/

            if(ucSIM800RunFlag) GSM_UpdateCSQHandler(0);    /*���µ�ǰ�ź�ǿ��*/

            if(ucSIM800RunFlag) GSM_StartupTimeoutHandler();/*ϵͳ������ʱ����*/

            if(ucSIM800RunFlag) GSM_UpgradeProgramHandler();/*ϵͳ������������*/

            if(ucPowerDownFlag) MCB_SysPowerDownHandler();  /*��Ħ������״̬�ϵ��˿��*/
            
            ChargeHandler();
        }
        
        if(ucFlag & 0x20)                       /*ÿ���3s�¼������֧*/
        {
            MCB_StartRunCheckHandler();         /*��ⰴĦ���Ƿ�������*/
        }

        WDOG_Feed();
    }
}


