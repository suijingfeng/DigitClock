#include<reg52.h>
//#include"seg.h"
#include"buzzer.h"
#include"timer.h"
#include"delay.h"


#define IrStateStart	0
#define IrStateWobble	1
#define IrStateData		2

#define IrStartUpLim	60
#define IrStartDownLim	50

#define IrData1UpLim	12//这个大一点，补偿程序执行时间。减少bug
#define IrData1DownLim	7

#define IrData0UpLim	7
#define IrData0DownLim	3

//I define The meanings of the key which i use

#define IrKeyVal_CH_UP		0x47
#define IrKeyVal_CH	  		0x46
#define IrKeyVal_CH_DOWN	0x45
#define IrKeyVal_100_		0x19	 
#define IrKeyVal_200_		0x0d



#define IrKeyVal_Next		0x44//seg flash>>
#define IrKeyVal_Last		0x40//seg flash<<
#define IrKeyVal_EQ         0x09//SET
#define IrKeyVal_Down		0x07//-
#define IrKeyVal_Up			0x15//+

#define IrKeyVal_Play		0x43//buzzer use

#define IrKeyVal_0			0x16
#define IrKeyVal_1			0x0c
#define IrKeyVal_2			0x18
#define IrKeyVal_3			0x5e
#define IrKeyVal_4			0x08
#define IrKeyVal_5			0x1c
#define IrKeyVal_6			0x5a
#define IrKeyVal_7			0x42
#define IrKeyVal_8			0x52
#define IrKeyVal_9			0x4a


static unsigned char IrLowAdr;
static unsigned char IrHighAdr;
static unsigned char IrLowCmd;
static unsigned char IrHighCmd;

bit SetModeFlag=0;
bit SegShowDisableFlag=0;

struct
{	
	unsigned char SegPicture;
	unsigned char Number;
	unsigned char IrCode[4];
}IrKeyVal;

//以状态转移的思想来编写遥控器解码。sjf原创

void Ex0IptInit()
{
	IT0=1;
	EX0=1;
	
	IrKeyVal.SegPicture=0;
	IrKeyVal.Number=0;
	IrKeyVal.IrCode[0]=0;
	IrKeyVal.IrCode[1]=0;
	IrKeyVal.IrCode[2]=0;
	IrKeyVal.IrCode[3]=0;			
}

void Ex0() interrupt 0
{
	static unsigned char IrStateFlag=IrStateStart;
    static unsigned char Ex0IptNum=0;
	static unsigned char IrData=0;
	static unsigned char IptIntervalTime=0;
									
	IptIntervalTime=T1_250us;
	T1_250us=0;
			
	switch(IrStateFlag)
	{	
		case IrStateStart:
		{
			IrStateFlag=IrStateWobble;
			T1_250us=0;
			TR1=1;							
		}break;
		
		case IrStateWobble://
		{
			if((IptIntervalTime>=IrStartDownLim)&&(IptIntervalTime<=IrStartUpLim))
			{
				IrStateFlag=IrStateData;		
			}
			else
			{
	 			IrStateFlag=IrStateStart;
				TR1=0;
			}
		}break;

		case IrStateData:
		{	
			IrData>>=1;
			if((IptIntervalTime>IrData1DownLim)&&(IptIntervalTime<=IrData1UpLim))
			{
				IrData|=0x80;	
			}

			Ex0IptNum++;
			
			if(Ex0IptNum==8)
			{
				IrLowAdr=IrData;			 
			}

			if(Ex0IptNum==16)
			{
				IrHighAdr=IrData;	
			}

			if(Ex0IptNum==24)
			{
				IrLowCmd=IrData;	
			}
			if(Ex0IptNum==32)
			{
				IrHighCmd=IrData;
				TR1=0;
				Ex0IptNum=0;
				IrStateFlag=IrStateStart;
		 	}
		}break;

		default:break;
	}
}

static void IrKeyValueProcess()
{ 
	if((IrLowCmd==(~IrHighCmd))&&(IrLowAdr==0x00)&&(IrHighAdr=0xff))
	{
		switch(IrLowCmd)
		{
			case IrKeyVal_0:{IrKeyVal.Number=0;}break;
			case IrKeyVal_1:{IrKeyVal.Number=1;}break;
			case IrKeyVal_2:{IrKeyVal.Number=2;}break;
			case IrKeyVal_3:{IrKeyVal.Number=3;}break;
			case IrKeyVal_4:{IrKeyVal.Number=4;}break;
			case IrKeyVal_5:{IrKeyVal.Number=5;}break;
			case IrKeyVal_6:{IrKeyVal.Number=6;}break;
			case IrKeyVal_7:{IrKeyVal.Number=7;}break;
			case IrKeyVal_8:{IrKeyVal.Number=8;}break;
			case IrKeyVal_9:{IrKeyVal.Number=9;}break;
			
			case IrKeyVal_Up:
			{
				if(++IrKeyVal.Number==10)
				{
					IrKeyVal.Number=0;
				}
			}break;

			case IrKeyVal_Down:
			{
				if(IrKeyVal.Number==0)
				{
					IrKeyVal.Number=9;
				}
				else
				{ 
					IrKeyVal.Number--;
				}
			}break;
			
			case IrKeyVal_Next:
			{
				if(++IrKeyVal.SegPicture==4)
				{
					IrKeyVal.SegPicture=0;
				}
			}break;

			case IrKeyVal_Last:
			{
				if(IrKeyVal.SegPicture==0)
				{
					IrKeyVal.SegPicture=4;
				}
				else
				{ 
					IrKeyVal.SegPicture--;
				}
			}break;

			case IrKeyVal_EQ:
			{
				SetModeFlag=~SetModeFlag; //
			}break;											     
			
			case IrKeyVal_Play:
			{
				 BuzzerFlag=~BuzzerFlag;  //
			}break;//play music
		
			default:
			{
				IrKeyVal.IrCode[0]=IrLowAdr;
				IrKeyVal.IrCode[1]=IrHighAdr;
				IrKeyVal.IrCode[2]=IrLowCmd;
				IrKeyVal.IrCode[3]=IrHighCmd;
			} break; 
		}
	IrLowAdr=0x00;
	IrHighAdr=0x00;
	IrLowCmd=0x00;
	IrHighCmd=0x00;
	}
}

void IrKeyMainLoop()
{
	if(IrScanFlag)
	{
		IrScanFlag=0;
		IrKeyValueProcess();
	
	}
}

