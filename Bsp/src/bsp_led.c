#include "bsp.h"


volatile uint32_t led_k,led_i;


void (*panel_led_fun)(void);






static void TIME_LED_OnOff(uint8_t sel);

static void Power_Breath_Two(void);






/***********************************************************
*
*Function Name : static void DRY_LED_OnOff(uint8_t sel)
*
*
*
************************************************************/



static void TIME_LED_OnOff(uint8_t sel)
{
    if(sel==1)LED_TIME_ON();
	else  LED_TIME_OFF();


}

void KeyLed_Power_On(void)
{
   LED_POWER_ON();
   LED_TIME_ON();


}



void ALL_LED_Off(void)
{
   LED_PLASMA_OFF();
   LED_WIFI_OFF();
  LED_DRY_OFF();
  LED_TIME_OFF();


}

/*******************************************************************************************
 	*
 	* Function Name:void Panel_Led_OnOff_Function(void)
 	* Function :panel of led turn on or off which key function 
 	* Input Ref:NO
 	* Return Ref:NO
 	* 
*******************************************************************************************/  
void Panel_Led_OnOff_Function(void)
{

  // static uint8_t ai_changed_flag;

		
	  LED_POWER_ON();



    if(run_t.gMouse  == 0 ){

       LED_MOUSE_OFF();//mouse_led_onoff(0); //display fan of grass is one 
    } 
    else if(run_t.gMouse  == 1){ 
      
       LED_MOUSE_ON();   
    }
	 
	 
    if(run_t.gDry==1 && gpro_t.g_manual_shutoff_dry_flag ==0 && gpro_t.smartphone_app_timer_power_on_flag==0){
		 
	    LED_DRY_ON();
      
     }
	 else if((run_t.gDry==0 && gpro_t.smartphone_app_timer_power_on_flag==0)||(gpro_t.g_manual_shutoff_dry_flag ==1)){
	   LED_DRY_OFF();

	 }
   	

	 if(run_t.gPlasma==1){
	 	LED_PLASMA_ON();
	     
     }
	 else{
	   LED_PLASMA_OFF();

	 }

	
     if(run_t.time_led_flag ==1){
	    TIME_LED_OnOff(1);
	 }
	 else
	 	TIME_LED_OnOff(0);

    
	
}
/***************************************************************
*
*Function Name: void LED_TempHum(void)
*Function : display temperature and humidity and times led 
*
*
*
**************************************************************/


/***************************************************************
*
*Function Name: void LED_TempHum(void)
*Function : display temperature and humidity and times led 
*
*LED_Power_Key_On();
*
**************************************************************/
static void Power_Breath_Two(void)
{
   static uint8_t breathing_time = 0;
    static uint8_t led_state = 0;
    
    breathing_time++;
    if(breathing_time >= 100)  // 呼吸周期1秒
    {
        breathing_time = 0;
        led_state = !led_state;
        
        if(led_state)
        {
            LED_POWER_ON();
        }
        else
        {
            LED_POWER_OFF();
        }
    }
   

}



#if 0
static void Power_BreathOnOff(void)
{
    
     static uint32_t k;
   
    k++;

	if(k<40000){

	LED_POWER_ON();

    }
          

	if(k>40000 && k <80000){

	    LED_POWER_OFF();

	}
	if(k>80000 && k< 120000){

	  LED_POWER_TOGGLE();


	}

	if(k>120000){

        k=0;
	}

}

#endif 


void Breath_Led(void)
{
    
   
   // Power_BreathOnOff();
   Power_Breath_Two();


   
}



/***************************************************************
*
*Function Name: void Panel_Led_OnOff_RunCmd(void (*panelledHandler)(void))
*Function : display temperature and humidity and times led 
*
*
*
**************************************************************/
void Panel_Led_OnOff_RunCmd(void (*panelledHandler)(void))
{

	 panel_led_fun= panelledHandler;
}

void Power_Off_Led_Off(void)
{
   LED_WIFI_OFF();
   LED_DRY_OFF();
   LED_TIME_OFF();
   LED_PLASMA_OFF();
   LED_MOUSE_OFF();
   LED_POWER_OFF();
   SMG_POWER_OFF();
   Power_Off_Fun();

}

void Power_ON_Led(void)
{
   if(gpro_t.smartphone_app_timer_power_on_flag ==0){
   LED_DRY_ON();
   LED_TIME_ON();
   LED_PLASMA_ON();
   LED_MOUSE_ON();
   LED_POWER_ON();
   SMG_POWER_ON();
   if(run_t.connect_wifi_state == wifi_connect_success){
	        LED_WIFI_ON();
    }
	 
   	}
    else{
	  LED_TIME_ON();

	  LED_POWER_ON();
	  SMG_POWER_ON();


	}
	

}

//if(run_t.gTimer_wifi_connect_counter <120){

/**
*@brief 
*@param
*@notice
**/
void wifi_connect_state_fun(void)
{

    static uint8_t led_blink_counter;

	if(run_t.wifi_led_fast_blink==1 && run_t.connect_wifi_state == wifi_connect_null && run_t.gPower_On ==1){
    
        LED_WIFI_TOGGLE() ;
    }
	else{
       if(run_t.connect_wifi_state == wifi_connect_null && run_t.gPower_On ==1){
              led_blink_counter ++ ;
			  if(led_blink_counter > 9){
				  led_blink_counter=0;

			      LED_WIFI_TOGGLE() ;

			  }

       }
       else if(run_t.connect_wifi_state == wifi_connect_success && run_t.gPower_On ==1){
	  
          LED_WIFI_ON();
      }

    }

}


