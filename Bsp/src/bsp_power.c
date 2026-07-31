#include "bsp.h"

#define THREADX_TICK_MS 10

#define DEFAULT_TEMP    40 

// 假设 ThreadX 1 Tick = 10ms (THREADX_TICK_MS = 10)
#define MS_TO_TICKS(ms)  ((ms) / THREADX_TICK_MS)


RUN_T run_t;

typedef void (*task_handler_t)(void);

typedef struct {
    task_handler_t task_handler; // 任务回调函数
    uint32_t period;              // 运行周期 (Ticks)
    uint32_t last_tick;           // 上次运行时间 (Ticks)
} task_t;



// 任务函数前置声明
static void task_ui_key(void);
static void task_keys_and_refresh(void);
static void task_dht11_display(void);
static void task_two_hours_timing(void);
static void task_send_version(void);
static void task_blink_colon(void);
static void task_compare_temp(void);

// 任务配置表 (Table-Driven)
static task_t g_ui_tasks[] = {
    { task_ui_key,             MS_TO_TICKS(10),   0 }, // 100m  
    { task_keys_and_refresh,   MS_TO_TICKS(50),   0 }, // 50ms 刷新UI和按键
    { task_blink_colon,        MS_TO_TICKS(500),  0 }, // 500ms 冒号闪烁
    { task_dht11_display,      MS_TO_TICKS(300),  0 }, // 300ms DHT11刷新
    { task_compare_temp,       MS_TO_TICKS(3000), 0 }, // 3s 控温比较
    { task_two_hours_timing,   MS_TO_TICKS(1200), 0 }, // 1s 运行计时
    { task_send_version,       MS_TO_TICKS(2000), 0 }, // 2s 发送版本号
};

#define TASK_NUM (sizeof(g_ui_tasks) / sizeof(task_t))






typedef enum{

  PTC_STATE_OFF = 0,
  PTC_STATE_ON  = 1
}PTC_State;

static PTC_State ptc_state = PTC_STATE_OFF;


typedef struct {

    uint8_t boot_done;            // 开机流程是否完成
    uint32_t ts_boot;             // 开机时间戳

    uint32_t ts_ui;               // UI 刷新周期
    uint32_t ts_dht11;            // DHT11 刷新周期
    uint32_t ts_version;          // 版本号发送周期
    uint32_t ts_two_hours;        // 两小时计时周期

    uint32_t ts_key;              // 按键去抖周期
    uint32_t ts_blink;            // 温度闪烁周期

	uint32_t ts_colon;     // 时间冒号闪烁
    uint32_t ts_timer_led; // 定时器 LED 闪烁
    uint32_t ts_wifi;      // WiFi 状态刷新
    uint32_t ts_compare_temp;    //

} ui_t;

static ui_t ui;


uint8_t power_on_off_flag;

void Power_Off(void);


static void set_temperature_compare_value_fun(void);
/******************************************************************************
	*
	*Function Name:void RunPocess_Command_Handler(void)
	*Funcion: display pannel run of process 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
uint32_t get_timestamp_ms(void)
{
    return tx_time_get()* THREADX_TICK_MS;   // 返回 tick 数
}
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
	run_t.gMouse =0;
	
			
}

/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void Power_On_Fun(void)
{
                
   static uint8_t hour_decade,hour_unit,minutes_one,minutes_two;

  
   if(gpro_t.smartphone_app_timer_power_on_flag == 0){
		run_t.gPlasma=1;
		run_t.gDry =1;
        run_t.gMouse = 1;
		run_t.ai_model_flag =ai_mode;
		

   	}
    else{

	    run_t.ai_model_flag =ai_mode;

	}
	run_t.time_led_flag=1;
	gpro_t.set_up_temperature_value =40;
	Power_ON_Led();
	

	run_t.fan_warning=0;
	run_t.ptc_warning=0;
 
	
    run_t.gTimer_timer_seconds_counter=0;
    gpro_t.set_timer_timing_value_success =0 ;
   
    run_t.timer_dispTime_hours =0;
	run_t.timer_dispTime_minutes=0;
		
        

	  hour_decade=0;
	  hour_unit=0;
    
	  minutes_one = 0;
      minutes_two = 0;
	  
	   SMG_POWER_ON(); //WT.EDIT 2023.03.02
      

	  run_t.hours_two_unit_bit = hour_unit;
	  run_t.minutes_one_decade_bit =  minutes_one;
      
	 TM1639_Write_4Bit_Time(hour_decade,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,minutes_two,0);
     //Display_DHT11_Value(); //WT.EIDT 2025.05.10
    
}


/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*
	*
	*
	*
************************************************************************/
void Power_Off_Fun(void)
{
	run_t.gPlasma=0;
	run_t.gDry =0;
	run_t.gMouse = 0;
} 


/**
*@brief 
*@param
*@notice
**/
static void ui_event_power_on(void)
{
    Power_On_Fun();

    run_t.gTimer_time_colon = 0;
    run_t.set_temperature_decade_value = 40;

    run_t.gTimer_display_dht11 = 20;

    gpro_t.set_timer_timing_doing_value = 0;
    gpro_t.g_manual_shutoff_dry_flag = 0;
    run_t.wifi_led_fast_blink = 0;

    run_t.timer_dispTime_hours = 0;
    run_t.timer_dispTime_minutes = 0;
    run_t.works_dispTime_hours = 0;
    run_t.works_dispTime_minutes = 0;
    run_t.gTimer_timing_seconds_counter = 0;

    gpro_t.gTimer_two_hours_seconds = 0;
    gpro_t.two_work_hours_flag = 0;
	


    gpro_t.key_disp_mode_flag = 0xff;
    gpro_t.ai_flag = ai_mode;
    key_t.disp_smg_mode_flag = disp_works_times;

	gpro_t.first_ptc_on=0;

    gpro_t.fan_run_one_minute = 0;
    gpro_t.gTimer_counter_one_minute = 0;

    SendData_Set_Command(0x11,0x01);

    ui.ts_boot = get_timestamp_ms();
    ui.ts_ui = ui.ts_boot;
    ui.ts_dht11 = ui.ts_boot;
    ui.ts_version = ui.ts_boot;
    ui.ts_two_hours = ui.ts_boot;

    gpro_t.boot_done = 0xfe;
}


#if 0

/**
*@brief 
*@param
*@notice
**/

static void ui_task_dht11(uint32_t now)
{
    if (now - ui.ts_dht11 >= 300) {
        disp_dht11_value();
        ui.ts_dht11 = now;
    }
}

/**
*@brief 
*@param
*@notice
**/

static void ui_task_two_hours(uint32_t now)
{
    if (now - ui.ts_two_hours >= 2000) {
        twoHours_works_timing();
        ui.ts_two_hours = now;
    }
}
/**
*@brief 
*@param
*@notice
**/

static void ui_task_version(uint32_t now)
{
    if (now - ui.ts_version >= 2000) {
        SendData_Set_Command(0x0F,0x02);
        ui.ts_version = now;
    }
}
/**
*@brief 
*@param
*@notice
**/
static void ui_task_keys(void)
{

    
   if(gpro_t.key_model_short_flag == 1 &&  gpro_t.gTimer_disp_mode_switch < 3){

         mode_key_short_fun();
         return ;
   }

   if(gpro_t.key_model_short_flag == 1 &&  gpro_t.gTimer_disp_mode_switch > 2){

      gpro_t.key_model_short_flag  =0;

   }

   if (gpro_t.set_timer_timing_doing_value == 1 &&  run_t.ptc_warning == 0 &&  run_t.fan_warning == 0) {

        Set_TimerTiming_Number_Value();

	    return ;
    }
    
     disp_smg_blink_set_tempeature_value();
   
}


/**
*@brief 
*@param
*@notice
**/
static void ui_task_refresh(uint32_t now)
{
    // UI 刷新周期：50ms
    if (now - ui.ts_ui < 50)
        return;

    ui.ts_ui = now;

    // 1. 有告警时优先显示告警
    if (run_t.ptc_warning || run_t.fan_warning) {
        Warning_Error_Numbers_Fun();
        return;
    }


    // 3. 特殊温度设置闪烁显示
    if (gpro_t.set_timer_timing_doing_value == 0 && run_t.set_temperature_special_flag ==1 ) {

        disp_smg_blink_set_tempeature_value();
        return;
    }

    // 4. 正常显示工作时间（你原来的 Display_SmgTiming_Value）
    if (gpro_t.set_timer_timing_doing_value == 0){
             
        if(gpro_t.key_model_short_flag == 1) return;
		
        Display_SmgTiming_Value();
        return;
    }

	
}

static void ui_task_colon(uint32_t now)
{

    if(gpro_t.set_timer_timing_doing_value == 1) return ;
	if (now - ui.ts_colon >= 500) {
        Display_TimeColon_Blink_Fun();
        ui.ts_colon = now;
    }
}

static void ui_task_timer_led(uint32_t now)
{
    if (now - ui.ts_timer_led >= 200) {
        set_timer_fun_led_blink();
        ui.ts_timer_led = now;
    }
}

static void ui_task_wifi(uint32_t now)
{
    if (now - ui.ts_wifi >= 300) {
        wifi_connect_state_fun();
        ui.ts_wifi = now;
    }
}

static void ui_task_compare_temperature_value(uint32_t now)
{
      if(now -ui.ts_compare_temp > 3000){

	     set_temperature_compare_value_fun();
	     ui.ts_compare_temp  = now;

	  }


}

/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*
	*
	*
	*
************************************************************************/
void ui_task(void)
{
    uint32_t now = get_timestamp_ms();
	
	LL_IWDG_ReloadCounter(IWDG);

    // 开机事件（只执行一次）
    if (gpro_t.boot_done < 0x08) {
        ui_event_power_on();
    }
	

    // 按键事件
    ui_task_keys();

    // UI 刷新
    ui_task_refresh(now);

    // DHT11 显示
    ui_task_dht11(now);

    // 两小时计时
    ui_task_two_hours(now);

    // 版本号发送
    ui_task_version(now);

	ui_task_colon(now);      // 新增
   // ui_task_timer_led(now);  // 新增
   // ui_task_wifi(now);       // 新增

	ui_task_compare_temperature_value(now);

	
}

#else

/**
*@brief 
*@param
*@notice
**/
static void task_ui_key(void)
{
   if(gpro_t.key_model_short_flag == 1 &&  gpro_t.gTimer_disp_mode_switch < 3){

         mode_key_short_fun();
         return ;
   }

   if(gpro_t.key_model_short_flag == 1 &&  gpro_t.gTimer_disp_mode_switch > 2){

      gpro_t.key_model_short_flag  =0;

   }

   if (gpro_t.set_timer_timing_doing_value == 1 &&  run_t.ptc_warning == 0 &&  run_t.fan_warning == 0) {

        Set_TimerTiming_Number_Value();

	    return ;
    }
    
     disp_smg_blink_set_tempeature_value();




}


/**
*@brief 
*@param
*@notice
**/
static void task_keys_and_refresh(void)
{
	
	// 1. 有告警时优先显示告警
		if (run_t.ptc_warning || run_t.fan_warning) {
			Warning_Error_Numbers_Fun();
			return;
		}
	
	
		// 3. 特殊温度设置闪烁显示
		if (gpro_t.set_timer_timing_doing_value == 0 && run_t.set_temperature_special_flag ==1 ) {
	
			disp_smg_blink_set_tempeature_value();
			return;
		}
	
		// 4. 正常显示工作时间（你原来的 Display_SmgTiming_Value）
		if (gpro_t.set_timer_timing_doing_value == 0){
				 
			if(gpro_t.key_model_short_flag == 1) return;
			
			Display_SmgTiming_Value();
			return;
		}

}
/**
*@brief 
*@param
*@notice
**/
static void task_dht11_display(void)
{
    disp_dht11_value();

}
/**
*@brief 
*@param
*@notice
**/
static void task_two_hours_timing(void)
{
	 twoHours_works_timing();

}
/**
*@brief 
*@param
*@notice
**/
static void task_send_version(void)
{
	 SendData_Set_Command(0x0F,0x02);
	 
}
/**
*@brief 
*@param
*@notice
**/
static void task_blink_colon(void)
{
    Display_TimeColon_Blink_Fun();
	if(run_t.wifi_led_fast_blink==1 && run_t.connect_wifi_state == wifi_connect_null && run_t.gTimer_wifi_connect_counter > 125 ){
	    run_t.wifi_led_fast_blink=0;

	}
	if(run_t.wifi_led_fast_blink==1 && run_t.connect_wifi_state == wifi_connect_success){

	    run_t.wifi_led_fast_blink=0;

	}
}
/**
*@brief 
*@param
*@notice
**/
static void task_compare_temp(void)
{
    set_temperature_compare_value_fun();

}

/************************************************************************************************
*
*Function Name:void power_on_cycle_handler(void)
*Function:
*Input Ref:
*Return Ref:
*
*************************************************************************************************/
void ui_task(void)
{
	uint32_t current_tick = tx_time_get();
	uint8_t i ;
	uint32_t init_tick;

   LL_IWDG_ReloadCounter(IWDG);

    // 开机事件（只执行一次）
    if (gpro_t.boot_done < 0x08) {
        ui_event_power_on();
		// 2. 初始化所有任务的 last_tick 镜像
	    init_tick = tx_time_get();
	    for (i = 0; i < TASK_NUM; i++) {
	        g_ui_tasks[i].last_tick = init_tick;
	    }
    }
	else{
	

	for (uint8_t i = 0; i < TASK_NUM; i++) {
		if ((current_tick - g_ui_tasks[i].last_tick) >= g_ui_tasks[i].period){
			// 防饱和截断：若卡顿超过 2 个周期，直接重置到当前 tick，放弃追赶
			if ((current_tick - g_ui_tasks[i].last_tick) > (g_ui_tasks[i].period * 2)) 
			{
				g_ui_tasks[i].last_tick = current_tick;
			} 
			else 
			{
				// 锁相滚动累加，消除长期运行漂移
				g_ui_tasks[i].last_tick += g_ui_tasks[i].period;
			}

			// 执行任务回调
			if (g_ui_tasks[i].task_handler != NULL) 
			{
				g_ui_tasks[i].task_handler();
			}
		}
	}

	}
}

#endif


/************************************************************************************************
*
*Function Name:void set_temperature_compare_value_fun(void)
*Function:
*Input Ref:
*Return Ref:
*
*************************************************************************************************/
static void set_temperature_compare_value_fun(void)
{
   // static uint8_t counter;
	uint8_t target_temp,real_temp;

    if(run_t.fan_warning ==1 || run_t.ptc_warning ==1 || gpro_t.g_manual_shutoff_dry_flag == 1\
		|| run_t.set_temperature_special_flag ==1)return ;


	real_temp = run_t.gReal_humtemp[1];//gpro_t.temp_real_value;
	target_temp = gpro_t.set_up_temperature_value;//gpro_t.key_set_temperature;
   

    

	if(real_temp >= target_temp){

           run_t.gDry = 0;
		   LED_DRY_OFF();
	       
		  ptc_state = PTC_STATE_OFF ;
	      gpro_t.first_set_ptc_on  = 1;
		  SendData_Set_Command(0x22,1);
	      tx_thread_sleep(2);
  
		  return ;
	}

	if(ptc_state == PTC_STATE_OFF){

	    if(gpro_t.first_ptc_on==0 || gpro_t.first_ptc_on==1){

			if(real_temp < target_temp){

			   if(gpro_t.g_manual_shutoff_dry_flag==0){
	               run_t.gDry = 1;
				    LED_DRY_ON();
				   ptc_state = PTC_STATE_ON ;
				   if(gpro_t.first_ptc_on==1)gpro_t.first_set_ptc_on  = 2;
				   
				   SendData_Set_Command(0x22,1);
				   tx_thread_sleep(2);
			   	}
			}
		}
		else{
            if(real_temp < (target_temp -2)){

			  if(gpro_t.g_manual_shutoff_dry_flag==0){

                run_t.gDry = 1;
				LED_DRY_ON();
	       
				ptc_state = PTC_STATE_ON ;
			   SendData_Set_Command(0x22,1);
			   tx_thread_sleep(2);
			  	}

			}


		}

	}
	else{
        if(real_temp >= target_temp){
            run_t.gDry = 0;
			 LED_DRY_OFF();
	        ptc_state = PTC_STATE_OFF ;
		   SendData_Set_Command(0x22,0);
	       tx_thread_sleep(2);
		}

	}


	
}

