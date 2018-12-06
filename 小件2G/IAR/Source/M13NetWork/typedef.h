#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__


#define QUEUE_SIZE  (768)


typedef struct
{
    unsigned char data[QUEUE_SIZE];
    unsigned int  head;
    unsigned int  tail;
} QUEUE_Init_TypeDef;


typedef struct
{
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} TIME_Init_TypeDef;


typedef struct
{
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char week;
} DATE_Init_TypeDef;


#endif


