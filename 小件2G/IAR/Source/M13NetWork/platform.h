#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "typedef.h"

#include "em_aes.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_leuart.h"
#include "em_msc.h"
#include "em_usart.h"
#include "em_wdog.h"


#if 0
LEUART0--->MONEY
LEUART1--->MCB
USART0---->SIM800
#endif


/*Use Range*/
#define PLATFORM_CHINA      1   /*中国，1->6900W，2->6037*/
#define PLATFORM_DUBAI      0   /*迪拜*/
#define PLATFORM_BRAZIL     0   /*巴西*/
#define PLATFROM_RUSSIA     0   /*俄罗斯*/


/*软件版本宏定义*/
#define FW_VERSION "0020"

/**Major version  of FW.*/
#define FW_VERSION_MAJOR 0

/**Minor version  of FW.*/
#define FW_VERSION_MINOR 0

/**Patch revision of FW.*/
#define FW_VERSION_PATCH 1


/*Board GPIO Function*/
//#define aP89085_CS_PORT         gpioPortA
//#define aP89085_CS_PIN          0   /*PA0*/
//#define aP89085_CS_MODE         gpioModePushPull

//#define aP89085_SCK_PORT        gpioPortA
//#define aP89085_SCK_PIN         1   /*PA1*/
//#define aP89085_SCK_MODE        gpioModePushPull

//#define aP89085_DI_PORT         gpioPortA
//#define aP89085_DI_PIN          2   /*PA2*/
//#define aP89085_DI_MODE         gpioModePushPull

//#define aP89085_BUSY_PORT       gpioPortA
//#define aP89085_BUSY_PIN        3   /*PA3*/
//#define aP89085_BUSY_MODE       gpioModeInputPullFilter

//#define aP89085_FULL_PORT       gpioPortA
//#define aP89085_FULL_PIN        4   /*PA4*/
//#define aP89085_FULL_MODE       gpioModeInputPullFilter

#define MB_CTRL_TXD_PORT        gpioPortA  
#define MB_CTRL_TXD_PIN         5   /*PA5*/
#define MB_CTRL_TXD_MODE        gpioModePushPull

#define MB_CTRL_RXD_PORT        gpioPortA  
#define MB_CTRL_RXD_PIN         6   /*PA6*/
#define MB_CTRL_RXD_MODE        gpioModePushPull

#define INDICATE_LED_PORT       gpioPortC
#define INDICATE_LED_PIN        0   /*PC0*/
#define INDICATE_LED_MODE       gpioModePushPull

#define PA_POWER_CTRL_PORT      gpioPortC
#define PA_POWER_CTRL_PIN       1   /*PC1*/
#define PA_POWER_CTRL_MODE      gpioModePushPull

#define SIM800_LIGHT_PORT       gpioPortA//gpioPortC
#define SIM800_LIGHT_PIN        1//8   /*PC8*/
#define SIM800_LIGHT_MODE       gpioModeInputPullFilter

#define PUT_COIN_TX_PORT        gpioPortD
#define PUT_COIN_TX_PIN         4   /*PD4*/
#define PUT_COIN_TX_MODE        gpioModePushPull

#define PUT_COIN_RX_PORT        gpioPortD
#define PUT_COIN_RX_PIN         5   /*PD5*/
#define PUT_COIN_RX_MODE        gpioModeInputPull

#define CHK_PUT_COIN_PORT       gpioPortD  
#define CHK_PUT_COIN_PIN        8   /*PB8*/
#define CHK_PUT_COIN_MODE       gpioModeInputPullFilter

#define SIM800_PWR_PORT         gpioPortA//gpioPortC
#define SIM800_PWR_PIN          2//7   /*PE15*/
#define SIM800_PWR_MODE         gpioModePushPull

#define SIM800_STATUS_PORT      gpioPortA//gpioPortE
#define SIM800_STATUS_PIN       0   /*PE12*/
#define SIM800_STATUS_MODE      gpioModeInputPullFilter

#define SIM800_RST_PORT         gpioPortE
#define SIM800_RST_PIN          3   /*PE14*/
#define SIM800_RST_MODE         gpioModePushPull

#define SIM800_RXD_PORT         gpioPortE
#define SIM800_RXD_PIN          6   /*PE6*/
#define SIM800_RXD_MODE         gpioModeInput

#define SIM800_TXD_PORT         gpioPortE
#define SIM800_TXD_PIN          7   /*PE7*/
#define SIM800_TXD_MODE         gpioModePushPull

#define SIT_CHECK_PORT          gpioPortC
#define SIT_CHECK_PIN           9   /*PC9*/
#define SIT_CHECK_MODE          gpioModeInputPullFilter

#define GSM_POWER_CTRL_PORT     gpioPortC
#define GSM_POWER_CTRL_PIN      6//14   /*PC0*/
#define GSM_POWER_CTRL_MODE     gpioModePushPull

#define BT_TXD_PORT             gpioPortC
#define BT_TXD_PIN              2   /*PC2*/
#define BT_TXD_MODE             gpioModePushPull

#define BT_RXD_PORT             gpioPortC
#define BT_RXD_PIN              3   /*PC2*/
#define BT_RXD_MODE             gpioModeInput

#define BT_LED_PORT             gpioPortA
#define BT_LED_PIN              7   /*PA7*/
#define BT_LED_MODE             gpioModePushPull


#endif


