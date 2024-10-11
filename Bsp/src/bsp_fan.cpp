#include "bsp.h"


void mouse_class::led_mouse_on(void)
{
   LED_MOUSE_ON() ;


}
void mouse_class::led_mouse_off(void)
{

 LED_MOUSE_OFF() ;

}


 void mouse_class::mouse_led_onoff(uint8_t sel)
{

	if(sel==1)LED_MOUSE_ON();
	else  LED_MOUSE_OFF();


}





