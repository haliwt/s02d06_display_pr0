#include "bsp.h"

// --- 1. 定义任务的时间周期（单位：毫秒，假设基础Tick为1ms） ---
#define PERIOD_KEY_MODE_STATE      3    // 10ms*2 = 20ms = 3s
#define PERIOD_DISP_TIME           120    // 10ms*100 = 2000ms = 2s
#define PERIOD_DISP_TEMP           50    //  10ms*150 = 1500ms = 1.5s
#define PERIOD_WORKS_TIME          200    //  10ms*250 = 2500ms = 2.5s
#define PERIOD_SET_TIMER           4    //   10ms * 500 = 50000ms = 5s 
#define PERIOD_SET_DISP_TIMER      100


// --- 2. 定义分时任务控制结构体 ---
typedef struct {
    uint32_t last_tick;        // 记录上一次真正运行时的系统绝对时间戳
    //uint32_t counter;       // 时间计数器
    uint32_t period;        // 任务运行周期
    void (*task_handler)(void); // 任务函数指针
} TimeSharingTask_t;

static void handler_key_short_mode(void);
static void handler_smg_disp(void);
static void handler_set_timer(void);

static void handler_disp_temperature(void);
static void handler_works_time(void);
static void handler_set_disp_timer(void);




// --- 4. 初始化分时任务表 ---
TimeSharingTask_t g_tasks[] = {
    {0, PERIOD_KEY_MODE_STATE,       handler_key_short_mode},
    {0, PERIOD_DISP_TIME,             handler_smg_disp},
    {0, PERIOD_DISP_TEMP,            handler_disp_temperature},
    {0, PERIOD_WORKS_TIME,           handler_works_time},
    {0, PERIOD_SET_TIMER,            handler_set_timer},
	{0, PERIOD_SET_DISP_TIMER ,      handler_set_disp_timer}
  
   
 };
	
#define TASK_NUM (sizeof(g_tasks) / sizeof(TimeSharingTask_t))


RUN_T run_t;


uint8_t power_on_off_flag;

void Power_Off(void);
static void power_on_init_handler(void);
static void power_on_cycle_handler(void) ;





/******************************************************************************
	*
	*Function Name:void RunPocess_Command_Handler(void)
	*Funcion: display pannel run of process 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void power_on_run_handler(void)
{
   if(run_t.power_on_step < 20){
	 power_on_init_handler();
   }
   else
   	  power_on_cycle_handler() ;

}

/******************************************************************************
	*
	*Function Name:
	*Funcion
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
static void power_on_init_handler(void)
{
   static uint8_t  step_state,counter_version;
 
    run_t.gTimer_time_colon =0;
	run_t.set_temperature_decade_value=40;

	run_t.gTimer_detect_mb_receive_flag =0;
	Power_On_Fun();
	run_t.gTimer_display_dht11 = 20; //at once display temperature and humidity value.
	gpro_t.set_timer_timing_doing_value = 0;
	gpro_t.g_manual_shutoff_dry_flag = 0; //allow open dry function .
	run_t.wifi_led_fast_blink=0;

	gpro_t.set_timer_timing_value_success=0;
	run_t.timer_dispTime_hours=0;
	run_t.timer_dispTime_minutes=0;

	if(run_t.connect_wifi_state == wifi_connect_success){

	       if(run_t.display_beijing_time_flag ==0){
				run_t.works_dispTime_hours=0;
				run_t.works_dispTime_minutes=0;
				run_t.gTimer_timing_seconds_counter =0;
			    

		    }

	}
	else{
	   
	   run_t.works_dispTime_hours=0;
	   run_t.works_dispTime_minutes=0;
	   run_t.gTimer_timing_seconds_counter =0;

	}

	gpro_t.gTimer_two_hours_seconds =0;
	gpro_t.two_work_hours_flag = 0;
	
	gpro_t.key_disp_mode_flag = 0xff;
	gpro_t.ai_flag = ai_mode; //don't AI
	key_t.disp_smg_mode_flag=disp_works_times;
	gpro_t.fan_run_one_minute =0; 
	gpro_t.gTimer_counter_one_minute =0;
    Display_DHT11_Value();
	
	// SendData_Set_Command(0x11,0x01); //notice thi is outside connect display board
	// tx_thread_sleep(1);

	run_t.power_on_step= 0xff;

}

/******************************************************************************
	*
	*Function Name:static void power_on_cycle_handler(void) 
	*Funcion: 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
static void power_on_cycle_handler(void) 
{


	 if(run_t.set_temperature_f ==1){
	 	run_t.set_temperature_f++;
         direct_comparison_temp();
     }

       // 获取当前系统的绝对时间戳
      uint32_t current_tick = tx_time_get();

      // 第二步：通过时间片轮询核心算法，分时调用各个功能模块
	   for (uint8_t i = 0; i < TASK_NUM; i++) {
		   //g_tasks[i].counter++; // 基础 Tick 自增
		   if ((current_tick - g_tasks[i].last_tick) >= g_tasks[i].period) {
		   
		        // 滚动更新该任务的历史时间戳基准
               //g_tasks[i].last_tick = current_tick;
               // 改进：滚动累加周期，消除长跑下的时间漂移
               g_tasks[i].last_tick += g_tasks[i].period;
			 
			   g_tasks[i].task_handler(); // 触发对应周期的执行函数
		   
	   }

	   }


}

/**
*
*@brief
*@notice
*@param
*@retrval
*
**/
static void handler_set_timer(void)
{
  set_timer_value();

}


static void handler_key_short_mode(void)
{
    
	if(gpro_t.mode_key_shot_flag ==1  && gpro_t.gTimer_disp_mode_switch <  3){
        mode_key_short_fun();

	}
	else if(gpro_t.mode_key_shot_flag ==1 && gpro_t.gTimer_disp_mode_switch > 2){
		gpro_t.gTimer_disp_mode_switch=0;
	    gpro_t.mode_key_shot_flag = 2;
	    mode_key_short_fun();
    }
}


static void handler_set_disp_timer(void)
{

	if(run_t.ptc_warning ==1 || run_t.fan_warning ==1) return ;

	 Set_TimerTiming_Number_Value();
}

/**
*
*@brief
*@notice
*@param
*@retrval
*
**/
static void handler_smg_disp(void)
{
	Led_Panel_OnOff();

	if((gpro_t.set_timer_timing_doing_value==0 || gpro_t.set_timer_timing_doing_value==3) && gpro_t.key_disp_mode_flag ==0xff){ //WT.EDIT 2025.05.07
	if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){ //read main board ptc_warning of ref.

	     Display_SmgTiming_Value();



	}
	else{

	    Warning_Error_Numbers_Fun();

	}

	}

}

/**
*
*@brief
*@notice
*@param
*@retrval
*
**/
static void handler_disp_temperature(void)
{
	
	 comparison_value_temperature();

	 disp_dht11_value();
		 
}

static void handler_works_time(void)
{
	static uint8_t counter_version = 0;
	  	
	 twoHours_works_timing();
      if(counter_version < 5){
			counter_version ++;
		    SendData_Set_Command(0x0F,0x02); //notice thi is new version
		    tx_thread_sleep(1);
     }
	 

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
	//run_t.gWifi_led = 0;
			
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


