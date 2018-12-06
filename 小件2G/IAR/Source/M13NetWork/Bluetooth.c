#define __BLUETOOTH_C__


#include "Bluetooth.h"
#include "MCU.h"

uint8_t gucBTData[200];           
uint8_t gucBTSize         = 0; 


void BT_SendPacket(uint8_t *pData, uint8_t length)
{
    uint8_t i = 0;

    /*判断SIM800的发送消息队列是否为空*/
    if(QUEUE_EMPTY(BT_QUEUE_TX_IDX) == 0)
    {
        return;
    }

    /*将即将要发送的数据或命令复制到SIM800的发送消息队列中*/
    for(i = 0; i < length; i++)
    {
        QUEUE_WRITE(BT_QUEUE_TX_IDX, pData[i]);
    }

    /*启动发送*/
    USART_IntEnable(USART2, UART_IF_TXBL);
}


void BT_TX_Process(uint8_t index)
{
    uint8_t  i = 0, buffer[100];

    uint8_t  HEX_TABLE[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    uint32_t ulChipUnique0 = *((uint32_t *)(0x0FE081F0));
    uint32_t ulChipUnique1 = *((uint32_t *)(0x0FE081F4));

    switch(index)
    {
        case BT_AT_IDX_RSET: for(i = 0; i < sizeof(BT_AT_CMD_RSET) - 1; i++) buffer[i] = BT_AT_CMD_RSET[i]; break;
        case BT_AT_IDX_CONB: for(i = 0; i < sizeof(BT_AT_CMD_CONB) - 1; i++) buffer[i] = BT_AT_CMD_CONB[i]; break;
        case BT_AT_IDX_REST: for(i = 0; i < sizeof(BT_AT_CMD_REST) - 1; i++) buffer[i] = BT_AT_CMD_REST[i]; break;

        case BT_AT_IDX_GCTO: for(i = 0; i < sizeof(BT_AT_CMD_GCTO) - 1; i++) buffer[i] = BT_AT_CMD_GCTO[i]; break;
        case BT_AT_IDX_VRES: for(i = 0; i < sizeof(BT_AT_CMD_VRES) - 1; i++) buffer[i] = BT_AT_CMD_VRES[i]; break;
        case BT_AT_IDX_GADD: for(i = 0; i < sizeof(BT_AT_CMD_GADD) - 1; i++) buffer[i] = BT_AT_CMD_GADD[i]; break;
        case BT_AT_IDX_GNAM: for(i = 0; i < sizeof(BT_AT_CMD_GNAM) - 1; i++) buffer[i] = BT_AT_CMD_GNAM[i]; break;
        case BT_AT_IDX_GCMA: for(i = 0; i < sizeof(BT_AT_CMD_GCMA) - 1; i++) buffer[i] = BT_AT_CMD_GCMA[i]; break;
        case BT_AT_IDX_GPWR: for(i = 0; i < sizeof(BT_AT_CMD_GPWR) - 1; i++) buffer[i] = BT_AT_CMD_GPWR[i]; break;
        case BT_AT_IDX_GSLA: for(i = 0; i < sizeof(BT_AT_CMD_GSLA) - 1; i++) buffer[i] = BT_AT_CMD_GSLA[i]; break;
        case BT_AT_IDX_GCMI: for(i = 0; i < sizeof(BT_AT_CMD_GCMI) - 1; i++) buffer[i] = BT_AT_CMD_GCMI[i]; break;
        case BT_AT_IDX_GURT: for(i = 0; i < sizeof(BT_AT_CMD_GURT) - 1; i++) buffer[i] = BT_AT_CMD_GURT[i]; break;
        case BT_AT_IDX_GAVI: for(i = 0; i < sizeof(BT_AT_CMD_GAVI) - 1; i++) buffer[i] = BT_AT_CMD_GAVI[i]; break;
        case BT_AT_IDX_GPAC: for(i = 0; i < sizeof(BT_AT_CMD_GPAC) - 1; i++) buffer[i] = BT_AT_CMD_GPAC[i]; break;
        case BT_AT_IDX_GPAE: for(i = 0; i < sizeof(BT_AT_CMD_GPAE) - 1; i++) buffer[i] = BT_AT_CMD_GPAE[i]; break;

        case BT_AT_IDX_UART: for(i = 0; i < sizeof(BT_AT_CMD_UART) - 1; i++) buffer[i] = BT_AT_CMD_UART[i];
            buffer[i++] = '1';
            buffer[i++] = '1';
            buffer[i++] = '5';
            buffer[i++] = '2';
            buffer[i++] = '0';
            buffer[i++] = '0';
            break;

        case BT_AT_IDX_SNAM: for(i = 0; i < sizeof(BT_AT_CMD_SNAM) - 1; i++) buffer[i] = BT_AT_CMD_SNAM[i];
            buffer[i++] = 'S';
            buffer[i++] = 'X';
            buffer[i++] = 'B';
            buffer[i++] = 'T';

            buffer[i++] = HEX_TABLE[(ulChipUnique1 >> 0x1C) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique1 >> 0x18) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique1 >> 0x14) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique1 >> 0x10) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique1 >> 0x0C) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique1 >> 0x08) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique1 >> 0x04) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique1 >> 0x00) & 0x0000000F];

            buffer[i++] = HEX_TABLE[(ulChipUnique0 >> 0x1C) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique0 >> 0x18) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique0 >> 0x14) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique0 >> 0x10) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique0 >> 0x0C) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique0 >> 0x08) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique0 >> 0x04) & 0x0000000F];
            buffer[i++] = HEX_TABLE[(ulChipUnique0 >> 0x00) & 0x0000000F];
            break;

        case BT_AT_IDX_SCMA: for(i = 0; i < sizeof(BT_AT_CMD_SCMA) - 1; i++) buffer[i] = BT_AT_CMD_SCMA[i]; break;
        case BT_AT_IDX_SPWR: for(i = 0; i < sizeof(BT_AT_CMD_SPWR) - 1; i++) buffer[i] = BT_AT_CMD_SPWR[i]; break;
        case BT_AT_IDX_SCMI: for(i = 0; i < sizeof(BT_AT_CMD_SCMI) - 1; i++) buffer[i] = BT_AT_CMD_SCMI[i]; break;
        case BT_AT_IDX_SPAC: for(i = 0; i < sizeof(BT_AT_CMD_SPAC) - 1; i++) buffer[i] = BT_AT_CMD_SPAC[i]; break;
        case BT_AT_IDX_SPAE: for(i = 0; i < sizeof(BT_AT_CMD_SPAE) - 1; i++) buffer[i] = BT_AT_CMD_SPAE[i]; break;
        case BT_AT_IDX_SSLA: for(i = 0; i < sizeof(BT_AT_CMD_SSLA) - 1; i++) buffer[i] = BT_AT_CMD_SSLA[i]; break;
        case BT_AT_IDX_SCTO: for(i = 0; i < sizeof(BT_AT_CMD_SCTO) - 1; i++) buffer[i] = BT_AT_CMD_SCTO[i]; break;
        case BT_AT_IDX_SAVI: for(i = 0; i < sizeof(BT_AT_CMD_SAVI) - 1; i++) buffer[i] = BT_AT_CMD_SAVI[i]; break;
        case BT_AT_IDX_ENLP: for(i = 0; i < sizeof(BT_AT_CMD_ENLP) - 1; i++) buffer[i] = BT_AT_CMD_ENLP[i]; break;
        case BT_AT_IDX_NOLP: for(i = 0; i < sizeof(BT_AT_CMD_NOLP) - 1; i++) buffer[i] = BT_AT_CMD_NOLP[i]; break;
        case BT_AT_IDX_ELED: for(i = 0; i < sizeof(BT_AT_CMD_ELED) - 1; i++) buffer[i] = BT_AT_CMD_ELED[i]; break;
        case BT_AT_IDX_DLED: for(i = 0; i < sizeof(BT_AT_CMD_DLED) - 1; i++) buffer[i] = BT_AT_CMD_DLED[i]; break;
        case BT_AT_IDX_ESLP: for(i = 0; i < sizeof(BT_AT_CMD_ESLP) - 1; i++) buffer[i] = BT_AT_CMD_ESLP[i]; break;
        case BT_AT_IDX_DSLP: for(i = 0; i < sizeof(BT_AT_CMD_DSLP) - 1; i++) buffer[i] = BT_AT_CMD_DSLP[i]; break;

        default: break;
    }

    buffer[i++] = BT_AT_CMD_END_CR;
    buffer[i++] = BT_AT_CMD_END_LF;

    BT_SendPacket(buffer, i);
}


void BT_RX_Handler(void)
{
    uint8_t ucData = 0;
    
    /*判断SIM800的接收消息队列是否为空*/
    if(QUEUE_EMPTY(BT_QUEUE_RX_IDX) == 0)
    {
        gucBTData[gucBTSize++] = ucData;
    }
}

void BT_TX_Handler(void)
{
  
}


void BT_Init(void)
{
    /*初始化蓝牙连接状态检测*/
    GPIO_PinModeSet(SIT_CHECK_PORT, SIT_CHECK_PIN, SIT_CHECK_MODE, 1);

    /*修改蓝牙的串口通讯速率*/
    BT_TX_Process(BT_AT_IDX_UART);  delay_ms(200 * DELAY_LEVEL_MS);

    /*重新配置串口参数*/
    InitUSART2(1);                  delay_ms(200 * DELAY_LEVEL_MS);

    /*蓝牙LED灯使能，连接状态判断*/
    BT_TX_Process(BT_AT_IDX_ELED);  delay_ms(200 * DELAY_LEVEL_MS);

    /*设置蓝牙模块名称*/
    BT_TX_Process(BT_AT_IDX_SNAM);  delay_ms(200 * DELAY_LEVEL_MS);

    /*设置名称后，模块需要复位*/
    BT_TX_Process(BT_AT_IDX_REST);  delay_ms(200 * DELAY_LEVEL_MS);
}


uint8_t BT_CheckConnectionState(void)
{
    if(GPIO_PinInGet(SIT_CHECK_PORT, SIT_CHECK_PIN))
    {
        return 1;   /*Connected*/
    }
    else
    {
        return 0;   /*Don't Connected*/
    }
}


