#include<reg52.h>
#include"delay.h"

#define ACTIVE_BUZZER

sbit buzzer=P3^7;

bit BuzzerFlag=0;

unsigned char BuzzerTime[9]={1,2,'-',3,4,'-',5,6,' '};//闹钟

void BuzzerOn()
{
	buzzer=~buzzer;
}

void BuzzerOff()
{
	buzzer=1;
}

/* 
void BuzzerPlay()
{
	BuzzerOn();
	DelayUs(250);
	DelayUs(250);
	DelayUs(250);
	DelayUs(250);
	//BuzzerOff();
}

void BuzzerPlay(void)
{
	unsigned char i,j;
	for(i=0;i<16;i++)
	{
		for(j=0;j<100;j++)
		{
			buzzer=0;
		//	delay8us(tone1[song[i]]);
			buzzer=1;
		//	delay8us(tone1[song[i]]);
		}
	}
}

void BeepPlay(void)
{
	unsigned char i,j,k;
	for(i=0;i<3;i++)
	{
		for(j=0;j<12;j++)
		{
			for(k=0;k<100;k++)
			{
				beep=0;
				delay8us(tone[i][j]);
				beep=1;
				delay8us(tone[i][j]);
			}
		 }
	}
} 

 //整点报时，还没完成//
void BellOnTime()
{
	if((CurrentTime.Minute==0x00)&&(CurrentTime.Second==0x00)
	&&((10*LedDisBuffer[0]+LedDisBuffer[1])<=20)&&((10*LedDisBuffer[0]+LedDisBuffer[1])>=8))
	{
		BuzzerOn();
	}
	else
		BuzzerOff();
}
*/
