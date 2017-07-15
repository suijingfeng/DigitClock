#include<reg52.h>
#include"MacroAndConst.h"
#include"Timer.h"

#define DataPort P0

sbit LedBitSel=P2^7;
sbit LedSegSel=P2^6;

bit SetTimeFlag=0;
bit SegJumpShowFlag=0;

const unsigned char code LedDisCode[45]=
{
	0x3f,0x06,0x5b,0x4f,0x66,//0,1,2,3,4,
	0x6d,0x7d,0x07,0x7f,0x6f,//5,6,7,8,9,//5=S
	0x77,0x7c,0x39,0x5e,0x79,//A,b,C,d,E,
	0x71,0x40,0x63,0x72,0x00,//F,'-','.','R',' ',
	0xbf,0x86,0xdb,0xcf,0xe6,//0.,1.,2.,3.,4.,
	0xed,0xfd,0x87,0xff,0xef,//5.,6.,7.,8.,9.
	0x76,0x30,0x38,0x33,0x27,//H,I,L,M,m
	0x37,0x31,0x3e,0x3c,0x1e,//N,T,u,W,w
	0x5c,0x3e,0x6e,0x73,0x75//o,U,y,P
};


const unsigned char code ERROR[5]={14,'R','R',0,'R'};

//unsigned char LedDisBuffer[9]={0,0,'-',0,0,'-',0,0,' '};//hour,minute,second
unsigned char SegFlashBuffer[24]={
			  2,0,0,0,'-',0,0,'-',0,0,'-',0,'-',
		          0,0,'-',0,0,'-',0,0,'-',
		 						 0,0};

unsigned char g_uBufPosition=7;//global

static void SendLedSegData(uchar dat)
{
	//DataPort=~dat;//共阳
	DataPort=dat;//共阴
	LedSegSel=1;
	LedSegSel=0;
}

static void SendLedBitData(unsigned char dat)
{
	uchar temp;
//	temp=(0x80>>dat);//共阳
	temp=~(0x80>>dat);
	DataPort=temp;

	LedBitSel=1;
	LedBitSel=0;
}
 
void SegJumpShow(unsigned char* pBuffer)
{
    unsigned char temp;

	if(SegJumpShowFlag&SegJump1HzFlag)
	{	
		SegJump1HzFlag=0;
		temp=pBuffer[g_uBufPosition];
		pBuffer[g_uBufPosition]=pBuffer[8];
		pBuffer[8]=temp;				
	}	
}

void LedDisplay(uchar* pBuffer)
{
	static uchar LedDisPos=0;
	
	if(SegScanFlag)
	{					
		SegScanFlag=0;	
		SendLedBitData(8);//消影，适用于共阳极数码管
		if(((pBuffer[LedDisPos]>=0)&&(pBuffer[LedDisPos]<=15))||((pBuffer[LedDisPos]>=20)&&(pBuffer[LedDisPos]<=29)))
		{
			SendLedSegData(LedDisCode[pBuffer[LedDisPos]]);	
		}
		else if(pBuffer[LedDisPos]=='-')
		{
			SendLedSegData(LedDisCode[16]);
		}
		else if(pBuffer[LedDisPos]=='.')
		{
			SendLedSegData(LedDisCode[17]);
		}
		else if(pBuffer[LedDisPos]=='R')
		{
			SendLedSegData(LedDisCode[18]);
		}
		else if(pBuffer[LedDisPos]==' ')
		{
			SendLedSegData(LedDisCode[19]);
		}
		else if(pBuffer[LedDisPos]=='H')
		{
			SendLedSegData(LedDisCode[30]);
		}
		else if(pBuffer[LedDisPos]=='I')
		{
			SendLedSegData(LedDisCode[31]);
		}
		else if(pBuffer[LedDisPos]=='L')
		{
			SendLedSegData(LedDisCode[32]);
		}
		else if(pBuffer[LedDisPos]=='M')
		{
			SendLedSegData(LedDisCode[33]);
		}
		else if(pBuffer[LedDisPos]=='m')
		{
			SendLedSegData(LedDisCode[34]);
		}
		else if(pBuffer[LedDisPos]=='N')
		{
			SendLedSegData(LedDisCode[35]);
		}
		else if(pBuffer[LedDisPos]=='T')
		{
			SendLedSegData(LedDisCode[36]);
		}
		else if(pBuffer[LedDisPos]=='U')
		{
			SendLedSegData(LedDisCode[37]);
		}
		else if(pBuffer[LedDisPos]=='W')
		{
			SendLedSegData(LedDisCode[38]);
		}
		else if(pBuffer[LedDisPos]=='w')
		{
			SendLedSegData(LedDisCode[39]);
		}
		else if(pBuffer[LedDisPos]=='o')
		{
			SendLedSegData(LedDisCode[40]);
		}
		SendLedBitData(LedDisPos);
		
		if(++LedDisPos>7)
		{
			LedDisPos=0;
		}
		
		SegJumpShow(pBuffer);	
	}			
}
/*
void SegFlash()
{
	unsigned char i=0;
	static unsigned char StartCpyPos=0;

	if(SegFlash1s)
	{
		SegFlash1s=0;
		
		for(i=0;i<8;i++)
		{
			LedDisBuffer[i]=SegFlashBuffer[i+StartCpyPos];	
		}
		StartCpyPos++;
		if(StartCpyPos==14)
		{
			StartCpyPos=0;
		}
	 }
}
*/

/*
void SetClock(uchar hour,uchar minute,uchar second)
{
	LedDisBuffer[0]=hour/10;
	LedDisBuffer[1]=hour%10;
	LedDisBuffer[2]='-';
	LedDisBuffer[3]=minute/10;
	LedDisBuffer[4]=minute%10;
	LedDisBuffer[5]='-';
	LedDisBuffer[6]=second/10;
	LedDisBuffer[7]=second%10;
}
*/

/*使用AT89S52系统时钟编辑显示时间*
void RunClock(void)
{
	if(Time1S)
	{
		Time1S = 0 ;
		if(++LedDisBuffer[7]==10)
		{
			LedDisBuffer[7]=0;
			if(++LedDisBuffer[6]==6)
			{
				LedDisBuffer[6] = 0 ;
				if(++LedDisBuffer[4] == 10)
				{	
					LedDisBuffer[4] = 0 ;
					if(++LedDisBuffer[3] == 6)
					{
						LedDisBuffer[3] = 0 ;
						if(LedDisBuffer[0]<2)
						{
							if(++LedDisBuffer[1]==10)
							{
								LedDisBuffer[1]=0;
								LedDisBuffer[0]++;
							}
						}
						else
						{
							if(++LedDisBuffer[1]==4)
							{
								LedDisBuffer[1] = 0 ;
								LedDisBuffer[0] = 0 ;
							}
						}
					}
				}
			}
		}
	}
}
*/
