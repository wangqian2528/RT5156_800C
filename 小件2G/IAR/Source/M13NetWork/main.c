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


/*起始运行程序跳转*/
void JumpRun(void)
{
    /*V2.XX & V4.XX支持远程升级功能*/
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
                ** 1:微信远程启动
                ** 2:微信远程关闭
                ** 3:微信远程续费
                ** 4:微信APP控制命令
                ** 5:按摩椅状态查询
                ** 6:微信远程升级命令
                ** 7:微信APP查询钱币总额、总识币数
                ** 8:微信APP清除钱币总额、总识币数
                ** 9:微信APP查询费率
                **10:微信APP设置费率
                **11:微信APP查询投币箱时间
                **12:微信APP设置投币7箱时间
                **13:微信APP发送开锁命令
                **14:微信查询投币箱门状态
                **15:微信查询投币箱累计开锁次数
                **16:微信清除投币箱累计合法开锁次数
                **17:微信清除投币箱累计不合法开锁次数
                **18:远程升级
                */
                if((GSM_IsTxStepIDLE() == 0) || (gucSIM800OnlineState == 0))
                {
                    ucSimulatorTime = 2;    return;
                }

                GSM_Demo(ucSimulatorStep);
                break;

            case 2:
                /*
                ** 1:投币或者微信扫码使能按摩椅
                ** 2:按摩椅禁止工作
                ** 3:微信加时命令
                */
                MCB_Demo(ucSimulatorStep);
                break;

            case 3:
                /*
                ** 1:主机查询当前一次接收到的钱币数目(MAX:99999)
                ** 2:查询接收到的总钱币累计数目、总时间数、总识币张数(MAX:99999)
                ** 3:清零总钱币累计数目、总时间数、总识币张数
                ** 4:设置费率
                ** 5:查询费率
                ** 6:设置年月日时分秒
                ** 7:查询年月日时分秒
                ** 8:禁止投币功能，上电后默认为允许投币
                ** 9:允许投币功能
                **10:允许断电保存功能
                **11:禁止断电保存功能，上电后默认为禁止断电保存
                **12:微信扫码开锁
                **13:主机查询投币箱门锁的状态
                **14:主机查询投币箱累计开锁次数
                **15:主机清零投币箱累计合法开锁次数
                **16:主机清零投币箱累计异常开锁次数
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


        if(ucFlag & 0x01)                                   /*每间隔10ms事件处理分支*/
        {
            if(ucMCBRunFlag   ) MCB_RX_Handler();           /*主控板通讯接收处理函数*/

            if(ucMoneyRunFlag ) MONEY_RX_Handler();         /*投币板通讯接收处理函数*/

            if(ucSIM800RunFlag) SIM800_RX_Handler();        /*SIM800通讯接收处理函数*/
            
        }


        if(ucFlag & 0x02)   /*每间隔100ms事件处理分支*/
        {
            //aP89085_Handler();

            LED_Handler();
        }


        if(ucFlag & 0x04)                                   /*每间隔250ms事件处理分支*/
        {
            if(ucSIM800RunFlag) GSM_UploadOrderHandler(0);  /*主动上传订单的处理*/

            if(ucSIM800RunFlag) GSM_HeartbeatHandler(0);    /*主动上传心跳的处理*/

            if(ucSIM800RunFlag) SIM800_TX_Handler();        /*SIM800通讯发送处理函数*/
                      
        }


        if(ucFlag & 0x08)                                   /*每间隔0.5s事件处理分支*/
        {
            if(ucMCBRunFlag   ) MCB_TX_Handler();           /*主控板通讯发送处理函数*/

            if(ucMoneyRunFlag ) MONEY_TX_Handler();         /*投币板通讯发送处理函数*/
        }


        if(ucFlag & 0x10)                                   /*每间隔1s事件处理分支*/
        {
            Simulator();

            if(ucMoneyRunFlag ) MONEY_Handler();            /*投币箱的相关处理*/

            if(ucSIM800RunFlag) GSM_UpdateCSQHandler(0);    /*更新当前信号强度*/

            if(ucSIM800RunFlag) GSM_StartupTimeoutHandler();/*系统启动超时处理*/

            if(ucSIM800RunFlag) GSM_UpgradeProgramHandler();/*系统程序升级处理*/

            if(ucPowerDownFlag) MCB_SysPowerDownHandler();  /*按摩椅运行状态断电退款处理*/
            
            ChargeHandler();
        }
        
        if(ucFlag & 0x20)                       /*每间隔3s事件处理分支*/
        {
            MCB_StartRunCheckHandler();         /*检测按摩椅是否有启动*/
        }

        WDOG_Feed();
    }
}


