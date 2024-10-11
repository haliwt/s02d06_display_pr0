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
    uint8_t  key_long_mode_counter;
    uint8_t  key_long_wifi_counter;
 
    
    uint8_t  key_long_power_flag;
    uint8_t  key_long_mode_flag;
   
    
    uint8_t  key_power_flag;
    uint8_t  key_mode_flag;
    uint8_t  key_add_flag;
    uint8_t  key_dec_flag;
    uint8_t  key_wifi_flag;
    uint8_t  key_plasma_flag;
    uint8_t  key_ptc_flag;
    uint8_t  key_ultrasonic_flag;

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
  
   while(1)
    {
      if(gpro_t.disp_rx_cmd_done_flag == 1){

           
        check_code =  bcc_check(gl_tMsg.usData,ulid);
          if(check_code == bcc_check_code ){
    
           receive_data_fromm_display(gl_tMsg.usData);
           }
           gpro_t.disp_rx_cmd_done_flag = 0;
           gl_tMsg.usData[0]=0;

            
          
       }

       if(gl_tMsg.key_power_flag == 1){
           if(POWER_KEY_VALUE()  ==KEY_UP){
             gl_tMsg.key_power_flag++;
              gl_tMsg.key_long_mode_counter =0; //clear counter numbers.
              gl_tMsg.key_long_wifi_counter=0;
               power_on_handler();
             

            }
        }
        else if(gl_tMsg.key_mode_flag == 1){

          if(MODEL_KEY_VALUE() ==KEY_UP){

              gl_tMsg.key_mode_flag++;
              gl_tMsg.key_long_wifi_counter=0;
            
              if(gl_tMsg.key_long_mode_flag == 1){
                gpro_t.gTimer_power_mode_key_long = 0;
                mode_key_long_fun();
              

              }
              else{
                 gl_tMsg.key_long_mode_counter =0; //clear counter numbers.
                 gl_tMsg.key_long_wifi_counter =0;
                 SendData_Buzzer();

                  mode_key_fun();

              }
            }
        }
        else if(gpro_t.key_long_wifi_flag== 1){
            
       
             gl_tMsg.key_long_mode_counter =0;
             gpro_t.gTimer_power_mode_key_long = 0;
              gl_tMsg.key_long_wifi_counter=0;
              gpro_t.gTimer_wifi_led_blink_time =0;
              run_t.wifi_led_fast_blink_flag =1;  
              gpro_t.wifi_link_net_success = 0;
              gpro_t.get_beijing_time_flag = 0;
              SendData_Set_Command(0x05,0x01); //link net flag is "1"
              osDelay(10);
              gpro_t.key_long_wifi_flag++;

        }
        else if(gl_tMsg.key_add_flag == 1 || gl_tMsg.key_dec_flag == 1){
            if(gl_tMsg.key_add_flag == 1   && ADD_KEY_VALUE() ==KEY_UP){

                  gl_tMsg.key_add_flag ++;
                  key_add_fun();

              }
              else if(gl_tMsg.key_dec_flag == 1  && DEC_KEY_VALUE() ==KEY_UP){
                 gl_tMsg.key_dec_flag ++;
                 key_dec_fun();


              } 

             if((gl_tMsg.key_add_flag ==2 || gl_tMsg.key_dec_flag==2) && gpro_t.set_timer_timing_doing_value==0){

             
               TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
                run_t.gTimer_key_temp_timing=0;

             }


        }
        else if(gl_tMsg.key_ptc_flag == 1 || gl_tMsg.key_plasma_flag ==1 || gl_tMsg.key_ultrasonic_flag ==1){

            if(gl_tMsg.key_ptc_flag == 1   && DRY_KEY_VALUE() ==KEY_UP){
                gl_tMsg.key_ptc_flag ++ ;
                ptc_on_off_handler();


            }
            else if(gl_tMsg.key_plasma_flag ==1 && PLASMA_KEY_VALUE()==KEY_UP){

                gl_tMsg.key_plasma_flag++ ;
                plasma_on_off_handler();

            }
            else if(gl_tMsg.key_ultrasonic_flag ==1 && MOUSE_KEY_VALUE() == KEY_UP){
                gl_tMsg.key_ultrasonic_flag ++ ;

                mouse_on_off_handler();

            }
                

         }
          
      if(run_t.gPower_On == power_on){

       smartphone_app_power_on_fun();
       led_fun_indicator_light();
       Display_SmgTiming_Handler();
       Set_Temperature_Value();
       RunLocal_Dht11_Data_Process();
             
       Display_TimeColon_Blink_Fun();
       wifi_icon_fast_blink();
       if((gpro_t.key_long_wifi_flag== 2 || gl_tMsg.key_long_mode_flag == 1) &&  gpro_t.gTimer_power_mode_key_long  > 1){

            if(gpro_t.key_long_wifi_flag== 2){
                  gl_tMsg.key_long_mode_counter =0;
                 gl_tMsg.key_long_wifi_counter=0;
                 gpro_t.key_long_wifi_flag=0;

            }
            else if(gl_tMsg.key_long_mode_flag == 1){
                 gl_tMsg.key_long_mode_counter =0;
                 gl_tMsg.key_long_wifi_counter=0;

                 gl_tMsg.key_long_mode_flag =0;

            }
       
         }
         
       }
       else if(run_t.gPower_On == power_off){

          power_off_run_handler();
      }
     // USART1_Cmd_Error_Handler();
      vTaskDelay(10);//vTaskDelay(30); //vTaskDelay(50)
    }

 }
/**********************************************************************************************************
*	vTask Name: AppTaskCreate
*	功能说明: 创建应用任务
*	Inpute Ref：
*	Return Ref:
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)
{
	while(1)
    {
      
    if(POWER_KEY_VALUE()  ==KEY_DOWN){

        gl_tMsg.key_long_mode_counter =0;
        gl_tMsg.key_long_wifi_counter=0;
        gl_tMsg.key_power_flag = 1;
    }
    else if( MODEL_KEY_VALUE() ==KEY_DOWN){

          
          gl_tMsg.key_long_wifi_counter=0;
          gl_tMsg.key_long_mode_counter++;
          gl_tMsg.key_power_flag = 0;
         if(gl_tMsg.key_long_mode_counter > 45  && run_t.gPower_On == power_on &&  run_t.ptc_warning ==0 && run_t.fan_warning ==0){
             gl_tMsg.key_long_mode_counter=0;   
         
             gl_tMsg.key_long_mode_flag =1;
            gpro_t.gTimer_power_mode_key_long = 0;
            
                SendData_Buzzer();
           }

         if(run_t.gPower_On == power_on   && run_t.ptc_warning ==0 && run_t.fan_warning == 0){
            gl_tMsg.key_mode_flag  =  1;

          }

    }
     else if(DEC_KEY_VALUE() == KEY_DOWN){
           gl_tMsg.key_long_wifi_counter=0;
           gl_tMsg.key_long_mode_counter=0;
     
          if(run_t.gPower_On == power_on  && run_t.ptc_warning ==0 && run_t.fan_warning == 0){
             gl_tMsg.key_dec_flag =1;
           }

     }
     else if(ADD_KEY_VALUE() ==KEY_DOWN){

            gl_tMsg.key_long_wifi_counter=0;
           gl_tMsg.key_long_mode_counter=0;

            if(run_t.gPower_On == power_on && run_t.ptc_warning ==0 && run_t.fan_warning == 0){
                gl_tMsg.key_add_flag =1;
              }
    }
    else if(WIFI_KEY_VALUE()==KEY_DOWN && gpro_t.key_long_wifi_flag==0){

        gl_tMsg.key_power_flag = 0;
        gl_tMsg.key_long_mode_counter=0;
        gl_tMsg.key_long_wifi_counter++;
        if(gl_tMsg.key_long_wifi_counter > 45 && run_t.gPower_On == power_on && run_t.ptc_warning ==0 && run_t.fan_warning == 0){
            gl_tMsg.key_long_wifi_counter =0;
            gpro_t.key_long_wifi_flag=1;
            gpro_t.gTimer_power_mode_key_long = 0;
            
             SendData_Buzzer();
         }

    }
     else if(PLASMA_KEY_VALUE()==KEY_DOWN){
            gl_tMsg.key_long_wifi_counter=0;
           gl_tMsg.key_long_mode_counter=0;
         if(run_t.gPower_On == power_on  && run_t.ptc_warning ==0 && run_t.fan_warning == 0){
           gl_tMsg.key_plasma_flag = 1;

         }
            
     }
     else if(DRY_KEY_VALUE()==KEY_DOWN){
           gl_tMsg.key_long_wifi_counter=0;
           gl_tMsg.key_long_mode_counter=0; 
          if(run_t.gPower_On == power_on   && run_t.ptc_warning ==0 && run_t.fan_warning == 0){
           gl_tMsg.key_ptc_flag =1;

            }

     }
     else if(MOUSE_KEY_VALUE() == KEY_DOWN){
           gl_tMsg.key_long_wifi_counter=0;
           gl_tMsg.key_long_mode_counter=0;
     
          if(run_t.gPower_On == power_on  && run_t.ptc_warning ==0 && run_t.fan_warning == 0){
           gl_tMsg.key_ultrasonic_flag = 1;

          }

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
//                xTaskNotifyFromISR(xHandleTaskRunPro,  /* 目标任务 */
//                DECODER_BIT_9,     /* 设置目标任务事件标志位bit0  */
//                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
//                &xHigherPriorityTaskWoken);
//
//                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
//                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//                
                  
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















