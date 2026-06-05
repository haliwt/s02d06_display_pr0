#include "bsp.h"

#define KEY_MODE_SHORT   (1UL << 0)
#define KEY_MODE_LONG    (1UL << 1)

#define KEY_UP_SHORT     (1UL << 2)


#define KEY_DOWN_SHORT   (1UL << 3)

#define KEY_POWER_SHORT  (1UL << 4)
#define KEY_POWER_LONG   (1UL << 5)

#define KEY_PLASMA_SHORT  (1UL << 6)

#define KEY_DRY_SHORT   (1UL << 7)

#define KEY_ULTR_SHORT  (1UL<<8)

#define KEY_MOUSE_SHORT (1UL<<9)

#define KEY_WIFI_LONG (1UL << 10)




#define STACK_SIZE_DECODER  512//384//256//256//128//1792//3072//2048//1024//896//768
#define STACK_SIZE_UI  896//1024//384//256
#define STACK_SIZE_KEY  256
#define STACK_SIZE_EVENT   512//256

#define  DEBUG_ENABLE     1



/*在 ThreadX 里，优先级数字越小，优先级越高：*/

static TX_THREAD thread_ui;
static TX_THREAD thread_start;
static TX_THREAD thread_decoder;
static TX_THREAD thread_key_event;

/* 定义信号量 */
TX_SEMAPHORE decoder_semaphore;

TX_EVENT_FLAGS_GROUP key_event;

//TX_TIMER buzzer_timer;

/*队列*/
//static TX_QUEUE uart1_rx_queue;
//static uint8_t uart1_rx_queue_buffer[UART1_RX_BUF_SIZE * sizeof(uint8_t)];


static UCHAR stack_decoder_pro[STACK_SIZE_DECODER];
static UCHAR stack_ui_pro[STACK_SIZE_UI];
static UCHAR stack_start_pro[STACK_SIZE_KEY];
static UCHAR stack_key_event[STACK_SIZE_EVENT];





static void vTaskUiPro(ULONG thread_input);
static void vTaskStart(ULONG thread_input);
static void vTaskDecoderPro(ULONG thread_input);
static void vTaskKeyEvent(ULONG thread_input);
//static void buzzer_timer_callback(ULONG input);


static void threadx_handler(void);
/* 创建任务通信机制 */
static void tx_thread_stack_error_handler(TX_THREAD *thread_ptr);


#if DEBUG_ENABLE

static void debug_stack_ui_check(void);

static void debug_stack_key_check(void);

static void debug_stack_decoder_check(void);

static void debug_stack_event_check(void);


#endif 

ULONG unused,unused_key,unused_decoder,unused_key_event ;




/**
 * @brief  :  static void vTaskStart(void *pvParameters
 * @note    
 * @param   None
 * @retval  None
 */

void tx_application_define(void *first_unused_memory)
{
  
    /* 1. 消除参数未引用警告 */
    (void)first_unused_memory;

    #if DEBUG_ENABLE
    /* 2. 只有当 stack_msg_pro 是全局定义的静态数组时，这样写才有效 */
    memset(stack_ui_pro, 0xEF, sizeof(stack_ui_pro));
    memset(stack_start_pro, 0xEF, sizeof(stack_start_pro));
	memset(stack_decoder_pro, 0xEF, sizeof(stack_decoder_pro));
	memset(stack_key_event, 0xEF, sizeof(stack_key_event));
    #endif 

    /* 3. 注册堆栈错误回调（推荐保持） */
 
    tx_thread_stack_error_notify(tx_thread_stack_error_handler);

    // 创建线程、信号量、事件组、队列
     threadx_handler();
}




uint8_t ucKeyCode;
uint8_t uckey_number;
uint8_t key_power_flag,decoder_flag ;
uint8_t check_code;
uint8_t keyvalue;



/**********************************************************************************************************
*   FunctionName: static void vTaskRunPro(void *pvParameters)
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  

*   priority : 1  (数priority is 2,number is large priority is large值越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskDecoderPro(ULONG thread_input)
{
   
	 (void)thread_input;  /* 消除未使用的参数警告 */

    while(1)
    {

       // 阻塞等待 ISR 投递
      if(tx_semaphore_get(&decoder_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS){
	

			   
		decoder_handler();
		 #if DEBUG_ENABLE
              debug_stack_decoder_check();
          #endif 
			    
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
static void vTaskUiPro(ULONG thread_input)
{
   (void)thread_input;  /* 消除未使用的参数警告 */
	
	static uint8_t power_on_theFirst_times;
 
	while(1)
    {

	 //process_keys() ;
	
    if(run_t.gPower_On == power_on){
		

	   power_on_run_handler();
     
       Display_TimeColon_Blink_Fun();
	
      
    }
	 else{

	    power_off_run_handler();

	  }
        #if DEBUG_ENABLE
              debug_stack_ui_check();
          #endif 
      tx_thread_sleep(1);
     } 
  }
/**********************************************************************************************************
*
*	Function Name: vTaskStart
*	Function: 
*	Input Ref: pvParameters 是在创建该任务时传递的形参
*	Return Ref:
*	priority: 3  (数值越小优先级越低，这个跟uCOS相反)
*
**********************************************************************************************************/
uint8_t counter_power_flag ;
static void vTaskStart(ULONG thread_input)
{
	
   (void)thread_input;  /* 消除未使用的参数警告 */

    static uint16_t mode_cnt = 0;
    static uint16_t up_cnt = 0;
    static uint16_t down_cnt = 0;
    static uint16_t power_cnt = 0;

	static uint8_t plasma_cnt = 0;
	static uint8_t dry_cnt = 0;
	static uint8_t mouse_cnt = 0;
	static uint8_t wifi_cnt = 0;

    const uint8_t LONG_PRESS_TIME = 40;   // 300 * 10ms = 3000ms

   
    while(1)
    {
      
	/* ================= POWER 键 ================= */
		  if(POWER_KEY_VALUE()==KEY_DOWN)
		  {
			  power_cnt++;
			  counter_power_flag++;
			 
		  }
		  else if(power_cnt > 0 && POWER_KEY_VALUE()==KEY_UP){
				  tx_event_flags_set(&key_event, KEY_POWER_SHORT, TX_OR);
	
			  power_cnt = 0;
		  }
          else if(MODEL_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on)
          {
            mode_cnt++;
            if(mode_cnt == LONG_PRESS_TIME){
				tx_event_flags_set(&key_event, KEY_MODE_LONG, TX_OR);
               
            }
          }
          else  if(mode_cnt > 0){
		  	   if(mode_cnt < LONG_PRESS_TIME)
                tx_event_flags_set(&key_event, KEY_MODE_SHORT, TX_OR);
            mode_cnt = 0;
          } 
          else if(PLASMA_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){

              plasma_cnt++;
           

		 }
		 else if(plasma_cnt > 0 ){
						 tx_event_flags_set(&key_event, KEY_PLASMA_SHORT, TX_OR);
				plasma_cnt = 0;

		 }
	     else if(DRY_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){ 

              dry_cnt++;
		 }
		 else  if(dry_cnt > 0 ){
					tx_event_flags_set(&key_event, KEY_DRY_SHORT, TX_OR);
			    dry_cnt = 0;


		 }
		 else if(MOUSE_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){ 
             mouse_cnt ++;

		 }
		 else if(mouse_cnt > 0 ){
					tx_event_flags_set(&key_event, KEY_MOUSE_SHORT, TX_OR);
			mouse_cnt = 0;


		 }
         else if(ADD_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on)
	     {
	            up_cnt++;
	           
	     }
	     else if(up_cnt > 0 ){
	                tx_event_flags_set(&key_event, KEY_UP_SHORT, TX_OR);

	            up_cnt = 0;
	     }
        else if(DEC_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on)
        {
            down_cnt++;
            
        }
        else  if(down_cnt > 0){
                tx_event_flags_set(&key_event, KEY_DOWN_SHORT, TX_OR);

            down_cnt = 0;
        }
		else if(WIFI_KEY_VALUE()==KEY_DOWN &&  run_t.gPower_On == power_on ){

             wifi_cnt++;
			  if(wifi_cnt == LONG_PRESS_TIME && run_t.gPower_On == power_on){
				  tx_event_flags_set(&key_event, KEY_WIFI_LONG, TX_OR);
			   }
		 }
		 else if(wifi_cnt >0){
		     wifi_cnt  =0;

		 }
        #if DEBUG_ENABLE
              debug_stack_key_check();
          #endif 
		tx_thread_sleep(5);
    }

}
/**********************************************************************************************************
 *	 Function Name: 
 *	 Function:
 *	 Input Ref: pvParameters 是在创建该任务时传的形参
 *	 Return Ref:
 *	 priority: 1  (数值越小优先级越低，这个跟uCOS相反)
 **********************************************************************************************************/
 static void vTaskKeyEvent(ULONG thread_input)
 {
   (void)thread_input;	/* 消除未使用的参数警告 */
   ULONG flags;
   UINT status;
  
 
   while(1)
   {
 
	  status = tx_event_flags_get(&key_event,    // 事件标志组控制块
							0xFFFFFFFF,          // 等待掩码 (Wait Mask)
							TX_OR_CLEAR,         // 选项 (Option)
							&flags,				 // 实际读取到的标志值
							TX_WAIT_FOREVER);//TX_NO_WAIT);//TX_WAIT_FOREVER);// 等待时间
							
	  if(status == TX_SUCCESS){
 
		 if(flags & KEY_POWER_SHORT){
              power_key_handler() ;
		 }
		 else if(flags & KEY_MODE_SHORT){
			  if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
		      SendData_Set_Command(0x06,0x01);
		      tx_thread_sleep(1);
	          mode_key_handler();
			  }
		 }
		 else if(flags & KEY_MODE_LONG){
		 	  if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
	              gpro_t.set_timer_timing_doing_value = 1;
				   gpro_t.key_add_dec_pressed_flag =0;
				   run_t.gTimer_key_timing = 0;
				   run_t.gTimer_smg_blink_times =0;
				   gpro_t.set_timer_first_smg_blink_flag=0;

				  SendData_Set_Command(0x06,0x01);
		          tx_thread_sleep(1);
		 	  }
		 }
		 else if(flags & KEY_UP_SHORT){
		 if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
			 	SendData_Set_Command(0x06,0x01);
			    tx_thread_sleep(1);
				gpro_t.gTimer_set_temp_counter = 0;
			
				key_add_fun();
			}

		 }
		 else if(flags & KEY_DOWN_SHORT){
		 	if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
			 	SendData_Set_Command(0x06,0x01);
			    tx_thread_sleep(1);
				gpro_t.gTimer_set_temp_counter = 0;
				key_dec_fun();
		 	}

		 }
		 else if(flags & KEY_PLASMA_SHORT){
		 	if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
             plasma_key_handler() ;
		 		}
		 }   
		 else if(flags & KEY_DRY_SHORT){
		 	if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
             dry_key_handler();
		 		}
		 }
		 else if(flags & KEY_MOUSE_SHORT){
		 	if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
		 	    mouse_key_handler() ;
		 	}

		 }
		 else if(flags & KEY_WIFI_LONG){
		 	if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){

	            run_t.connect_wifi_state = wifi_connect_null;
	            run_t.gTimer_wifi_connect_counter =0; //120s counte start
	            SendData_Set_Command(0x05,0x01); // link wifi of command .
	            tx_thread_sleep(1);
		 	}
		 }
		 
          #if DEBUG_ENABLE
              debug_stack_event_check();
          #endif 
		
	  }
	 
	 }
	
  }

 /**********************************************************************************************************
*	Function Name: AppTaskCreate
*	功能说明: 创建应用任务
*	Input Ref:   
*	Return Ref:
**********************************************************************************************************/
void threadx_handler(void)
{
 
  
      /* 创建信号量 */
   tx_semaphore_create(&decoder_semaphore, "DecoderSemaphore", 0);

      tx_event_flags_create(&key_event, "key_event");
   
	tx_thread_create(&thread_decoder,
					"DecoderPro",
					vTaskDecoderPro,   // 每个消息大小，这里用 1 字节
					0,
					stack_decoder_pro,
					STACK_SIZE_DECODER,
					1,
					1,
					TX_NO_TIME_SLICE,
					TX_AUTO_START);
				
	tx_thread_create(&thread_ui,                  /* 任务控制块地址 */ 
 	                 "UiPro",                     /* 任务名 */
                     vTaskUiPro,                  /* 启动任务函数地址 */
                     0,                            /* 传递给任务的参数 */
                     stack_ui_pro,                /* 堆栈基地址 */
                     STACK_SIZE_UI,               /* 堆栈空间大小 */ 
                     3,							   /* 任务优先级*/
                     3,							   /* 任务抢占阀值 , 允许它不被优先级 1-0 之间的任务抢占，除非是中断 */
                     TX_NO_TIME_SLICE,             /* 不开启时间片 */
                     TX_AUTO_START);               /* 创建后立即启动 */
 #if 1

    tx_thread_create(&thread_start,                /* 任务控制块地址 */    
    				 "Start",                      /* 任务名 */
                     vTaskStart,                   /* 启动任务函数地址 */
                     0,                            /* 传递给任务的参数 */
                     stack_start_pro,              /* 堆栈基地址 */
                     STACK_SIZE_KEY,			   /* 堆栈空间大小 */  
                     0, 						   /* 任务优先级*/
                     0, 						   /* 任务抢占阀值 */
                     TX_NO_TIME_SLICE, 			   /* 不开启时间片 */
                     TX_AUTO_START);               /* 创建后立即启动 */
  #endif 

   tx_thread_create(&thread_key_event, 			   /* 任务控制块地址 */	  
					 "KeyEvent",						/* 任务名 */
					  vTaskKeyEvent,					/* 启动任务函数地址 */
					  0,							/* 传递给任务的参数 */
					  stack_key_event,				/* 堆栈基地址 */
					  STACK_SIZE_EVENT,				/* 堆栈空间大小 */  
					  2,							/* 任务优先级*/
					  2,							/* 任务抢占阀值 */
					  TX_NO_TIME_SLICE, 			/* 不开启时间片 */
					  TX_AUTO_START);				/* 创建后立即启动 */

  #if 0
	tx_timer_create(&buzzer_timer,
		            "Buzzer_Timer",
		            buzzer_timer_callback,
		            0,
		            2,               // 2ms 后关闭
		            0,               //0-表示单次触发,不自动重载循环
		            TX_NO_ACTIVATE);  //初始化不启动
   #endif 
   
  
}

/**********************************************************************
	*
	*Function Name:void vTaskDecoder_USART1_handler(void)
	*Function:
	*Input Ref:
	*Return Ref:
	*
***********************************************************************/
void vTaskDecoder_USART1_handler(void)
{

	tx_semaphore_put(&decoder_semaphore);


}


/****************************************************************
*
*	Function Name: vTaskStart
*	Function:
*	Input Ref: pvParameters 是在创建该任务时传�?�的形参
*	Return Ref:
*	priority: 3  (数�?�越小优先级越低，这个跟uCOS相反)
*
******************************************************************/
void tx_thread_stack_error_handler(TX_THREAD *thread_ptr)
{
    /* 栈溢出处理：这里你可以做任何你想做的动作 */

    /* 1. 打印线程名（如果有 UART） */
    // printf("Stack overflow in thread: %s\n", thread_ptr->tx_thread_name);

    /* 2. 拉高故障指示灯 */
    // Fault_LED_On();

    /* 3. 记录日志（如果有 Flash/EEPROM） */
    // Log_Fault(FAULT_STACK_OVERFLOW, thread_ptr->tx_thread_name);

    /* 4. 触发系统复位（汽车级） */
    // NVIC_SystemReset();

    while(1);  // 调试阶段可以卡住
}


#if DEBUG_ENABLE
static void debug_stack_ui_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计

  
    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_UI; i++)
    {
        if (stack_ui_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
  
 
	
	unused = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}

static void debug_stack_key_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计


    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_KEY; i++)
    {
        if (stack_start_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_key = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}

static void debug_stack_decoder_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计


    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_DECODER; i++)
    {
        if (stack_decoder_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_decoder = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}


static void debug_stack_event_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计


    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_EVENT; i++)
    {
        if (stack_key_event[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_key_event = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}


#endif 



