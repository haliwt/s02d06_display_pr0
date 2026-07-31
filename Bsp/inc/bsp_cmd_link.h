#ifndef __BSP_CMD_LINK_H_
#define __BSP_CMD_LINK_H_

#include "main.h"
#define  MAX_BUFFER_SIZE   12
#define  MAX_BUFFER_BUF   12

//extern uint8_t inputBuf[MAX_BUFFER_SIZE];
//extern uint8_t outputBuf[8];

#define USART1_INTERRUPT        0

typedef enum{

     WIFI_CLOUD_FAIL,
	 WIFI_CLOUD_SUCCESS

}wifi_cloud_state;





void SendData_PowerOnOff(uint8_t index);

void SendData_Tx_Data(uint8_t dcmd,uint8_t ddata);



void SendData_Time_Data(uint8_t tdata,uint8_t datalen);

void SendData_Buzzer(void);

void SendData_Buzzer_Has_Ack(void);

void SendData_Set_Command(uint8_t cmd,uint8_t data);

void SendData_Temp_Data(uint8_t tdata);

void SendData_ToMainboard_Data(uint8_t cmd,uint8_t *tdata,uint8_t datalen);


void SendWifiData_Answer_Cmd(uint8_t cmd ,uint8_t data);

void sendCmdNote_to_threeData(uint8_t cmd ,uint8_t h,uint8_t m,uint8_t s);

void sendCmdNote_to_Data(uint8_t cmd ,uint8_t tdata);

uint8_t SendData_Set_Command_Safe(uint8_t cmd, uint8_t val);

uint8_t SendData_Answer_Command_Safe(uint8_t cmd, uint8_t val);


#endif 




