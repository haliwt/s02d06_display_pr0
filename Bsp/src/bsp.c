#include "bsp.h"


pro_run_t  gpro_t;

typedef enum {
    TEMP_SETTING_MODE = 0,
    TIMER_SETTING_MODE = 1
} SettingMode;


uint8_t hours_one,hours_two,minutes_one,minutes_two;

uint8_t  step_state;
uint8_t  first_set_temperature_value;


void bsp_init(void)
{

    run_t.power_on_step =0;
    run_t.gPower_On = power_off;

#if Enable_EventRecorder == 1  
	/* 0…60‹10‡80†40†30…4EventRecorder0…50„40†70„90‡40‹0 */
	EventRecorderInitialize(EventRecordAll, 1U);
	EventRecorderStart();
#endif


}

void power_on_handler(void)
{

	run_t.gTimer_set_temp_times=0; //conflict with send temperatur value

	run_t.gPower_On = power_on;
	run_t.power_on_step =0;
	run_t.power_off_step = 0;
	
}

void power_off_handler(void)
{

    run_t.gPower_On = power_off;
	run_t.power_on_step =0L;

 }


void detected_ptc_or_fan_warning_fun(void)
{

    Warning_Error_Numbers_Fun();

}

/******************************************************************************
	*
	*Function Name:void mode_key_long_fun(void)
	*Funcion: exit this mode set fun ,
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void mode_key_short_fun(void)
{

   if(gpro_t.set_timer_timing_value_success==0 && gpro_t.key_disp_mode_flag == no_ai_mode ){
	     gpro_t.ai_flag = no_ai_mode  ; //don't AI MODE    
		run_t.timer_dispTime_hours=0;
		run_t.timer_dispTime_minutes=0;

		Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);
		key_t.disp_smg_mode_flag = disp_works_times;
		gpro_t.key_disp_mode_flag =0xff;
		SendData_Set_Command(0x07,0x02); //reverse switch don't displayb "AI"
		tx_thread_sleep(1);
    
   }
   else if(gpro_t.set_timer_timing_value_success==1 && gpro_t.key_disp_mode_flag == no_ai_mode){ 
	     gpro_t.ai_flag = no_ai_mode; //don't AI

		Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);
	    key_t.disp_smg_mode_flag = disp_timer_times;
	    gpro_t.key_disp_mode_flag =0xff;
		SendData_Set_Command(0x07,0x02); //reverse switch don't displayb "AI"
		tx_thread_sleep(1);

   }
   else if(gpro_t.key_disp_mode_flag == ai_mode){
		gpro_t.ai_flag = ai_mode; //don't AI

        Display_Timing(run_t.works_dispTime_hours,run_t.works_dispTime_minutes,0);
        key_t.disp_smg_mode_flag = disp_works_times;
        gpro_t.key_disp_mode_flag =0xff;
		SendData_Set_Command(0x07,0x01); //reverse switch don't displayb "AI"
		tx_thread_sleep(1);
			
			 

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
    static uint8_t power_on_off_flag,counter_flag = 0;
    switch(run_t.power_off_step){
     case 0://2
	  
		 run_t.ptc_warning =0;
		 run_t.fan_warning =0;
		 run_t.power_on_step =0;
         run_t.gFan_RunContinue =1;
	     run_t.gTimer_fan_continue=0;
         run_t.gTimer_detect_mb_receive_flag=0;

          Power_Off_Led_Off();
		 gpro_t.smartphone_app_timer_power_on_flag =0;
		   gpro_t.set_timer_timing_value_success=0;
			run_t.timer_dispTime_hours=0;
		    run_t.timer_dispTime_minutes=0;
			gpro_t.fan_run_one_minute =0; 
			gpro_t.gTimer_counter_one_minute =0;
			
         run_t.power_off_step = 1;
		 
	    SendData_Set_Command(0x11,0x01); //notice thi is new version
		tx_thread_sleep(1);
       break;

       case 1://4


            if(run_t.gTimer_fan_continue < 61 && run_t.gFan_RunContinue == 1 && power_on_off_flag !=0){
                   
					//LED_FAN_ON() ;
		      }
			  else if(run_t.gTimer_fan_continue > 59 ){
                    run_t.gTimer_fan_continue =0;
				 
				   run_t.gFan_RunContinue =2;
                 
			       SendData_Set_Command(0x12,1); //turn off fun .mainboard.WT.EDIT 2026.01.04
				   tx_thread_sleep(1); //WT.EDIT 2026.01.04
				   SendData_Set_Command(0x10,0); //power off mainboard.WT.EDIT 2026.01.04
                   tx_thread_sleep(1); //WT.EDIT 2026.01.04

			}


		  
            Breath_Led();

		
		    run_t.power_off_step = 2;
		 break;

		 case 2:

		 	if(run_t.gTimer_display_dht11 > 2){
               run_t.gTimer_display_dht11 =0;
			   counter_flag = counter_flag ^ 0x01;

			   if(counter_flag ==1){
			   SendData_Set_Command(0x0F,0x02); //notice thi is new version
	           tx_thread_sleep(1);
			   }
			   else{
                 SendData_Set_Command(0x11,0x01); //notice thi is new version
	             tx_thread_sleep(1);

			   }

			}

		    run_t.power_off_step = 1;

		 break;
       }

}


/*******************************************************
*
*Function Name: void twoHours_works_timing(void)
*Function :
*
*
*******************************************************/
void twoHours_works_timing(void)
{
   static uint8_t counter_send;

   if(gpro_t.gTimer_two_hours_seconds > 7119 &&  gpro_t.two_work_hours_flag ==0){
         
      gpro_t.gTimer_two_hours_seconds =0;
	  gpro_t.two_work_hours_flag = 1;
      gpro_t.fan_run_one_minute =1; //one minute is flag .
	  gpro_t.gTimer_counter_one_minute =0;
      SendData_Set_Command(0x19,0x01) ;//works two hours ,then have a rest 10 minutes.
	  tx_thread_sleep(1);
     
   }
   else if(gpro_t.two_work_hours_flag == 1 && gpro_t.gTimer_two_hours_seconds > 600){
        gpro_t.gTimer_two_hours_seconds =0;
		gpro_t.two_work_hours_flag = 0;
        gpro_t.fan_run_one_minute =3; //one minute is flag .
        SendData_Set_Command(0x19,0x0);
	    tx_thread_sleep(1);
	     SendData_Set_Command(0x18,0x0);//fan run .
	    tx_thread_sleep(1);
   }

   if(gpro_t.two_work_hours_flag == 1)counter_send++;

   
  if(gpro_t.fan_run_one_minute==1 && gpro_t.gTimer_counter_one_minute >59){
		  gpro_t.fan_run_one_minute++;
		  SendData_Set_Command(0x18,0x01);//fan stop run .
		  tx_thread_sleep(1);
 }
 else if(gpro_t.fan_run_one_minute==3){
	
		   gpro_t.fan_run_one_minute++;
		   SendData_Set_Command(0x18,0x0);//fan run .
		   tx_thread_sleep(1);
	
	 }
	 else if(gpro_t.two_work_hours_flag ==1 && counter_send >5 &&	gpro_t.fan_run_one_minute ==2){
		 counter_send=0;
	
		 SendData_Set_Command(0x19,0x01);
		 tx_thread_sleep(1);
		 if(gpro_t.fan_run_one_minute==2){
			SendData_Set_Command(0x18,0x01);//fan stop run .
			tx_thread_sleep(1);
		 }
	
	 }
}


