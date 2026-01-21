#include "bsp.h"




/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 10ms interrupt call back function 
	*
	*
*******************************************************************************/
void tim17_isr_callback_hanlder(void)
{
  static uint16_t tm0;


    tm0++;
	
    run_t.gTimer_smg_blink_times++;
	gpro_t.gTimer_4bitsmg_blink_times++;
	
    if(tm0>999){ //100 *10ms = 1000ms = 1s
		tm0=0;

        run_t.gTimer_error_digital++;
		run_t.gTimer_run_ico++;
	    run_t.gTimer_colon++;
		run_t.gTimer_display_dht11++;
		
		run_t.gTimer_fan_continue++;
        run_t.gTimer_detect_mb_receive_flag++;
		run_t.gTimer_time_colon++ ;
		//--------------//

        run_t.gTimer_timer_seconds_counter ++;
		run_t.gTimer_timing_seconds_counter ++;
     
	
	    run_t.gTimer_key_timing++;
		
		run_t.gTimer_key_temp_timing++ ;
		run_t.gTimer_wifi_connect_counter ++;
		run_t.gTimer_set_temp_times++;

        //usart
      
	   run_t.gTimer_usart_error++;

      gpro_t.gTimer_mode_long_key_counter++;
	  gpro_t.gTimer_temp_compare_counter++;

      gpro_t.gTimer_again_send_power_on_off++;
	  gpro_t.gTimer_set_temperature_value++;
	  gpro_t.gTimer_wifi_led_blink ++;
	  gpro_t.gTimer_disp_mode_switch++;

       
       
	 }
    
    
 }
	





