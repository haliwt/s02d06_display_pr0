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

	if (LL_USART_IsActiveFlag_RXNE_RXFNE(USART1))
    {
        data = LL_USART_ReceiveData8(USART1); 

        // 写入环形缓冲区
      
        read_isr_usart1_data(data);

       
     
    }

    // 清除错误标志
    if (LL_USART_IsActiveFlag_ORE(USART1)) LL_USART_ClearFlag_ORE(USART1);
    if (LL_USART_IsActiveFlag_FE(USART1))  LL_USART_ClearFlag_FE(USART1);
    if (LL_USART_IsActiveFlag_NE(USART1))  LL_USART_ClearFlag_NE(USART1);

}
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
            
        case 1:
            if(data == 0x10){  // 0x5A --main board singla
                gl_tMsg.rx_data_counter++;
				gl_tMsg.usData[gl_tMsg.rx_data_counter] = data;
                state = 2;
            }
            else{
              state = 0;
            }
            
        break;

       
		case 2: //#1

           // if(gl_tMsg.disp_rx_cmd_done_flag ==0){
              /* 初始化结构体指针 */
               gl_tMsg.rx_data_counter++;
		     
	          gl_tMsg.usData[gl_tMsg.rx_data_counter] = data;

			  if(gl_tMsg.usData[gl_tMsg.rx_data_counter] ==0xFE){
                state = 3;
              }
          //  }
	    break;

	   case 3:
	           gl_tMsg.rx_data_counter++;
	           gl_tMsg.usData[gl_tMsg.rx_data_counter]=data;
               state = 0;
            
                gl_tMsg.data_numbers = gl_tMsg.rx_data_counter;
                

                gl_tMsg.rx_data_counter =0;

                gl_tMsg.disp_rx_cmd_done_flag = 1 ;

                gl_tMsg.bcc_check_code=data;

                vTaskDecoder_USART1_handler();
                  
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




