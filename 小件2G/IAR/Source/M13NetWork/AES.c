#define __AES_C__


#include "AES.h"
#include "Base64.h"


const uint8_t exampleData[31] =
{
    'w','w','w','.','5','1','s','h',
    'a','o','x','i','.','c','o','m',
    '-','E','0','1','7','1','-','x',
    'u','l','i','d','o','n','g'
};


const uint8_t expectedEncryptedData[44] =
{
    '4','N','R','q','c','F','R','h',
    'D','K','J','9','1','h','2','G',
    'A','7','O','A','h','F','x','Y',
    'U','b','T','j','v','9','/','k',
    'J','2','X','B','B','G','J','e',
    'h','T','U','='
};


const uint8_t gucAESEncryptionKey[16] =
{
    '@','S','h','a','o','X','i','%',
    'M','1','3','-','N','E','T','#'
};


uint8_t gucAESEncryptBuffer[32];    /*AES���ܻ���,���ܵ����ݳ��ȷ�Χ:[16,32)*/
uint8_t gucAESDecryptBuffer[32];    /*AES���ܻ���*/
uint8_t gucAESDecryptionKey[16];    /*AES������Կ*/
uint8_t gucAESBase64Buffer[100];


void InitAES(void)
{
    /*Enable AES Clock*/
    CMU_ClockEnable(cmuClock_AES, true);
}


extern uint16_t guiBase64DecodeLength;

void AES_Demo(void)
{
    uint32_t i = 0;

    /*���������Կ*/
    AES_DecryptKey128(gucAESDecryptionKey, gucAESEncryptionKey);

    /*����Ҫ���ܵ����ķŵ�������*/
    for(i = 0; i < sizeof(exampleData) / sizeof(exampleData[0]); i++)
    {
        gucAESEncryptBuffer[i] = exampleData[i];
    }

    /*����AES128λ��ECB����*/
    AES_ECB128(gucAESEncryptBuffer, gucAESEncryptBuffer, sizeof(gucAESEncryptBuffer), gucAESEncryptionKey, true);

    /*��AES����ת����BASE64����*/
    BASE64_Encode(gucAESBase64Buffer, gucAESEncryptBuffer, sizeof(gucAESEncryptBuffer));

#if 0
    /*��BASE64����ʽ���AES����*/
    for(i = 0; i < sizeof(gucAESBase64Buffer); i++)
    {        
        LEUART_Tx(LEUART0, gucAESBase64Buffer[i]);
    }

    LEUART_Tx(LEUART0, 0x30);
#endif

    /*����Ҫ���ܵ����ķŵ������У���ʱ�����е�������BASE64�����ʽ*/
    for(i = 0; i < sizeof(expectedEncryptedData) / sizeof(expectedEncryptedData[0]); i++)
    {
        gucAESBase64Buffer[i] = expectedEncryptedData[i];
    }

    /*�����Ľ���BASE64����*/
    BASE64_Decode(gucAESBase64Buffer, gucAESBase64Buffer, sizeof(expectedEncryptedData));

    /*����AES128λ��ECB����*/
    AES_ECB128(gucAESDecryptBuffer, gucAESBase64Buffer, guiBase64DecodeLength, gucAESDecryptionKey, false);

#if 0
    /*�������*/
    for(i = 0; i < sizeof(gucAESDecryptBuffer); i++)
    {
        LEUART_Tx(LEUART0, gucAESDecryptBuffer[i]);
    }

    LEUART_Tx(LEUART0, 0x39);
#endif
}


void AES_Decode(uint8_t *out, uint8_t *in, uint8_t len)
{
    uint32_t i = 0;

    /*���������Կ*/
    AES_DecryptKey128(gucAESDecryptionKey, gucAESEncryptionKey);

    for(i = 0; i < len; i++)
    {
        gucAESBase64Buffer[i] = in[i];
    }

    for(; i < 100; i++) gucAESBase64Buffer[i] = 0x00;

    /*�����Ľ���BASE64����*/
    BASE64_Decode(gucAESBase64Buffer, gucAESBase64Buffer, len);

    for(i = guiBase64DecodeLength; i < 100; i++)
    {
        gucAESBase64Buffer[i] = 0x00;
    }
    
    /*����AES128λ��ECB����*/
    AES_ECB128(gucAESDecryptBuffer, gucAESBase64Buffer, guiBase64DecodeLength, gucAESDecryptionKey, false);

    for(i = 0; i < sizeof(gucAESDecryptBuffer); i++)
    {
        out[i] = gucAESDecryptBuffer[i];
    }
}


