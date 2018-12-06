#define __AP89085_C__


#include "aP89085.h"
#include "board.h"
#include "MCB.h"
#include "MCU.h"


uint8_t  gucVoicePlayStep = 0;
uint8_t  gucVoicePlayTime = 0;
uint8_t  gucVoicePlayMode = 0;  /*0->触发播放、1->主动播放*/
uint16_t guiVoicePlayStartCount = 0;


void aP89085_Delay(void)
{
    uint8_t i = 0;

    for(i = 0; i < 10; i++)
    {
        __no_operation(); __no_operation();
        __no_operation(); __no_operation();
        __no_operation(); __no_operation();
        __no_operation(); __no_operation();
        __no_operation(); __no_operation();
    }
}


void aP89085_SendByte(uint8_t ucData)
{
    uint8_t i = 0;

    for(i = 0; i < 8; i++)
    {
        if(ucData & 0x01)
        {
//            aP89085_DI_SET;
        }
        else
        {
//            aP89085_DI_CLR;
        }

//        aP89085_Delay(); aP89085_SCK_SET;

//        aP89085_Delay(); aP89085_SCK_CLR;

        ucData >>= 0x01;
    }
}


void aP89085_PowerUp(void)
{
 //   aP89085_CS_SET; aP89085_Delay();

    aP89085_SendByte(aP89085_CMD_PUP1);
    aP89085_Delay();

   // aP89085_CS_CLR; aP89085_Delay();
}


/*Power Amplifier*/
void aP89085_PAPowerUp(void)
{
    GPIO_PinOutSet(PA_POWER_CTRL_PORT, PA_POWER_CTRL_PIN);
    //GPIO_PinOutClear(PA_POWER_CTRL_PORT, PA_POWER_CTRL_PIN);
}


void aP89085_PAPowerDown(void)
{
    GPIO_PinOutClear(PA_POWER_CTRL_PORT, PA_POWER_CTRL_PIN);
    //GPIO_PinOutSet(PA_POWER_CTRL_PORT, PA_POWER_CTRL_PIN);
}


void aP89085_Play(uint8_t ucGroupAddr)
{
 //   aP89085_CS_SET; aP89085_Delay();

    aP89085_SendByte(aP89085_CMD_PREFETCH);
    aP89085_SendByte(ucGroupAddr);
    aP89085_Delay();

//    aP89085_CS_CLR; aP89085_Delay();
}


uint8_t aP89085_GetPlayState(void)
{
   /// if(GPIO_PinInGet(aP89085_BUSY_PORT, aP89085_BUSY_PIN))
   // {
   //     return 1;   /*语音芯片正处于播放的状态*/
   // }
   // else
   // {
        return 0;   /*语音芯片正处于空闲的状态*/
   // }
}


void aP89085_Handler(void)
{
    if(gucVoicePlayStep == 0)                   /*在语音不播放的状态下再进行状态切换状态*/
    {
        if((MCB_GetChairRunningFlag() == 0) && (BSP_SitDetected() == 1))
        {
            guiVoicePlayStartCount++;

            if(guiVoicePlayStartCount == 300)   /*30秒后没有扫码、投币，开始语音提示警告*/
            {
                guiVoicePlayStartCount = 0;
                gucVoicePlayStep       = aP89085_STEP_ALARM;
            }
        }
        else
        {
            guiVoicePlayStartCount = 0;
        }
    }
    else                                        /*语音在播放的过程中检测到按摩椅开始运行、或者人离开，则停止语音提示*/
    {
        guiVoicePlayStartCount = 0;

        if(gucVoicePlayMode == 0)               /*在触发方式下对其进行检测判断*/
        {
            if((MCB_GetChairRunningFlag() == 1) || (BSP_SitDetected() == 0))
            {
                gucVoicePlayStep  = 0;
                aP89085_PAPowerDown();
            }
        }
    }


    /*判断当前语音是否播放结束，如果结束则间隔时间开始累加*/
    if(aP89085_GetPlayState() == 0)
    {
        gucVoicePlayTime++;

        if(PLATFORM_CHINA == 1)
        {
            switch(gucVoicePlayStep)
            {
                case 0:
                    gucVoicePlayMode  = 0;
                    aP89085_PAPowerDown();
                    break;

                case aP89085_STEP_ALARM+0:
                    if(gucVoicePlayTime >= 1)       /*语音之间播放间隔为0秒钟*/
                    {
                        aP89085_PAPowerUp();

                        aP89085_Play(aP89085_VGA_01);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+1;
                    }
                    break;

                case aP89085_STEP_ALARM+1:
                    if(gucVoicePlayTime >= 50)      /*语音之间播放间隔为5秒钟*/
                    {
                        aP89085_Play(aP89085_VGA_02);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+2;
                    }
                    break;

                case aP89085_STEP_ALARM+2:          /*等待语音播放结束再切换到空闲*/
                    if(gucVoicePlayTime >= 1)
                    {
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = 0;
                    }
                    break;

                case aP89085_STEP_WELCOME:
                    aP89085_PAPowerUp();
                    aP89085_Play(aP89085_VGA_00);
                    gucVoicePlayTime = 0;
                    gucVoicePlayStep = 0;
                    break;

                default:
                    break;
            }
        }

        if(PLATFORM_CHINA == 2)
        {
            switch(gucVoicePlayStep)
            {
                case 0:
                    gucVoicePlayMode  = 0;
                    aP89085_PAPowerDown();
                    break;

                case aP89085_STEP_ALARM+0:
                    if(gucVoicePlayTime >= 1)       /*语音之间播放间隔为0秒钟*/
                    {
                        aP89085_PAPowerUp();

                        aP89085_Play(aP89085_VGA_06);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+1;
                    }
                    break;

                case aP89085_STEP_ALARM+1:
                    if(gucVoicePlayTime >= 100)     /*语音之间播放间隔为10秒钟*/
                    {
                        aP89085_Play(aP89085_VGA_06);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+2;
                    }
                    break;

                case aP89085_STEP_ALARM+2:
                    if(gucVoicePlayTime >= 100)     /*语音之间播放间隔为10秒钟*/
                    {
                        aP89085_Play(aP89085_VGA_06);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+3;
                    }
                    break;

                case aP89085_STEP_ALARM+3:
                    if(gucVoicePlayTime >= 100)     /*语音之间播放间隔为10秒钟*/
                    {
                        aP89085_Play(aP89085_VGA_06);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+4;
                    }
                    break;

                case aP89085_STEP_ALARM+4:
                    if(gucVoicePlayTime >= 200)     /*语音之间播放间隔为20秒钟*/
                    {
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+5;
                    }
                    break;

                case aP89085_STEP_ALARM+5:
                    if(gucVoicePlayTime >= 1)       /*语音之间播放间隔为0秒钟*/
                    {
                        aP89085_Play(aP89085_VGA_07);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+6;
                    }
                    break;

                case aP89085_STEP_ALARM+6:
                    if(gucVoicePlayTime >= 20)      /*语音之间播放间隔为2秒钟*/
                    {
                        aP89085_Play(aP89085_VGA_08);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+7;
                    }
                    break;

                case aP89085_STEP_ALARM+7:
                    if(gucVoicePlayTime >= 1)       /*语音之间播放间隔为0秒钟*/
                    {
                        aP89085_Play(aP89085_VGA_08);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+8;
                    }
                    break;

                case aP89085_STEP_ALARM+8:
                    if(gucVoicePlayTime >= 1)       /*语音之间播放间隔为0秒钟*/
                    {
                        aP89085_Play(aP89085_VGA_08);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+9;
                    }
                    break;

                case aP89085_STEP_ALARM+9:
                    if(gucVoicePlayTime >= 50)      /*语音之间播放间隔为5秒钟*/
                    {
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+5;
                    }
                    break;

                default:
                    break;
            }
        }

        if(PLATFORM_DUBAI == 1)     /*语音之间播放无间隔*/
        {
            switch(gucVoicePlayStep)
            {
                case 0:
                    gucVoicePlayMode  = 0;
                    aP89085_PAPowerDown();
                    break;
        
                case aP89085_STEP_ALARM+0:
                    if(gucVoicePlayTime >= 1)       /*语音之间播放间隔为0秒钟*/
                    {
                        aP89085_PAPowerUp();

                        aP89085_Play(aP89085_VGA_03);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+1;
                    }
                    break;

                case aP89085_STEP_ALARM+1:
                    if(gucVoicePlayTime >= 1)
                    {
                        aP89085_Play(aP89085_VGA_03);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+2;
                    }
                    break;

                case aP89085_STEP_ALARM+2:
                    if(gucVoicePlayTime >= 1)
                    {
                        aP89085_Play(aP89085_VGA_05);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+3;
                    }
                    break;

                case aP89085_STEP_ALARM+3:
                    if(gucVoicePlayTime >= 1)
                    {
                        aP89085_Play(aP89085_VGA_05);
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM+4;
                    }
                    break;

                case aP89085_STEP_ALARM+4:          /*循环间隔为14秒，一个周期为40秒*/
                    if(gucVoicePlayTime == 140)
                    {
                        gucVoicePlayTime = 0;
                        gucVoicePlayStep = aP89085_STEP_ALARM;
                    }
                    break;
        
                default:
                    break;
            }
        }
    }
    else
    {
        gucVoicePlayTime = 0;
    }
}


void aP89085_CtrlTest(void)
{
  //  GPIO_PinOutSet(aP89085_CS_PORT,  aP89085_CS_PIN);
  //  GPIO_PinOutSet(aP89085_SCK_PORT, aP89085_SCK_PIN);
  //  GPIO_PinOutSet(aP89085_DI_PORT,  aP89085_DI_PIN);

   // GPIO_PinOutClear(aP89085_CS_PORT,  aP89085_CS_PIN);
   // GPIO_PinOutClear(aP89085_SCK_PORT, aP89085_SCK_PIN);
   // GPIO_PinOutClear(aP89085_DI_PORT,  aP89085_DI_PIN);
}


void InitAP89085(void)
{
    /*aP89085*/
  //  GPIO_PinModeSet(aP89085_CS_PORT,   aP89085_CS_PIN,   aP89085_CS_MODE,   0);
   // GPIO_PinModeSet(aP89085_SCK_PORT,  aP89085_SCK_PIN,  aP89085_SCK_MODE,  0);
   // GPIO_PinModeSet(aP89085_DI_PORT,   aP89085_DI_PIN,   aP89085_DI_MODE,   0);
   // GPIO_PinModeSet(aP89085_BUSY_PORT, aP89085_BUSY_PIN, aP89085_BUSY_MODE, 1);
   // GPIO_PinModeSet(aP89085_FULL_PORT, aP89085_FULL_PIN, aP89085_FULL_MODE, 1);

    /*Sit Detect*/
  //  GPIO_PinModeSet(SIT_CHECK_PORT,    SIT_CHECK_PIN,    SIT_CHECK_MODE,    1);

    /*Power Amplifier*/
   // GPIO_PinModeSet(PA_POWER_CTRL_PORT,PA_POWER_CTRL_PIN,PA_POWER_CTRL_MODE,1);

  //  aP89085_Delay();

   // aP89085_CtrlTest();

   // aP89085_PowerUp();

    /*给功放芯片供电*/
   // aP89085_PAPowerUp();
}


void aP89085_Demo(uint8_t index)
{
    switch(index)
    {
        case  1: aP89085_Play(aP89085_VGA_00); break;   /*摩摩哒感谢您的体验*/
        case  2: aP89085_Play(aP89085_VGA_01); break;   /*请扫码支付，谢谢*/
        case  3: aP89085_Play(aP89085_VGA_02); break;   /*请投币检验，谢谢*/
        case  4: aP89085_Play(aP89085_VGA_03); break;   /*英语提示*/
        case  5: aP89085_Play(aP89085_VGA_04); break;
        case  6: aP89085_Play(aP89085_VGA_05); break;
        default: break;
    }
}


