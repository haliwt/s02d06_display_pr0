#ifndef __BSP_H
#define __BSP_H
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"


#include "usart.h"
#include "dma.h"
#include "app_threadx.h"
#include "iwdg.h"
#include "tim.h"
#include "gpio.h"


//BSP FUNTION
#include "bsp_smg.h"
#include "bsp_cmd_link.h"
#include "bsp_run.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_fan.h"
#include "bsp_display_dynamic.h"
#include "bsp_display.h"
#include "interrupt_manager.h"
#include "bsp_delay.h"
#include "bsp_wifi.h"
#include "bsp_power.h"
#include "bsp_usart.h"



#include "bsp_key_app.h"
#include "bsp_message.h"
#include "bsp_threadx.h"








#define  USE_Threadx      1

#define DEBUG_FALG   Enable_EventRecorder

#define Enable_EventRecorder    0


#if Enable_EventRecorder == 1
	#include "EventRecorder.h"
#endif


#if USE_Threadx == 1
	#define DISABLE_INT()    TX_INTERRUPT_DISABLE
    #define ENABLE_INT()     TX_INTERRUPT_RESTORE
#else
	/* ����ȫ���жϵĺ� */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
#endif


typedef enum{

  disp_works_times,
  disp_timer_times

}TIMER_STATE;


typedef enum power_onoff_state_t{

    power_off,
    power_on


}power_onoff_stae;


typedef enum{

  no_ai_mode,
  ai_mode

}ai_mode_typedef;

typedef struct _pro_t{

   
   uint8_t set_timer_timing_value_success;
   uint8_t set_timer_timing_doing_value;
 
   uint8_t set_up_temperature_value;
  

   uint8_t first_set_ptc_on;
   uint8_t first_rcoder_ptc_on_flag;

   uint8_t two_work_hours_flag;

  
   uint8_t answer_signal_flag;
   uint8_t set_timer_first_smg_blink_flag;
   uint8_t boot_done;
   uint8_t first_ptc_on;

   uint8_t send_ack_cmd;
   uint8_t receive_copy_cmd;
   uint8_t g_manual_shutoff_dry_flag;
  
  
   uint8_t DMA_txComplete;
   uint8_t main_board_set_timer_flag ;

   uint8_t key_disp_mode_flag ;
   uint8_t ai_flag ;
   uint8_t fan_run_one_minute; 
  
  
   uint8_t g_time_disp_colon_flag;
   uint8_t smartphone_app_timer_power_on_flag;
   uint8_t key_add_dec_pressed_flag;
   
volatile    uint8_t gTimer_wifi_led_blink ;
volatile    uint8_t gTimer_disp_mode_switch;



volatile    uint8_t  gTimer_set_temp_counter ;

 
volatile    uint8_t gTimer_counter_one_minute;
   
 volatile   uint16_t gTimer_two_hours_seconds;

   


volatile   uint16_t gTimer_4bitsmg_blink_times;
  
   




}pro_run_t;

extern pro_run_t  gpro_t;


void bsp_init(void);

void power_on_handler(void);





void power_off_run_handler(void);

void plasma_key_fun(uint8_t data);


void ai_key_fun(uint8_t data);



void bsp_dry_fun(uint8_t data);






void SetDataTemperatureValue(void);


void compare_temp_value(void);

void detected_ptc_or_fan_warning_fun(void);


void works_run_two_hours_handler(void);

void mode_key_short_fun(void);

void twoHours_works_timing(void);


#endif 


