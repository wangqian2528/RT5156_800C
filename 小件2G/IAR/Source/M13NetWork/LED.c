#define __LED_C__


#include "LED.h"


extern uint8_t gucFTPUpgradeState;
extern uint8_t gucSIM800ConnectFlag;
extern uint8_t gucSIM800OnlineState;


uint8_t gucLEDBlinkInterval = 0;


void InitLED(void)
{
    GPIO_PinModeSet(INDICATE_LED_PORT, INDICATE_LED_PIN, INDICATE_LED_MODE, 0);
}


void LED_Handler(void)
{
    if(gucFTPUpgradeState != 1)
    {
        gucLEDBlinkInterval++;

        if((gucSIM800ConnectFlag == 1) && (gucSIM800OnlineState == 1))
        {
            if(gucLEDBlinkInterval >= 1)
            {
                IndicateLEDToggle; gucLEDBlinkInterval = 0;
            }
        }
        else if((gucSIM800ConnectFlag == 1) && (gucSIM800OnlineState == 0))
        {
            if(gucLEDBlinkInterval >= 5)
            {
                IndicateLEDToggle; gucLEDBlinkInterval = 0;
            }
        }
        else if((gucSIM800ConnectFlag == 0) && (gucSIM800OnlineState == 0))
        {
            if(gucLEDBlinkInterval >= 10)
            {
                IndicateLEDToggle; gucLEDBlinkInterval = 0;
            }
        }
        else
        {
            gucLEDBlinkInterval = 0;
        }
    }
    else
    {
        IndicateLEDON;

        gucLEDBlinkInterval = 0;
    }
}


