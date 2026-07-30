#include "bsp.h"


#define KEY_MODE_SHORT   (1 << 0)
#define KEY_MODE_LONG    (1 << 1)

#define KEY_UP_SHORT     (1 << 2)
#define KEY_UP_LONG      (1 << 3)

#define KEY_DOWN_SHORT   (1 << 4)
#define KEY_DOWN_LONG    (1 << 5)

#define KEY_POWER_SHORT  (1 << 6)
#define KEY_POWER_LONG   (1 << 7)

#define KEY_MOUSE_SHORT   (1UL <<8 )

#define KEY_PLASMA_SHORT   (1UL<<9)

#define KEY_DRY_SHORT      (1UL<< 10)

#define KEY_AI_SHORT       (1UL << 11)

#define KEY_AI_LONG        (1UL << 12)




#define STACK_SIZE_UI     768//512//1792//1536//1024//1664 
#define STACK_SIZE_KEY    512//512
#define STACK_SIZE_DEC    512//
#define STACK_SIZE_EVENT  512


__attribute__((aligned(8)))  static UCHAR stack_ui_pro[STACK_SIZE_UI];
__attribute__((aligned(8)))  static UCHAR stack_key_pro[STACK_SIZE_KEY];
__attribute__((aligned(8)))  static UCHAR stack_decoder_pro[STACK_SIZE_DEC];
__attribute__((aligned(8)))  static UCHAR stack_key_event[STACK_SIZE_EVENT];


static TX_THREAD  thread_ui;
static TX_THREAD  thread_key;
static TX_THREAD  thread_decoder;
static TX_THREAD  thread_key_event;



static void ui_thread_entry(ULONG thread_input);
static void key_thread_entry(ULONG thread_input);
static void decoder_thread_entry(ULONG thread_input);
static void key_event_entry(ULONG thread_input);



TX_EVENT_FLAGS_GROUP key_event;

TX_SEMAPHORE      decoder_semaphore;


static void app_threadx_handler(void);

static void tx_thread_stack_error_handler(TX_THREAD * thread_ptr);

uint8_t ucKeyCode;
uint8_t uckey_number;
uint8_t key_power_flag,decoder_flag ;
uint8_t check_code;
uint8_t keyvalue;
/**
*@brief 
*@param
*@notice
*@retval
**/
void tx_application_define(VOID * first_unused_memory)
{

	  /* 1. 消除参数未引用警告 */
		(void)first_unused_memory;


  #if DEBUG_ENABLE
	  // --- 关键点：在创建任务之前填充魔术字 ---
	  memset(stack_ui_pro,0xEF,sizeof(stack_ui_pro));

	  memset(stack_key_pro,0xEF,sizeof(stack_key_pro));
	  memset(stack_decoder_pro,0xEF,sizeof(stack_decoder_pro));
	  memset(stack_key_event,0xEF,sizeof(stack_key_event));
  #endif 

  /* 3. 注册堆栈错误回调（推荐保持） */
 
   tx_thread_stack_error_notify(tx_thread_stack_error_handler);

  app_threadx_handler();

}

/**********************************************************************************************************
*   FunctionName: static void vTaskRunPro(void *pvParameters)
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  

*   priority : 1  (数priority is 2,number is large priority is large值越低，这个跟uCOS相反)
**********************************************************************************************************/
static void decoder_thread_entry(ULONG thread_input)
{  
    (void)thread_input;
	
	

    while(1)
    {

        if(tx_semaphore_get(&decoder_semaphore,TX_WAIT_FOREVER)==TX_SUCCESS){
			/* 接收到消息，检测那个位被按下 */

	       decoder_handler();
			    
		}

       }

 }

 
/**********************************************************************************************************
*	Function Name: static void vTaskRunPro(void *pvParameters)
*	Function: 
*	Input Ref: pvParameters 是在创建该任务时传��的形参
*	Return Ref:
*   priority: 2  (数值越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void ui_thread_entry(ULONG thread_input)
{

	(void)thread_input;
	static uint8_t power_on_theFirst_times;
 
	while(1)
    {

	
    if(run_t.gPower_On == power_on){
		

	   ui_task();//power_on_run_handler();
     
       //Display_TimeColon_Blink_Fun();
	
      // set_timer_fun_led_blink();
       ///wifi_connect_state_fun();
	  
	 
       if(power_on_theFirst_times < 10 && (gpro_t.set_timer_timing_doing_value==0 || gpro_t.set_timer_timing_doing_value==3)){
         power_on_theFirst_times ++;
         Display_DHT11_Value();

       }
    }
	 else{
        LL_IWDG_ReloadCounter(IWDG);
	    power_off_run_handler();

	  }

      tx_thread_sleep(1);
     } 
  }
/**********************************************************************************************************
*
*	Function Name: 
*	Function: 
*	Input Ref: pvParameters 是在创建该任务时传递的形参
*	Return Ref:
*	priority: 3  (数值越小优先级越低，这个跟uCOS相反)
*
**********************************************************************************************************/
uint16_t mode_cnt = 0;
uint16_t up_cnt = 0;
uint16_t down_cnt = 0;
uint16_t power_cnt = 0;
uint16_t  plasma_cnt  =0 ;
uint16_t  ptc_cnt =0;
uint16_t  mouse_cnt = 0;
uint16_t  ai_cnt = 0;

const uint16_t LONG_PRESS_TIME = 40;   //20ms * 100= 2000ms



static void key_thread_entry(ULONG thread_input)
{
	
    (void)thread_input;
#if 0
	static uint16_t mode_cnt = 0;
    static uint16_t up_cnt = 0;
    static uint16_t down_cnt = 0;
    static uint16_t power_cnt = 0;
	static uint16_t  plasma_cnt  =0 ;
	static uint16_t  ptc_cnt =0;
	static uint16_t  mouse_cnt = 0;
	static uint16_t  ai_cnt = 0;

    const uint16_t LONG_PRESS_TIME = 40;   //20ms * 100= 2000ms
   #endif 


	
    while(1)
    {
      
		/* 接收到消息，检测那个位被按下 */
		if(POWER_KEY_VALUE()==KEY_DOWN ){

			power_cnt++;
			if(power_cnt == LONG_PRESS_TIME && run_t.gPower_On == power_on){
			    tx_event_flags_set(&key_event, KEY_POWER_LONG, TX_OR);
			}

		}
		else if(power_cnt > 0 && POWER_KEY_VALUE() == KEY_UP){

			if(power_cnt < LONG_PRESS_TIME)
			tx_event_flags_set(&key_event, KEY_POWER_SHORT, TX_OR);

			power_cnt = 0;
		}
		else if(MODEL_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){   /* 接收到消息，检测那个位被按下 */
			mode_cnt++;
			if(mode_cnt == LONG_PRESS_TIME){

			tx_event_flags_set(&key_event, KEY_MODE_LONG, TX_OR);

			}

		}
		else if(mode_cnt > 0 && MODEL_KEY_VALUE() == KEY_UP){
			if(mode_cnt < LONG_PRESS_TIME){
			    tx_event_flags_set(&key_event, KEY_MODE_SHORT, TX_OR);
			}
			mode_cnt = 0;

		}
		else if(DEC_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){

			down_cnt++;

		}
		else  if(down_cnt > 0 && DEC_KEY_VALUE()  == KEY_UP){
			if(down_cnt < LONG_PRESS_TIME)
			tx_event_flags_set(&key_event, KEY_DOWN_SHORT, TX_OR);

			down_cnt = 0;
		}
		else if(ADD_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){   /* 接收到消息，检测那个位被按下 */
			up_cnt++;
		}
		else if(up_cnt > 0 && ADD_KEY_VALUE()== KEY_UP){

			if(up_cnt < LONG_PRESS_TIME)
			tx_event_flags_set(&key_event, KEY_UP_SHORT, TX_OR);

			up_cnt = 0;
		}
		else if(PLASMA_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){   /* 接收到消息，检测那个位被按下 */

			plasma_cnt ++ ;
		}
		else if(plasma_cnt > 0 && PLASMA_KEY_VALUE() == KEY_UP){

			if(plasma_cnt < LONG_PRESS_TIME)
				tx_event_flags_set(&key_event, KEY_PLASMA_SHORT, TX_OR);

			plasma_cnt = 0;
		}	
		else if(DRY_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){   /* 接收到消息，检测那个位被按下 */

			ptc_cnt ++ ;
	

		}
		else if(ptc_cnt > 0 && DRY_KEY_VALUE() == KEY_UP){

			if(ptc_cnt < LONG_PRESS_TIME)
			tx_event_flags_set(&key_event, KEY_DRY_SHORT, TX_OR);

			ptc_cnt = 0;
		}	
		else if(MOUSE_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){   /* 接收到消息，检测那个位被按下 */

			mouse_cnt ++;

		}
		else if(mouse_cnt > 0 && MOUSE_KEY_VALUE() == KEY_UP){

			if(mouse_cnt < LONG_PRESS_TIME)
			tx_event_flags_set(&key_event, KEY_MOUSE_SHORT, TX_OR);

			mouse_cnt = 0;
		}	
		else if(WIFI_KEY_VALUE()==KEY_DOWN &&  run_t.gPower_On == power_on ){

			ai_cnt ++ ;

			if(ai_cnt == LONG_PRESS_TIME ){
			tx_event_flags_set(&key_event, KEY_AI_LONG, TX_OR);
			}
        }
		else if(ai_cnt > 0 && WIFI_KEY_VALUE() == KEY_UP){

              ai_cnt = 0;
		}

		tx_thread_sleep(3);

	}


 }

	  #if 0

		if(gpro_t.mode_Key_long_counter < 200){

		     gpro_t.mode_Key_long_counter++;

	    if( run_t.wifi_led_fast_blink==1 && gpro_t.mode_Key_long_counter > 30 &&  gpro_t.mode_Key_long_counter < 200){
			 gpro_t.mode_Key_long_counter=200;
			 gpro_t.gTimer_mode_long_key_counter=0;

		      gpro_t.set_timer_timing_doing_value = 1;
			   gpro_t.key_add_dec_pressed_flag =0;
			   run_t.gTimer_key_timing = 0;
			   run_t.gTimer_smg_blink_times =0;
			   gpro_t.set_timer_first_smg_blink_flag=0;
			    gpro_t.mode_key_shot_flag=0xff;

			  SendData_Set_Command(0x06,0x01);
	          tx_thread_sleep(2);
		    


	    }
	    else if(gpro_t.mode_Key_long_counter > 79 && gpro_t.mode_Key_long_counter < 200 && run_t.wifi_led_fast_blink==0){

		     gpro_t.mode_Key_long_counter=200;
			 key_t.key_mode_flag = 0;
		     gpro_t.gTimer_mode_long_key_counter=0;
		
		     gpro_t.set_timer_timing_doing_value = 1;
			   gpro_t.key_add_dec_pressed_flag =0;
			   run_t.gTimer_key_timing = 0;
			   run_t.gTimer_smg_blink_times =0;
			   gpro_t.set_timer_first_smg_blink_flag=0;
			    gpro_t.mode_key_shot_flag=0xff;

			    SendData_Set_Command(0x06,0x01);
	             tx_thread_sleep(2);
		
			 
           }
        }

		if(gpro_t.mode_Key_long_counter==200) key_t.key_mode_flag = 0;
		else
           key_t.key_mode_flag = 1;
       #endif  
		 
   



/**********************************************************************************************************
*
*	Function Name: 
*	Function: 
*	Input Ref: pvParameters 是在创建该任务时传递的形参
*	Return Ref:
*	priority: 3  (数值越小优先级越低，这个跟uCOS相反)
*
**********************************************************************************************************/
static void key_event_entry(ULONG thread_input)
{
     (void)thread_input;
	  ULONG flags;
      UINT status;

	while(1){
      status = tx_event_flags_get(&key_event,
                           0xFFFFFFFF,
                           TX_OR_CLEAR,
                           &flags,
                           TX_WAIT_FOREVER);//TX_NO_WAIT);//TX_WAIT_FOREVER);//

	 if(status == TX_SUCCESS){
                           
        if(flags & KEY_POWER_SHORT){

       key_t.key_power_flag++;
	   power_key_handler() ;

	}
    else if(flags & KEY_MODE_SHORT){

          key_t.key_mode_flag++;
          gpro_t.mode_Key_long_counter=0;
	      SendData_Set_Command(0x06,0x01);
	      tx_thread_sleep(2);
          mode_key_handler();
		  mode_key_short_fun();
		  
	}
	else if(flags & KEY_MODE_LONG){

         SendData_Set_Command(0x06,0x01);
	     tx_thread_sleep(2);

		 gpro_t.set_timer_timing_doing_value = 1;
		 gpro_t.key_add_dec_pressed_flag =0;
		 run_t.gTimer_key_timing = 0;
		 run_t.gTimer_smg_blink_times =0;
		 gpro_t.set_timer_first_smg_blink_flag=0;
		 gpro_t.mode_key_shot_flag=0xff;

	}
	else if( flags & KEY_UP_SHORT){
       	key_t.key_add_flag++;
		SendData_Set_Command(0x06,0x01);
	    tx_thread_sleep(2);
		gpro_t.gTimer_set_temp_counter = 0;
		key_add_fun();
	}
	else if( flags & KEY_DOWN_SHORT){
   	  key_t.key_dec_flag++;
	  SendData_Set_Command(0x06,0x01);
	  tx_thread_sleep(2);
	  gpro_t.gTimer_set_temp_counter = 0;
	  key_dec_fun();

	}
	else if(flags & KEY_PLASMA_SHORT){
		
		plasma_key_handler() ;

	}
	else if(flags & KEY_MOUSE_SHORT) {
	
		mouse_key_handler() ;
	}
	else if(flags & KEY_DRY_SHORT) {
		
		dry_key_handler() ;

	}
    else if( flags & KEY_AI_LONG){
       
    
	     // 处理WiFi键
       
	     SendData_Set_Command(0x05,0x01); // link wifi of command .
          tx_thread_sleep(2);
          run_t.connect_wifi_state = wifi_connect_null;
           run_t.gTimer_wifi_connect_counter =0; //120s counte start
           

	 }
	
	 }
	 
	}
}

#if 0
	
	if(gpro_t.mode_Key_long_counter==200 && gpro_t.gTimer_mode_long_key_counter > 1){
           gpro_t.mode_key_shot_flag =0xff;

           gpro_t.mode_Key_long_counter=0;

	
	}
	
    if(gpro_t.done_set_temp_flag == 1 && gpro_t.gTimer_set_temp_counter > 2 ){

	     gpro_t.done_set_temp_flag = 0;

	     if(gpro_t.set_up_temperature_value > run_t.gReal_humtemp[1]){
              
              run_t.gDry = 1;
			  LED_DRY_ON();
		 }
		 else{

             run_t.gDry =0;
			 LED_DRY_OFF();
		 }
		 
         sendCmdNote_to_Data(0x2A,gpro_t.set_up_temperature_value);
         tx_thread_sleep(2);

#endif 



/**
*@brief 
*@param prority numbers small this power high.
*@notice
*@retval
**/
void app_threadx_handler(void)
{

  //tx_event_flags_create(&commEventFlags,"commEventFlags");
  tx_semaphore_create(&decoder_semaphore,"decoderSemaphore",0);
  tx_event_flags_create(&key_event, "key_event");

  tx_thread_create(&thread_decoder,
  					"DecoderPro",
  					decoder_thread_entry,
  					0,
  					stack_decoder_pro,
  					STACK_SIZE_DEC,
  					2,
  					2,
  					TX_NO_TIME_SLICE,
  					TX_AUTO_START);


   tx_thread_create(&thread_ui,
   					"MsgPro",
   					ui_thread_entry,
   					0,
   					stack_ui_pro,
   					STACK_SIZE_UI,
   					3,
   					3,
   					TX_NO_TIME_SLICE,
   					TX_AUTO_START);

	tx_thread_create(&thread_key,
					"KeyPro",
					key_thread_entry,
					0,
					stack_key_pro,
					STACK_SIZE_KEY,
					0,
					0,
					TX_NO_TIME_SLICE,
					TX_AUTO_START);
	
	 tx_thread_create(&thread_key_event, 		   /* 任务控制块地址 */	  
					 "KeyEvent",				    /* 任务名 */
					  key_event_entry,				/* 启动任务函数地址 */
					  0,							/* 传递给任务的参数 */
					  stack_key_event,				/* 堆栈基地址 */
					  STACK_SIZE_EVENT,				/* 堆栈空间大小 */  
					  1,							/* 任务优先级*/
					  1,							/* 任务抢占阀值 */
					  TX_NO_TIME_SLICE, 			/* 不开启时间片 */
					  TX_AUTO_START);				/* 创建后立即启动 */
   
}



/*************************************************************************
*
*	Funtion Name:
*	Function: 
*	Input Ref: 
*	Return Ref:
*
**************************************************************************/
void tx_thread_semaphore_isr(void)
{
   //tx_event_flags_set(&commEventFlags,(1<<9),TX_OR);
     tx_semaphore_put(&decoder_semaphore);
}


void tx_application_stack_error_handler(TX_THREAD *thread_ptr)
{
     (void)thread_ptr;
    //printf("stack overflow in thread:%s \n", thread_ptr->tx_thread_name );
}

void tx_thread_stack_error_handler(TX_THREAD * thread_ptr)
{
   (void)thread_ptr;
   tx_thread_sleep(20);

}



