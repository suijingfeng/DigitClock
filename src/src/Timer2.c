#include<reg52.h>
#include<intrins.h>
#include"MacroAndConst.h"
//#include"SEG.h"
#include"lcd12864.h"
#include"AT24c02.h"
#include"ds1302.h"
#include"buzzer.h"
#include"delay.h"
sfr T2MOD=0xC9;
/*  bit:SJF		7	 6	  5	   4    3    2    1	   0 		*/
/*	T2CON=0C8h:	TF2	EXF2 RCLK TCLK EXEN2 TR2 C_T2 CP_RL2    */
/*  T2MOD=0C9H:	-	 -	  -	   -	-	 -	 T2OE  DCEN      */
/* 	P1.0=T2:外部计数输入---P1^1=T2EX:捕捉重载触发信号----*/
unsigned int temp=0;
unsigned int PowerNumber=0;
unsigned int EveragePower=0;
unsigned char dt;

static unsigned char second_start=0;//用于计算功率
static unsigned char minute_start=0;//用于计算功率
static unsigned char second_end=0;//用于计算功率
static unsigned char minute_end=0;//用于计算功率


void Timer2Init()
{
	RCAP2H=0x00;
	RCAP2L=0x00;
	
	TH2=RCAP2H;
	TL2=RCAP2L;

	EXEN2=1;//外部P1^1=T2EX允许位;
	C_T2=1;//内部计时器;
	CP_RL2=1;//捕捉;
	T2MOD=0;
	TR2=0;//开始计数;//stop
	ET2=1;//Enable Timer2 bit;	
}

void Timer2() interrupt 5
{
	if(TF2==1)
	{
		//Timer2FlowOut++;
		TF2=0;
	}
	if(EXF2==1)
	{
		EXF2=0;
		temp=read_word(5);
		PowerNumber=(temp<<8);
		temp=read_word(4);
		PowerNumber=PowerNumber+temp;
		PowerNumber++;
	
		At24c02ByteWrite(4,(unsigned char)(PowerNumber&0x00ff));
		At24c02ByteWrite(5,(unsigned char)(PowerNumber>>8));

		//记录脉冲间隔时间dt
		second_start=second_end;
		minute_start=minute_end;

		second_end=(CurrentTime.Second>>4)*10+(CurrentTime.Second&0x0f);
		minute_end=(CurrentTime.Minute>>4)*10+(CurrentTime.Minute&0x0f);
	    //更新功率信息
		if(minute_end>=minute_start)
		{
			dt=(minute_end-minute_start)*60+second_end-second_start;
			//EveragePower=(1/800)/(dt/3600)*1000w;
			EveragePower=2250/dt;
			PowerDt[0]=EveragePower/1000+'0';
			PowerDt[1]=EveragePower%1000/100+'0';
			PowerDt[2]=EveragePower%100/10+'0';
			PowerDt[3]=EveragePower%10+'0';
			PowerDt[4]='.';
			PowerDt[5]=(22500/dt-22500)%10+'0';
		}
	}
}
