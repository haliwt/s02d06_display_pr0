#ifndef __BSP_H
#define __BSP_H



#include "main.h"


#include "usart.h"



#include "smg.h"
#include "cmd_link.h"
#include "run.h"
#include "key.h"
#include "led.h"
#include "bsp_fan.h"
#include "bsp_display_dynamic.h"
#include "interrupt_manager.h"
#include "delay.h"
#include "display.h"
#include "bsp_decoder.h"
#include "bsp_freertos.h"



#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"



#define  USE_FreeRTOS      1

#if USE_FreeRTOS == 1
	//#include "FreeRTOS.h"
	///#include "task.h"
	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* ����ȫ���жϵĺ� */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
#endif


typedef enum{

  WORKS_TIME,
  TIMER_SUCCESS,
  SET_TIMER_ITEM
 

}TIMER_STATE;


typedef enum power_onoff_state_t{

    power_off= 0,
    power_on =0x01


}power_onoff_stae;

typedef struct _pro_t{

   uint8_t gmouse;
 
   uint8_t wifi_link_net_success;
   uint8_t disp_rx_cmd_done_flag;
   uint8_t disp_timer_or_time_mode;
   uint8_t manual_turn_off_ptc_flag ;
   uint8_t  key_long_wifi_flag;
   

   uint8_t temp_switch_on_flag;
   uint8_t  temp_switch_off_flag;
   
   uint8_t set_timer_timing_value_success;
   uint8_t set_timer_timing_doing_value;
   uint8_t set_temp_value_success_flag;
   uint8_t set_up_temperature_value;
   
   uint8_t answer_signal_flag;
  
   uint8_t smart_phone_app_power_on_flag ;
   uint8_t get_beijing_time_flag;

   uint8_t gTimer_wifi_led_blink_time ;
   uint8_t gTimer_wifi_led_blink;
   uint8_t gTimer_power_mode_key_long;
   uint16_t gTimer_4bitsmg_blink_times;
  
}pro_run_t;

extern pro_run_t  gpro_t;


void bsp_init(void);

void power_on_handler(void);

void power_off_run_handler(void);

void mode_key_handler(void);

void mouse_on_off_handler(void);

void key_add_fun(void);

void key_dec_fun(void);

void mode_key_fun(void);

void compare_temp_value(void);




void mode_key_long_fun(void);


void ptc_on_off_handler(void);

void plasma_on_off_handler(void);


void smartphone_app_power_on_fun(void);


#endif



