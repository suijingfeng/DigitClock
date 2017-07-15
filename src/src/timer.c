#include<reg52.h>
#include"TIMER.h"

sfr T2MOD=0xC9;
/*  bit:SJF		7	 6	  5	   4    3    2    1	   0 	 */
/*	T2CON=0C8h:	TF2	EXF2 RCLK TCLK EXEN2 TR2 C/T2 CP/RL2 */
/*  T2MOD=0C9H:	-	 -	  -	   -	-	 -	 T2OE  DCEN   */
/* 	P1.0=T2:外部计数输入----P1^1=T2EX:捕捉重载触发信号----*/

static unsigned char T0KeyScanTime2ms=0;
static unsigned char T0SegJumpTime2ms=0;
static unsigned char T0IrScanTime2ms=0;
static unsigned int T0Ds18b20Time2ms=0;
static unsigned int SegFlashFlag=0;

unsigned char T1_250us=0;

bit SegScanFlag=0;
bit SegJump1HzFlag=0;//0.5s时标
bit	SegFlash1s=0;

bit KeyScanFlag=0;
bit Ds1302UpdateFlag=0;
bit Ds18b20UpdateFlag=0;
bit IrScanFlag=0;

bit Flag_4KHz=0;

bit Led_PWM_Flag=0;


void Timer0Init()
{
	TMOD&=0xf0;
	TMOD|=0x01;
	TH0=(65536-2000)/256;
	TL0=(65536-2000)%256;
	TR0=1;
	ET0=1;
}

void Timer1Init()
{
	TMOD&=0x0f;   
  	TMOD|=0x10;   
  	TH1=(65536-250)/256;
  	TL1=(65536-250)%256;
    TR1=1;
	ET1=1; 	
}

void Timer2Init()
{
	RCAP2H=(65536-200)/256;// 200us
	RCAP2L=(65536-200)%256;// 0.2ms
	
	TH2=RCAP2H;
	TL2=RCAP2L;

	EXEN2=0;//不允许外部P1^1(=T2EX）位;
	C_T2=0;//counter
	CP_RL2=0;//auto reload
	T2MOD=0;
	ET2=1;//Enable Timer2 interrupt;	
	TR2=1;//Timer2 start//stop 
}

void Timer0Interrupt(void) interrupt 1
{
	TH0=(65536-2000)/256;
	TL0=(65536-2000)%256;

	SegScanFlag=1;

	if(++T0KeyScanTime2ms==10);//for key scan
	{
		KeyScanFlag=1;
		T0KeyScanTime2ms=0;
	}
	
	if(++T0IrScanTime2ms==25)
	{
		T0IrScanTime2ms=0;
		IrScanFlag=1;
	}	

	if(++T0SegJumpTime2ms==250)
	{
		SegJump1HzFlag=1;
		Ds1302UpdateFlag=1;
		T0SegJumpTime2ms=0;
	}
	if(++SegFlashFlag==1500)
	{
		SegFlashFlag=0;
		SegFlash1s=1;	
	}	

	if(++T0Ds18b20Time2ms==1000)
	{
		T0Ds18b20Time2ms=0;
		Ds18b20UpdateFlag=1;	
	}		
}

void Timer1Interrupt(void) interrupt 3
{
	TH1=(65536-250)/256;;
  	TL1=(65536-250)%256; //50ms
		
	if(++T1_250us>=250)//50000us=50ms
	{
		T1_250us=0;
	}
}

void Timer2Interrupt(void) interrupt 5
{
	TF2=0;
	Led_PWM_Flag=1;						
}




