#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


#define POWER_BIT_0	        (1 << 0)
#define MODE_BIT_1	        (1 << 1)
#define DEC_BIT_2           (1 << 2)
#define ADD_BIT_3           (1 << 3)

#define MOUSE_BIT_4           (1 << 4)
#define PLASMA_BIT_5        (1 << 5)
#define DRY_BIT_6           (1<< 6)
#define AI_BIT_7            ( 1<< 7)

#define MODE_LONG_KEY_8         (1 << 8)
#define DECODER_BIT_9          (1<< 9)




/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
#if 0

static void vTaskRunPro(void *pvParameters);
static void vTaskDecoderPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
#else
/*------------------ 静态任务内存定义 ------------------*/

static StaticTask_t xTaskRunProTCB;
static StackType_t xTaskRunProStack[256];

static StaticTask_t xTaskDecoderProTCB;
static StackType_t xTaskDecoderProStack[128];

static StaticTask_t xTaskStartTCB;
static StackType_t xTaskStartStack[128];




#endif 


static void AppTaskCreate (void);



/* 创建任务通信机制 */
//static void AppObjCreate(void);


/***********************************************************************************************************
											变量声明
***********************************************************************************************************/
static TaskHandle_t xHandleTaskRunPro = NULL;
static TaskHandle_t xHandleTaskDecoderPro= NULL;
static TaskHandle_t xHandleTaskStart = NULL;

//static QueueHandle_t xQueue1 = NULL;
//static QueueHandle_t xQueue2 = NULL;
//static QueueHandle_t xQueue3 = NULL;



///#define QUEUE_LENGTH            1                   /* 队列支持的消息个数 */
///#define QUEUE_ITEM_SIZE         sizeof(uint32_t)    /* 队列中每条消息的大小 */
//#define SEMAPHORE_BINARY_LENGTH 1                   /* 二值信号量的有效长度 */
//#define QUEUESET_LENGTH         ((2 * QUEUE_LENGTH) + SEMAPHORE_BINARY_LENGTH)  /* 队列集支持的消息个数 */



uint8_t ucKeyCode;
uint8_t uckey_number;
uint8_t key_power_flag,decoder_flag ;
uint8_t check_code;
uint8_t keyvalue;

/**********************************************************************************************************
*	凄1�7 敄1�7 各1�7: vTaskTaskUserIF
*	功能说明: 接口消息处理〄1�7
*	彄1�7    叄1�7: pvParameters 是在创建该任务时传��的形参
*	迄1�7 囄1�7 倄1�7: 旄1�7
*   伄1�7 兄1�7 纄1�7: 1  (数��越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
void freeRTOS_Handler(void)
{
     /* 创建任务 */
	  AppTaskCreate();
	  
	  /* 创建任务通信机制 */
	 //  AppObjCreate();
	  
	  /* 启动调度，开始执行任劄1�7 */
	   vTaskStartScheduler();


}

/**********************************************************************************************************
*   FunctionName: static void vTaskRunPro(void *pvParameters)
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  

*   priority : 1  (数priority is 2,number is large priority is large值越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskDecoderPro(void *pvParameters)
{
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(3000); /* 设置最大等待时间为30ms */
	uint32_t ulValue;
	

    while(1)
    {


	xResult = xTaskNotifyWait(0x00000000,
							  0xFFFFFFFF,     /* Reset the notification value to 0 on */
							   &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
							   xMaxBlockTime );//portMAX_DELAY);  /* 阻塞时间30ms，释放CUP控制权,给其它任务执行的权限*/

		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */

			if((ulValue & DECODER_BIT_9) != 0){

			   
			    decoder_handler();
			    
			   }

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
static void vTaskRunPro(void *pvParameters)
{

	
	static uint8_t power_on_theFirst_times;
 
	while(1)
    {

	 process_keys() ;
	
    if(run_t.gPower_On == power_on){
		

	    power_on_run_handler();
     
       Display_TimeColon_Blink_Fun();
	
       set_timer_fun_led_blink();
       wifi_connect_state_fun();
	  
	 
       if(power_on_theFirst_times < 10 && (gpro_t.set_timer_timing_doing_value==0 || gpro_t.set_timer_timing_doing_value==3)){
         power_on_theFirst_times ++;
         Display_DHT11_Value();

       }
    }
	 else{

	    power_off_run_handler();

	  }

      vTaskDelay(30);
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
static void vTaskStart(void *pvParameters)
{
	
   
    while(1)
    {
      
    /* 接收到消息，检测那个位被按下 */
	if(POWER_KEY_VALUE()==KEY_DOWN ){
        key_t.key_wifi_flag =0;
	    key_t.key_power_flag =1;

	}
	else if(MODEL_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){   /* 接收到消息，检测那个位被按下 */

       

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
	          vTaskDelay(100);
		     


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
	             vTaskDelay(100);
		
			 
           }
        }

		if(gpro_t.mode_Key_long_counter==200) key_t.key_mode_flag = 0;
		else
           key_t.key_mode_flag = 1;
         
		 
   }
   else if(DEC_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){
	
      key_t.key_dec_flag =1;
      key_t.key_wifi_flag =0;
      gpro_t.mode_Key_long_counter=0;

    
               
  }
  else if(ADD_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){   /* 接收到消息，检测那个位被按下 */
	if(run_t.gPower_On == power_on){
	 key_t.key_add_flag =1;
	 key_t.key_wifi_flag =0;
	  gpro_t.mode_Key_long_counter=0;            
	}
  }
  else if(PLASMA_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){   /* 接收到消息，检测那个位被按下 */
 
    key_t.key_plasma_flag =1;
    key_t.key_wifi_flag =0;
    gpro_t.mode_Key_long_counter=0;
    	                
    
  }
  else if(DRY_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){   /* 接收到消息，检测那个位被按下 */

    key_t.key_dry_flag =1;
    key_t.key_wifi_flag =0;
    gpro_t.mode_Key_long_counter=0;
               
  
   }
   else if(MOUSE_KEY_VALUE() == KEY_DOWN && run_t.gPower_On == power_on){   /* 接收到消息，检测那个位被按下 */

       
	    key_t.key_mouse_flag =1;
        key_t.key_wifi_flag =0;
        gpro_t.mode_Key_long_counter=0;
      
	                 
	}
    else if(WIFI_KEY_VALUE()==KEY_DOWN &&  run_t.gPower_On == power_on ){

        
	     // 处理WiFi键
         if( key_t.key_wifi_flag < 150 && run_t.gPower_On == power_on){
		 	  key_t.key_wifi_flag++;
        
         if(key_t.key_wifi_flag > 99){
            key_t.key_wifi_flag = 200;
			
		    run_t.connect_wifi_state = wifi_connect_null;
            run_t.gTimer_wifi_connect_counter =0; //120s counte start
            SendData_Set_Command(0x05,0x01); // link wifi of command .
            vTaskDelay(100);
		    key_t.key_wifi_flag =0;
	     }
      	}

    }

    vTaskDelay(20);

	}
}

 /**********************************************************************************************************
*	Function Name: AppTaskCreate
*	功能说明: 创建应用任务
*	Input Ref:   
*	Return Ref:
**********************************************************************************************************/
void AppTaskCreate (void)
{
  #if 0
  xTaskCreate( vTaskDecoderPro,    		/* 任务函数  */
                 "vTaskDecoderPro",  		/* 任务各1�7    */
                 128,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 2,           		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskDecoderPro); /* 任务句柄  */

  xTaskCreate( vTaskRunPro,    		/* 任务函数  */
                 "vTaskRunPro",  		/* 任务各1�7    */
                 128,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 1,           		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskRunPro); /* 任务句柄  */
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务各1�7    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,              		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskStart );   /* 任务句柄  */

  #else 
	
	xHandleTaskDecoderPro = xTaskCreateStatic(
				vTaskDecoderPro,			/* 任务函数 */
				"vTaskDecoderPro", 		/* 任务名 */
				128,					/* 栈大小（word） */
				NULL,					/* 参数 */
				3,						/* 优先级 */
				xTaskDecoderProStack,		/* 栈数组 */
				&xTaskDecoderProTCB		/* TCB */
		);
	
		
		
	xHandleTaskRunPro = xTaskCreateStatic(
				vTaskRunPro, 		/* 任务函数 */
				"vTaskRunPro",			/* 任务名 */
				256,					/* 栈大小（word） */
				NULL,					/* 参数 */
				1,						/* 优先级 */
				xTaskRunProStack,		/* 栈数组 */
				&xTaskRunProTCB		/* TCB */
		);
		
	xHandleTaskStart = xTaskCreateStatic(
				vTaskStart,			/* 任务函数 */
				"vTaskStart",			/* 任务名 */
				128,					/* 栈大小（word） */
				NULL,					/* 参数 */
				2,						/* 优先级 */
				xTaskStartStack,		/* 栈数组 */
				&xTaskStartTCB 		/* TCB */
		);

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

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	 xTaskNotifyFromISR(xHandleTaskDecoderPro,  /* 目标任务 */
                    DECODER_BIT_9,     /* 设置目标任务事件标志位bit0  */
                    eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                    &xHigherPriorityTaskWoken);
	 
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);



}








