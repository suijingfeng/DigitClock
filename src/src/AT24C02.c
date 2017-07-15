//隋景峰 AT24C02 驱动
#include<reg52.h> 
#include<intrins.h>
#include"MacroAndConst.h"
#include"AT24c02.h"
#define NOP()	_nop_()	
///////////// 
sbit SDA=P2^0;
sbit SCL=P2^1;
/////////////

static uchar datatable[8]={0};

static void At24c02Start()
{
	AT24C02_SDA=1;
	NOP();
    AT24C02_SCL=1;
	NOP();NOP();NOP();NOP();NOP();
	AT24C02_SDA=0;
	NOP();NOP();NOP();NOP();NOP();
	AT24C02_SCL=0;
	NOP();NOP();
}

static void At24c02Stop()
{
	AT24C02_SDA=0;
	NOP();NOP();
	AT24C02_SCL=1;
	NOP();NOP();
	AT24C02_SDA=1;
	NOP();NOP();
}

static void At24c02Acknowledge()
{
	uchar AckVar=0;
	AT24C02_SCL=1;
	NOP();NOP();

	while((AT24C02_SDA==1)&&((++AckVar)<250));
	
	AT24C02_SCL=0;
	NOP();NOP();
}

static void At24c02WriteStatus(uchar dat)
{
	uchar i=0;
	for(i=0;i<8;i++)
	{
		AT24C02_SCL=0;
		NOP();NOP();NOP();NOP();
		AT24C02_SDA=(bit)(dat&0x80);
		NOP();NOP();NOP();NOP();
		AT24C02_SCL=1;
		NOP();NOP();NOP();NOP();
		dat<<=1;
	}

	AT24C02_SCL=0;
	NOP();NOP();
	AT24C02_SDA=1;//写完之后释放sda,等待应答；
	NOP();NOP();
}
/////////////////////////////////

void delay_5us()
{
 	_nop_();_nop_();
}

void start()
{
	SDA=1;
    SCL=1;
	delay_5us();
	SDA=0;
	delay_5us();
}


void stop()
{
	SDA=0;
	SCL=1;
	delay_5us();
	SDA=1;
	delay_5us();
}


void acknowledge()
{
	uchar flag=0;
	SCL=1;
	delay_5us();
	
	while((1==SDA)&&(++flag)<255);//acknowledge
	 	
	SCL=0;
	delay_5us();
}


void write(uchar dat)
{
	uchar i=0,temp;
	temp=dat;
	EA=0;
	for(i=0;i<8;i++)
	{
		SCL=0;
		temp<<=1;
		delay_5us();
		SDA=CY;
		delay_5us();
		SCL=1;
		delay_5us();
	}
	SCL=0;
	delay_5us();
	SDA=1;//写完之后释放sda
	delay_5us();
	EA=1;
}
uchar read()
{
	uchar i,dat=0;
	EA=0;
	SCL=0;
	delay_5us();
	SDA=1;
	delay_5us();

	for(i=0;i<8;i++)
	{
		SCL=1;
		delay_5us();
		dat=(dat<<1)|SDA;
		SCL=0;
		delay_5us();
	}
	EA=1;
	return dat;
}

uchar read_word(uchar address)
{
	 uchar dat;
	 EA=0;
	 start();
	 write(0xa0);
	 acknowledge();
	 write(address);
	 acknowledge();
	 start();
	 write(0xa1);
	 acknowledge();
	 dat=read();
	 stop();
	 EA=1;
	 return dat;
} 

void write_word(uchar address,uchar dat)
{
	EA=0;
	start();
	write(0xa0);
	acknowledge();
	write(address);
	acknowledge();
	write(dat);
	acknowledge();
	stop();
	delay_5us();
	EA=1;
}

//////////////////////////
static unsigned char At24c02ReadStatus(void)
{
	unsigned char dat=0;
	unsigned char j=0;

	AT24C02_SCL=0;
	NOP();NOP();NOP();NOP();
	AT24C02_SDA=1;
	NOP();NOP();NOP();NOP();

	for(j=0;j<8;j++)
	{
		dat<<=1;
		if(1==AT24C02_SDA)
		{
			dat|=0x01;
		}

		AT24C02_SCL=1;
		NOP();NOP();NOP();NOP();
		AT24C02_SCL=0;
		NOP();NOP();NOP();NOP();
	}
	return dat;
}

void At24c02ByteWrite(uchar address,uchar content)
{
	At24c02Start();

	At24c02WriteStatus(0xa0);//Device Address hardware:A0=A1=A2=WP=GND;
	At24c02Acknowledge();
	
	At24c02WriteStatus(address);
	At24c02Acknowledge();
	
	At24c02WriteStatus(content);
	At24c02Acknowledge();

	At24c02Stop();
	delay_5us();
}
/*
void At24c02PageWrite(uchar address,uchar *pData)
{
	unsigned char number=0;
	At24c02Start();
	At24c02WriteStatus(0xa0);//Device Address hardware:A0=A1=A2=WP=GND;
	At24c02Acknowledge();

	At24c02WriteStatus(address);
	At24c02Acknowledge();
	for(;*pData!='\0';pData++)
	{
		At24c02WriteStatus(*pData);
		At24c02Acknowledge();
	}
	At24c02Stop();
}

unsigned char At24c02CurrentRead(void)
{
	uchar value;
	At24c02Start();

	At24c02WriteStatus(0xa1);//Device Address
	At24c02Acknowledge();

	value=At24c02ReadStatus();
	At24c02Stop();
	return value;
}
*/
unsigned char At24c02RandomRead(unsigned char address)
{
	uchar value;
	At24c02Start();

	At24c02WriteStatus(0xa0);//Device Address
	At24c02Acknowledge();
	At24c02WriteStatus(address);
	At24c02Acknowledge();
	
 	At24c02Start();
	At24c02WriteStatus(0xa1);//Device Address
	At24c02Acknowledge();
	value=At24c02ReadStatus();
	At24c02Stop();

	return value;
} 
/*
void At24c02SequentialRead(unsigned char *pD)
{
	uchar k=0;

	At24c02Start();
	At24c02WriteStatus(0xa1);//Device Address
	At24c02Acknowledge();

	for(k=0;k<8;k++)
	{
		At24c02Acknowledge();
		datatable[k]=At24c02ReadStatus();
	}
	At24c02Stop();
	pD=datatable;	
}
*/
