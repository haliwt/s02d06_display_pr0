#include "bsp.h"



void (*single_ai_fun)(uint8_t cmd);
void (*single_add_fun)(void);
void (*single_buzzer_fun)(void);
void (*sendAi_usart_fun)(uint8_t senddat);
void (*dispose_key)(uint8_t dsdat);
void (*display_fan_speed_value)(uint8_t fan_level);





//static void DisplayPanel_DHT11_Value(void);

static void Display_Works_Time_Fun(void);
static void WorksTime_DonotDisplay_Fun(void);
static void Timer_Timing_Donot_Display(void);

uint8_t disp_times;

/*******************************************************
	*
	*Function Name: static void RunLocal_Dht11_Data_Process(void)
	*Function : display pannel display conetent
	*
	*
	*
*******************************************************/
void RunLocal_Dht11_Data_Process(void)
{
   

   if((run_t.gTimer_display_dht11 > 6  || disp_times < 10) && run_t.set_temperature_special_value==0){
	    run_t.gTimer_display_dht11=0;
        disp_times ++ ;
         Display_DHT11_Value();
       
     
	}

    if(run_t.gTimer_compare_temp_value > 4 &&  run_t.set_temperature_special_value==0){
        
        run_t.gTimer_compare_temp_value=0;

        compare_temp_value();


    }

}
void Led_Panel_OnOff(void)
{
	Panel_Led_OnOff_Function() ;
}

/******************************************************************************
* 
* Function Name: static void Timer_Timing_Donot_Display(void)
* Function :function of pointer 
* Input Ref:NO
* Return Ref:NO
* 
*******************************************************************************/
static void Display_Works_Time_Fun(void)
{
     static uint8_t works_timing_flag,alternate_flag;

	 if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
     if(run_t.gTimes_time_seconds > 59 ){
            run_t.gTimes_time_seconds=0;
            works_timing_flag =1;
			run_t.works_dispTime_minutes++; //1 minute 
		
            run_t.send_app_wokes_minutes_two++;
			if(run_t.works_dispTime_minutes> 59){ //1 hour
			run_t.works_dispTime_minutes=0;
			run_t.works_dispTime_hours++;
			if(run_t.works_dispTime_hours > 99){ //WT.edit times over 99hours 2023.09.20
			run_t.works_dispTime_hours =0;
			}
        
            }

     	}
       
	       if(works_timing_flag==1){
	          works_timing_flag=0;
			 
			 Display_Timing(run_t.works_dispTime_hours,run_t.works_dispTime_minutes);
		  
	        }
		    
        }
		else{

		    if(run_t.gTimer_error_digital < 60){//10ms * 51= 510

		      
               if(alternate_flag ==0){
			   	  
                     
			     if(run_t.ptc_warning ==1){
                 
					Display_Error_Digital(0x01,0);
			     }
				 else {
			        if(run_t.fan_warning ==1){

					  
                      Display_Error_Digital(0x02,0);

			        }

				 }
			    

               }
			   else{

			      alternate_flag=2;
				   if(run_t.ptc_warning ==1 && run_t.fan_warning ==1){

					     Display_Error_Digital(0x02,0);

				   	}
				    else  if(run_t.ptc_warning ==1 && run_t.fan_warning ==0){
                       
					    Display_Error_Digital(0x01,0);
			        }
					else  if(run_t.ptc_warning ==0 && run_t.fan_warning ==1){
                       
					    Display_Error_Digital(0x02,0);
			        }


			   }
			   

		   }
		   else if(run_t.gTimer_error_digital > 59 && run_t.gTimer_error_digital  < 121 ){
		   		alternate_flag++;
 				Display_Error_Digital(0x10,1);
 				if(alternate_flag==2 ||alternate_flag>2 )alternate_flag=0;
		   }
		    else if(run_t.gTimer_error_digital > 119){

			  run_t.gTimer_error_digital=0;


			 }

		}

}
/****************************************************************
 * 
 * Function Name: static void Timer_Timing_Donot_Display(void)
 * Function :function of pointer 
 * Input Ref:NO
 *
 * 
*****************************************************************/
static void Timer_Timing_Donot_Display(void)
{
  if(run_t.gTimer_timer_timing_counter > 59 && gpro_t.set_timer_timing_value_success==TIMER_SUCCESS){
    run_t.gTimer_timer_timing_counter =0;
    run_t.timer_dispTime_minutes -- ;

    if(run_t.timer_dispTime_minutes <  0 ){

    run_t.timer_dispTime_hours -- ;
    run_t.timer_dispTime_minutes =59;
    }



    if(run_t.timer_dispTime_hours <0){ 
    run_t.gTimer_timer_timing_counter = 57 ;
    run_t.timer_dispTime_hours=0;
    run_t.timer_dispTime_minutes=0;
    power_off_run_handler();

    }
			
  }

}

/****************************************************************
 * 
 * Function Name: static void WorksTime_DonotDisplay_Fun(void)
 * Function :function of pointer 
 * 
 *
 * 
****************************************************************/
static void WorksTime_DonotDisplay_Fun(void)
{
//send to APP works times every minute onece
   if(run_t.gTimes_time_seconds > 59 &&  gpro_t.set_timer_timing_value_success ==TIMER_SUCCESS ){
		   run_t.gTimes_time_seconds=0;
		 
		   run_t.works_dispTime_minutes++; //1 minute 
		
		   run_t.send_app_wokes_minutes_two++;
		   if(run_t.works_dispTime_minutes> 59){ //1 hour
		   run_t.works_dispTime_minutes=0;
		   run_t.works_dispTime_hours++;
		   if(run_t.works_dispTime_hours > 24){
		   run_t.works_dispTime_hours =0;
		   }
	       }
  }
}


/******************************************************************************
*
*Function Name:static void Setup_Timer_Times(void)
*Funcion:display setup timer times  //__asm("NOP");//等待1个指令周期，系统主频24M
*Iinput Ref:NO
*Return Ref:NO
*
******************************************************************************/
void Display_SmgTiming_Value(void)
{

 

    switch(gpro_t.set_timer_timing_value_success){

	   case TIMER_SUCCESS:
	   if(run_t.gTimer_timer_timing_counter > 59){
	    run_t.gTimer_timer_timing_counter =0;
	
		run_t.timer_dispTime_minutes -- ;
	
	    if(run_t.timer_dispTime_minutes <  0 ){
			 
		   run_t.timer_dispTime_hours -- ;
		   run_t.timer_dispTime_minutes =59;
         }

		
		
		 if(run_t.timer_dispTime_hours < 0 ){
		 
			run_t.gTimer_timer_timing_counter = 57 ;
			run_t.timer_dispTime_hours=0;
			run_t.timer_dispTime_minutes=0;
             
	        run_t.gPower_On = power_off;
            run_t.power_off_flag = 0;
            SendData_PowerOff(0);
			power_off_run_handler();
			
	      }
		    
        }

        Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
        WorksTime_DonotDisplay_Fun();
        
	    break;

		case WORKS_TIME: //NO_AI_MODE by timer timing  auto be changed AI_MODE
			
        
          if(run_t.gTimes_time_seconds > 59){
    		   run_t.gTimes_time_seconds=0;
    		 
    		   run_t.works_dispTime_minutes++; //1 minute 
    		
    		   run_t.send_app_wokes_minutes_two++;
    		   if(run_t.works_dispTime_minutes> 59){ //1 hour
    		   run_t.works_dispTime_minutes=0;
    		   run_t.works_dispTime_hours++;
    		   if(run_t.works_dispTime_hours > 24){
    		        run_t.works_dispTime_hours =0;
    		   }
    	      }
           }
  
            Display_Timing(run_t.works_dispTime_hours,run_t.works_dispTime_minutes);
            Display_Works_Time_Fun();
			Timer_Timing_Donot_Display();

          break;

	}

  

     #if 0

           if(run_t.gTimes_time_seconds > 59){
    		   run_t.gTimes_time_seconds=0;
    		 
    		   run_t.works_dispTime_minutes++; //1 minute 
    		
    		   run_t.send_app_wokes_minutes_two++;
    		   if(run_t.works_dispTime_minutes> 59){ //1 hour
    		   run_t.works_dispTime_minutes=0;
    		   run_t.works_dispTime_hours++;
    		   if(run_t.works_dispTime_hours > 24){
    		        run_t.works_dispTime_hours =0;
    		   }
    	      }
           }
  
         Display_Timing(run_t.works_dispTime_hours,run_t.works_dispTime_minutes);
         Timer_Timing_Donot_Display();

     #endif 
  

}




/****************************************************************
 * 
 * Function Name:
 * Function :function of pointer 
 * 
 * 
****************************************************************/
void Single_Add_RunCmd(void(*addHandler)(void))
{
    single_add_fun = addHandler;   

}

void Single_SendBuzzer_RunCmd(void(*buzzerHandler)(void))
{
	single_buzzer_fun = buzzerHandler;

}
void Single_SendAi_Usart_RunCmd(void(*sendaiHandler)(uint8_t seddat))
{
    sendAi_usart_fun = sendaiHandler;

}

void SplitDispose_Key_RunCmd(void(*keyHandler)(uint8_t dat))
{

  dispose_key = keyHandler;

}

//void Smg_DisplayFan_Leve(void(*fandisplayHandler)(uint8_t fan_level))
//{
//   display_fan_speed_value = fandisplayHandler;

//}




