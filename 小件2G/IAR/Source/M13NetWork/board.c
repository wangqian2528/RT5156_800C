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

    if(GPIO_PinInGet(SIT_CHECK_PORT, SIT_CHECK_PIN))    /*没有检测到压力*/
    {
        if(guiSitCount0 > 0x00) guiSitCount0--;
        if(guiSitCount1 < 0x64) guiSitCount1++;
    }
    else                                                /*检测到压力*/
    {
        if(guiSitCount0 < 0x64) guiSitCount0++;
        if(guiSitCount1 > 0x00) guiSitCount1--;
    }

    if((guiSitCount0 - guiSitCount1) >= 60)             /*占比60%滤波*/
    {
        return 1;
    }
    else
    {
        return 0;
    }


#if 0
    /*
    **当信号干扰严重，且不稳定时，很容易出现状态震荡，永远无法确定压力状态
    */
    static uint16_t guiSitCount = 0;
    static uint16_t guiSitState = 0;

    uint16_t uiState = GPIO_PinInGet(SIT_CHECK_PORT, SIT_CHECK_PIN);

    if(uiState == 0)                /*本次有检测到压力*/
    {
        if(guiSitState == 1)        /*当前处于压力状态*/
        {
            return 1;               /*返回当前压力状态*/
        }
        else
        {
            guiSitCount++;          /*压力时间累计累加*/

            if(guiSitCount >= 50)   /*达到压力时间门限值*/
            {
                guiSitState = 1;

                return 1;           /*确认无误是有压力值*/
            }
            else
            {
                return 0;           /*压力检测处于滤波态*/
            }
        }
    }
    else                            /*本次没有检测到压力*/
    {
        if(guiSitState == 0)        /*当前处于无压力状态*/
        {
            return 0;               /*返回当前压力状态*/
        }
        else
        {
            guiSitCount--;          /*压力时间累计递减*/

            if(guiSitCount == 0)    /*达到压力时间门限值*/
            {
                guiSitState = 0;

                return 0;           /*确认无误是没有压力值*/
            }
            else
            {
                return 1;           /*压力检测处于滤波态*/
            }
        }
    }
#endif


#if 0
    /*实时的返回当前的状态压力状态*/

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


