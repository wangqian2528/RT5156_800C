#ifndef __AES_H__
#define __AES_H__


#undef  EXTERN

#ifdef  __AES_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include <stdio.h>
#include <string.h>
#include "platform.h"


EXTERN void InitAES(void);
EXTERN void AES_Demo(void);
EXTERN void AES_Decode(uint8_t *out, uint8_t *in, uint8_t len);

#endif


