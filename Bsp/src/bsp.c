#include "bsp.h"


pro_run_t  gpro_t;




uint8_t hours_one,hours_two,minutes_one,minutes_two;

uint8_t  step_state;



void bsp_init(void)
{

  
    run_t.gPower_On = power_off;


}

void power_on_handler(void)
{

     if(run_t.gPower_On == power_off){

        run_t.gTimer_set_temp_times=0; //conflict with send temperatur value

     
        run_t.gPower_On = power_on;
        run_t.power_off_flag = 0;
        run_t.link_wifi_net_flag= 0;

        SendData_PowerOff(1);
      
        power_on_init();


    }
    else{
      
            SendData_PowerOff(0);
            run_t.gPower_On = power_off;
         

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
void disp_timer_times_handler(void)
{

    static uint8_t step_state;

        if(gpro_t.set_timer_timing_doing_value == 1){

                 Set_TimerTiming_Number_Value();
                   
        }
        else{

              switch(step_state){

					case 0:
						Led_Panel_OnOff();
					    step_state=2;
					break;
                    
                    case 2:
                        if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){ //read main board ptc_warning of ref.
                            Display_SmgTiming_Value();
                            }
                        


                     step_state=0;
                    break;

              }
            
        }    
   

	 

  }



void detected_ptc_or_fan_warning_fun(void)
{

    Warning_Error_Numbers_Fun();

}

/******************************************************************************
	*
	*Function Name:void mode_key_fun(void)
	*Funcion: exit this mode set fun ,
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void mode_key_fun(void)
{

    if(gpro_t.mode_key_flag == AI_MODE){ //display beijing time
            gpro_t.mode_key_flag = NO_AI_MODE;
       
           Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
       
        }
         else{ //display timer timing 
           gpro_t.mode_key_flag = AI_MODE;
           Display_Timing(run_t.works_dispTime_hours,run_t.works_dispTime_hours);
           
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
    static uint8_t power_on_off_flag;
    switch(run_t.power_off_flag){
     case 0://2
	  
		 run_t.ptc_warning =0;
		 run_t.fan_warning =0;
			
         run_t.gFan_RunContinue =1;
	     run_t.gTimer_fan_continue=0;
       
		  Power_Off_Led_Off();
         run_t.power_off_flag = 1;
       break;

       case 1://4


            if(run_t.gTimer_fan_continue < 61 && run_t.gFan_RunContinue == 1 && power_on_off_flag !=0){
                   
					
		      }
			  else if(run_t.gTimer_fan_continue > 59){
                    run_t.gTimer_fan_continue =0;
				
				   run_t.gFan_RunContinue ++;
                   power_on_off_flag = 1;

			}


		  
            Breath_Led();
		 
		 break;
       }

}
/*********************************************************************************
 * 
 * Function Name:void mode_key_handler(void)
 * 
 * 
 **********************************************************************************/
#if 0
void mode_key_handler(void)
{
    if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
       switch(gpro_t.set_timer_timing_doing_value){

        case 0:  //don't set timer timing item

        if(gpro_t.mode_key_flag==AI_MODE){
            gpro_t.mode_key_flag=NO_AI_MODE;
            LED_WIFI_OFF();
            SendData_Set_Command(0x07,0x0); //AI,OFF
            if(gpro_t.set_timer_timing_value_success ==TIMER_SUCCESS)
              Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
        }
        else{
            if(gpro_t.mode_key_flag==NO_AI_MODE){
            gpro_t.mode_key_flag=AI_MODE;
            SendData_Set_Command(0X07,0x01); //AI ON

      
            run_t.gDry= 1;
            
            run_t.gPlasma = 1;
            gpro_t.gmouse = 1;
            led_mouse_on();
            LED_PLASMA_ON();
            LED_DRY_ON();
            LED_WIFI_ON();
            Display_Timing(run_t.works_dispTime_hours,run_t.works_dispTime_minutes);
           
            }  

        }
        break;

       

        }


    }


}
#endif 
/*********************************************************************************
 * 
 * Function Name:void mouse_on_off_handler(void)
 * 
 * 
 **********************************************************************************/
void mouse_on_off_handler(void)
{
   if(run_t.fan_warning ==0 && run_t.ptc_warning == 0){ 



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
 }
 



/******************************************************
*
*Function Name:void key_add_fun(void)
*
*
******************************************************/
void key_add_fun(void)
{
    if(run_t.ptc_warning ==0){
    

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

       // run_t.set_temperature_flag=set_temperature_value;
        run_t.set_temperature_special_value=1;
        run_t.gTimer_key_temp_timing=0;
        run_t.gTimer_time_colon=0;
        gpro_t.manual_turn_off_dry_flag =0; //after set temperature allow shut off dry .
      
        
   

    break;
    

    case 1: //set timer timing value 
        SendData_Buzzer();

    
        run_t.gTimer_key_timing =0;
    
        if(run_t.temporary_timer_dispTime_hours !=24)
            run_t.temporary_timer_dispTime_minutes =  run_t.temporary_timer_dispTime_minutes + 30;
        else if(run_t.temporary_timer_dispTime_hours ==24)
            run_t.temporary_timer_dispTime_minutes =  run_t.temporary_timer_dispTime_minutes + 60;
        
        if(run_t.temporary_timer_dispTime_minutes >59){
            run_t.temporary_timer_dispTime_hours ++;
            if(run_t.temporary_timer_dispTime_hours ==24){
                run_t.temporary_timer_dispTime_minutes=0;
            }
            else if(run_t.temporary_timer_dispTime_hours >24){

                run_t.temporary_timer_dispTime_hours=0;
                run_t.temporary_timer_dispTime_minutes=0;


            }
            else{

                run_t.temporary_timer_dispTime_minutes =0;


            }

        }

       run_t.gTimer_time_colon =0;
        
     run_t.hours_two_decade_bit = run_t.temporary_timer_dispTime_hours /10;
    run_t.hours_two_unit_bit   = run_t.temporary_timer_dispTime_hours %10;

    run_t.minutes_one_decade_bit =  run_t.temporary_timer_dispTime_minutes /10;

    run_t.minutes_one_unit_bit = run_t.temporary_timer_dispTime_minutes %10;

   
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


        //set temperature value of flag bit : run_t.set_temperature_special_value=1
        run_t.set_temperature_special_value=1;
        run_t.gTimer_key_temp_timing=0;
        run_t.gTimer_time_colon=0;
        gpro_t.manual_turn_off_dry_flag =0; //after set temperature allow shut off dry .

    
        

    break;

    case 1: //set timer timing value
    SendData_Buzzer();

  
    run_t.gTimer_key_timing =0;
    
    run_t.temporary_timer_dispTime_minutes =  run_t.temporary_timer_dispTime_minutes -30;
    if(run_t.temporary_timer_dispTime_minutes < 0){
        run_t.temporary_timer_dispTime_hours--;
        if(run_t.temporary_timer_dispTime_hours <0){

        run_t.temporary_timer_dispTime_hours=24;
        run_t.temporary_timer_dispTime_minutes=0;

        }
        else{

        run_t.temporary_timer_dispTime_minutes =30;


        }

     }
     run_t.gTimer_time_colon =0;

    run_t.hours_two_decade_bit = run_t.temporary_timer_dispTime_hours /10;
    run_t.hours_two_unit_bit   = run_t.temporary_timer_dispTime_hours %10;

    run_t.minutes_one_decade_bit =  run_t.temporary_timer_dispTime_minutes /10;

    run_t.minutes_one_unit_bit = run_t.temporary_timer_dispTime_minutes %10;
    
  
    
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
    static uint8_t first_one_flag;

    if(gpro_t.set_temp_value_success ==1){
    

     if(gpro_t.set_up_temperature_value >run_t.gReal_humtemp[1] && gpro_t.manual_turn_off_dry_flag ==0){ //PTC TURN ON
         run_t.gDry =1;
    	
        LED_DRY_ON();
       
		SendData_Set_Command(0x20,0x01);//DRY_ON_NO_BUZZER);no buzzer sound 
        
     }
     else{ //PTC turn off 
         run_t.gDry =0;
         LED_DRY_OFF();
    	 SendData_Set_Command(0x20,0x0);//DRY_OFF_NO_BUZZER);no buzzer sound 


     }

   }
   else{
        if(run_t.gReal_humtemp[1] >39){

         run_t.gDry =0;
         LED_DRY_OFF();
    	 SendData_Set_Command(0x20,0x0);  //DRY_OFF_NO_BUZZER);no buzzer sound 
         first_one_flag =1;
        }
        else{

           if(first_one_flag==1 && (run_t.gReal_humtemp[1] <39)){

              
                 run_t.gDry =1;
            
                LED_DRY_ON();
             
        		SendData_Set_Command(0x20,0x01);//DRY_ON_NO_BUZZER);

               

             }
           }

      }
}


/**********************************************************************
    *
    *Function Name:void receive_data_fromm_display(uint8_t *pdata,uint8_t len)
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void receive_data_fromm_display(uint8_t *pdata)
{

   uint8_t hum1,hum2; 
   uint8_t temp1,temp2;

    switch(pdata[2]){

     case 0:


     break;

     case 0x01: //表示开机指令

        if(pdata[3] == 0x01){ //open
          

         

        }
        else if(pdata[3] == 0x0){ //close 
        
         


        }

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[3] == 0x01){
        
         
       }
       else if(pdata[3] == 0x0){
        
         

       }

     break;

     case 0x03: //PLASMA 打开关闭指令

        if(pdata[3] == 0x01){
           

          
        }
        else if(pdata[3] == 0x0){

         

        }


     break;


      case 0x04: //ultrasonic  打开关闭指令

        if(pdata[3] == 0x01){  //open 
          
          
        }
        else if(pdata[3] == 0x0){ //close 

           

        }


     break;

      case 0x05: // link wifi command

        if(pdata[3] == 0x01){  // link wifi 

             

        }
        else if(pdata[3] == 0x0){ //don't link wifi 



        }


     break;

     case 0x06: //buzzer sound done

        if(pdata[3] == 0x01){  //buzzer sound 
             

        }
        else if(pdata[3] == 0x0){ // don't buzzer sound .



        }


     break;


      case 0x1A: //温度数据

        if(pdata[4] == 0x02){ //数据


         if(run_t.set_temperature_special_value ==0){

            run_t.gReal_humtemp[0] = pdata[5];
            run_t.gReal_humtemp[1] = pdata[6];

            hum1 =  run_t.gReal_humtemp[0]/10;  //Humidity 
        	hum2 =  run_t.gReal_humtemp[0]%10;

	       
             
              temp1 = run_t.gReal_humtemp[1]/10 ;  // temperature
              temp2 = run_t.gReal_humtemp[1]%10;

            
                  
              TM1639_Write_2bit_TempData(temp1,temp2);
              TM1639_Write_2bit_HumData(hum1,hum2);

          
           }
           

        }
        else if(pdata[4] == 0x01){ //数据)

              run_t.gReal_humtemp[1] = pdata[5];
              temp1 = run_t.gReal_humtemp[1]/10 ;  // temperature
              temp2 = run_t.gReal_humtemp[1]%10;
                  
              TM1639_Write_2bit_TempData(temp1,temp2);


         }
      break;

      case 0x2A:

           if(pdata[4] == 0x01){ //手机APP 温度设置的值




            }

      break;

      case 0x1B: //湿度数据

        if(pdata[4] == 0x01){ //数据
        
            run_t.gReal_humtemp[0] = pdata[5];
            hum1 =  run_t.gReal_humtemp[0]/10;  //Humidity 
        	hum2 =  run_t.gReal_humtemp[0]%10;

	        TM1639_Write_2bit_HumData(hum1,hum2);
	
   
	  
        }
      break;

      case 0x1C: //表示时间：小时，分，秒

        if(pdata[4] == 0x03){ //数据

        run_t.works_dispTime_hours= pdata[5];
        run_t.works_dispTime_minutes  = pdata[6];
        run_t.gTimes_time_seconds  = pdata[7];


        }
      break;

        case 0x1D: //表示日期： 年，月，日

        if(pdata[3] == 0x0F){ //数据

             
            

        }
      break;
     
    }

}
/**********************************************************************
    *
    *Function Name:uint8_t bcc_check(const unsigned char *data, int len) 
    *Function: BCC校验函数
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
uint8_t bcc_check(const unsigned char *data, int len) 
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}


