#define __BOARD_C__


#include "AES.h"
#include "aP89085.h"
#include "Base64.h"
#include "Bluetooth.h"
#include "board.h"
#include "flash.h"
#include "LED.h"
#include "MCB.h"
#include "money.h"
#include "SIM800.h"


void InitBoard(void)
{
    InitAES();

    InitBASE64();

    InitFLASH();

    InitLED();
    
    //InitAP89085();

    BT_Init();

    InitMCB();

    InitMoney();
    
    InitSIM800();
}


uint8_t BSP_SitDetected(void)
{
    static uint16_t guiSitCount0 = 0;
    static uint16_t guiSitCount1 = 0;

    if(GPIO_PinInGet(SIT_CHECK_PORT, SIT_CHECK_PIN))    /*û�м�⵽ѹ��*/
    {
        if(guiSitCount0 > 0x00) guiSitCount0--;
        if(guiSitCount1 < 0x64) guiSitCount1++;
    }
    else                                                /*��⵽ѹ��*/
    {
        if(guiSitCount0 < 0x64) guiSitCount0++;
        if(guiSitCount1 > 0x00) guiSitCount1--;
    }

    if((guiSitCount0 - guiSitCount1) >= 60)             /*ռ��60%�˲�*/
    {
        return 1;
    }
    else
    {
        return 0;
    }


#if 0
    /*
    **���źŸ������أ��Ҳ��ȶ�ʱ�������׳���״̬�𵴣���Զ�޷�ȷ��ѹ��״̬
    */
    static uint16_t guiSitCount = 0;
    static uint16_t guiSitState = 0;

    uint16_t uiState = GPIO_PinInGet(SIT_CHECK_PORT, SIT_CHECK_PIN);

    if(uiState == 0)                /*�����м�⵽ѹ��*/
    {
        if(guiSitState == 1)        /*��ǰ����ѹ��״̬*/
        {
            return 1;               /*���ص�ǰѹ��״̬*/
        }
        else
        {
            guiSitCount++;          /*ѹ��ʱ���ۼ��ۼ�*/

            if(guiSitCount >= 50)   /*�ﵽѹ��ʱ������ֵ*/
            {
                guiSitState = 1;

                return 1;           /*ȷ����������ѹ��ֵ*/
            }
            else
            {
                return 0;           /*ѹ����⴦���˲�̬*/
            }
        }
    }
    else                            /*����û�м�⵽ѹ��*/
    {
        if(guiSitState == 0)        /*��ǰ������ѹ��״̬*/
        {
            return 0;               /*���ص�ǰѹ��״̬*/
        }
        else
        {
            guiSitCount--;          /*ѹ��ʱ���ۼƵݼ�*/

            if(guiSitCount == 0)    /*�ﵽѹ��ʱ������ֵ*/
            {
                guiSitState = 0;

                return 0;           /*ȷ��������û��ѹ��ֵ*/
            }
            else
            {
                return 1;           /*ѹ����⴦���˲�̬*/
            }
        }
    }
#endif


#if 0
    /*ʵʱ�ķ��ص�ǰ��״̬ѹ��״̬*/

    if(GPIO_PinInGet(SIT_CHECK_PORT, SIT_CHECK_PIN))
    {
        return 0;   /*Don't Detected*/
    }
    else
    {
        return 1;   /*Detected*/
    }
#endif
}


