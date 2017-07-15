#include<reg52.h>
#include<intrins.h>
//#include<string.h>//
#include"MacroAndConst.h"
#include"delay.h"

sbit DS18B20_DQ=P3^3;

unsigned char CurrentTemperature[9]={' ','3','0','0','0',' ','.',12,' '}; 

bit SetTemperatureFlag=0;

static void DS18B20_Initial()
{
 	DS18B20_DQ=1;
	_nop_();
	DS18B20_DQ=0;

	DelayUs(80);//>480,<960us //491us
	DS18B20_DQ=1;
	DelayUs(14);//59us
}

static void DS18B20_WriteOneByte(uchar content)
{
	uchar i=0;
	EA=0;//
	for(i=0;i<8;i++)	
	{
		if((content&0x01)==0)
		{
			DS18B20_DQ=1;//写完之后，一定要释放总线
			_nop_();_nop_();
			DS18B20_DQ=0;
			DelayUs(8);//59us//就错在这一步
			DS18B20_DQ=1;
		}
		else
		{   
			DS18B20_DQ=1;
			_nop_();_nop_();
			DS18B20_DQ=0;
			_nop_();_nop_();
			DS18B20_DQ=1;
			DelayUs(8);//107us	
		}
		content>>=1;
	}
	EA=1;
}

static unsigned int DS18B20_ReadOneByte(void)
{
	uchar j=0;
	uint result=0;
	EA=0;
	for(j=0;j<8;j++)
	{
		DS18B20_DQ=1;
		_nop_();_nop_();
		DS18B20_DQ=0;
		result>>=1;// also can delay	
		DS18B20_DQ=1;
		_nop_();_nop_();_nop_();//>10us,<13us  //12us
		_nop_();_nop_();
	
		if(DS18B20_DQ==1)
		{
			result=result|0x80;
		}
		else
		{
			result=result&0x7f;
		}
		DelayUs(8);
	}
	EA=1;
	return result;
}
 
static unsigned int DS18B20_get_tempareture()
{
	uint TH1=0;
	uint TH2=0;
	uint temp=0;
	
 	DS18B20_Initial();
	DelayUs(5);
	DS18B20_WriteOneByte(0xcc);
	DelayUs(5);
	DS18B20_WriteOneByte(0x44);//convert 

	DS18B20_Initial();
	DelayUs(5);
	DS18B20_WriteOneByte(0xcc);
	DelayUs(5);
	DS18B20_WriteOneByte(0xbe); 
    DelayUs(5);
	TH1=DS18B20_ReadOneByte();
	TH2=DS18B20_ReadOneByte();
	TH2<<=8;
	temp=TH2|TH1;
	
	return (temp);
}


void TemperatureUpdate()
{
	unsigned int TempDat;
	unsigned int TempBehindDot;//小数点后的数据

	TempDat=DS18B20_get_tempareture();
	TempBehindDot=(TempDat&0x000f);
                                   //一般情况下，温度为正，这几句不用	
	if(TempDat&0x8000)				 //提高效率
	{
		CurrentTemperature[0]='-';//Sign bit:'+'or'-'	
	}
	else
	{
		CurrentTemperature[0]=' ';//' '=不显示	
	}	
	
	TempDat>>=4;

	CurrentTemperature[1]=TempDat/10;
	CurrentTemperature[2]=TempDat%10;
	CurrentTemperature[2]+=20;//为了显示小数点。
	 
	TempBehindDot*=100;
	TempBehindDot>>=4;

	CurrentTemperature[3]=TempBehindDot/10;
	CurrentTemperature[4]=TempBehindDot%10;

//	CurrentTemperature[5]=' ';
//	CurrentTemperature[6]='.';
//	CurrentTemperature[7]=12;

//	strcpy (LedDisBuffer,CurrentTemperature);
}




