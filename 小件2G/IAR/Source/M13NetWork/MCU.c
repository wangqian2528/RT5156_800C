#define __MCU_C__


#include "MCU.h"


volatile uint8_t  gucSysTickFlag  = 0x00;
volatile uint32_t gulSysTickCount = 0;
volatile uint32_t gulSysTimeDelay = 0;


QUEUE_Init_TypeDef COM_QUEUE[QUEUE_NUMBER];


void InitGPIO(void)
{
}


void InitSysTick(void)
{
    /*System Tick : 1ms*/
    SysTick_Config(SystemCoreClock / 1000);
}


void InitWatchDog(void)
{
    WDOG_Init_TypeDef WDOG_Init_Structure;

    WDOG_Init_Structure.enable     = true;
    WDOG_Init_Structure.debugRun   = false;
    WDOG_Init_Structure.em2Run     = true;
    WDOG_Init_Structure.em3Run     = true;
    WDOG_Init_Structure.em4Block   = false;
    WDOG_Init_Structure.swoscBlock = false;
    WDOG_Init_Structure.clkSel     = wdogClkSelULFRCO;
    WDOG_Init_Structure.perSel     = wdogPeriod_2k;

    WDOG_Init(&WDOG_Init_Structure);
}


void InitLEUART0(void)  /*投币板通讯串口参数配置*/
{
    LEUART_Init_TypeDef LEUART_Init_Structure;

    LEUART_Reset(LEUART0);

    /*LEUART1 GPIO Configuration*/
    GPIO_PinModeSet(PUT_COIN_RX_PORT, PUT_COIN_RX_PIN, PUT_COIN_RX_MODE, 1);
    GPIO_PinModeSet(PUT_COIN_TX_PORT, PUT_COIN_TX_PIN, PUT_COIN_TX_MODE, 1);

    LEUART_Init_Structure.enable   = leuartEnable;
    LEUART_Init_Structure.refFreq  = 0;
    LEUART_Init_Structure.baudrate = 9600;
    LEUART_Init_Structure.databits = leuartDatabits8;
    LEUART_Init_Structure.parity   = leuartNoParity;
    LEUART_Init_Structure.stopbits = leuartStopbits1;

    LEUART_Init(LEUART0, &LEUART_Init_Structure);

    LEUART0->ROUTE = LEUART_ROUTE_TXPEN | LEUART_ROUTE_RXPEN | LEUART_ROUTE_LOCATION_LOC0;

    /*Enable LEUART0 interrupt vector*/
    NVIC_EnableIRQ(LEUART0_IRQn);

    /*Enable LEUART0 Interrupt*/
    LEUART_IntEnable(LEUART0, LEUART_IEN_RXDATAV);
}


void InitLEUART1(void)  /*主控板通讯串口参数配置*/
{
    LEUART_Init_TypeDef LEUART_Init_Structure;

    LEUART_Reset(LEUART1);

    /*LEUART1 GPIO Configuration*/
    GPIO_PinModeSet(MB_CTRL_RXD_PORT, MB_CTRL_RXD_PIN, MB_CTRL_RXD_MODE, 1);
    GPIO_PinModeSet(MB_CTRL_TXD_PORT, MB_CTRL_TXD_PIN, MB_CTRL_TXD_MODE, 1);

    LEUART_Init_Structure.enable   = leuartEnable;
    LEUART_Init_Structure.refFreq  = 0;
    LEUART_Init_Structure.baudrate = 9600;
    LEUART_Init_Structure.databits = leuartDatabits8;
    LEUART_Init_Structure.parity   = leuartNoParity;
    LEUART_Init_Structure.stopbits = leuartStopbits1;

    LEUART_Init(LEUART1, &LEUART_Init_Structure);

    LEUART1->ROUTE = LEUART_ROUTE_TXPEN | LEUART_ROUTE_RXPEN | LEUART_ROUTE_LOCATION_LOC1;

    /*Enable LEUART0 interrupt vector*/
    NVIC_EnableIRQ(LEUART1_IRQn);

    /*Enable LEUART0 Interrupt*/
    LEUART_IntEnable(LEUART1, LEUART_IEN_RXDATAV);
}


void InitUSART0(void)   /*SIM800通讯串口参数配置*/
{
    USART_InitAsync_TypeDef USART_InitAsync_Structure;

    USART_Reset(USART0);

    /*Configure GPIO pins*/
    GPIO_PinModeSet(SIM800_RXD_PORT, SIM800_RXD_PIN, SIM800_RXD_MODE, 0);
    GPIO_PinModeSet(SIM800_TXD_PORT, SIM800_TXD_PIN, SIM800_TXD_MODE, 1);

    /*Prepare struct for initializing UART in asynchronous mode*/
    USART_InitAsync_Structure.enable       = usartDisable;
    USART_InitAsync_Structure.refFreq      = 0;
    USART_InitAsync_Structure.baudrate     = 115200;
    USART_InitAsync_Structure.oversampling = usartOVS16;
    USART_InitAsync_Structure.databits     = usartDatabits8;
    USART_InitAsync_Structure.parity       = usartNoParity;
    USART_InitAsync_Structure.stopbits     = usartStopbits1;

    /*Initialize USART with uartInit struct*/
    USART_InitAsync(USART0, &USART_InitAsync_Structure);

    /*Enable I/O pins at USART0 location #1*/
    USART0->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_LOCATION_LOC1;

    /*Prepare USART Rx and Tx interrupts*/
    USART_IntClear( USART0, _UART_IF_MASK);
    USART_IntEnable(USART0,  UART_IF_RXDATAV);

    NVIC_ClearPendingIRQ(USART0_RX_IRQn);
    NVIC_ClearPendingIRQ(USART0_TX_IRQn);

    NVIC_EnableIRQ(USART0_RX_IRQn);
    NVIC_EnableIRQ(USART0_TX_IRQn);

    /*Enable USART*/
    USART_Enable(USART0, usartEnable);
}


void InitUSART2(uint8_t ucMode)   /*BT通讯串口参数配置*/
{
    USART_InitAsync_TypeDef USART_InitAsync_Structure;

    USART_Reset(USART2);

    /*Configure GPIO pins*/
    GPIO_PinModeSet(BT_RXD_PORT, BT_RXD_PIN, BT_RXD_MODE, 0);
    GPIO_PinModeSet(BT_TXD_PORT, BT_TXD_PIN, BT_TXD_MODE, 1);

    /*Prepare struct for initializing UART in asynchronous mode*/
    USART_InitAsync_Structure.enable       = usartDisable;
    USART_InitAsync_Structure.refFreq      = 0;

    if(ucMode == 0)
    {
        USART_InitAsync_Structure.baudrate = 9600;
    }
    else
    {
        USART_InitAsync_Structure.baudrate = 115200;
    }

    USART_InitAsync_Structure.oversampling = usartOVS16;
    USART_InitAsync_Structure.databits     = usartDatabits8;
    USART_InitAsync_Structure.parity       = usartNoParity;
    USART_InitAsync_Structure.stopbits     = usartStopbits1;

    /*Initialize USART with uartInit struct*/
    USART_InitAsync(USART2, &USART_InitAsync_Structure);

    /*Enable I/O pins at USART2 location #0*/
    USART2->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_LOCATION_LOC0;

    /*Prepare USART Rx and Tx interrupts*/
    USART_IntClear( USART2, _UART_IF_MASK);
    USART_IntEnable(USART2,  UART_IF_RXDATAV);

    NVIC_ClearPendingIRQ(USART2_RX_IRQn);
    NVIC_ClearPendingIRQ(USART2_TX_IRQn);

    NVIC_EnableIRQ(USART2_RX_IRQn);
    NVIC_EnableIRQ(USART2_TX_IRQn);

    /*Enable USART*/
    USART_Enable(USART2, usartEnable);
}


void InitMCU(void)
{
    uint8_t i = 0;

    __disable_irq();

    CHIP_Init();

    /*Main System Clock : Internal RC 28MHz*/
    CMU_HFRCOBandSet(cmuHFRCOBand_28MHz);

    SystemCoreClockUpdate();

    /*Disable 32.768Hz Low Frequency RC Oscillator(LFRCO)*/
    CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);

    /*Enable 1-28 MHz High Frequency RC Oscillator(HFRCO)*/
    CMU_ClockEnable(cmuClock_HFPER, true);

    /*Select : 32.768Hz Low Frequency Crystal Oscillator(LFXO)*/
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
    CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);

    CMU_ClockEnable(cmuClock_CORELE,  true);    /*Enable CORELE  Clock*/
    CMU_ClockEnable(cmuClock_GPIO,    true);    /*Enable GPIO    Clock*/
    CMU_ClockEnable(cmuClock_LEUART0, true);    /*Enable LEUART0 Clock*/
    CMU_ClockEnable(cmuClock_LEUART1, true);    /*Enable LEUART1 Clock*/
    CMU_ClockEnable(cmuClock_USART0,  true);    /*Enable USART0  Clock*/
    CMU_ClockEnable(cmuClock_USART2,  true);    /*Enable USART0  Clock*/

    InitWatchDog();

    InitGPIO();

    InitSysTick();  /*系统滴嗒时钟: 1ms*/

    InitLEUART0();  /*初始化与投币板通讯的串口配置*/

    InitLEUART1();  /*初始化与主控板通讯的串口配置*/

    InitUSART0();   /*初始化与SIM800通讯的串口配置*/

    InitUSART2(0);

    for(i = 0; i < QUEUE_NUMBER; i++)
    {
        QUEUE_INIT(i);
    }

    __enable_irq();
}


void LEUART0_IRQHandler(void)   /*投币板通讯串口中断处理函数*/
{
    uint32_t ulIntFlag = 0;

    ulIntFlag = LEUART_IntGet(LEUART0);

    LEUART_IntClear(LEUART0, ulIntFlag);

    if(ulIntFlag & LEUART_IF_RXDATAV)
    {
        COM_QUEUE[MONIES_QUEUE_RX_IDX].data[COM_QUEUE[MONIES_QUEUE_RX_IDX].tail++] = LEUART0->RXDATA;
        COM_QUEUE[MONIES_QUEUE_RX_IDX].tail %= QUEUE_SIZE;
    }
}


void LEUART1_IRQHandler(void)   /*主控板通讯串口中断处理函数*/
{
    uint32_t ulIntFlag = 0;

    ulIntFlag = LEUART_IntGet(LEUART1);

    LEUART_IntClear(LEUART1, ulIntFlag);

    if(ulIntFlag & LEUART_IF_RXDATAV)
    {
        COM_QUEUE[MBCTRL_QUEUE_RX_IDX].data[COM_QUEUE[MBCTRL_QUEUE_RX_IDX].tail++] = LEUART1->RXDATA;
        COM_QUEUE[MBCTRL_QUEUE_RX_IDX].tail %= QUEUE_SIZE;
    }
}


void SysTick_Handler(void)
{
    gulSysTickCount = (gulSysTickCount + 1) % 10000;

    if((gulSysTickCount % 10  ) == 0) gucSysTickFlag |= 0x01;
    if((gulSysTickCount % 100 ) == 0) gucSysTickFlag |= 0x02;
    if((gulSysTickCount % 250 ) == 0) gucSysTickFlag |= 0x04;
    if((gulSysTickCount % 500 ) == 0) gucSysTickFlag |= 0x08;
    if((gulSysTickCount % 1000) == 0) gucSysTickFlag |= 0x10;
    if((gulSysTickCount % 3000) == 0) gucSysTickFlag |= 0x20;

    if(gulSysTimeDelay != 0) gulSysTimeDelay--;
}


void USART0_RX_IRQHandler(void)
{
    if(USART0->STATUS & UART_STATUS_RXDATAV)
    {
        COM_QUEUE[SIM800_QUEUE_RX_IDX].data[COM_QUEUE[SIM800_QUEUE_RX_IDX].tail++] = USART_Rx(USART0);
        COM_QUEUE[SIM800_QUEUE_RX_IDX].tail %= QUEUE_SIZE;

        /*Clear RXDATAV interrupt*/
        USART_IntClear(USART0, UART_IF_RXDATAV);
    }
}


void USART0_TX_IRQHandler(void)
{
    USART_IntGet(USART0);

    if(USART0->STATUS & UART_STATUS_TXBL)
    {
        if(QUEUE_EMPTY(SIM800_QUEUE_TX_IDX) == 0)
        {
            USART_Tx(USART0, QUEUE_READ(SIM800_QUEUE_TX_IDX));
        }
        else
        {
            USART_IntDisable(USART0, UART_IF_TXBL);
        }
    }
}


void USART2_RX_IRQHandler(void)
{
    if(USART2->STATUS & UART_STATUS_RXDATAV)
    {
        COM_QUEUE[BT_QUEUE_RX_IDX].data[COM_QUEUE[BT_QUEUE_RX_IDX].tail++] = USART_Rx(USART2);
        COM_QUEUE[BT_QUEUE_RX_IDX].tail %= QUEUE_SIZE;

        /*Clear RXDATAV interrupt*/
        USART_IntClear(USART2, UART_IF_RXDATAV);
    }
}


void USART2_TX_IRQHandler(void)
{
    USART_IntGet(USART2);

    if(USART2->STATUS & UART_STATUS_TXBL)
    {
        if(QUEUE_EMPTY(BT_QUEUE_TX_IDX) == 0)
        {
            USART_Tx(USART2, QUEUE_READ(BT_QUEUE_TX_IDX));
        }
        else
        {
            USART_IntDisable(USART2, UART_IF_TXBL);
        }
    }
}


uint8_t GetSystemRunFlag(void)
{
    uint8_t ucFlag = 0x00;

    __disable_irq();        ucFlag = gucSysTickFlag;
    gucSysTickFlag = 0x00;  __enable_irq();

    return ucFlag;
}


void delay_ms(uint32_t count)
{
    gulSysTimeDelay = count * DELAY_LEVEL_MS;

    while(gulSysTimeDelay > 0) WDOG_Feed();
}


void QUEUE_INIT(uint8_t index)
{
    uint16_t i = 0;

    for(i = 0; i < QUEUE_SIZE; i++)
    {
        COM_QUEUE[index].data[i] = 0x00;
    }

    COM_QUEUE[index].head = 0;
    COM_QUEUE[index].tail = 0;
}


uint8_t QUEUE_EMPTY(uint8_t index)
{
    if(COM_QUEUE[index].head == COM_QUEUE[index].tail)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}


uint8_t QUEUE_READ(uint8_t index)
{
    uint8_t ucData = COM_QUEUE[index].data[COM_QUEUE[index].head++];

    COM_QUEUE[index].head %= QUEUE_SIZE;

    return ucData;
}


void QUEUE_WRITE(uint8_t index, uint8_t value)
{
    COM_QUEUE[index].data[COM_QUEUE[index].tail++] = value;
    COM_QUEUE[index].tail %= QUEUE_SIZE;
}


