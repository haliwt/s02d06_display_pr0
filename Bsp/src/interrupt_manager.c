#include "bsp.h"




/*******************************************************************************
	*
	*Function Name:
	*Function : timer 17 is 10ms 
	*
	*
*******************************************************************************/
void tim17_isr_callback_hanlder(void)
{
  static uint16_t tm0;
  

    tm0++;
	
   
	gpro_t.gTimer_4bitsmg_blink_times++;
	
    if(tm0>99){ //100 *10ms = 1000ms = 1s
		tm0=0;
	

        run_t.gTimer_error_digital++;
		
	    run_t.gTimer_colon++;
		run_t.gTimer_display_dht11++;
		
		run_t.gTimer_fan_continue++;
       
		run_t.gTimer_time_colon++ ;
		//--------------//

        run_t.gTimer_timer_seconds_counter ++;
		run_t.gTimer_timing_seconds_counter ++;
     
	
	    run_t.gTimer_key_timing++;
		
		run_t.gTimer_key_temp_timing++ ;
		run_t.gTimer_wifi_connect_counter ++;
		run_t.gTimer_set_temp_times++;

        //usart
      


    
	
       gpro_t.gTimer_two_hours_seconds++;
	
	   gpro_t.gTimer_counter_one_minute++;
	   gpro_t.gTimer_wifi_led_blink ++;
	  gpro_t.gTimer_disp_mode_switch++;

       
       
	 }
    
    
 }
	





