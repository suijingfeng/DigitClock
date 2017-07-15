#include<reg52.h>
#include"MacroAndConst.h"
#include"intrins.h"


#define DS1302_SECOND_W 0x80
#define DS1302_MINUTE_W 0x82
#define DS1302_HOUR_W	0x84
#define DS1302_DATE_W	0x86
#define DS1302_MONTH_W	0x88
#define DS1302_WEEK_W	0x8a
#define DS1302_YEAR_W	0x8c

#define DS1302_SECOND_R 0x81
#define DS1302_MINUTE_R 0x83
#define DS1302_HOUR_R	0x85
#define DS1302_DATE_R	0x87
#define DS1302_MONTH_R	0x89
#define DS1302_WEEK_R	0x8b
#define DS1302_YEAR_R	0x8d

//clock burst mode
#define DS1302_CLOCK_BURST_R	0xbf
#define DS1302_CLOCK_BURST_W	0xbe
//RAM burst mode
#define DS1302_RAM_BURST_R	0xff
#define DS1302_RAM_BURST_W	0xfe

sbit DS1302_SCLK=P2^0;
sbit DS1302_IO=P2^1;
sbit DS1302_RST=P2^2;

/*为了提高效率，我觉得不必要使用结构体
struct
{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Date;
	unsigned char Month;
	unsigned char Week;
	unsigned char Year; 
}CurrentTime;
*/
unsigned char TimeBuf1[9]={0,0,'-',0,0,'-',0,0,' '};//hour,minute,second;
unsigned char TimeBuf2[9]={0,0,'-',0,0,'-',0,0,' '};//year,month,date
unsigned char TimeBuf3[9]={'-','-','-','-','-','-','-',0,' '};

static void DS1302Write(unsigned char content)
{
	uchar i;
	for(i=8;i>0;i--)
	{
		_nop_();_nop_();
		if(content&0x01)
		{
			DS1302_IO=1;	
		}
		else
		{
			DS1302_IO=0;
		}
		content>>=1;

		DS1302_SCLK=1;
		DS1302_SCLK=0;
	}
}

static uchar DS1302Read(void)
{
	uchar i,ReadValue;
	//DS1302_IO=1;
	_nop_();_nop_();
	for(i=8;i>0;i--)
	{
		ReadValue>>=1;
		_nop_();_nop_();_nop_();_nop_();
		if(DS1302_IO==1)
		{
			ReadValue|=0x80;
		}
		else
		{
			ReadValue&=0x7f;
		}
		DS1302_SCLK=1;
		_nop_();_nop_();
		DS1302_SCLK=0;
		_nop_();_nop_();
	}
	return ReadValue;
}

void DS1302WriteByte(uchar address,uchar content)
{
	DS1302_RST=0;
	DS1302_SCLK=0;
	DS1302_RST=1;

	DS1302Write(address);
	DS1302Write(content);

	DS1302_RST=0;
	DS1302_SCLK=1;
}

unsigned char DS1302ReadByte(uchar address)
{
	uchar ReadValue;
	DS1302_RST=0;
	DS1302_SCLK=0;
	DS1302_RST=1;

	DS1302Write(address);
	ReadValue=DS1302Read();

	DS1302_RST=0;
	DS1302_SCLK=1;

	return ReadValue;
}

void DS1302_ClockInit()
{
	if(DS1302ReadByte(0xc1)!=0xf0)//在没有电池的情况下，这句要
	{								//注释掉，因为断电后无法初始化				
									//这是我调试2小时的经验啊。sjf
		DS1302WriteByte(0x8e,0x00);//写保护，允许写
		DS1302WriteByte(DS1302_YEAR_W,0x14);
		DS1302WriteByte(DS1302_MONTH_W,0x06);
		DS1302WriteByte(DS1302_DATE_W,0x05);
		
		DS1302WriteByte(DS1302_WEEK_W,0x04);
		DS1302WriteByte(DS1302_HOUR_W,0x10);
		DS1302WriteByte(DS1302_MINUTE_W,0x20);
		DS1302WriteByte(DS1302_SECOND_W,0x30);

		//DS1302WriteByte(0x90,0xa5);//涓流充电对于普通的锂电池不可充电
		DS1302WriteByte(0xc0,0xf0);
		DS1302WriteByte(0x8e,0x80);//写保护，不允许写
	}
}

void DS1302_SetTime_HMS(uchar hour,uchar minute,uchar second)
{
	DS1302WriteByte(0x8e,0x00);//写保护，允许写
	DS1302WriteByte(DS1302_HOUR_W,hour);
	DS1302WriteByte(DS1302_MINUTE_W,minute);
	DS1302WriteByte(DS1302_SECOND_W,second);
	DS1302WriteByte(0x8e,0x80);//写保护，不允许写
}

void DS1302_SetTime_YMD(uchar year,uchar month,uchar day)
{
	DS1302WriteByte(0x8e,0x00);//写保护，允许写
	DS1302WriteByte(DS1302_YEAR_W,year);
	DS1302WriteByte(DS1302_MONTH_W,month);
	DS1302WriteByte(DS1302_DATE_W,day);
	DS1302WriteByte(0x8e,0x80);//写保护，不允许写
}

void DS1302_SetTime_WEEK(unsigned char week)
{
	DS1302WriteByte(0x8e,0x00);//写保护，允许写
	DS1302WriteByte(DS1302_WEEK_W,week);
	DS1302WriteByte(0x8e,0x80);//写保护，不允许写		
}

void DS1302_ClockUpdata_SMH(unsigned char* pLedCode)
{
	unsigned char Second,Minute,Hour;

	Second=DS1302ReadByte(DS1302_SECOND_R);
	Minute=DS1302ReadByte(DS1302_MINUTE_R);
	Hour=DS1302ReadByte(DS1302_HOUR_R);

	pLedCode[0]=Hour>>4;
	pLedCode[1]=Hour&0x0f;
	pLedCode[2]='-';
	pLedCode[3]=Minute>>4;
	pLedCode[4]=Minute&0x0f;
	pLedCode[5]='-';
	pLedCode[6]=Second>>4;
	pLedCode[7]=Second&0x0f;
}
void DS1302_ClockUpdata_DMY(unsigned char* pLedCode)
{
	unsigned char Date,Month,Year;

	Date=DS1302ReadByte(DS1302_DATE_R);
	Month=DS1302ReadByte(DS1302_MONTH_R);
	Year=DS1302ReadByte(DS1302_YEAR_R);
	
	pLedCode[0]=Year>>4;
	pLedCode[1]=Year&0x0f;
	pLedCode[2]='-';
	pLedCode[3]=Month>>4;
	pLedCode[4]=Month&0x0f;
	pLedCode[5]='-';
	pLedCode[6]=Date>>4;
	pLedCode[7]=Date&0x0f;		
}

void DS1302_ClockUpdata_WEEK(unsigned char* pBuffer)
{
	pBuffer[7]=DS1302ReadByte(DS1302_WEEK_R);
	switch(pBuffer[7])
	{
		case 1:
		{
			pBuffer[0]='M';
			pBuffer[1]='m';
			pBuffer[2]=0;
			pBuffer[3]='N';
		}break;

		case 2:
		{
			pBuffer[0]='T';
			pBuffer[1]='U';
			pBuffer[2]=14;
			pBuffer[3]=5;
		}break;

		case 3:
		{
			pBuffer[0]='W';
			pBuffer[1]='w';
			pBuffer[2]=14;
			pBuffer[3]=13;
		}break;

		case 4:
		{
			pBuffer[0]='T';
			pBuffer[1]='H';
			pBuffer[2]='U';
			pBuffer[3]='R';
		}break;

		case 5:
		{
			pBuffer[0]=15;
			pBuffer[1]='R';
			pBuffer[2]='I';
			pBuffer[3]='-';
		}break;

		case 6:
		{
			pBuffer[0]=5;
			pBuffer[1]=10;
			pBuffer[2]='T';
			pBuffer[3]='-';
		}break;

		case 7:
		{
			pBuffer[0]=5;
			pBuffer[1]='U';
			pBuffer[2]='N';
			pBuffer[3]='-';
		}break;
		default:break;
	}			
}
/*
void DS1302_ClockBurstRead(unsigned char* pSegFlaBuf)
{
	unsigned char Second,Minute,Hour,Date,Month,Week,Year;
	DS1302WriteByte(0x8e,0x00);
	
	DS1302_RST=1;
	DS1302Write(DS1302_CLOCK_BURST_R);

	Second=DS1302Read();
	Minute=DS1302Read();
	Hour=DS1302Read();
	Date=DS1302Read();
	Month=DS1302Read();
	Week=DS1302Read();
	Year=DS1302Read();
	
	DS1302_RST=0;

	DS1302WriteByte(0x8e,0x80);
					
}



/*
void DS1302_ClockBurstWrite(unsigned char* pContent)
{
	DS1302WriteByte(0x8e,0x00);
	DS1302_RST=1;
	DS1302Write(DS1302_CLOCK_BURST_W);
	DS1302Write(pContent[0]);
	DS1302Write(pContent[1]);
	DS1302Write(pContent[2]);
	DS1302Write(pContent[3]);
	DS1302Write(pContent[4]);
	DS1302Write(pContent[5]);
	DS1302Write(pContent[6]);
	DS1302Write(pContent[7]);
	DS1302_RST=0;
	DS1302WriteByte(0x8e,0x80);
}
*/


