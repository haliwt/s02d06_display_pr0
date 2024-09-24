#include "bsp.h"


RUN_T run_t;

uint8_t power_on_off_flag;

void Power_Off(void);


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
	gpro_t.gmouse =0;
			
}


/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void power_on_init(void)
{
             
    if(gpro_t.smart_phone_app_power_on_flag == 0){
        run_t.gPlasma=1;
		run_t.gDry =1;
        gpro_t.gmouse = 1;
     }


    if(gpro_t.get_beijing_time_flag==0){
		
        run_t.works_dispTime_hours=0;
	    run_t.works_dispTime_minutes=0;
	    run_t.gTimes_time_seconds =0;
    }
 

 
	Power_ON_Led();

	run_t.fan_warning=0;
	run_t.ptc_warning=0;

     run_t.wifi_led_fast_blink_flag =0;   

	
	
    run_t.gTimer_timer_timing_counter=0;
    gpro_t.set_timer_timing_value_success =0 ;
    gpro_t.disp_timer_or_time_mode = WORKS_TIME;
    gpro_t.set_temp_value_success_flag = 0;
   

    run_t.gTimer_time_colon =0;
	run_t.set_temperature_decade_value=40;
    gpro_t.set_timer_timing_doing_value = 0;
			
		
      SMG_POWER_ON(); //WT.EDIT 2023.03.02
      
     run_t.hours_two_unit_bit = 0;
	 run_t.minutes_one_decade_bit =  0;
     Display_DHT11_Value();
	 TM1639_Write_4Bit_Time(0,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,0,0);
     
    
}



/**********************************************************************
*
*Functin Name: void Power_Off_Fun(void)
*Function : run order from main command 
*Input Ref:  run_t.single_dat is command 
*Return Ref: NO
*
**********************************************************************/
void Power_Off_Fun(void)
{
	
 
		run_t.gPlasma=0;
		run_t.gDry =0;
		gpro_t.gmouse = 0;
		
        run_t.gPower_On =power_off;
		run_t.gTimer_set_temp_times=0; //conflict with send temperatur value 
       Power_Off_Led_Off();

  
} 

