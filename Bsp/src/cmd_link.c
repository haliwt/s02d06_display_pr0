#include "bsp.h"


volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run
uint8_t outputBuf[8];
static uint8_t transferSize;
uint8_t inputBuf[MAX_BUFFER_SIZE];



/****************************************************************************************************
**
*Function Name:static void selectLight(uint8_t index)
*Function: UART2 transmit interrupt process ---4D 58 4C 53 32 30 32 
*Input Ref: LED number 
*Return Ref:NO
*
****************************************************************************************************/
void SendData_PowerOff(uint8_t index)
{
	
    //crc=0x55;
	outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number: No:01 maybe is No:02.
	outputBuf[2] =0x01; //command power or off 
	outputBuf[3]=index; // command order -> 01 - power on , 00- power off
	outputBuf[4]=0x00; // data is length: 00 ->don't data 
	outputBuf[5]=0xFE; // frame of end code -> 0xFE.
	
	outputBuf[6] = bcc_check(outputBuf,6);
	transferSize=7;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
}
/*********************************************************
 * 
 * Function Name:void SendData_Buzzer(void)
 * Function: 
 * Input Ref:NO
 * Return Ref:NO
 * 
*********************************************************/
void SendData_Buzzer(void)
{
	
    outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number:is 0x01
	outputBuf[2]=0x06; // command type = 0x06 ->buzzer sound open or not
	outputBuf[3]=0x01; // command order -> 01 - buzzer sound done, 00- don't buzzer sound 
	outputBuf[4]=0x00; // data is length: 00 ->don't data 
	outputBuf[5]=0xFE; // frame of end code -> 0xFE.
	
	outputBuf[6] = bcc_check(outputBuf,6);
	transferSize=7;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}

/*********************************************************
 * 
 * Function Name:void SendData_Set_Command(uint8_t cmd,uint8_t cmddata)
 * Function: 
 * Input Ref:cmd- command , dmddata-> command of mode .
 * Return Ref:NO
 * 
*********************************************************/
void SendData_Set_Command(uint8_t cmd,uint8_t cmddata)

{
	outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number:is 0x01
	outputBuf[2]= cmd; // command type = 0x06 ->buzzer sound open or not
	outputBuf[3]= cmddata; // command order -> 01 - buzzer sound done, 00- don't buzzer sound 
	outputBuf[4]=0x00; // data is length: 00 ->don't data 
	outputBuf[5]=0xFE; // frame of end code -> 0xFE.
    outputBuf[6] = bcc_check(outputBuf,6);


		transferSize=7;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Tx_Data(uint8_t dcmd,uint8_t ddata)
 * Function: 
 * Input Ref:cmd- command , dmddata-> command of mode .
 * Return Ref:NO
 * 
*********************************************************/
void SendData_Tx_Data(uint8_t dcmd,uint8_t ddata)

{
	outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number:is 0x01
	outputBuf[2]= dcmd; // command type = 0x06 ->buzzer sound open or not
	outputBuf[3]= 0x0f; //  0x0f -> is data ,don't command.
	outputBuf[4]=0x01; // data is length: 00 ->don't data ,0x01 -> has one data.
    outputBuf[5]=ddata; // frame of end code -> 0xFE.
    outputBuf[6]=0xFE; // frame of end code -> 0xFE.
    outputBuf[7] = bcc_check(outputBuf,7);


		transferSize=8;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}

/*********************************************************
 * 
 * Function Name:
 * Function:
 * Input Ref:NO
 * Return Ref:NO
 * 
*********************************************************/
void SendData_Temp_Data(uint8_t tdata)
{

    outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number:is 0x01
	outputBuf[2]=0x1A; // command type = 0x1A -> temperature of value 
	outputBuf[3]=0x0f; // command order -> 0x0f -> is data , don't order.
	outputBuf[4]=0x01; // data is length: 00 ->don't data 
	outputBuf[5]=tdata; // frame of end code -> 0xFE.
	outputBuf[6]=0xFE; // frame of end code -> 0xFE.
    outputBuf[7] = bcc_check(outputBuf,7);
		
		transferSize=8;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}


/*********************************************************
 * 
 * Function Name:
 * Function:
 * Input Ref:NO
 * Return Ref:NO
 * 
*********************************************************/
void SendData_SetTemp_Data(uint8_t tdata)
{

    outputBuf[0]=0xA5; //display board head = 0xA5
	outputBuf[1]= 0x01; //display device Number:is 0x01
	outputBuf[2]=0x2A; // command type = 0x1A -> temperature of value 
	outputBuf[3]=0x0f; // command order -> 0x0f -> is data , don't order.
	outputBuf[4]=0x01; // data is length: 00 ->don't data 
	outputBuf[5]=tdata; // frame of end code -> 0xFE.
	outputBuf[6]=0xFE; // frame of end code -> 0xFE.
    outputBuf[7] = bcc_check(outputBuf,7);
		
		transferSize=8;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}



/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart1)
	{
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}
	
}

