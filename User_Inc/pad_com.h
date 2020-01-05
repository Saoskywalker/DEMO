#ifndef _PAD_COM_H
#define _PAD_COM_H

#include "UserBaseLib.h"

#define STATE_COMMAND 1
#define ADJ_COMMAND 2
#define PARAMETER_COMMAND 3

extern u8 pad_rec_fail[6];
extern u8 pad_rec_success[6];
extern u8 pad_water_full[6];
extern u8 pad_key[6];
extern u8 pad_rec_data[6];
void pad_rec(u8 d);
void pad_send(u8 *data, u8 cnt);
u8 pad_agreement(void);
void pad_clear(void);

#endif
