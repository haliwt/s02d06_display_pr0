#include "bsp.h"


pro_run_t  gpro_t;




uint8_t hours_one,hours_two,minutes_one,minutes_two;

uint8_t  step_state,power_off_flag_recoder ;



void bsp_init(void)
{

  
    run_t.gPower_On = power_off;


}

void power_on_handler(void)
{

     if(run_t.gPower_On == power_off){

      

     
        run_t.gPower_On = power_on;
        run_t.power_off_flag = 0;
    

        SendData_PowerOff(1);
        osDelay(10);
      
        power_on_init();


    }
    else{
        power_off_flag_recoder ++;
        SendData_PowerOff(0);
        osDelay(20);
        run_t.gPower_On = power_off;
      }


  }

void smartphone_app_power_on_fun(void)
{
    
    if(gpro_t.smart_phone_app_power_on_flag ==1){

      gpro_t.smart_phone_app_power_on_flag++;

  
    
        
    run_t.gPower_On = power_on;
    run_t.power_off_flag = 0;
       
    
        
     power_on_init();
     }


}



/******************************************************************************
	*
	*Function Name:void mode_key_fun(void)
	*Funcion: switch display timer timing value or beijing time.
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void mode_key_fun(void)
{

    if(gpro_t.disp_timer_or_time_mode== WORKS_TIME){ //display beijing time
           
            gpro_t.disp_timer_or_time_mode = TIMER_SUCCESS;

          if(gpro_t.set_timer_timing_value_success==TIMER_SUCCESS){
       
             Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
             SendData_Set_Command(0x27, 0x02); //not AI mode that is timer mode
             osDelay(10);

           }
           else{

               run_t.timer_dispTime_hours=0;
               run_t.timer_dispTime_minutes=0;
               run_t.gTimer_timer_timing_counter =0;

               Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
               SendData_Set_Command(0x27, 0x02); //not AI mode that is timer mode
                osDelay(10);
       
              }
         }
         else{ //display timer timing 
 
           gpro_t.disp_timer_or_time_mode = WORKS_TIME;
           Display_Timing(run_t.works_dispTime_hours,run_t.works_dispTime_hours);
           SendData_Set_Command(0x27, 0x01); //open AI mode that works time.
            osDelay(10);
          
         }



 }

void mode_key_long_fun(void)
{
  if(run_t.ptc_warning ==0  && run_t.fan_warning == 0){

       gpro_t.disp_timer_or_time_mode = SET_TIMER_ITEM;
       gpro_t.set_timer_timing_doing_value= 1;
       run_t.gTimer_key_timing=0;
    }
}
 
/******************************************************************************
	*
	*Function Name:void RunPocess_Command_Handler(void)
	*Funcion: display pannel run of process 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void power_off_run_handler(void)
{
    static uint8_t power_on_off_flag,fan_run_one_minute;
    switch(run_t.power_off_flag){
     case 0://2
	  
		 run_t.ptc_warning =0;
		 run_t.fan_warning =0;
			
         fan_run_one_minute =1;
	     run_t.gTimer_fan_continue=0;
       
		  Power_Off_Led_Off();
          run_t.gDry =0;
          run_t.gPlasma =0;
          gpro_t.gmouse = 0;
         run_t.power_off_flag = 1;
       break;

       case 1://4


            if(run_t.gTimer_fan_continue < 61 && fan_run_one_minute == 1 && power_on_off_flag !=0){
                   
					
		      }
			  else if(run_t.gTimer_fan_continue > 59){
                    run_t.gTimer_fan_continue =0;
				
				   fan_run_one_minute ++;
                   power_on_off_flag = 1;

			}


		  
            Breath_Led();
		 
		 break;
       }

}

/*********************************************************************************
 * 
 * Function Name:void mouse_on_off_handler(void)
 * 
 * 
 **********************************************************************************/
 void ptc_on_off_handler(void)
{
    
    if(run_t.gDry== 1){
        gpro_t.manual_turn_off_ptc_flag = 1;
        run_t.gDry =0;
        LED_DRY_OFF();
        SendData_Set_Command(0x02,0x0); //DRY_OFF);
    


    }
    else{
        gpro_t.manual_turn_off_ptc_flag = 0;
        run_t.gDry =1;

        LED_DRY_ON();

        SendData_Set_Command(0x02,0x01); //DRY_ON);
       

    }  


}

void plasma_on_off_handler(void)
{
    if(run_t.gPlasma ==1){  //turun off kill 

        run_t.gPlasma = 0;
        LED_PLASMA_OFF();
        SendData_Set_Command(0x03,0x00);//PLASMA_OFF);

    }  
    else{
        run_t.gPlasma = 1;

        LED_PLASMA_ON();

        SendData_Set_Command(0x03,0x01); //PLASMA_ON);

    }

}

void mouse_on_off_handler(void)
{
  
     if(gpro_t.gmouse ==1){

            gpro_t.gmouse =0; //tur Off
           led_mouse_off();
           
            
           SendData_Set_Command(0x04,0x0);//stop dirver mouse //MOUSE_STOP);

        }
        else {

          gpro_t.gmouse =1; //turn on
          led_mouse_on();
          
          SendData_Set_Command(0x04,0x01);//MOUSE_RUN);
        }


       
 }
 



/******************************************************
*
*Function Name:void key_add_fun(void)
*
*
******************************************************/
void key_add_fun(void)
{
    if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
    

    run_t.gTimer_time_colon=0;

    switch(gpro_t.set_timer_timing_doing_value){

    case 0:  //set temperature value 
         SendData_Buzzer();
        run_t.gTimer_key_temp_timing=0;
        gpro_t.set_up_temperature_value ++;
        if(gpro_t.set_up_temperature_value < 20){
        gpro_t.set_up_temperature_value=20;
        }

        if(gpro_t.set_up_temperature_value > 40)gpro_t.set_up_temperature_value= 20;

        run_t.set_temperature_decade_value = gpro_t.set_up_temperature_value / 10 ;
        run_t.set_temperature_unit_value  =gpro_t.set_up_temperature_value % 10; //

        run_t.set_temperature_special_value=1;
        run_t.gTimer_key_timing =0;
        run_t.gTimer_time_colon=0;
    
        run_t.gTimer_key_temp_timing=0;

        gpro_t.manual_turn_off_ptc_flag = 0;
        gpro_t.temp_switch_on_flag++;
        gpro_t.temp_switch_off_flag++;

        g_tDisp.first_disp_set_temp_flag=0;
       
        

    break;
    

    case 1: //set timer timing value 
        SendData_Buzzer();

    
        run_t.gTimer_key_timing =0;
        run_t.temporary_timer_dispTime_hours ++;
        if(run_t.temporary_timer_dispTime_hours >24){
           run_t.temporary_timer_dispTime_hours=0;
        }
           

      run_t.temporary_timer_dispTime_minutes=0;
      run_t.gTimer_time_colon =0;

    
     run_t.hours_two_decade_bit = run_t.temporary_timer_dispTime_hours /10;
    run_t.hours_two_unit_bit   = run_t.temporary_timer_dispTime_hours %10;

    run_t.minutes_one_decade_bit = 0;

    run_t.minutes_one_unit_bit = 0;

     
    //  SendData_Tx_Data(0x4C, run_t.timer_dispTime_hours);
   
 //   TM1639_Write_4Bit_Time(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,0) ; //timer is default 12 hours "12:00" 
    break;

    }

   }

}
/******************************************************
*
*Function Name:void key_dec_fun(void)
*
*
******************************************************/
void key_dec_fun(void)
{
    
    if(run_t.ptc_warning ==0 ){
    run_t.gTimer_time_colon=0;
  
    switch(gpro_t.set_timer_timing_doing_value){

    case 0: //set temperature value

        SendData_Buzzer();
        run_t.gTimer_key_temp_timing=0;
        //setup temperature of value,minimum 20,maximum 40
        gpro_t.set_up_temperature_value--;
        if(gpro_t.set_up_temperature_value<20) gpro_t.set_up_temperature_value=40;
        else if(gpro_t.set_up_temperature_value >40)gpro_t.set_up_temperature_value=40;

        run_t.set_temperature_decade_value = gpro_t.set_up_temperature_value / 10 ;
        run_t.set_temperature_unit_value  =gpro_t.set_up_temperature_value % 10; //


        //set temperature value of flag bit 
        run_t.set_temperature_special_value=1;
        run_t.gTimer_key_timing =0;
        run_t.gTimer_time_colon=0;
        run_t.gTimer_key_temp_timing=0;

         gpro_t.manual_turn_off_ptc_flag = 0;
             gpro_t.temp_switch_on_flag++;
        gpro_t.temp_switch_off_flag++;
        g_tDisp.first_disp_set_temp_flag=0;

    break;

    case 1: //set timer timing value
    SendData_Buzzer();

  
    run_t.gTimer_key_timing =0;
    
 
    
        run_t.temporary_timer_dispTime_hours--;
        if(run_t.temporary_timer_dispTime_hours <0){

            run_t.temporary_timer_dispTime_hours=24;
        

        }
      

   
     run_t.gTimer_time_colon =0;
     run_t.temporary_timer_dispTime_minutes=0;

  

    run_t.hours_two_decade_bit = run_t.temporary_timer_dispTime_hours /10;
    run_t.hours_two_unit_bit   = run_t.temporary_timer_dispTime_hours %10;

    run_t.minutes_one_decade_bit =  run_t.temporary_timer_dispTime_minutes ;

    run_t.minutes_one_unit_bit = run_t.temporary_timer_dispTime_minutes;
    
     // SendData_Tx_Data(0x4C, run_t.timer_dispTime_hours);
    
  //  TM1639_Write_4Bit_Time(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,0) ; //timer is default 12 hours "12:00" 

  

    break;
    }

   }


}

/*******************************************************
*
*Function Name: void compare_temp_value()
*Function :by display pannel of calculate after send to main board 
*
*
*******************************************************/
void compare_temp_value(void)
{
   
    static uint8_t  send_1_off =0xff,send_1_on=0xff,send_2_on=0xff,send_2_off=0xff;
    static uint8_t  first_one_flag ;

    switch(gpro_t.set_temp_value_success_flag){

    case 1:
    

     if(gpro_t.set_up_temperature_value >run_t.gReal_humtemp[1] ){ //PTC TURN ON

        if(gpro_t.manual_turn_off_ptc_flag ==0){
         run_t.gDry =1;
    	
        LED_DRY_ON();

        if(send_1_on != gpro_t.temp_switch_on_flag){
             send_1_on = gpro_t.temp_switch_on_flag;
             gpro_t.temp_switch_off_flag++;
             
		  SendData_Set_Command(0x22,0x01);//DRY_ON_NO_BUZZER);no buzzer sound 
          osDelay(20);

         }

        }
     }
     else{ //PTC turn off 
         run_t.gDry =0;
         LED_DRY_OFF();

          
         if(send_1_off !=gpro_t.temp_switch_on_flag){
             send_1_off = gpro_t.temp_switch_on_flag;

            gpro_t.temp_switch_off_flag++;
            
    	   SendData_Set_Command(0x22,0x0);//DRY_OFF_NO_BUZZER);no buzzer sound 
            osDelay(20);

          }


     }

   break;

   case 0:
  
      if(run_t.gReal_humtemp[1] >39){
         
         first_one_flag =1;

         run_t.gDry =0;
         LED_DRY_OFF();

         
        if(send_2_on !=gpro_t.temp_switch_on_flag){
             send_2_on = gpro_t.temp_switch_on_flag;
            gpro_t.temp_switch_off_flag++;
    	    SendData_Set_Command(0x22,0x0);  //DRY_OFF_NO_BUZZER);no buzzer sound 
         }
         
        }
        else if(run_t.gReal_humtemp[1] <38 && first_one_flag==1){

          if(gpro_t.manual_turn_off_ptc_flag ==0){

             run_t.gDry =1;
            
             LED_DRY_ON();
            if(send_2_off !=gpro_t.temp_switch_off_flag ){
             send_2_off = gpro_t.temp_switch_off_flag;
                gpro_t.temp_switch_on_flag++;
             
                  SendData_Set_Command(0x22,0x01);//DRY_ON_NO_BUZZER);

                }

               }
           }
         else if(run_t.gReal_humtemp[1] <40 && first_one_flag==0){

           if(gpro_t.manual_turn_off_ptc_flag ==0){

             run_t.gDry =1;
            
             LED_DRY_ON();
            if(send_2_off != gpro_t.temp_switch_on_flag){
             send_2_off =  gpro_t.temp_switch_on_flag;
             gpro_t.temp_switch_off_flag ++;
                  SendData_Set_Command(0x22,0x01);//DRY_ON_NO_BUZZER);

                }

             }
          }
       break;
      }
}





