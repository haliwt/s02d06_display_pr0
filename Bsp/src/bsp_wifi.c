#include "bsp.h"

static void  wifi_ico_fast_blink(void);


void wifi_connect_state_fun(void)
{

  volatile static uint8_t wifi_counter= 0;
  if(run_t.wifi_led_fast_blink == 1 && run_t.connect_wifi_state==0 && run_t.gPower_On == power_on){
    
        if(run_t.gTimer_wifi_connect_counter <120){
           
            LED_WIFI_TOGGLE() ;
            
        }
        else{
            run_t.wifi_led_fast_blink =0;
           
        }

  	}
    else if(run_t.wifi_led_fast_blink == 0 && run_t.connect_wifi_state == wifi_connect_null && run_t.gPower_On == power_on){

	     
		   if(++wifi_counter > 9){//100ms * 10 = 1000ms = 1s.
		   	 wifi_counter =0;
	         LED_WIFI_TOGGLE() ;
		   }

    }
    else if(run_t.connect_wifi_state == 1 && run_t.gPower_On == power_on){

         LED_WIFI_ON()  ;  
	 }
   
}



static void  wifi_ico_fast_blink(void)
{

   static uint8_t ai_led_blink;
    if(gpro_t.set_timer_timing_doing_value==1 || gpro_t.set_timer_timing_doing_value==3){
    if(run_t.gTimer_smg_blink_times > 100){

        run_t.gTimer_smg_blink_times=0;
        ai_led_blink ++;

        if(ai_led_blink ==1){
        
              LED_WIFI_ON();
         }
          else{
            ai_led_blink =0;
            LED_WIFI_OFF();
       
           }
          
       }
  }

}

