#include "bsp.h"

display_state_ref g_tDisp;


static void first_disp_set_temp_value_fun(uint8_t data);

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

        if(pdata[3] == 0x01){ //the firs display set power on 
          
        run_t.gPower_On = power_on;
        run_t.power_off_flag = 0;


        power_on_init();
         

        }
        else if(pdata[3] == 0x0){ //close 
        
           run_t.gPower_On = power_off;
           
            power_off_run_handler();

        }

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[3] == 0x01){

         run_t.gDry =1;
         gpro_t.manual_turn_off_ptc_flag = 0;
            
      }
       else if(pdata[3] == 0x0){
         gpro_t.manual_turn_off_ptc_flag = 0;
         run_t.gDry =0;

       }

     break;

     case 0x03: //PLASMA 打开关闭指令

        if(pdata[3] == 0x01){
           
              run_t.gPlasma=1;
          
        }
        else if(pdata[3] == 0x0){

          run_t.gPlasma=0;

        }


     break;


      case 0x04: //ultrasonic  打开关闭指令

        if(pdata[3] == 0x01){  //open 
          
             gpro_t.gmouse = 1;
        }
        else if(pdata[3] == 0x0){ //close 

             gpro_t.gmouse = 0;

        }


     break;

      case 0x05: // link wifi command

        if(pdata[3] == 0x01){  // link wifi 

             gpro_t.gTimer_wifi_led_blink_time =0;
             run_t.wifi_led_fast_blink_flag =1;  
             gpro_t.wifi_link_net_success = 0;
             gpro_t.get_beijing_time_flag = 0;

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

            run_t.gReal_humtemp[0] = pdata[5];
            run_t.gReal_humtemp[1] = pdata[6];

            hum1 =  run_t.gReal_humtemp[0]/10;  //Humidity 
        	hum2 =  run_t.gReal_humtemp[0]%10;

	       
             
              temp1 = run_t.gReal_humtemp[1]/10 ;  // temperature
              temp2 = run_t.gReal_humtemp[1]%10;

            
                  
              TM1639_Write_2bit_TempData(temp1,temp2);
              TM1639_Write_2bit_HumData(hum1,hum2);

          
         
        }
        else if(pdata[4] == 0x01){ //数据)

              run_t.gReal_humtemp[1] = pdata[5];
              temp1 = run_t.gReal_humtemp[1]/10 ;  // temperature
              temp2 = run_t.gReal_humtemp[1]%10;
                  
              TM1639_Write_2bit_TempData(temp1,temp2);


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
        gpro_t.get_beijing_time_flag = 1;
        run_t.works_dispTime_hours= pdata[5];
        run_t.works_dispTime_minutes  = pdata[6];
        run_t.gTimes_time_seconds  = pdata[7];


        }
      break;

      case 0x1D: //表示日期： 年，月，日

        if(pdata[3] == 0x0F){ //数据

             
            

        }
      break;

      case 0x1F: //wifi link net is ok or don't link.

        if(pdata[5] == 0x01){ //数据

             gpro_t.wifi_link_net_success = 1;
            

        }
        else if(pdata[5] == 0x00){

          gpro_t.wifi_link_net_success = 0;
          gpro_t.get_beijing_time_flag = 0;

        }


      break;

      case 0x20: //smart phone timer power on and power off command
        if(pdata[4] == 0x04){ //4 command 

          if(pdata[5]==1){
             gpro_t.smart_phone_app_power_on_flag  =1;

             run_t.gDry  = pdata[6];
             run_t.gPlasma  = pdata[7];
             gpro_t.gmouse = pdata[8];

            run_t.gPower_On = power_on;
            run_t.power_off_flag = 0;


             power_on_init();
            }
       }


      break;

//      case 0x21:  //smart phone timer timing power on .
//
//      
//        if(pdata[3] == 0x01){ //数据
//           gpro_t.smart_phone_app_power_on_flag  =1;
//           run_t.gPower_On = power_on;
//           run_t.power_off_flag = 0;
//        
//
//           power_on_init();
//
//        }
//
//      break;

      case 0x2A: //set temperature value 

        if(pdata[4] == 0x01){ //only receive data  1 word .

             gpro_t.manual_turn_off_ptc_flag = 0;
            g_tDisp.first_disp_set_temp_flag = 1;
            g_tDisp.gTimer_disp_set_temp =0;
            
            first_disp_set_temp_value_fun(pdata[5]);

         }


      break;


      case 0x27:  //ai mode -beijing mode . not ai mode ->timer timing mode

         if(pdata[3] == 0x01){
           
            gpro_t.disp_timer_or_time_mode = WORKS_TIME;
         }
         else{
            gpro_t.disp_timer_or_time_mode = TIMER_SUCCESS;

         }
          

      break;

      case 0x4C: //display 1 and 2 of set timer timing value 

        if(pdata[4]==0x01){

           run_t.timer_dispTime_hours = pdata[5]   ;
           gpro_t.disp_timer_or_time_mode = TIMER_SUCCESS;
		   gpro_t.set_timer_timing_value_success=TIMER_SUCCESS;
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


static void first_disp_set_temp_value_fun(uint8_t data)
{

    gpro_t.set_up_temperature_value = data;
    
    
    run_t.set_temperature_decade_value = gpro_t.set_up_temperature_value / 10 ;
    run_t.set_temperature_unit_value  =gpro_t.set_up_temperature_value % 10; //


    run_t.gTimer_compare_temp_value=0;
    gpro_t.set_temp_value_success_flag=1;
    run_t.gTimer_display_dht11 = 2;
    run_t.set_temperature_special_value =0;

    TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
    compare_temp_value();


}




