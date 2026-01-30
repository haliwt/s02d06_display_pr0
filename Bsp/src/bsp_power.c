#include "bsp.h"

RUN_T run_t;



uint8_t power_on_off_flag;

void Power_Off(void);

/******************************************************************************
	*
	*Function Name:void RunPocess_Command_Handler(void)
	*Funcion: display pannel run of process 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void power_on_run_handler(void)
{

   static uint8_t  step_state;
   switch(run_t.power_on_step){

      case 0:
          
	  
           run_t.gTimer_time_colon =0;
	       run_t.set_temperature_decade_value=40;
           
            run_t.gTimer_detect_mb_receive_flag =0;
			Power_On_Fun();
			run_t.gTimer_display_dht11 = 20; //at once display temperature and humidity value.
			gpro_t.set_timer_timing_doing_value = 0;
            gpro_t.g_manual_shutoff_dry_flag = 0; //allow open dry function .
            run_t.wifi_led_fast_blink=0;
		
			gpro_t.set_timer_timing_value_success=0;
			run_t.timer_dispTime_hours=0;
		    run_t.timer_dispTime_minutes=0;

			if(run_t.connect_wifi_state == wifi_connect_success){

                   if(run_t.display_beijing_time_flag ==0){
						run_t.works_dispTime_hours=0;
						run_t.works_dispTime_minutes=0;
						run_t.gTimer_timing_seconds_counter =0;
					    

				    }

			}
			else{
			   
    		   run_t.works_dispTime_hours=0;
			   run_t.works_dispTime_minutes=0;
			   run_t.gTimer_timing_seconds_counter =0;

			}
			gpro_t.gTimer_two_hours_seconds =0;
	        gpro_t.two_work_hours_flag = 0;
			gpro_t.set_temp_value_success=0;
			gpro_t.key_disp_mode_flag = 0xff;
		    gpro_t.ai_flag = ai_mode; //don't AI
		    key_t.disp_smg_mode_flag=disp_works_times;

			 SendData_Set_Command(0x11,0x01); //notice thi is outside connect display board
	         vTaskDelay(pdMS_TO_TICKS(100));
			
			run_t.power_on_step= 1;


            
	  break;

      case 1:

              if(gpro_t.mode_key_shot_flag ==1){

                  if(gpro_t.gTimer_disp_mode_switch <  3){
				  	
			          mode_key_short_fun();

                  }
				  else{
				     gpro_t.mode_key_shot_flag++;


				  }

			  }
              else if(gpro_t.set_timer_timing_doing_value == 1 && run_t.ptc_warning ==0 && run_t.fan_warning ==0){

                   Set_TimerTiming_Number_Value();
                   
              }
              else if((gpro_t.set_timer_timing_doing_value == 0 ||gpro_t.set_timer_timing_doing_value == 3 )&&  run_t.set_temperature_special_flag   >0 &&  run_t.set_temperature_special_flag != 0xff ){

                   disp_smg_blink_set_tempeature_value();
	              
						
             }
             else{

              switch(step_state){

					case 0:
						Led_Panel_OnOff();
					    step_state=1;
					break;

					case 1:

						 
               			// disp_dht11_value();
                       
				        step_state=2;
	                    
				   break;
                    
                    case 2: //display 1:   timing times  2: timer times.

					    if(gpro_t.mode_key_shot_flag == 1){
                              if(gpro_t.gTimer_disp_mode_switch > 2){
							  	gpro_t.gTimer_disp_mode_switch=0;
								gpro_t.mode_key_shot_flag++;
                                mode_key_short_fun();

                              }
					    }
                        else if((gpro_t.set_timer_timing_doing_value==0 || gpro_t.set_timer_timing_doing_value==3) && gpro_t.key_disp_mode_flag ==0xff){ //WT.EDIT 2025.05.07
                        if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){ //read main board ptc_warning of ref.
                            
							   Display_SmgTiming_Value();

                            

                         }
                        else{

                            Warning_Error_Numbers_Fun();

                        }
                        
                        }

                     step_state=0;
                    break;

              }
            
             }

			 run_t.power_on_step=2;
      break;

	  case 2:
	    Display_SetTemperature_Value();

	   run_t.power_on_step=3;

	  break;

	  case 3:
          disp_dht11_value();
		   run_t.power_on_step=4;
	  break;

	  case 4:
	  	
	  	twoHours_works_timing();
	  run_t.power_on_step=1;

	  break;
	  	

	}
}




/**********************************************************************
*
*Functin Name: void Display_DHT11_Value(void)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Power_Off(void)
{
    Smg_AllOff();
    SMG_POWER_OFF()	;
	run_t.gPlasma=0;
	run_t.gDry=0;
	run_t.gMouse =0;
	//run_t.gWifi_led = 0;
			
}


/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void Power_On_Fun(void)
{
                
   static uint8_t hour_decade,hour_unit,minutes_one,minutes_two;

  
   if(gpro_t.smartphone_app_timer_power_on_flag == 0){
		run_t.gPlasma=1;
		run_t.gDry =1;
        run_t.gMouse = 1;
		run_t.ai_model_flag =ai_mode;

   	}
    else{

	    run_t.ai_model_flag =ai_mode;

	}

  
    

    run_t.time_led_flag=1;
	Power_ON_Led();

	run_t.fan_warning=0;
	run_t.ptc_warning=0;
 
	
    run_t.gTimer_timer_seconds_counter=0;
    gpro_t.set_timer_timing_value_success =0 ;
    gpro_t.set_temp_value_success = 0;
    run_t.timer_dispTime_hours =0;
	run_t.timer_dispTime_minutes=0;
		
        

	  hour_decade=0;
	  hour_unit=0;
    
	  minutes_one = 0;
      minutes_two = 0;
	  
	   SMG_POWER_ON(); //WT.EDIT 2023.03.02
      

	  run_t.hours_two_unit_bit = hour_unit;
	  run_t.minutes_one_decade_bit =  minutes_one;
      
	 TM1639_Write_4Bit_Time(hour_decade,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,minutes_two,0);
     //Display_DHT11_Value(); //WT.EIDT 2025.05.10
    
}


/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*
	*
	*
	*
************************************************************************/
void Power_Off_Fun(void)
{
	
 
		run_t.gPlasma=0;
		run_t.gDry =0;
		run_t.gMouse = 0;
		
       

  
} 


