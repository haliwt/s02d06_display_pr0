#ifndef __BSP_LED_H_
#define __BSP_LED_H_
#include "main.h"


#define LED_POWER_TOGGLE()            do(LED_POWER_GPIO_Port->ODR ^= LED_POWER_Pin;}while(1)//HAL_GPIO_TogglePin(LED_POWER_GPIO_Port, LED_POWER_Pin)
#define LED_POWER_ON()				  do{LED_POWER_GPIO_Port->BSRR =LED_POWER_Pin;}while(0)//HAL_GPIO_WritePin(LED_POWER_GPIO_Port, LED_POWER_Pin,GPIO_PIN_SET)
#define LED_POWER_OFF()            	  do{LED_POWER_GPIO_Port->BSRR =(uint32_t)LED_POWER_Pin<<16;}while(0)//HAL_GPIO_WritePin(LED_POWER_GPIO_Port, LED_POWER_Pin,GPIO_PIN_RESET)



#if 0
#define LED_WIFI_ON()			  HAL_GPIO_WritePin(WIFI_LED_EN_GPIO_Port , WIFI_LED_EN_Pin,GPIO_PIN_SET)
#define LED_WIFI_OFF()    	      HAL_GPIO_WritePin(WIFI_LED_EN_GPIO_Port , WIFI_LED_EN_Pin,GPIO_PIN_RESET)
#define LED_WIFI_TOGGLE()         HAL_GPIO_TogglePin(WIFI_LED_EN_GPIO_Port, WIFI_LED_EN_Pin)
#else 
// 寄存器方式控制 LED
#define LED_WIFI_ON()             do{WIFI_LED_EN_GPIO_Port->BSRR = WIFI_LED_EN_Pin;}while(0)
#define LED_WIFI_OFF()            do{WIFI_LED_EN_GPIO_Port->BSRR = (uint32_t)WIFI_LED_EN_Pin<<16;}while(0)
#define LED_WIFI_TOGGLE()         do{WIFI_LED_EN_GPIO_Port->ODR ^= WIFI_LED_EN_Pin;}while(0)
#endif 


#define LED_DRY_ON()			   do{DRY_LED_EN_GPIO_Port->BSRR = DRY_LED_EN_Pin;}while(0)//HAL_GPIO_WritePin(DRY_LED_EN_GPIO_Port, DRY_LED_EN_Pin,GPIO_PIN_SET)
#define LED_DRY_OFF()              do{DRY_LED_EN_GPIO_Port->BSRR =(uint32_t)DRY_LED_EN_Pin<<16;}while(0)///HAL_GPIO_WritePin(DRY_LED_EN_GPIO_Port, DRY_LED_EN_Pin,GPIO_PIN_RESET)

#define LED_TIME_ON()			    do{TIME_LED_EN_GPIO_Port->BSRR = TIME_LED_EN_Pin;}while(0)///HAL_GPIO_WritePin(TIME_LED_EN_GPIO_Port, TIME_LED_EN_Pin ,GPIO_PIN_SET)
#define LED_TIME_OFF()              do{TIME_LED_EN_GPIO_Port->BSRR = (uint32_t)TIME_LED_EN_Pin<<16;}while(0)//HAL_GPIO_WritePin(TIME_LED_EN_GPIO_Port, TIME_LED_EN_Pin ,GPIO_PIN_RESET)

#define LED_PLASMA_ON()				 do{PLASMA_LED_EN_GPIO_Port->BSRR = PLASMA_LED_EN_Pin;}while(0)//HAL_GPIO_WritePin(PLASMA_LED_EN_GPIO_Port, PLASMA_LED_EN_Pin,GPIO_PIN_SET)
#define LED_PLASMA_OFF()             do{PLASMA_LED_EN_GPIO_Port->BSRR = (uint32_t)PLASMA_LED_EN_Pin<<16;}while(0)///HAL_GPIO_WritePin(PLASMA_LED_EN_GPIO_Port, PLASMA_LED_EN_Pin,GPIO_PIN_RESET)


#define LED_MOUSE_ON()  			 do{MOUSE_LED_EN_GPIO_Port ->BSRR = MOUSE_LED_EN_Pin ;}while(0)//HAL_GPIO_WritePin(GPIOB, MOUSE_LED_Pin,GPIO_PIN_SET)
#define LED_MOUSE_OFF()				 do{MOUSE_LED_EN_GPIO_Port ->BSRR =(uint16_t)MOUSE_LED_EN_Pin<<16;}while(0)//HAL_GPIO_WritePin(GPIOB, MOUSE_LED_Pin,GPIO_PIN_RESET)


//control SMG supply power 
#define SMG_POWER_ON()				 do{SMG_POWER_GPIO_Port->BSRR = SMG_POWER_Pin;}while(0)//HAL_GPIO_WritePin(SMG_POWER_GPIO_Port , SMG_POWER_Pin,GPIO_PIN_SET)
#define SMG_POWER_OFF()              do{SMG_POWER_GPIO_Port->BSRR = (uint32_t)SMG_POWER_Pin << 16;}while(0)//HAL_GPIO_WritePin(SMG_POWER_GPIO_Port , SMG_POWER_Pin,GPIO_PIN_RESET)







extern void (*panel_led_fun)(void);

void Panel_Led_OnOff_RunCmd(void (*panelledHandler)(void));



void Panel_Led_OnOff_Function(void);

void AI_Auto_Stop(void);
void Breath_Led(void);

void Power_Off_Led_Off(void);


void Power_ON_Led(void);


void wifi_connect_state_fun(void);




#endif 
