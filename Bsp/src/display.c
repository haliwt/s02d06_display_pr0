#include "bsp.h"

static void TimeColon_Smg_Blink_Fun(void);


/**********************************************************************
*
*Functin Name: void Display_DHT11_Value(void)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Display_DHT11_Value(void)
{
    
  static uint8_t hum1,hum2; 
  static uint8_t temp1,temp2;

	hum1 =  run_t.gReal_humtemp[0]/10;  //Humidity 
	hum2 =  run_t.gReal_humtemp[0]%10;

	temp1 = run_t.gReal_humtemp[1]/10 ;  // temperature
	temp2 = run_t.gReal_humtemp[1]%10;

 	TM1639_Write_2bit_TempData(temp1,temp2);
    
	TM1639_Write_2bit_HumData(hum1,hum2);
}  
/**********************************************************************
*
*Functin Name: void Display_Timing(uint8_t hours,uint8_t minutes)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Display_Timing(uint8_t hours,uint8_t minutes)
{ 
    static uint8_t m,q;
	m = hours /10 ;
	run_t.hours_two_unit_bit =	hours%10; 
	run_t.minutes_one_decade_bit= minutes/10 ;
	q=  minutes%10;
	TM1639_Write_4Bit_Time(m,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,q,0) ; //timer is default 12 hours "12:00"

}
/********************************************************************************
*
*Functin Name: void Display_Error_Digital(uint8_t errnumbers,uint8_t sel)
*Function : Timer of key be pressed handle
*Input Ref:  error digital 
*Return Ref: NO
*
********************************************************************************/
void Display_Error_Digital(uint8_t errnumbers,uint8_t sel)
{ 
    static uint8_t m,q;
	m = 0x0E;
	
	run_t.hours_two_unit_bit= 0x0d;
	
	run_t.minutes_one_decade_bit= errnumbers/10;
	q=errnumbers%10;
	TM1639_Write_4Bit_Time_D2bit(m,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,q,sel) ; //timer is default 12 hours "12:00"
 

}

/********************************************************************************
*
*Functin Name: static void TimeColon_Smg_Blink_Fun(void)
*Function : Timer of key be pressed handle
*Input Ref:  NO
*Return Ref: NO
*
********************************************************************************/
static void TimeColon_Smg_Blink_Fun(void)
{
	
	static uint8_t i;

    if(gpro_t.disp_timer_or_time_mode != SET_TIMER_ITEM || gpro_t.disp_timer_or_time_mode != PTC_WARNING || gpro_t.disp_timer_or_time_mode != FAN_WARNING){
     i++ ;
     if(i ==1){
		  SmgBlink_Colon_Function(run_t.hours_two_unit_bit ,run_t.minutes_one_decade_bit,0);
     }
     else{
          if(i > 5){
           i=0;

          }
	
		   SmgBlink_Colon_Function(run_t.hours_two_unit_bit ,run_t.minutes_one_decade_bit,1);
        }

    }
}


void Display_TimeColon_Blink_Fun(void)
{

  if(run_t.gTimer_time_colon >200){ //10*20ms=300ms

	   run_t.gTimer_time_colon =0;
       
	   TimeColon_Smg_Blink_Fun();
	}
}


void fan_disp_error_number(void)
{
    static uint8_t alternate_flag;

   if(run_t.fan_warning ==1){ //display "Er: 02"

		

					  
        Display_Error_Digital(0x02,0);

        osDelay(200);
        Display_Error_Digital(0x10,1);
        osDelay(200);
                     
}
}

void ptc_disp_error_number(void)
{
    static uint8_t alternate_flag;

   if(run_t.ptc_warning ==1){ //display "Er: 02"

		Display_Error_Digital(0x01,0);

        osDelay(200);
        Display_Error_Digital(0x10,1);
        osDelay(200);
                     
  }
}



