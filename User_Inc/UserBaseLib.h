#ifndef _USER_BASE_LIB_H
#define _USER_BASE_LIB_H

#include "sys.h"
#include "wdg.h"

#define DEBUG_TIME_PIN PAout(11)

//#ifndef uchar
//#define uchar unsigned char
//#endif

//#ifndef uint
//#define uint  unsigned int
//#endif

//#ifndef uint32
//#define uint32 unsigned long int
//#endif

#define BIT_CLEAR(dv,db) (dv &= (~(1<<db)))
#define BIT_SET(dv,db) (dv |= (1<<db))
#define BIT_GET(dv,db) (dv&(1<<db))
#define BIT0_1 0x01
#define BIT1_1 0x02
#define BIT2_1 0x04
#define BIT3_1 0x08
#define BIT4_1 0x10
#define BIT5_1 0x20
#define BIT6_1 0x40
#define BIT7_1 0x80

u8 RestoreFlag(void);	//Check MCU restore reason

#endif
