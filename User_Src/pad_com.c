#include "pad_com.h"
#include "usart.h"

static u8 pad_state = 0;

u8 pad_agreement(void)
{
    return pad_state;
}

void pad_clear(void)
{
    pad_state = 0;
}

u8 pad_rec_fail[] = {0xaa, 0x00, 0xcc, 0x33, 0xc3, 0x3c};
u8 pad_rec_success[] = {0xaa, 0x01, 0xcc, 0x33, 0xc3, 0x3c};
u8 pad_water_full[] = {0xaa, 0x02, 0xcc, 0x33, 0xc3, 0x3c};
u8 pad_key[] = {0xaa, 0x03, 0xcc, 0x33, 0xc3, 0x3c};
void pad_send(u8 *data, u8 cnt)
{
    for(; cnt>0; cnt--)
    {
        uasrt1SendByte(*data);
        data++;
    }
}

u8 pad_rec_data[6];
void pad_rec(u8 d)
{
    static u8 pad_rec_data_temp[6];
    u8 i = 0, k = 0;
    static u8 start = 0, j = 0;
    
    if(start)
    {
        if(j>=6)
        {
            start = 0;
            j = 0;
            k = pad_rec_data_temp[0]^pad_rec_data_temp[1]^pad_rec_data_temp[2] \
                ^pad_rec_data_temp[3]^pad_rec_data_temp[4]^pad_rec_data_temp[5];
            if(k==d)
            {
                // if(pad_state) //last without handle
                // {
                //     pad_send(&pad_rec_fail[0], sizeof(pad_rec_fail));
                // }
                // else
                {
                    pad_state = 1;
                    for(i = 0; i<6; i++)
                        pad_rec_data[i] = pad_rec_data_temp[i];
                    pad_send(&pad_rec_success[0], sizeof(pad_rec_success));
                }
            }
            else
            {
                pad_send(&pad_rec_fail[0], sizeof(pad_rec_fail));
            }
            return;
        }
        pad_rec_data_temp[j] = d;
        j++;
    }
    else
    {
        if(d==0xa7)
            start = 1;
    } 
}

