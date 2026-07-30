#include "bsp.h"

static void read_isr_usart1_data(uint8_t data);



typedef struct Msg
{
	
	uint8_t  usData[12];
    uint8_t  rx_data_counter;
    uint8_t  disp_rx_cmd_done_flag;
    uint8_t  bcc_check_code;
    uint8_t  data_numbers;
	uint8_t  compare_bcc_check;
 
}MSG_T;

MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */



/********************************************************************************
	**
	*Function Name:
	*Function 
	*Input Ref:
	*Return Ref:NO
	*
*******************************************************************************/
void USART1_IRQHandler(void)
{
   volatile uint8_t data ;


     // 1. 优先处理异常错误 (ORE / FE / NE)，做防御性闭环，防止 ISR 死锁
    if (LL_USART_IsActiveFlag_ORE(USART1) || 
        LL_USART_IsActiveFlag_FE(USART1)  || 
        LL_USART_IsActiveFlag_NE(USART1))
    {
        // 【核心操作】必须先强行读取 RDR 寄存器，丢弃脏数据，释放硬件接收锁
        volatile uint8_t dummy_read = LL_USART_ReceiveData8(USART1);
        (void)dummy_read; // 消除未引用变量警告

        // 【核心操作】针对性地清除错误标志位
        LL_USART_ClearFlag_ORE(USART1);
        LL_USART_ClearFlag_FE(USART1);
        LL_USART_ClearFlag_NE(USART1);
    }

	if (LL_USART_IsActiveFlag_RXNE_RXFNE(USART1))
    {
        data = LL_USART_ReceiveData8(USART1); 

        // 写入环形缓冲区
      
        read_isr_usart1_data(data);

       
     
    }
}
#if 0
    // 清除错误标志
   // if (LL_USART_IsActiveFlag_ORE(USART1)) LL_USART_ClearFlag_ORE(USART1);
    if (LL_USART_IsActiveFlag_ORE(USART1))
    {
        /* 
           【核心解锁步骤】
           必须先强行读取一次数据寄存器（DR）。
           这一步是为了清空移位寄存器和硬件缓冲区，向外设硬件发出“允许继续接收”信号。
           如果不读 DR，单纯清除标志位，硬件的接收移位状态机依然处于锁死状态。
        */
        volatile uint32_t dummy_read = USART1->RDR; 
        
        // 使用 volatile 防止这行没用的读取代码被编译器优化掉
        ((void)dummy_read); 
        
        /* 
           可以增加一个错误计数器，方便你在 ThreadX 任务中监控串口健康度
           g_uart1_error_count.ore_count++; 
        */
    }
    if (LL_USART_IsActiveFlag_FE(USART1))  LL_USART_ClearFlag_FE(USART1);
    if (LL_USART_IsActiveFlag_NE(USART1))  LL_USART_ClearFlag_NE(USART1);

#endif 
/**

**/
static void read_isr_usart1_data(uint8_t data)
{
     static uint8_t state;
      switch(state)
		{
		case 0:  //#0
			if(data == 0x5A){  // 0x5A --main board singla
               gl_tMsg.rx_data_counter=0;
               gl_tMsg.usData[gl_tMsg.rx_data_counter] = data;
				state=1; //=1

             }
            else
                state=0;
		break;

       
		case 1: //#1

           // if(gl_tMsg.disp_rx_cmd_done_flag ==0){
              /* 初始化结构体指针 */
               gl_tMsg.rx_data_counter++;
		     
	          gl_tMsg.usData[gl_tMsg.rx_data_counter] = data;

			  if(gl_tMsg.usData[gl_tMsg.rx_data_counter] ==0xFE){
                state = 2;
              }
          //  }
	    break;

	   case 2:
	           gl_tMsg.rx_data_counter++;
	           gl_tMsg.usData[gl_tMsg.rx_data_counter]=data;
               state = 0;
            
                gl_tMsg.data_numbers = gl_tMsg.rx_data_counter;
                

                gl_tMsg.rx_data_counter =0;

                gl_tMsg.disp_rx_cmd_done_flag = 1 ;

                gl_tMsg.bcc_check_code=data;

                tx_thread_semaphore_isr();// vTaskDecoder_USART1_handler();
                  
        break;


			
		}

  
   }



/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void decoder_handler(void)
{
	    gl_tMsg.disp_rx_cmd_done_flag = 0;

		gl_tMsg.compare_bcc_check =  bcc_check(gl_tMsg.usData,gl_tMsg.data_numbers);

		if(gl_tMsg.compare_bcc_check== gl_tMsg.bcc_check_code ){

		receive_data_from_mainboard(gl_tMsg.usData);
		}
			

}




