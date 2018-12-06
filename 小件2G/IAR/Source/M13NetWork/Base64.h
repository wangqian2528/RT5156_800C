#ifndef __BASE64_H__
#define __BASE64_H__


#undef  EXTERN

#ifdef  __BASE64_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


EXTERN void InitBASE64(void);
EXTERN void BASE64_Demo(void);
EXTERN void BASE64_Encode(uint8_t *out, uint8_t *in, uint16_t len);
EXTERN void BASE64_Decode(uint8_t *out, uint8_t *in, uint16_t len);


#endif


