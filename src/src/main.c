#include<reg52.h>
#include<intrins.h>
#include"MacroAndConst.h"
#include"Timer.h"
#include"ds1302.h"
#include"Seg.h"
#include"PWMLED.h"
#include"key.h"
#include"buzzer.h"
#include"ds18b20.h"
#include"delay.h"
#include"ExInterrupt.h"
//还想实现的秒表,设定闹钟,数码管的飞入显示,温度报警,农历

sfr AUXR=0x8E;

void main()
{	
	Ex0IptInit();
	Timer0Init();
	Timer1Init();
	Timer2Init();
	/////////
	AUXR=0x01;
	/////////
	KeyInit();
	
	DS1302_ClockInit();
	EA=1;

	while(1)
	{	
		KeyMainLoop();
		IrKeyMainLoop();
		
		if(SetModeFlag) 
		{
			//SegFlash();
		}

	   	if(BuzzerFlag)
		{
			BuzzerOn();
		}
		else
		{
			BuzzerOff();	
		}
	  
	    
		if(Led_PWM_Flag)
		{
			Led_PWM_Flag=0;
			LEDisplay();
		}

		switch(IrKeyVal.SegPicture)//IrLowCmd=IrKeyValue
		{
			case 0://DS1302时钟0.5秒更新一次。显示时、分、秒
			{
				LedDisplay(TimeBuf1);//原来是：LedDisplay(LedDisBuffer);
				
				if(Ds1302UpdateFlag&(~SetTimeFlag))
				{
					Ds1302UpdateFlag=0;
					DS1302_ClockUpdata_SMH(TimeBuf1);//原来是：DS1302_ClockUpdata_SMH(LedDisBuffer);	
				}			
			}break;

			case 1://年、月、日	
			{							   
				LedDisplay(TimeBuf2);//LedDisplay(LedDisBuffer);
				if(Ds1302UpdateFlag&(~SetTimeFlag)) //DS1302时钟0.5秒更新一次。
				{
					Ds1302UpdateFlag=0;
					DS1302_ClockUpdata_DMY(TimeBuf2);//原来是：DS1302_ClockUpdata_DMY(LedDisBuffer);
				}
				
			}break;

			case 2://week
			{
				LedDisplay(TimeBuf3);
				if(Ds1302UpdateFlag&(~SetTimeFlag)) //DS1302时钟0.5秒更新一次。
				{
					Ds1302UpdateFlag=0;
					DS1302_ClockUpdata_WEEK(TimeBuf3);
				}	
			}break;
			
			case 3://temperature
			{
				 LedDisplay(CurrentTemperature);
				 if(Ds18b20UpdateFlag&(~SetTemperatureFlag))
				 {
		  		 	Ds18b20UpdateFlag=0;
		 			TemperatureUpdate();
	   			 }	
			}break;

			case 4:
			{
				//DS1302_ClockBurstRead();
			} break;

			case 5:
			{
		/*		LedDisplay(LedDisBuffer);
				LedDisBuffer[0]=IrKeyVal.IrCode[0]>>4;
				LedDisBuffer[1]=IrKeyVal.IrCode[0]&0x0f;

				LedDisBuffer[2]=IrKeyVal.IrCode[1]>>4;
				LedDisBuffer[3]=IrKeyVal.IrCode[1]&0x0f;

				LedDisBuffer[4]=IrKeyVal.IrCode[2]>>4;
				LedDisBuffer[5]=IrKeyVal.IrCode[2]&0x0f;

				LedDisBuffer[6]=IrKeyVal.IrCode[3]>>4;	
				LedDisBuffer[7]=IrKeyVal.IrCode[3]&0x0f;  */
			}break;
			default:break;
		}			 
	}  
}



	

