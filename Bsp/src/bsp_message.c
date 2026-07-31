/*
 * bsp_message.c
 *
 *  Created on: 2025年2月19日
 *      Author: Administrator
 */
#include "bsp.h"

static void copy_cmd_data_from_mainboard(uint8_t *pdata);

 uint8_t recoder_counter;

/******************************************************************************
	*
	*Function Name:void receive_data_from_mainboard(uint8_t *pdata)
	*Funcion: handle of tall process
	*Input Ref:
	*Return Ref:
	*
******************************************************************************/
void receive_data_from_mainboard(uint8_t *pdata)
{
    
   static uint8_t power_on_counter;
    switch(pdata[2]){

     case 0:


     break;

     case  power_cmd:
           if(pdata[3] == 0x01){ //power on

            run_t.gPower_On = power_on;
            gpro_t.boot_done = 0;
       
            power_on_handler();
            SendWifiData_Answer_Cmd(0x01 ,0x01);//SendData_Set_Command(0x11,0x01); //0x11 :send to main has the second display board exit.
			tx_thread_sleep(2);
           }
           else{ //power off

            run_t.gPower_On = power_off;
            gpro_t.boot_done = 0;
            SendWifiData_Answer_Cmd(0x01,0x0);
			tx_thread_sleep(2);
           
           }
          

     break;

	 case 0x21: //smart phone power on or off that App timer .
       

		   if(pdata[3]==0x01){

		   run_t.connect_wifi_state = wifi_connect_success;
		   	
           gpro_t.smartphone_app_timer_power_on_flag =1;
		    run_t.gPower_On = power_on;
			 power_on_handler();
		   	}
		    else{
                run_t.connect_wifi_state = wifi_connect_success;
				run_t.gPower_On = power_off;
                gpro_t.boot_done = 0;

			}
           
             
         
       
     break; 

	 case 0x20: //手机定时开机，发送的数据，3个。

	     if(pdata[3]==0x0F){ //power on by smart phone APP

		   if(pdata[4]==0x03){

                run_t.gDry =pdata[5];
				if(run_t.gDry == 0){
                  gpro_t.g_manual_shutoff_dry_flag =1;
                  LED_DRY_OFF();
				}
				else{
                   LED_DRY_ON();
				}

				run_t.gPlasma=pdata[6];
				if(run_t.gPlasma ==1){
                  LED_PLASMA_ON();
				}
				else{
				   LED_PLASMA_OFF();

				}
		       
                run_t.gMouse =pdata[7];
				if(run_t.gMouse==1){
					LED_MOUSE_ON();
				}
				 else{
                   LED_MOUSE_OFF();

				}



		   	}

	     }
	 


	 break;

	 

     case dry_cmd://0x02 //PTC打开关闭指令
       
    

	   if(pdata[3]== 0x01 && run_t.gPower_On == power_on){

            run_t.gDry =1 ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
            LED_DRY_ON();
	        gpro_t.g_manual_shutoff_dry_flag=0;
	       SendWifiData_Answer_Cmd(0x02,0x0);
			tx_thread_sleep(2);
        }
        else if(pdata[3] == 0x0){

            run_t.gDry =0;
		    LED_DRY_OFF();
            gpro_t.g_manual_shutoff_dry_flag=1;
		    SendWifiData_Answer_Cmd(0x02,0x0);
		    tx_thread_sleep(2);

        }
    	
     break;

     case plasma_cmd: //0x03//PLASMA 打开关闭指令

		
		
			if(pdata[3]== 0x01){
	          
	        run_t.gPlasma =1;


	        }
	        else if(pdata[3] == 0x0){
	          
	         run_t.gPlasma =0;

	        }
		

     break;


      case mouse_cmd: //0x04//ultrasonic  打开关闭指令

        
	   if(pdata[3] == 0x00){
		   
		   if(pdata[4]== 0x01){
	           
	           run_t.gMouse = 1;

	        }
	        else { //close
	         run_t.gMouse = 0;
	        }

		}


     break;

	 case wifi_cmd://0x05


		if(pdata[3] == 0x01){

		    run_t.wifi_led_fast_blink=1;
			run_t.connect_wifi_state = wifi_connect_null;
			run_t.gTimer_wifi_connect_counter =0; //120s counte start
			 
		}
		else if(pdata[3] == 0){

           //run_t.wifi_led_fast_blink=1;
		   //run_t.connect_wifi_state = wifi_connect_null;
		  // run_t.gTimer_wifi_connect_counter =0; //120s counte start


		}
		
    break;

	case 0x07:
		
	  if(pdata[4]== 0x01){
           recoder_counter++;
	       gpro_t.key_model_short_flag  =1;
		   gpro_t.gTimer_disp_mode_switch=0;
		   gpro_t.key_disp_mode_flag = ai_mode;

		}
		else if(gpro_t.ai_flag == no_ai_mode){
		       gpro_t.key_disp_mode_flag = no_ai_mode;//gpro_t.key_disp_mode_flag = ai_mode;

		   }
	     
	 	

	break;

	case temp_warning://0x08 //temperature of high warning.

		if(pdata[3] == 0x01){
			
			run_t.ptc_warning = 1;
	        run_t.gDry =0;
			LED_DRY_OFF();
	    }
	    else if(pdata[3] == 0x0){ //close

	          run_t.ptc_warning = 0;


	       }

      break;

      case fan_warning: //fan of default of warning.

         if(pdata[3] == 0x01){  //warning

           
            run_t.fan_warning = 1;

           run_t.gDry =0;
		   LED_DRY_OFF();
           //SendData_Set_Command(0x22,0x0); //0x22:PTC notice close .
           

        }
        else if(pdata[3] == 0x0){ //close

           run_t.fan_warning = 0;


        }


      break;

 		//接收的是数据
	 case temp_hum_data: //0x1A //温度,湿度数据
        
        if(pdata[4] == 0x02){ //数据,two 
            
             if(pdata[5] !=0){
			    run_t.gReal_humtemp[0] = pdata[5] ;//humidity value 

             }
           
             run_t.gReal_humtemp[1] = pdata[6]; //temperature value 

			 if(run_t.gPower_On == power_on && power_on_counter < 10){
			 	 power_on_counter++;
		          Display_DHT11_Value();

			 }
		
		   

        }
      
        
      break;

      case 0x1B: //湿度数据

        if(pdata[2] == 0x0F){ //数据


        }
      break;

      case beijing_times_data://0x1c //表示时间：小时，分，秒

        if(pdata[4] == 0x03){ //数据,has three data

            if(pdata[5] < 24){ //WT.EDIT 2024.11.23
      
		     run_t.connect_wifi_state = wifi_connect_success;
            run_t.display_beijing_time_flag= 1;
          
          run_t.works_dispTime_hours= pdata[5];// run_t.dispTime_hours  =  pdata[5];
          run_t.works_dispTime_minutes =pdata[6];//run_t.dispTime_minutes = pdata[6];
          run_t.gTimer_timing_seconds_counter =  pdata[7];//run_t.gTimer_disp_time_seconds =  pdata[7];
           }


        }
      break;

	  case 0x1F: //0x1f notice is command
	  	
     
	    if(pdata[3] == 0x01){ //0x1F ,is notice don't data--(1,2,3,4,5,6,7,9)

		    
             run_t.wifi_led_fast_blink = 0;
			 run_t.connect_wifi_state = wifi_connect_success;
		}
		else if(pdata[3] == 0){ //close
	  
		      
			 run_t.connect_wifi_state = wifi_connect_null;
		}
	    
	  break;


	  case 0x22: //Command ,set temperature compare dht11 result open or close

	  

	   if(pdata[3]== 0x01 && run_t.gPower_On == power_on){

             run_t.gDry =1 ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
             LED_DRY_ON();// gpro_t.g_manual_shutoff_dry_flag = 0;
        }
        else if(pdata[3] == 0x0 && run_t.gPower_On == power_on){

            //gpro_t.g_manual_shutoff_dry_flag = 0;
            run_t.gDry =0;
		    LED_DRY_OFF();
          

        }
    	

	  break;

	  

        case 0x1D: //表示日期： 年，月，日

        if(pdata[2] == 0x0F){ //数据



        }
      break;

	  

	  case 0x2A: //main board set temperature value 
	  
          
		  if(pdata[3] == 0x0F){ //is data.
		  
			if(pdata[4]== 0x01){ // one only data 

		    gpro_t.g_manual_shutoff_dry_flag=0;
				
			gpro_t.set_up_temperature_value =pdata[5];//warning
	  
			 gpro_t.g_manual_shutoff_dry_flag = 0 ;//  allow open dry function
            

			 if(gpro_t.set_up_temperature_value <= run_t.gReal_humtemp[1] ){
                 run_t.gDry =0 ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
                 LED_DRY_OFF();

			 }
			 else{
			    run_t.gDry =1 ;//&& run_t.gPlasma ==1	&& run_t.gUltransonic==1
				LED_DRY_ON();


			 }
        
             run_t.gTimer_key_temp_timing=0;

			 run_t.set_temperature_decade_value = gpro_t.set_up_temperature_value / 10 ;
             run_t.set_temperature_unit_value  =gpro_t.set_up_temperature_value % 10; //

             TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
			 tx_thread_sleep(100);//vTaskDelay(1000);
	  
			 

				}
		  	}
	break;

	case mainboard_set_timer_value://0x2B, timer timing value .
          if(pdata[4]== 0x01){ // one only data 
			 
             if(pdata[5] > 0){
	
			  gpro_t.set_timer_timing_value_success=1;
			  key_t.disp_smg_mode_flag=disp_timer_times;
			  gpro_t.ai_flag = no_ai_mode;
 
			   run_t.timer_dispTime_hours=pdata[5];
			   run_t.timer_dispTime_minutes=0;
	  
			 	run_t.hours_two_decade_bit    = run_t.timer_dispTime_hours / 10;
    			run_t.hours_two_unit_bit      = run_t.timer_dispTime_hours % 10;
				
   				 run_t.minutes_one_decade_bit  = 0;
    			run_t.minutes_one_unit_bit    = 0;
				run_t.gTimer_timer_seconds_counter=0;
             }
			 else if(pdata[5]== 0){

	             gpro_t.set_timer_timing_value_success=0;
				 key_t.disp_smg_mode_flag=disp_works_times;
				 gpro_t.ai_flag = ai_mode;
	 
				  run_t.timer_dispTime_hours=0;
				  run_t.timer_dispTime_minutes=0;
		 
				  run_t.hours_two_decade_bit  = run_t.timer_dispTime_hours / 10;
				  run_t.hours_two_unit_bit 	  = run_t.hours_two_decade_bit;//run_t.timer_dispTime_hours % 10;
				   
				  run_t.minutes_one_decade_bit  = 0;
				  run_t.minutes_one_unit_bit	 = 0;
				  run_t.gTimer_timer_seconds_counter=0;


			 }
				
      	  }
		  	
			

	break;

	

     case copy_cmd: // copy send cmd acknowlege
          copy_cmd_data_from_mainboard(pdata);

     break;

     }

 }
/******************************************************************************
	*
	*Function Name:static void copy_cmd_data_from_mainboard(uint8_t *pdata )
	*Funcion: parse received data is copy command.
	*Input Ref:
	*Return Ref:
	*
******************************************************************************/
static void copy_cmd_data_from_mainboard(uint8_t *pdata )
{
    
    switch(pdata[3]){

    case CMD_POWER : //power_on 


	 if(pdata[4]==0x01){
	 	run_t.gPower_On = power_on;
		gpro_t.boot_done=0;//run_t.power_on_step =0;
        run_t.gTimer_set_temp_times=0; //conflict with send temperatur value

	    run_t.power_off_step = 0;
        

     }
     else{ //power offf

        run_t.gPower_On = power_off;
        run_t.power_off_step=0;
       

     }
    
    break;

    case ack_ptc:
   
    if(pdata[4]==1){

         gpro_t.receive_copy_cmd = 1;
		 run_t.gDry =1 ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
         gpro_t.g_manual_shutoff_dry_flag = 0;
		 LED_DRY_ON();
    }
    else{
      gpro_t.receive_copy_cmd = 2;
	  gpro_t.g_manual_shutoff_dry_flag = 0;
      run_t.gDry =0;
	  LED_DRY_OFF();

    }

    

    break;

    case ack_plasma:

    if(pdata[4]==1){

        gpro_t.receive_copy_cmd = 1;
    }
    else{
      gpro_t.receive_copy_cmd = 2;

    }
    break;

    case ack_ai:

    if(pdata[4]==1){

        gpro_t.receive_copy_cmd = 1;
    }
    else{
      gpro_t.receive_copy_cmd = 2;

    }

    break;

    case ack_wifi: // link wifi command

    

	  if(pdata[4] == 0x01){  // link wifi

	    run_t.wifi_led_fast_blink=1;
		run_t.connect_wifi_state = wifi_connect_null;
		run_t.gTimer_wifi_connect_counter =0; //120s counte start
	
			  
	  
	  }
	  else if(pdata[4] == 0){ //close
	  
			  run_t.connect_wifi_state = wifi_connect_null;
	          run_t.wifi_led_fast_blink=0;
			  run_t.display_beijing_time_flag =0;
	  
		}

      
     

   break;

   


    
    
    case ack_with_buzzer:
        if(pdata[4] == 1){  //buzzer answer command

           

        }
        else{
         


        }
     break;

	 case 0x19:
		 if(pdata[4] == 1){  //buzzer answer command
		 
			gpro_t.answer_two_works_flag=1;	
		 
		}
		else if(pdata[4] == 0){
				  
		    gpro_t.answer_two_works_flag=0;	
		 
		}

        

	 break;
    }


}


/******************************************************************************
*
*Function Name:uint8_t bcc_check(const unsigned char *data, int len)
*Funcion: BCC check code
*Input Ref: data:input data, len: data of length
*Return Ref:
*
******************************************************************************/
uint8_t bcc_check(const unsigned char *data, int len)
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}



