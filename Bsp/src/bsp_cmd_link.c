#include "bsp.h"

// 定义宏，提高代码可读性
#define FRAME_HEADER        0xA5
#define DEVICE_NUMBER       0x01
#define FRAME_END           0xFE
#define NO_DATA             0x00
#define HAS_DATA            0x0F



volatile uint8_t transOngoingFlag; // UART传输标志
uint8_t outputBuf[MAX_BUFFER_BUF];
volatile uint8_t transferSize;

void usart1_dma_send(uint8_t *txbuf,uint16_t txlen)
{

   


    LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_1);

    LL_DMA_ConfigAddresses(DMA1,LL_DMA_CHANNEL_1,
                            (uint32_t)txbuf,
                            (uint32_t)&USART1->TDR,
                            LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_1,txlen);

   // LL_DMA_ClearFlag_TC1(DMA1);//TC1 -> clear transfer complete flag
   // LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_1);

   // LL_USART_EnableDMAReq_TX(USART1);

	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_BYTE);
	
	LL_USART_EnableDMAReq_TX(USART1);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

}


/****************************************************************************************************
 * Function Name: static void sendUartData
 * Function: 通过UART发送数据
 * Input Ref: data - 数据指针, size - 数据大小
 * Return Ref: 无
 ****************************************************************************************************/
static void sendUartData(uint8_t *data, uint8_t size) 
{


	   usart1_dma_send(data,size);
	
	
   
}

/****************************************************************************************************
 * Function Name: static void fillFrame
 * Function: 填充帧数据
 * Input Ref: cmd - 命令, frameType - 帧类型, data - 数据指针, dataLen - 数据长度
 * Return Ref: 无
 ****************************************************************************************************/
static void fillFrame(uint8_t cmd, uint8_t frameType, uint8_t *data, uint8_t dataLen) 
{
    outputBuf[0] = FRAME_HEADER;       // 帧头
    outputBuf[1] = DEVICE_NUMBER;      // 设备号
    outputBuf[2] = cmd;                // 命令
    outputBuf[3] = frameType;          // 帧类型（0x0F 表示数据类型，其他表示命令类型）

    if (frameType == HAS_DATA){       // 数据类型
        outputBuf[4] = dataLen;        // 数据长度
        for (uint8_t i = 0; i < dataLen; i++) {
            outputBuf[5 + i] = data[i]; // 填充数据
        }
        outputBuf[5 + dataLen] = FRAME_END; // 帧尾
        outputBuf[6 + dataLen] = bcc_check(outputBuf, 6 + dataLen); // 校验码
        transferSize = 7 + dataLen;    // 计算帧总长度
    } else {                           // 命令类型
        outputBuf[4] =0x0;      // 功能码
        outputBuf[5] = FRAME_END;      // 帧尾
        outputBuf[6] = bcc_check(outputBuf, 6); // 校验码
        transferSize = 7;              // 帧总长度
    }
}


/****************************************************************************************************
 * Function Name: static void fillFrame_copy(uint8_t cmd, uint8_t frameType, uint8_t *data, uint8_t dataLen)
 * Function: 填充帧数据
 * Input Ref: cmd - 命令, frameType - 帧类型, data - 数据指针, dataLen - 数据长度
 * Return Ref: 无
 ****************************************************************************************************/
static void fillFrame_copy(uint8_t cmd, uint8_t frameType, uint8_t *data, uint8_t dataLen) 
{
    outputBuf[0] = FRAME_HEADER;       // 帧头
    outputBuf[1] = DEVICE_NUMBER;      // 设备号
    outputBuf[2] = 0xff ;                //copy command
    outputBuf[3] = cmd;                // 命令
    outputBuf[4] = frameType;          // 帧类型（0x0F 表示数据类型，其他表示命令类型）

    if (frameType == HAS_DATA){       // 数据类型
        outputBuf[5] = dataLen;        // 数据长度
        for (uint8_t i = 0; i < dataLen; i++) {
            outputBuf[6 + i] = data[i]; // 填充数据
        }
        outputBuf[6 + dataLen] = FRAME_END; // 帧尾
        outputBuf[7 + dataLen] = bcc_check(outputBuf, 7 + dataLen); // 校验码
        transferSize = 8 + dataLen;    // 计算帧总长度
    } else {                           // 命令类型
        outputBuf[5] = 0x00;//data[0];      // 功能码--
        outputBuf[6] = FRAME_END;      // 帧尾
        outputBuf[7] = bcc_check(outputBuf, 6); // 校验码
        transferSize = 8;              // 帧总长度
    }
}


/****************************************************************************************************
 * Function Name: SendData_Buzzer
 * Function: 发送蜂鸣器命令
 * Input Ref: 无
 * Return Ref: 无
 ****************************************************************************************************/
void SendData_Buzzer(void) {
    uint8_t cmd = 0x06; // 蜂鸣器命令
    uint8_t cmdData = 0x01; // 打开蜂鸣器
    fillFrame(cmd,NO_DATA,&cmdData,0);
    sendUartData(outputBuf, transferSize);
}

/****************************************************************************************************
 * Function Name: SendData_Set_Command
 * Function: 发送设置命令
 * Input Ref: cmd - 命令, cmdData - 命令数据
 * Return Ref: 无
 ****************************************************************************************************/
void SendData_Set_Command(uint8_t cmd, uint8_t cmdData) 
{
    outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= DEVICE_NUMBER; //display device Number:is 0x01
	outputBuf[2]=cmd; // command type = 0x06 ->buzzer sound open or not
	outputBuf[3]= cmdData; // command order -> 01 - buzzer sound done, 00- don't buzzer sound 
	outputBuf[4]=0x00; // data is length: 00 ->don't data 
	outputBuf[5]=0xFE; // frame of end code -> 0xFE.
    outputBuf[6] = bcc_check(outputBuf,6);
    transferSize=7;
	usart1_dma_send(outputBuf,transferSize);
}

/****************************************************************************************************
 * Function Name: SendData_Tx_Data
 * Function: 发送数据命令
 * Input Ref: cmd - 命令, data - 数据
 * Return Ref: 无
 ****************************************************************************************************/
void SendData_Tx_Data(uint8_t cmd, uint8_t data) 
{
    fillFrame(cmd, HAS_DATA, &data, 1);
    sendUartData(outputBuf, 8);
}

/****************************************************************************************************
 * Function Name: SendData_Temp_Data
 * Function: 发送温度数据
 * Input Ref: tdata - 温度数据
 * Return Ref: 无
 ****************************************************************************************************/
void SendData_Temp_Data(uint8_t tdata) {
    fillFrame(0x1A, HAS_DATA, &tdata, 1);
    sendUartData(outputBuf, transferSize);
}

/****************************************************************************************************
 * Function Name: copy_cmd_data_from_mainboard
 * Function: 发送设置温度数据
 * Input Ref: tdata - 温度数据
 * Return Ref: 无
 ****************************************************************************************************/
void SendData_ToMainboard_Data(uint8_t cmd,uint8_t *pdata,uint8_t datalen) 
{
    fillFrame(cmd, HAS_DATA, pdata, datalen);
    sendUartData(outputBuf, (7+datalen));
}
/****************************************************************************************************
 * Function Name: copy_cmd_data_from_mainboard
 * Function: 发送设置温度数据
 * Input Ref: tdata - 温度数据
 * Return Ref: 无
 ****************************************************************************************************/
void SendData_CopyCmd_Data(uint8_t cmd,uint8_t *pdata,uint8_t datalen) 
{
    fillFrame_copy(cmd, HAS_DATA, pdata, datalen);
    sendUartData(outputBuf, (8+datalen));
}

/****************************************************************************************************
 * Function Name: SendData_PowerOnOff
 * Function: 发送电源开关命令
 * Input Ref: index - 电源状态 (0: 关, 1: 开)
 * Return Ref: 无
 ****************************************************************************************************/
void SendData_PowerOnOff(uint8_t index) {
    fillFrame(0x01,NO_DATA,&index,0);
    sendUartData(outputBuf, transferSize);
}

/********************************************************************************
    **
    *Function Name
    *Function : commad order , data -command type
    *Input Ref: commad order , data -command type
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_Answer_Cmd(uint8_t cmd ,uint8_t data)
{
        outputBuf[0]=0xA5; //display board head = 0xA5
        outputBuf[1]=DEVICE_NUMBER; //display device Number:is 0x01
        outputBuf[2]=0xFF; // answer or copy command
        outputBuf[3]= cmd; // 0x0F : is data ,don't command order.
        outputBuf[4]= data; // don't data ,onlay is command order,recieve data is 1byte .

		outputBuf[5]=0x00;
        outputBuf[6] = 0xFE; //frame is end of byte.
        outputBuf[7] = bcc_check(outputBuf,7);
        
        transferSize=8;
		usart1_dma_send(outputBuf,transferSize);
        // if(transferSize)
        // {
        //     while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
        //     transOngoingFlag=1;
        //    // HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        // }
	
}


/****************************************************************************************************
 * Function Name: HAL_UART_TxCpltCallback
 * Function: UART发送完成回调函数
 * Input Ref: UART_HandleTypeDef 指针
 * Return Ref: 无
 ****************************************************************************************************/
void HAL_UART_TxCpltCallback(void)
{
    
	 #if USART1_INTERRUPT
        transOngoingFlag = 0; // 清除传输标志
     #else
        gpro_t.DMA_txComplete  = 1;
	 #endif 
    
}



