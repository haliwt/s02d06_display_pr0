#ifndef __BSP_DECONDER_H
#define __BSP_DECONDER_H
#include "main.h"



typedef struct{

    uint8_t first_disp_set_temp_value ;
    uint8_t first_disp_set_temp_flag;
    uint8_t first_disp_set_timer_flag;



}display_state_ref;


extern display_state_ref g_tDisp;

void receive_data_fromm_display(uint8_t *pdata);



uint8_t bcc_check(const unsigned char *data, int len) ;



#endif 

