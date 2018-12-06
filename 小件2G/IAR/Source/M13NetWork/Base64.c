#define __BASE64_C__


#include "Base64.h"


const uint8_t gucBase64EncodeTable[] =
{
    'A','B','C','D','E','F','G','H',
    'I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X',
    'Y','Z','a','b','c','d','e','f',
    'g','h','i','j','k','l','m','n',
    'o','p','q','r','s','t','u','v',
    'w','x','y','z','0','1','2','3',
    '4','5','6','7','8','9','+','/'
};


const uint8_t gucBase64DecodeTable[] =
{
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,  62, 128, 128, 128,  63,
     52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 128, 128, 128, 128, 128, 128,
    128,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 128, 128, 128, 128, 128,
    128,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
     41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 128, 128, 128, 128, 128
};


uint8_t  gucBase64EncodeBuffer[250];    /*Base64±‡¬Îª∫¥Ê*/
uint8_t  gucBase64DecodeBuffer[250];    /*Base64“Î¬Îª∫¥Ê*/
uint16_t guiBase64EncodeLength = 0;     /*Base64±‡¬Î≥§∂»*/
uint16_t guiBase64DecodeLength = 0;     /*Base64“Î¬Î≥§∂»*/


void InitBASE64(void)
{
    uint16_t i = 0;

    for(i = 0; i < sizeof(gucBase64EncodeBuffer); i++)
    {
        gucBase64EncodeBuffer[i] = 0;
        gucBase64DecodeBuffer[i] = 0;
    }

    guiBase64EncodeLength = 0;
    guiBase64DecodeLength = 0;
}


void BASE64_Encode(uint8_t *out, uint8_t *in, uint16_t len)
{
    uint16_t i = 0;

    uint8_t  ucByte1 = 0, ucByte2 = 0, ucByte3 = 0;

    guiBase64EncodeLength = 0;

    while(i < len)
    {
        ucByte1 = in[i++];

        if(i == len)
        {
            gucBase64EncodeBuffer[guiBase64EncodeLength++] = gucBase64EncodeTable[ucByte1 >> 0x02];
            gucBase64EncodeBuffer[guiBase64EncodeLength++] = gucBase64EncodeTable[(ucByte1 & 0x03) << 0x04];
            gucBase64EncodeBuffer[guiBase64EncodeLength++] = '=';
            gucBase64EncodeBuffer[guiBase64EncodeLength++] = '=';
            break;
        }

        ucByte2 = in[i++];

        if(i == len)
        {
            gucBase64EncodeBuffer[guiBase64EncodeLength++] = gucBase64EncodeTable[ucByte1  >> 0x02];
            gucBase64EncodeBuffer[guiBase64EncodeLength++] = gucBase64EncodeTable[((ucByte1 & 0x03) << 0x04) | ((ucByte2 & 0xF0) >> 0x04)];
            gucBase64EncodeBuffer[guiBase64EncodeLength++] = gucBase64EncodeTable[((ucByte2 & 0x0F) << 0x02)];
            gucBase64EncodeBuffer[guiBase64EncodeLength++] = '=';
            break;
        }

        ucByte3 = in[i++];

        gucBase64EncodeBuffer[guiBase64EncodeLength++] = gucBase64EncodeTable[ucByte1  >> 0x02];
        gucBase64EncodeBuffer[guiBase64EncodeLength++] = gucBase64EncodeTable[((ucByte1 & 0x03) << 0x04) | ((ucByte2 & 0xF0) >> 0x04)];
        gucBase64EncodeBuffer[guiBase64EncodeLength++] = gucBase64EncodeTable[((ucByte2 & 0x0F) << 0x02) | ((ucByte3 & 0xC0) >> 0x06)];
        gucBase64EncodeBuffer[guiBase64EncodeLength++] = gucBase64EncodeTable[ucByte3   & 0x3F];
    }

    for(i = 0; i < guiBase64EncodeLength; i++)
    {
        out[i] = gucBase64EncodeBuffer[i];

      /*LEUART_Tx(LEUART0, out[i]);*/
    }
}


void BASE64_Decode(uint8_t *out, uint8_t *in, uint16_t len)
{
    uint16_t i = 0;

    uint8_t  ucByte1 = 0, ucByte2 = 0, ucByte3 = 0, ucByte4 = 0;

    guiBase64DecodeLength = 0;

    while(i < len)
    {
        /*ucByte1*/
        do
        {
            ucByte1 = gucBase64DecodeTable[in[i++] & 0xFF];

        }while((i < len) && (ucByte1 == 128));

        if(ucByte1 == 128) break;

        /*ucByte2*/
        do
        {
            ucByte2 = gucBase64DecodeTable[in[i++] & 0xFF];

        }while((i < len) && (ucByte2 == 128));

        if(ucByte2 == 128) break;

        gucBase64DecodeBuffer[guiBase64DecodeLength++] = (ucByte1 << 0x02) | ((ucByte2 & 0x30) >> 0x04);

        /*ucByte3*/
        do
        {
            ucByte3 = in[i++] & 0xFF;

            if(ucByte3 == 61) goto BASE64_DECODE_FINISHED;

            ucByte3 = gucBase64DecodeTable[ucByte3];

        }while((i < len) && (ucByte3 == 128));

        if(ucByte3 == 128) break;

        gucBase64DecodeBuffer[guiBase64DecodeLength++] = ((ucByte2 & 0x0F) << 0x04) | ((ucByte3 & 0x3C) >> 0x02);

        /*ucByte4*/
        do
        {
            ucByte4 = in[i++] & 0xFF;

            if(ucByte4 == 61) goto BASE64_DECODE_FINISHED;

            ucByte4 = gucBase64DecodeTable[ucByte4];

        }while((i < len) && (ucByte4 == 128));

        if(ucByte4 == 128) break;

        gucBase64DecodeBuffer[guiBase64DecodeLength++] = ((ucByte3 & 0x03) << 0x06) | ucByte4;
    }

BASE64_DECODE_FINISHED:

    for(i = 0; i < guiBase64DecodeLength; i++)
    {
        out[i] = gucBase64DecodeBuffer[i];

      /*LEUART_Tx(LEUART0, out[i]);*/
    }
}


void BASE64_Demo(void)
{
    uint8_t in[23] =
    {
        'T','h','i','s',' ','i','s',' ',
        'B','a','s','e','6','4',' ','e',
        'x','a','m','p','l','e','.'
    };

    uint8_t out[100];

    BASE64_Encode(out, in, 23);

    BASE64_Decode(out, gucBase64EncodeBuffer, guiBase64EncodeLength);
}


