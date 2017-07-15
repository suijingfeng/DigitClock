#include<reg52.h>
#include"MacroAndConst.h"
#include"delay.h"
#include"ds1302.h"
#include"at24c02.h"
#define LCD12864_DATA_PORT	 P0
#define IMP_KWH1  	6400
#define IMP_KWH2  	800

sbit dula=P2^6;
sbit wela=P2^7;
sbit LCD12864_EN=P3^4;
sbit LCD12864_RS=P3^2;
sbit LCD12864_WR=P3^3;
//sbit LCD12864_RD=P3^7;
unsigned char code SystemInit[]="初始化";
unsigned char code InventBy[]="Invented by:";
unsigned char code Name1[]="隋景峰";
unsigned char code Name2[]="张敏哲";
unsigned char code Name3[]="余书鹏";
unsigned char code Smart[]="智能电表";
unsigned char code Welcome[]="欢迎老师指导";
unsigned char code PowerConsume[]="耗电量:";
unsigned char code Gonglv[]="功率";
unsigned char code Uint[]="Wh";
unsigned char code Year[]="年";
unsigned char code Month[]="月";
unsigned char code Date[]="日";

unsigned char PowerShowTable[7];
unsigned char PowerDt[6]={'0','0','3','0','.','0'};

void LCD12864_WriteData(uchar content);
void LCD12864_WriteCommand(uchar content);

void LCD12864_WriteCommand(uchar content)
{
	ES=0;
	ET0=0;
	LCD12864_EN=0;
	delayus(10);
	LCD12864_WR=0;
	delayus(10);
	LCD12864_RS=0;
	delayus(10);
	LCD12864_DATA_PORT=content;
	delayus(10);
	LCD12864_EN=1;
	delayus(10);
	LCD12864_EN=0;
	delayus(10);
	ES=1;
	ET0=1;	
}

void LCD12864_WriteData(uchar content)
{
	ES=0;
	ET0=0;
	LCD12864_EN=0;
	delayus(10);
	LCD12864_WR=0;
	delayus(10);
	LCD12864_RS=1;
	delayus(10);
	LCD12864_DATA_PORT=content;
	delayus(10);
	LCD12864_EN=1;
	delayus(10);
	LCD12864_EN=0;
	ES=1;
	ET0=1;
}

void LCD12864_Initial()
{
	unsigned char i=0;
	dula=1;
	P0=0x00;
	dula=0;
	
	wela=1;
	P0=0x00;
	wela=0;

	LCD12864_WriteCommand(0x0c);
	delay(100);
	LCD12864_WriteCommand(0x30);
	delay(10);
	LCD12864_WriteCommand(0x06);
	delay(10);
	LCD12864_WriteCommand(0x01);
	delay(10);
	
	LCD12864_WriteCommand(0x92);
	for(i=0;SystemInit[i]!='\0';i++)
	{
		LCD12864_WriteData(SystemInit[i]);
	}
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);

	LCD12864_WriteCommand(0x81);
	for(i=0;InventBy[i]!='\0';i++)
	{
		LCD12864_WriteData(InventBy[i]);
	}

	LCD12864_WriteCommand(0x92);
	for(i=0;Name1[i]!='\0';i++)
	{
		LCD12864_WriteData(Name1[i]);
	}

	LCD12864_WriteCommand(0x88+2);
	for(i=0;Name2[i]!='\0';i++)
	{
		LCD12864_WriteData(Name2[i]);
	}

	LCD12864_WriteCommand(0x98+2);
	for(i=0;Name3[i]!='\0';i++)
	{
		LCD12864_WriteData(Name3[i]);
	}
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	
	LCD12864_WriteCommand(0x01);
	delay(10);
	
	LCD12864_WriteCommand(0x80+2);
	for(i=0;Smart[i]!='\0';i++)
	{
		LCD12864_WriteData(Smart[i]);
	}

	LCD12864_WriteCommand(0x98+2);
	for(i=0;Welcome[i]!='\0';i++)
	{
		LCD12864_WriteData(Welcome[i]);
	}
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);
	delay(250);delay(250);delay(250);delay(250);delay(250);

	LCD12864_WriteCommand(0x01);
	delay(10);
}
// 显示所有要显示的信息；
void LCD12864_Show()
{
    unsigned int PowerCounter=0;
	unsigned char i=0;
	unsigned char temp;
	//show time
	LCD12864_WriteCommand(0x80);
	LCD12864_WriteData('2');
	LCD12864_WriteData('0');
	LCD12864_WriteData((CurrentTime.Year>>4)+'0');
	LCD12864_WriteData((CurrentTime.Year&0x0f)+'0');
	LCD12864_WriteData(Year[0]);
	LCD12864_WriteData(Year[1]);
	LCD12864_WriteData((CurrentTime.Month>>4)+'0');
	LCD12864_WriteData((CurrentTime.Month&0x0f)+'0');
	LCD12864_WriteData(Month[0]);
	LCD12864_WriteData(Month[1]);
	LCD12864_WriteData((CurrentTime.Date>>4)+'0');
	LCD12864_WriteData((CurrentTime.Date&0x0f)+'0');
    LCD12864_WriteData(Date[0]);
	LCD12864_WriteData(Date[1]);

	LCD12864_WriteCommand(0x90);
	LCD12864_WriteData((CurrentTime.Hour>>4)+'0');
	LCD12864_WriteData((CurrentTime.Hour&0x0f)+'0');
	LCD12864_WriteData(':');
	LCD12864_WriteData((CurrentTime.Minute>>4)+'0');
	LCD12864_WriteData((CurrentTime.Minute&0x0f)+'0');
	LCD12864_WriteData(':');
	LCD12864_WriteData((CurrentTime.Second>>4)+'0');
	LCD12864_WriteData((CurrentTime.Second&0x0f)+'0');
	
	//show the power of consume

	LCD12864_WriteCommand(0x88);
	for(i=0;PowerConsume[i]!='\0';i++) 	
	{
		LCD12864_WriteData(PowerConsume[i]);
	}
	//i=At24c02CurrentRead();
	//At24c02ByteWrite(20,99);
	//i=read_word(2);
	
	temp=read_word(4);
	PowerCounter=read_word(5);//6400 imp/kw.h 800imp/kwh
	PowerCounter<<=8;
	PowerCounter+=temp;

	PowerShowTable[0]=((PowerCounter*10>>3)/1000+'0');
	PowerShowTable[1]=((PowerCounter*10>>3)%1000/100+'0');
	PowerShowTable[2]=((PowerCounter*10>>3)%100/10+'0');
	PowerShowTable[3]=((PowerCounter*10>>3)%10+'0');
	PowerShowTable[4]='.';
	PowerShowTable[5]=((PowerCounter*100>>3)%10+'0');
	PowerShowTable[6]=((PowerCounter*1000>>3)%10+'0');
	
	for(i=0;i<7;i++)
	{
		LCD12864_WriteData(PowerShowTable[i]);		
	}
	LCD12864_WriteData(Uint[0]);
	LCD12864_WriteData(Uint[1]);

//功率部分：
	LCD12864_WriteCommand(0x98);
//显示功率二字
	for(i=0;Gonglv[i]!='\0';i++)
	{
		LCD12864_WriteData(Gonglv[i]);
	}
	LCD12864_WriteData(':');
//显示数据
	for(i=0;i<6;i++)
	{
		LCD12864_WriteData(PowerDt[i]);
	}
		LCD12864_WriteData('W');
}
	

	

