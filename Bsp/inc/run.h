#ifndef __RUN_H_
#define __RUN_H_
#include "main.h"


typedef enum WIFI_STATE_T{
   
    WIFI_POWER_ON = 0x80,
    WIFI_POWER_OFF=0X81,
    WIFI_MODE_1=0X08,   //state ->normal works
    WIFI_MODE_2=0X18,   //state->sleeping works
    WIFI_KILL_ON=0x04,  //Anion(plasma)
    WIFI_KILL_OFF=0x14,
    WIFI_PTC_ON = 0x02, 
    WIFI_PTC_OFF = 0x12,
    WIFI_SONIC_ON = 0x01,       //ultrasonic
    WIFI_SONIC_OFF = 0x11,
    WIFI_TEMPERATURE = 0xA0,
    WIFI_POWER_ON_NORMAL= 0xB0,

	PTC_WARNING= 0xE1,
	FAN_WARNING = 0xE2,

	FAN_REMOVE_WARNING = 0xF2
    


}wifi_state_t;


typedef enum TIMING_T{

   timing_donot,
   timing_success ,
   timing_power_off,
   timing_null
}timing_t;


typedef enum _Signal{
  
	PANEL_DATA=0x01,
    ORDER_DATA,
    ANSWER_DATA,
   
    POWER_NULL

}signal_t;

typedef enum _WIFI_POWER_STATE_{
	WIFI_POWER_ON_ITEM = 0x01,
	WIFI_POWER_OFF_ITEM,
	WIFI_POWER_NULL

}wifi_power_state;



typedef struct __RUN{

 
   uint8_t gPower_On;
  
   uint8_t gmode_flag;
   
  

   uint8_t power_off_flag;
  

    uint8_t wifi_led_fast_blink_flag;
 

 

   

   uint8_t  gPlasma;
 
   uint8_t  gDry;
  


   uint8_t  ptc_warning;
   uint8_t  fan_warning;

   //key ref
   uint8_t recoder_start_conuter_flag;

 
   //fan ref
   uint8_t gFan_level;

 
  


   //dry ref 




 

 


  

   uint8_t Timer_model_flag;
   uint8_t temperature_set_flag;

   

  
	
   uint8_t gReal_humtemp[2];
   uint8_t rx_mb_data_tag;
 
   /***/
   uint8_t 	gTimer_numbers_one_two_blink;

   volatile uint8_t set_temperature_decade_value;
   volatile   uint8_t set_temperature_unit_value;
   volatile uint8_t gTimer_time_colon;
 
 


   uint8_t gTimer_key_timing;
   uint8_t power_on_recoder_times;

   //timer timing 
 

   int8_t temporary_timer_dispTime_hours;
   int8_t temporary_timer_dispTime_minutes;
  

 
 
  


 

   uint8_t works_dispTime_minutes;
   uint8_t works_dispTime_hours;

   uint8_t gTimer_colon;
   volatile   uint8_t hours_two_decade_bit;
   volatile   uint8_t hours_two_unit_bit;
   volatile   uint8_t minutes_one_decade_bit;
   volatile   uint8_t minutes_one_unit_bit;
   uint8_t set_temperature_special_value;
   uint8_t set_timer_special_value;
   uint8_t gTimer_run_ico;
   uint8_t gTimer_fan_continue;

    
   
   

  volatile int8_t timer_dispTime_hours;
  volatile int8_t timer_dispTime_minutes;
	

	uint8_t gTimer_usart_error;

   uint8_t gTimer_smg_blink_times;

    uint8_t gTimer_timer_timing_counter ;
    uint8_t gTimer_display_dht11;
     uint8_t gTimer_compare_temp_value;
     uint8_t gTimes_time_seconds;
     
     uint8_t gTimer_key_temp_timing;

    
 
   
  


   
 }RUN_T;

extern RUN_T run_t;


void Power_Off(void);
void SplitDispose_Key(uint8_t value);



void Decode_Handler(uint8_t data);



void Power_Off_Fun(void);



void Receive_MainBoard_Data_Handler(uint8_t ulid,uint8_t cmd,uint8_t data);
void power_on_init(void);


#endif 



