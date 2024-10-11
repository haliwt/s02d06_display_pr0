#ifndef __BSB_FAN_H
#define  __BSP_FAN_H
#include "main.h"


#ifdef __cplusplus
extern "C" {
#endif



#define LED_MOUSE_ON()  		    HAL_GPIO_WritePin(FAN_LED_EN_GPIO_Port, FAN_LED_EN_Pin,GPIO_PIN_SET)
#define LED_MOUSE_OFF()				HAL_GPIO_WritePin(FAN_LED_EN_GPIO_Port, FAN_LED_EN_Pin,GPIO_PIN_RESET)


class mouse_class{


   public:

    void led_mouse_on(void);
    void led_mouse_off(void);

    void mouse_led_onoff(uint8_t sel);

   

};








#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

