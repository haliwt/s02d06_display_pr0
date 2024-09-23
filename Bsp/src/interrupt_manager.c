#include "bsp.h"


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
   uint32_t temp;

	if(huart->Instance==USART1){
	
		if(__HAL_UART_GET_IT(&huart1,UART_FLAG_ORE)!=RESET){

		__HAL_UART_CLEAR_OREFLAG(&huart1);
		
 		__HAL_UART_CLEAR_IT(&huart1,UART_CLEAR_OREF); //UART_CLEAR_RTOF
 		__HAL_UART_CLEAR_IT(&huart1,UART_CLEAR_RTOF);
		
		 UART_Start_Receive_IT(&huart1,inputBuf,1);

		}
		__HAL_UNLOCK(&huart1);

		temp = USART1->RDR;
		UART_Start_Receive_IT(&huart1,inputBuf,1);
	
		}




}
/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 1ms interrupt call back function 
	*
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint16_t tm0;
    if(htim->Instance==TIM3){  
    tm0++;
	
    run_t.gTimer_smg_blink_times++;

	run_t.gTimer_set_temp_times++;
	run_t.gTimer_time_colon++ ;
    run_t.gTimer_error_digital++;
    gpro_t.gTimer_4bitsmg_blink_times++;
	
    if(tm0>999){ //1000 *1ms = 1000ms = 1s
		tm0=0;
		run_t.gTimer_run_ico++;
	    run_t.gTimer_colon++;
		run_t.gTimer_display_dht11++;
        run_t.gTimer_compare_temp_value++ ;
		
		run_t.gTimer_fan_continue++;
       

		//--------------//

        run_t.gTimer_timer_timing_counter ++;
		run_t.gTimes_time_seconds ++;
     
	
	    run_t.gTimer_key_timing++;
		
		run_t.gTimer_key_temp_timing++ ;
        run_t.gTimer_usart_error++;
        // process ref
        gpro_t.gTimer_wifi_led_blik_time ++;
        gpro_t.gTimer_wifi_led_blink++;
           

        }
    
    }
 }
	





