#ifndef __BSP_DECONDER_H
#define __BSP_DECONDER_H
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif



#if 0

typedef struct{

    uint8_t first_disp_set_temp_value ;
    uint8_t first_disp_set_temp_flag;
    uint8_t first_disp_set_timer_flag;



}display_state_ref;


extern display_state_ref g_tDisp;

#endif 
class decoder_class{

  public:

   void receive_data_fromm_display(uint8_t *pdata);
   uint8_t bcc_check(const unsigned char *data, int len) ;

   
 
   void set_temp_flag_to_first_disp(uint8_t fdata);
   void set_timer_flag_to_first_disp(uint8_t tdata);

   uint8_t  get_temp_flag_for_first_disp(void);
   uint8_t  get_timer_flag_for_first_disp(void);
    

   private:
   
    uint8_t first_disp_set_temp_flag;
    uint8_t first_disp_set_timer_flag;

   

};

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */


