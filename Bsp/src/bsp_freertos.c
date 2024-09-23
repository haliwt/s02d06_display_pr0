#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


#define POWER_KEY_0	        (1 << 0)
#define MODE_KEY_1	        (1 << 1)
#define DEC_KEY_2           (1 << 2)
#define ADD_KEY_3           (1 << 3)

#define MOUSE_KEY_4           (1 << 4)
#define PLASMA_KEY_5        (1 << 5)
#define DRY_KEY_6           (1<< 6)
#define WIFI_KEY_7            ( 1<< 7)

#define MODE_LONG_KEY_8         (1 << 8)
#define DECODER_BIT_9            (1<< 9)



/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
static void vTaskRunPro(void *pvParameters);
//static void vTaskDecoderPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);



/* 创建任务通信机制 */
//static void AppObjCreate(void);


/***********************************************************************************************************
											变量声明
***********************************************************************************************************/
static TaskHandle_t xHandleTaskRunPro = NULL;
//static TaskHandle_t xHandleTaskDecoderPro= NULL;
static TaskHandle_t xHandleTaskStart = NULL;

//static QueueHandle_t xQueue1 = NULL;
//static QueueHandle_t xQueue2 = NULL;
//static QueueHandle_t xQueue3 = NULL;



///#define QUEUE_LENGTH            1                   /* 队列支持的消息个数 */
///#define QUEUE_ITEM_SIZE         sizeof(uint32_t)    /* 队列中每条消息的大小 */
//#define SEMAPHORE_BINARY_LENGTH 1                   /* 二值信号量的有效长度 */
//#define QUEUESET_LENGTH         ((2 * QUEUE_LENGTH) + SEMAPHORE_BINARY_LENGTH)  /* 队列集支持的消息个数 */



typedef struct Msg
{
	uint8_t  ucMessageID;
	uint8_t usData[12];

}MSG_T;

MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */


uint8_t rx_data_counter,rx_end_flag;

uint8_t  rx_end_counter,ulid;

uint8_t check_code;

uint8_t bcc_check_code;

uint8_t decoder_rx_flag;





/*
*********************************************************************************************************
*	凄1�7 敄1�7 各1�7: vTaskTaskUserIF
*	功能说明: 接口消息处理〄1�7
*	彄1�7    叄1�7: pvParameters 是在创建该任务时传��的形参
*	迄1�7 囄1�7 倄1�7: 旄1�7
*   伄1�7 兄1�7 纄1�7: 1  (数��越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
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

*   伄1�7 兄1�7 纄1�7: 1  (数��越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskRunPro(void *pvParameters)
{
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为30ms */
	uint32_t ulValue;
    
    static volatile uint8_t power_on_off_flag,mouse_on_off_flag ;
    static uint8_t dry_on_off_flag,plasma_on_off_flag, wifi_on_off_flag ;
    static uint8_t key_add_flag,key_dec_flag,key_mode_flag;
    
    while(1)
    {
		/*
			第一个参数 ulBitsToClearOnEntry的作用（函数执行前）：
		          ulNotifiedValue &= ~ulBitsToClearOnEntry
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

		          这里ulBitsToClearOnEntry = 0x00000000就是函数执行前保留所有位。
		
		    第二个参数 ulBitsToClearOnExit的作用（函数退出前）：			
				  ulNotifiedValue &= ~ulBitsToClearOnExit
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

				  这里ulBitsToClearOnExi = 0xFFFFFFFF就是函数退出前清楚所有位。
		
		    注：ulNotifiedValue表示任务vTaskMsgPro的任务控制块里面的变量。		
		*/
		
		xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          xMaxBlockTime);  /* 最大允许延迟时间-等待时间 */
		
		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
             
			if((ulValue & POWER_KEY_0) != 0)
			{
        	  power_on_off_flag = 1;
            }
            else if((ulValue & DECODER_BIT_9 ) != 0){
            
                decoder_rx_flag = 1;

            }
            else if((ulValue & MODE_KEY_1) != 0){
            if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
                if(run_t.gPower_On == power_on)
                 key_mode_flag = 1;
      
               }

            }
            else if((ulValue & DEC_KEY_2) != 0){
                if(run_t.gPower_On == power_on)
                  key_dec_flag = 1;
                 //key_dec_fun();

            }
            else if((ulValue & ADD_KEY_3) != 0){
                if(run_t.gPower_On == power_on)
                  key_add_flag = 1;
                  //key_add_fun();
           
            }
            else if((ulValue & MOUSE_KEY_4) != 0){
                 if(run_t.gPower_On == power_on)
                 mouse_on_off_flag = 1;
             
				  
            }
            else if((ulValue & PLASMA_KEY_5) != 0){
              if(run_t.gPower_On == power_on)
               plasma_on_off_flag = 1;

           }
            else if((ulValue & DRY_KEY_6) != 0){
             
                 if(run_t.gPower_On == power_on)
                  dry_on_off_flag =1;
    		     
            }   
            else if((ulValue & WIFI_KEY_7) != 0){
               if(run_t.gPower_On == power_on)
                  wifi_on_off_flag = 1;

            }

            
          

        
    }
    else{

      if(decoder_rx_flag == 1){

           decoder_rx_flag  =0 ;
           rx_end_flag = 0;
        
          check_code =  bcc_check(gl_tMsg.usData,ulid);
          if(check_code == bcc_check_code ){
    
           receive_data_fromm_display(gl_tMsg.usData);
           }
           gpro_t.disp_rx_cmd_done_flag = 0;
           gl_tMsg.usData[0]=0;

            
          
       }
      

       if(power_on_off_flag == 1){
             power_on_off_flag++;

             power_on_handler();

        }
        else if(plasma_on_off_flag ==1 || dry_on_off_flag == 1 || wifi_on_off_flag ==1 ||mouse_on_off_flag ==1 \
                     || key_add_flag ==1 || key_dec_flag ==1 || key_mode_flag == 1){
                

              if(key_add_flag == 1 && run_t.ptc_warning ==0 && run_t.fan_warning == 0){

                  key_add_flag ++;
                  key_add_fun();

              }
              else if(key_dec_flag == 1 && run_t.ptc_warning ==0 && run_t.fan_warning == 0){
                 key_dec_flag ++;
                 key_dec_fun();


              } 
              else if(key_mode_flag == 1){
                 key_mode_flag++;
                 SendData_Buzzer();
                 mode_key_fun();

                  

              }
              else if(plasma_on_off_flag==1){
               plasma_on_off_flag++;
              

                 if(run_t.gPlasma ==1){  //turun off kill 

                    run_t.gPlasma = 0;
                    LED_PLASMA_OFF();
                    SendData_Set_Command(0x03,0x00);//PLASMA_OFF);

                }  
                else{
                        run_t.gPlasma = 1;
                    
                        LED_PLASMA_ON();
                      
                        SendData_Set_Command(0x03,0x01); //PLASMA_ON);

                }
                
               
               }
               else if(dry_on_off_flag ==1){
                    dry_on_off_flag++;

                   
                if(run_t.ptc_warning ==0){

                 
    			  if(run_t.gDry== 1){
    				    run_t.gDry =0;
                        LED_DRY_OFF();
    					SendData_Set_Command(0x02,0x0); //DRY_OFF);
                        gpro_t.manual_turn_off_dry_flag =1;
                     
                       
                   }
                   else{
                        run_t.gDry =1;
    			    
                        LED_DRY_ON();
                     
    					SendData_Set_Command(0x02,0x01); //DRY_ON);
                        gpro_t.manual_turn_off_dry_flag =0;
                    
                     }  
    			   }
    		    }
                else if(wifi_on_off_flag ==1){

                    wifi_on_off_flag++;
                    gpro_t.gTimer_wifi_led_blik_time =0;
                    run_t.wifi_led_fast_blink_flag =1;
                    
                    SendData_Set_Command(0x05,0x01); //link wifi net 
               
                }
                else if(mouse_on_off_flag==1){
                      mouse_on_off_flag++;

                      mouse_on_off_handler();

                 }
       }
          
          
      if(run_t.gPower_On == power_on){

      
       if((key_add_flag ==2 ||  key_dec_flag==2) && gpro_t.set_timer_timing_doing_value==1){
           if(key_add_flag==2)key_add_flag++;
           else if(key_dec_flag==2)key_dec_flag ++ ;

         wifi_icon_fast_blink();
         TM1639_Write_4Bit_Time(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,0) ;

       }
       else if((key_add_flag ==2 ||  key_dec_flag==2) && gpro_t.set_timer_timing_doing_value==0 ){
            if(key_add_flag ==2)key_add_flag++;
            else if(key_dec_flag == 2)key_dec_flag++;
            run_t.gTimer_key_temp_timing=0;
            TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
            run_t.gTimer_key_temp_timing=0;


       }

       disp_timer_times_handler();
       Set_Temperature_Value();
       RunLocal_Dht11_Data_Process();
             
       Display_TimeColon_Blink_Fun();
       set_timer_fun_led_blink();

       Warning_Error_Numbers_Fun();

       wifi_icon_fast_blink();
     
       
       }
       else if(run_t.gPower_On == power_off){

          power_off_run_handler();

       }
     // USART1_Cmd_Error_Handler();

    }

   }
}

/**********************************************************************************************************
*	凄1�7 敄1�7 各1�7: vTaskStart
*	功能说明: 消息处理，使用函数comGetChar获取串口命令，使用函数comSendBuf发��串口消恄1�7
*	彄1�7    叄1�7: pvParameters 是在创建该任务时传��的形参
*	迄1�7 囄1�7 倄1�7: 旄1�7
*   伄1�7 兄1�7 纄1�7: 3  (数��越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)
{
	

    while(1)
    {
      
    if(POWER_KEY_VALUE()  ==KEY_DOWN){

           
           xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                        POWER_KEY_0,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

    }
    else if( MODEL_KEY_VALUE() ==KEY_DOWN){

        

       xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                         MODE_KEY_1,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
    }
     else if(DEC_KEY_VALUE() == KEY_DOWN){

           xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                            DEC_KEY_2,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

     }
     else if(ADD_KEY_VALUE() ==KEY_DOWN){

          xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                            ADD_KEY_3,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

    }
    else if(WIFI_KEY_VALUE()==KEY_DOWN){

          xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                            WIFI_KEY_7,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

     }
     else if(PLASMA_KEY_VALUE()==KEY_DOWN){

            xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                            PLASMA_KEY_5,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
     }
     else if(DRY_KEY_VALUE()==KEY_DOWN){
            xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                            DRY_KEY_6,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

     }
     else if(MOUSE_KEY_VALUE() == KEY_DOWN){
         xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                            MOUSE_KEY_4,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

     }
     
    

    

     vTaskDelay(20);
     
    }

}
/**********************************************************************************************************
*	凄1�7 敄1�7 各1�7: AppTaskCreate
*	功能说明: 创建应用任务
*	彄1�7    参：旄1�7
*	迄1�7 囄1�7 倄1�7: 旄1�7
**********************************************************************************************************/
void AppTaskCreate (void)
{

	xTaskCreate( vTaskRunPro,    		/* 任务函数  */
                 "vTaskRunPro",  		/* 任务各1�7    */
                 128,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 1,           		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskRunPro); /* 任务句柄  */
//	
//	xTaskCreate( vTaskDecoderPro,     		/* 任务函数  */
//                 "vTaskDecoderPro",   		/* 任务各1�7    */
//                 128,             		/* 任务栈大小，单位word，也就是4字节 */
//                 NULL,           		/* 任务参数  */
//                 1,               		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
//                 &xHandleTaskDecoderPro );  /* 任务句柄  */
//	
//	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务各1�7    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,              		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskStart );   /* 任务句柄  */
}



/*
*********************************************************************************************************
*	凄1�7 敄1�7 各1�7: AppObjCreate
*	功能说明: 创建任务通信机制
*	彄1�7    叄1�7: 旄1�7
*	迄1�7 囄1�7 倄1�7: 旄1�7
*********************************************************************************************************
*/
# if 0
void AppObjCreate (void)
{
    #if 1

//   /* 创建10个uint8_t型消息队刄1�7 */
//	xQueue1 = xQueueCreate(4, sizeof(uint8_t));
//    if( xQueue1 == 0 )
//    {
//        /* 没有创建成功，用户可以在这里加入创建失败的处理机刄1�7 */
//    }
	
	/* 创建10个存储指针变量的消息队列，由于CM3/CM4内核昄1�732位机，一个指针变量占甄1�74个字芄1�7 */
	xQueue2 = xQueueCreate(4, sizeof(struct Msg *));
    if( xQueue2 == 0 )
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机刄1�7 */
    }

	

	#endif 

    #if 0

	 /* 创建队列雄1�7 */
    xQueueSet = xQueueCreateSet(QUEUESET_LENGTH);
    /* 创建队列*/
    xQueue1 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
    xQueue2 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
	
    /* 创建二��信号量 */
    xSemaphore = xSemaphoreCreateBinary();
	
    /* 将队列和二��信号量添加到队列集丄1�7 */
    xQueueAddToSet(xQueue1, xQueueSet);
    xQueueAddToSet(xQueue2, xQueueSet);
    xQueueAddToSet(xSemaphore, xQueueSet);
    #endif 
}

#endif 
/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
     static uint8_t state;
     BaseType_t xHigherPriorityTaskWoken = pdFALSE;



    if(huart==&huart1) // Motor Board receive data (filter)
	{
       DISABLE_INT() ;
      switch(state)
		{
		case 0:  //#0
		   if(gpro_t.disp_rx_cmd_done_flag ==0){
			if(inputBuf[0] == 0x5A){  // 0x5A --main board singla
               rx_data_counter=0;
               gl_tMsg.usData[rx_data_counter] = inputBuf[0];
				state=1; //=1

             }
            else{
                state=0;

              }
           }
		break;

       
		case 1: //#1
       

            
              /* 初始化结构体指针 */
             
               rx_data_counter++;
		     
	          gl_tMsg.usData[rx_data_counter] = inputBuf[0];
              

              if(rx_end_flag == 1){

               
            
                ulid = rx_data_counter;
                rx_end_flag=0;

                rx_data_counter =0;

                gpro_t.disp_rx_cmd_done_flag = 1 ;

         

                bcc_check_code=inputBuf[0];

                 state = 0;
                xTaskNotifyFromISR(xHandleTaskRunPro,  /* 目标任务 */
                DECODER_BIT_9,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                
                  
              }

            

              if(gl_tMsg.usData[rx_data_counter] ==0xFE && rx_end_flag == 0 &&  rx_data_counter > 4){
                     
                          rx_end_flag = 1 ;
                          
                        
               }

        break;


			
		}
        ENABLE_INT();
       __HAL_UART_CLEAR_OREFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
	}
}

void USART1_Cmd_Error_Handler(void)
{
   uint32_t temp;
   if(run_t.gTimer_usart_error >4){
	  	run_t.gTimer_usart_error=0;
	    
         
        __HAL_UART_CLEAR_OREFLAG(&huart1);
        temp = USART1->RDR;
		UART_Start_Receive_IT(&huart1,inputBuf,1);
       
		}
      
    
  
 }















