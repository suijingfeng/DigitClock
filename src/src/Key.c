#include<reg52.h>
#include"MacroAndConst.h"
#include"timer.h"
#include"seg.h"
#include"ds1302.h"
#include"buzzer.h"
#include"exinterrupt.h"
#include"ds18b20.h"


#define KEY_LONG_PERIOD 100
#define KEY_CONTINUE_PERIOD 25

#define KEY_DOWN 	 0x80
#define KEY_LONG 	 0x40
#define KEY_CONTINUE 0x20
#define KEY_UP		 0x10

#define KEY_VALUE1	 0x0e
#define KEY_VALUE2	 0x0d
#define KEY_VALUE3  0x0b
#define KEY_VALUE4  0x07
#define KEY_NULL	 0x0f

#define KEY_STATE_INIT 		0
#define KEY_STATE_WOBBLE	1
#define KEY_STATE_PRESS		2
#define KEY_STATE_LONG		3
#define KEY_STATE_CONTINUE	4
#define KEY_STATE_RELEASE	5

sbit KEY1=P1^0;//调整
sbit KEY2=P1^1;//+
sbit KEY3=P1^2;//-
sbit KEY4=P1^3;//确定

void KeyInit()
{
	KEY1=1;			
	KEY2=1;
	KEY3=1;
	KEY4=1;
}

static uchar KeyScan()
{
	if(KEY1==0) return KEY_VALUE1;
	if(KEY2==0) return KEY_VALUE2;
	if(KEY3==0) return KEY_VALUE3;
	if(KEY4==0) return KEY_VALUE4;
	return KEY_NULL;
}

void GetKeyValue(unsigned char* pKeyValue)
{
	static uchar KeyState=KEY_STATE_INIT;
	static uchar KeyTimeCount=0;
	static uchar LastKey=KEY_NULL;
	
	uchar KeyTemp=KEY_NULL;

	KeyTemp=KeyScan();

	switch(KeyState)
	{
		case KEY_STATE_INIT:
		{
			if(KEY_NULL!=KeyTemp)
			{
				KeyState=KEY_STATE_WOBBLE;
			}
		}break;

		case KEY_STATE_WOBBLE:
		{
			if(KEY_NULL!=KeyTemp)
			{
				KeyState=KEY_STATE_PRESS;
			}
			else
			{
				KeyState=KEY_STATE_INIT;
			}
		}break;

		case KEY_STATE_PRESS:
		{
			if(KEY_NULL!=KeyTemp)
			{
				LastKey=KeyTemp;
				KeyTemp|=KEY_DOWN;
				KeyState=KEY_STATE_LONG;
			}
			else
			{
				KeyState=KEY_STATE_INIT;
			}
		}break;

		case KEY_STATE_LONG:
		{
			if(KEY_NULL!=KeyTemp)
			{
				if(++KeyTimeCount>KEY_LONG_PERIOD)
				{
					KeyTimeCount=0;
					KeyTemp|=KEY_LONG;
					KeyState=KEY_STATE_CONTINUE;
				}
				
			}
			else
			{
				KeyState=KEY_STATE_RELEASE;
			}
		}break;

	
		case KEY_STATE_CONTINUE:
		{
			if(KEY_NULL!=KeyTemp)
			{
				if(++KeyTimeCount>KEY_CONTINUE_PERIOD)
				{
					KeyTimeCount=0;
					KeyTemp|=KEY_CONTINUE;
				}
			}
			else
			{
				KeyState=KEY_STATE_RELEASE;
			}
		}break;

		case KEY_STATE_RELEASE:
		{
			LastKey|=KEY_UP;
			KeyTemp=LastKey;
			KeyState=KEY_STATE_INIT;
		}break;
	
		default:break;
	}
	*pKeyValue=KeyTemp;
}

void KeyMainLoop(void)
{
	unsigned char KeyValue;
	unsigned char BufPosition=0;

	if(KeyScanFlag)
	{	
		KeyScanFlag=0;
		GetKeyValue(&KeyValue);//
		
		if((KeyValue&0xf0)==KEY_DOWN)
		{
			BuzzerFlag=1;	
		}

		if((KeyValue&0xf0)==KEY_UP)
		{
			BuzzerFlag=0;
		}

		if((KeyValue==(KEY_UP|KEY_VALUE2))&&(SetTimeFlag==0))
		{
			if(++IrKeyVal.SegPicture==5)//4代表着一共4幅画面
			{
				IrKeyVal.SegPicture=0;
			}		
		}

		if(KeyValue==(KEY_UP|KEY_VALUE3)&&(SetTimeFlag==0))
		{
			if(IrKeyVal.SegPicture==0)
			{
				IrKeyVal.SegPicture=4;
			}
			else
			{
				IrKeyVal.SegPicture--;
			}		
		}

		switch(IrKeyVal.SegPicture)//IrLowCmd=IrKeyValue
		{
			case 0://设置时、分、秒
			{
				if(KeyValue==(KEY_UP|KEY_VALUE1))//设置	
				{
					SetTimeFlag=1;//Start Set Time Mode And 
								  //Stop Read DS1302;
					SegJumpShowFlag=0;//停止闪烁，调整

					if(TimeBuf1[g_uBufPosition]==' ')//' '=空格；
					{
						TimeBuf1[g_uBufPosition]=TimeBuf1[8];
						TimeBuf1[8]=' ';
					}
					g_uBufPosition++;
					
					SegJumpShowFlag=1;//Seg start jumpshow

					if((g_uBufPosition==2)||(g_uBufPosition==5))
					{
						g_uBufPosition++;
					}//DO NOT CHANGE BITS OF '-'
			
					if(g_uBufPosition==8)
					{
						g_uBufPosition=0;
					}
				 }

				//
				if((KeyValue==(KEY_UP|KEY_VALUE2))&&(SetTimeFlag==1))	
				{
					SegJumpShowFlag=0;
					if(TimeBuf1[g_uBufPosition]==' ')
					{
						TimeBuf1[g_uBufPosition]=TimeBuf1[8];
						TimeBuf1[8]=' ';
					}//adjust
			
					if(g_uBufPosition==0)
					{
						if(TimeBuf1[0]==2)
						{
							TimeBuf1[0]=0;		
						}
						else
						{
							TimeBuf1[0]++; 	
						}	
					}

					else if(g_uBufPosition==1)
					{
						if(TimeBuf1[0]==2)
						{
							if(TimeBuf1[1]==3)
							{
								TimeBuf1[1]=0;	
							}

							else
							{
								TimeBuf1[1]++;	
							}
						}
						
						else if(TimeBuf1[1]==9)
						{
							TimeBuf1[1]=0;		
						}
						
						else
						{
							TimeBuf1[1]++; 	
						}	
					}

					else if(g_uBufPosition==3)
					{
						if(TimeBuf1[3]==5)
						{
							TimeBuf1[3]=0;	
						}
						else
						{
							TimeBuf1[3]++;
						}	
					}

					else if(g_uBufPosition==4)
					{
					 	if(TimeBuf1[4]==9)
						{
							TimeBuf1[4]=0;	
						}
						else
						{
							TimeBuf1[4]++;
						}
					}

					else if(g_uBufPosition==6)
					{
						if(TimeBuf1[6]==5)
						{
							TimeBuf1[6]=0;	
						}
						else
						{
							TimeBuf1[6]++;
						}	
					}

					else //if(g_uBufPosition==7)
					{
					 	if(TimeBuf1[7]==9)
						{
							TimeBuf1[7]=0;	
						}
						else
						{
							TimeBuf1[7]++;
						}
					}
					SegJumpShowFlag=1;
				}


				//
				if(KeyValue==(KEY_UP|KEY_VALUE3)&&(SetTimeFlag==1))
				{
					SegJumpShowFlag=0;
					if(TimeBuf1[g_uBufPosition]==' ')
					{
						TimeBuf1[g_uBufPosition]=TimeBuf1[8];
						TimeBuf1[8]=' ';
					}

					if(g_uBufPosition==0)
					{
						if(TimeBuf1[0]==0)
						{
							TimeBuf1[0]=2;
						}
						else
						{
							TimeBuf1[0]--;
						}
					}
					else if(g_uBufPosition==1)
					{
						if(TimeBuf1[0]==2)
						{
							if(TimeBuf1[1]==0)
							{
								TimeBuf1[1]=3;
							}
							else
							{
								TimeBuf1[1]--;
							}
						}
						else 
						{
							if(TimeBuf1[1]==0)
							{
								TimeBuf1[1]=9;
							}
							else
							{
								TimeBuf1[1]--;
							}
						}
					}
					else if(g_uBufPosition==3)
					{
						if(TimeBuf1[3]==0)
						{
							TimeBuf1[3]=5;
						}
						else
						{
							TimeBuf1[3]--;
						}
					}
					else if(g_uBufPosition==4)
					{
						if(TimeBuf1[4]==0)
						{
							TimeBuf1[4]=9;
						}
						else
						{
							TimeBuf1[4]--;
						}
					}
					else if(g_uBufPosition==6)
					{
						if(TimeBuf1[6]==0)
						{
							TimeBuf1[6]=5;
						}
						else
						{
							TimeBuf1[6]--;
						}
					}
					else//if(g_uBufPosition==7)
					{
						if(TimeBuf1[7]==0)
						{
							TimeBuf1[7]=9;
						}
						else
						{
							TimeBuf1[7]--;
						}	
					}
					SegJumpShowFlag=1;
				}


				//确定
				if((KeyValue==(KEY_UP|KEY_VALUE4))&&(SetTimeFlag==1))
				{
					SegJumpShowFlag=0;
					if(TimeBuf1[g_uBufPosition]==' ')
					{
						TimeBuf1[g_uBufPosition]=TimeBuf1[8];
						TimeBuf1[8]=' ';
					}

					if((TimeBuf1[0]*10+TimeBuf1[1])<=23)
					{
						DS1302_SetTime_HMS((TimeBuf1[0]<<4)|TimeBuf1[1],
											(TimeBuf1[3]<<4)|TimeBuf1[4],
											(TimeBuf1[6]<<4)|TimeBuf1[7]);
					}	//向ds1302写入所设的小时，分钟，秒
					
					SetTimeFlag=0;
					//SegJumpShowFlag=1;结束了，数码管不用再蹦
				}
			}break;

			case 1://年、月、日	
			{							   
				if(KeyValue==(KEY_UP|KEY_VALUE1))//设置	
				{
					SetTimeFlag=1;//Start Set Time Mode And 
								  //Stop Read DS1302;
					SegJumpShowFlag=0;//停止闪烁，调整

					if(TimeBuf2[g_uBufPosition]==' ')//' '=空格；
					{
						TimeBuf2[g_uBufPosition]=TimeBuf2[8];
						TimeBuf2[8]=' ';
					}
					g_uBufPosition++;
					
					SegJumpShowFlag=1;//Seg start jumpshow

					if((g_uBufPosition==2)||(g_uBufPosition==5))
					{
						g_uBufPosition++;
					}//DO NOT CHANGE BITS OF '-'
			
					if(g_uBufPosition==8)
					{
						g_uBufPosition=0;
					}
				 }

				//+
				if((KeyValue==(KEY_UP|KEY_VALUE2))&&(SetTimeFlag==1))	
				{
					SegJumpShowFlag=0;
					if(TimeBuf2[g_uBufPosition]==' ')
					{
						TimeBuf2[g_uBufPosition]=TimeBuf2[8];
						TimeBuf2[8]=' ';
					}//adjust
			
					if(g_uBufPosition==0)
					{
						if(TimeBuf2[0]==9)
						{
							TimeBuf2[0]=0;	
						}
						else
						{
							TimeBuf2[0]++;	
						}							
					}

					else if(g_uBufPosition==1)
					{
						if(TimeBuf2[1]==9)
						{
							TimeBuf2[1]=0;	
						}
						else
						{
							TimeBuf2[1]++;	
						}							
					}

					else if(g_uBufPosition==3)
					{
						if(TimeBuf2[3]==1)
						{
							TimeBuf2[3]=0;	
						}
						else
						{
							TimeBuf2[3]++;	
						}						
					}

					else if(g_uBufPosition==4)
					{
						if(TimeBuf2[3]==1)
						{
							if(TimeBuf2[4]>=2)
							{
								TimeBuf2[4]=0;
							}
							else
							{
								TimeBuf2[4]++;
							}
						}
						else
						{
							if(TimeBuf2[4]>=9)
							{
								TimeBuf2[4]=1;
							}
							else
							{
								TimeBuf2[4]++;
							}
						}
					}

					else if(g_uBufPosition==6)
					{
						if((TimeBuf2[3]==0)&&(TimeBuf2[4]==2))//2月
						{
							if(TimeBuf2[6]>=2)
							{
								TimeBuf2[6]=0;
							}
							else
							{
								TimeBuf2[6]++;
							}		
						}
						else//非二月
						{
							if(TimeBuf2[6]>=3)
							{
								TimeBuf2[6]=0;
							}
							else
							{
								TimeBuf2[6]++;
							}		
						}	
					}
					
					else //if(g_uBufPosition==7)
					{
						if((TimeBuf2[3]==0)&&(TimeBuf2[4]==2))//2月	
						{
							if((((TimeBuf2[0]*10)+TimeBuf2[1])%4)==0)//闰年
							{
								
								if(TimeBuf2[7]>=9)//29					
								{
									if(TimeBuf2[6]==0)
									{
										TimeBuf2[7]=1;
									}
									else
									{
										TimeBuf2[7]=0;	
									}
								}
								else
								{
									TimeBuf2[7]++;
								}
							}
							else //非闰年
							{
								if(TimeBuf2[6]==2)
								{
									if(TimeBuf2[7]>=8)//28
									{
										TimeBuf2[7]=0;
									}
									else
									{
										TimeBuf2[7]++;
									}
								}
								else if(TimeBuf2[6]==1)
								{
									if(TimeBuf2[7]>=9)					
									{
										TimeBuf2[7]=0;
									}
									else
									{
										TimeBuf2[7]++;
									}	
								}
								else //if(TimeBuf2[6]==0)
								{
									if(TimeBuf2[7]>=9)					
									{
										TimeBuf2[7]=1;
									}
									else
									{
										TimeBuf2[7]++;
									}	
								}
							}		
						}	 
						else if((TimeBuf2[3]==0)&&(
								(TimeBuf2[4]==1)||
								(TimeBuf2[4]==3)||
								(TimeBuf2[4]==5)||
								(TimeBuf2[4]==7)||
								(TimeBuf2[4]==8))||
								((TimeBuf2[3]==1)&&
								((TimeBuf2[4]==0)||
								(TimeBuf2[4]==2))))
						{//1,3,5,7,8,10,12月有31天
							if(TimeBuf2[6]==3)
							{
								if(TimeBuf2[7]>=1)
								{
									TimeBuf2[7]=0;
								}
								else
								{
									TimeBuf2[7]++;
								}
							}

							else if(TimeBuf2[6]==0)
							{
								if(TimeBuf2[7]>=9)
								{
									TimeBuf2[7]=1;
								}
								else
								{
									TimeBuf2[7]++;
								}	
							}

							else
							{
								if(TimeBuf2[7]>=9)
								{
									TimeBuf2[7]=0;
								}
								else
								{
									TimeBuf2[7]++;
								}	
							}
						}
						else//4,6,9,11月
						{
							if(TimeBuf2[6]==3)
							{
								TimeBuf2[7]=0;//30天
							}

							else if(TimeBuf2[6]==0)
							{
								if(TimeBuf2[7]>=9)
								{
									TimeBuf2[7]=1;	
								}
								else
								{
									TimeBuf2[7]++;		
								}			
							}
							else
							{
								if(TimeBuf2[7]>=9)
								{
									TimeBuf2[7]=0;	
								}
								else
								{
									TimeBuf2[7]++;		
								}		
							}		
						}//这个else可以去掉，加到这里为了完整。	
					}
				   SegJumpShowFlag=1;
				}	
				//-
				if(KeyValue==(KEY_UP|KEY_VALUE3)&&(SetTimeFlag==1))
				{
					SegJumpShowFlag=0;
					if(TimeBuf2[g_uBufPosition]==' ')
					{
						TimeBuf2[g_uBufPosition]=TimeBuf2[8];
						TimeBuf2[8]=' ';
					}

					if(g_uBufPosition==0)
					{
						if(TimeBuf2[0]==0)
						{
							TimeBuf2[0]=9;	
						}
						else
						{
							TimeBuf2[0]--;	
						}							
					}

					else if(g_uBufPosition==1)
					{
						if(TimeBuf2[1]==0)
						{
							TimeBuf2[1]=9;	
						}
						else
						{
							TimeBuf2[1]--;	
						}							
					}

					else if(g_uBufPosition==3)
					{
						if(TimeBuf2[3]==0)
						{
							TimeBuf2[3]=1;	
						}
						else
						{
							TimeBuf2[3]--;	
						}						
					}

					else if(g_uBufPosition==4)
					{
						if(TimeBuf2[3]==1)
						{
							if(TimeBuf2[4]==0)
							{
								TimeBuf2[4]=2;
							}
							else
							{
								TimeBuf2[4]--;
							}
						}
						else
						{
							if(TimeBuf2[4]==1)
							{
								TimeBuf2[4]=9;
							}
							else
							{
								TimeBuf2[4]--;
							}
						}
					}

					else if(g_uBufPosition==6)
					{
						if((TimeBuf2[3]==0)&&(TimeBuf2[4]==2))//2月
						{
							if(TimeBuf2[6]==0)
							{
								TimeBuf2[6]=2;
							}
							else
							{
								TimeBuf2[6]--;
							}		
						}
						else//非二月
						{
							if(TimeBuf2[6]==0)
							{
								TimeBuf2[6]=3;
							}
							else
							{
								TimeBuf2[6]--;
							}		
						}	
					}
					
					else //if(g_uBufPosition==7)
					{
						if((TimeBuf2[3]==0)&&(TimeBuf2[4]==2))//2月	
						{
							if((((TimeBuf2[0]*10)+TimeBuf2[1])%4)==0)//闰年
							{
								if(TimeBuf2[6]==0)
								{
									if(TimeBuf2[7]==1)//29					
									{
										TimeBuf2[7]=9;
									}
									else
									{
										TimeBuf2[7]--;
									}
								}
								else
								{
									if(TimeBuf2[7]==0)//29					
									{
										TimeBuf2[7]=9;
									}
									else
									{
										TimeBuf2[7]--;
									}
								}
							}
							else //非闰年
							{
								if(TimeBuf2[6]==2)
								{
									if(TimeBuf2[7]==0)//28
									{
										TimeBuf2[7]=8;
									}
									else
									{
										TimeBuf2[7]--;
									}
								}
								
								else if(TimeBuf2[6]==0)
								{
								 	if(TimeBuf2[7]==1)					
									{
										TimeBuf2[7]=9;
									}
									else
									{
										TimeBuf2[7]--;
									}
								}
								else 
								{
									if(TimeBuf2[7]==0)					
									{
										TimeBuf2[7]=9;
									}
									else
									{
										TimeBuf2[7]--;
									}
								}
							}		
						}	 
						else if((TimeBuf2[3]==0)&&(
								(TimeBuf2[4]==1)||
								(TimeBuf2[4]==3)||
								(TimeBuf2[4]==5)||
								(TimeBuf2[4]==7)||
								(TimeBuf2[4]==8))||
								((TimeBuf2[3]==1)&&(TimeBuf2[4]==0))||
								((TimeBuf2[3]==1)&&(TimeBuf2[4]==2)))
						{//1,3,5,7,8,10,12月有31天
							if(TimeBuf2[6]==3)
							{
								if(TimeBuf2[7]==0)
								{
									TimeBuf2[7]=1;
								}
								else
								{
									TimeBuf2[7]--;
								}
							}
							else if(TimeBuf2[6]==0)
							{
								if(TimeBuf2[7]==1)
								{
									TimeBuf2[7]=9;
								}
								else
								{
									TimeBuf2[7]--;
								}	
							}
							else
							{
								if(TimeBuf2[7]==0)
								{
									TimeBuf2[7]=9;
								}
								else
								{
									TimeBuf2[7]--;
								}	
							}
							
						}
						else//4,6,9,11月
						{
							if(TimeBuf2[6]==3)
							{
								TimeBuf2[7]=0;//30天
							}
							else if(TimeBuf2[6]==0)
							{
								if(TimeBuf2[7]==1)
								{
									TimeBuf2[7]=9;	
								}
								else
								{
									TimeBuf2[7]--;		
								}		
							}
							else
							{
								if(TimeBuf2[7]==0)
								{
									TimeBuf2[7]=9;	
								}
								else
								{
									TimeBuf2[7]--;		
								}		
							}	
						}//这个else可以去掉，加到这里为了完整。
					}
					SegJumpShowFlag=1;
				}
				//确定
				if((KeyValue==(KEY_UP|KEY_VALUE4))&&(SetTimeFlag==1))
				{
					SegJumpShowFlag=0;
					if(TimeBuf2[g_uBufPosition]==' ')
					{
						TimeBuf2[g_uBufPosition]=TimeBuf2[8];
						TimeBuf2[8]=' ';
					}
					

					if(((TimeBuf2[3]*10+TimeBuf2[4])==1)||
					   ((TimeBuf2[3]*10+TimeBuf2[4])==3)||
					   ((TimeBuf2[3]*10+TimeBuf2[4])==5)||
					   ((TimeBuf2[3]*10+TimeBuf2[4])==7)||
					   ((TimeBuf2[3]*10+TimeBuf2[4])==8)||
					   ((TimeBuf2[3]*10+TimeBuf2[4])==10)||
					   ((TimeBuf2[3]*10+TimeBuf2[4])==12))
					{
						if((TimeBuf2[6]*10+TimeBuf2[7])<=31)
						{
							DS1302_SetTime_YMD(
							(TimeBuf2[0]<<4)|TimeBuf2[1],
							(TimeBuf2[3]<<4)|TimeBuf2[4],
							(TimeBuf2[6]<<4)|TimeBuf2[7]);	
						}		
					}

					if(((TimeBuf2[3]*10+TimeBuf2[4])==4)||
					   ((TimeBuf2[3]*10+TimeBuf2[4])==6)||
					   ((TimeBuf2[3]*10+TimeBuf2[4])==9)||
					   ((TimeBuf2[3]*10+TimeBuf2[4])==11))
					{
						if((TimeBuf2[6]*10+TimeBuf2[7])<=30)
						{
							DS1302_SetTime_YMD(
							(TimeBuf2[0]<<4)|TimeBuf2[1],
							(TimeBuf2[3]<<4)|TimeBuf2[4],
							(TimeBuf2[6]<<4)|TimeBuf2[7]);	
						}	
					}

					if((TimeBuf2[3]*10+TimeBuf2[4])==2)
					{
						if((TimeBuf2[0]*10+TimeBuf2[1])%4==0)
						{
							if((TimeBuf2[6]*10+TimeBuf2[7])<=29)
							{	//向ds1302写入所设的年，月，日
								DS1302_SetTime_YMD(
								(TimeBuf2[0]<<4)|TimeBuf2[1],
								(TimeBuf2[3]<<4)|TimeBuf2[4],
								(TimeBuf2[6]<<4)|TimeBuf2[7]);		
							}
						}
						else 
						{
							
							if((TimeBuf2[6]*10+TimeBuf2[7])<=28)
							{	//向ds1302写入所设的年，月，日
								DS1302_SetTime_YMD(
								(TimeBuf2[0]<<4)|TimeBuf2[1],
								(TimeBuf2[3]<<4)|TimeBuf2[4],
								(TimeBuf2[6]<<4)|TimeBuf2[7]);		
							}			
						}
					}
					SetTimeFlag=0;
					//SegJumpShowFlag=1;结束了，数码管不用再蹦
				}				
			}break;

			case 2:
			{
				if(KeyValue==(KEY_UP|KEY_VALUE1))//设置	
				{
					SetTimeFlag=1;//Start Set Time Mode And 
								  //Stop Read DS1302;
					SegJumpShowFlag=0;//停止闪烁，调整

					if(TimeBuf3[7]==' ')//' '=空格；
					{
						TimeBuf3[7]=TimeBuf3[8];
						TimeBuf3[8]=' ';
					}
					
					SegJumpShowFlag=1;//Seg start jumpshow
				 }

				if((KeyValue==(KEY_UP|KEY_VALUE2))&&(SetTimeFlag==1))	
				{
					SegJumpShowFlag=0;
					if(TimeBuf3[7]==' ')
					{
						TimeBuf3[7]=TimeBuf3[8];
						TimeBuf3[8]=' ';
					}//adjust
					if(++TimeBuf3[7]==8)
					{
						TimeBuf3[7]=1;
					}
					SegJumpShowFlag=1;
				}

				if(KeyValue==(KEY_UP|KEY_VALUE3)&&(SetTimeFlag==1))
				{
					SegJumpShowFlag=0;
					if(TimeBuf3[7]==' ')
					{
						TimeBuf3[7]=TimeBuf3[8];
						TimeBuf3[8]=' ';
					}
					if(--TimeBuf3[7]==0)
					{
						TimeBuf3[7]=7;
					}
					SegJumpShowFlag=1;
				}


				//确定
				if((KeyValue==(KEY_UP|KEY_VALUE4))&&(SetTimeFlag==1))
				{
					SegJumpShowFlag=0;
					if(TimeBuf3[7]==' ')
					{
						TimeBuf3[7]=TimeBuf3[8];
						TimeBuf3[8]=' ';
					}

					if((TimeBuf3[7]<=7)&&(TimeBuf3[7]>=1))
					{
						DS1302_SetTime_WEEK(TimeBuf3[7]);
					}	//向ds1302写入所设的星期
					
					SetTimeFlag=0;
					//SegJumpShowFlag=1;结束了，数码管不用再蹦
				}						
			}	
			case 3://temperature
			{
				if(KeyValue==(KEY_UP|KEY_VALUE1))//设置	
				{
		
					SetTemperatureFlag=1;//Start Set Temperature Mode 
										//And Stop Read DS18B02;
					SegJumpShowFlag=0;//停止闪烁，调整
			  		
					if(CurrentTemperature[BufPosition]==' ')//' '=空格；
					{
						CurrentTemperature[BufPosition]=CurrentTemperature[8];
						CurrentTemperature[8]=' ';
					}
	
					SegJumpShowFlag=1;// Seg start jumpshow
					
					BufPosition++;
					
					if((BufPosition<=1)&&(BufPosition>=5))
					{
						BufPosition=1;
					}
				}		 
				//+
				if((KeyValue==(KEY_UP|KEY_VALUE2))&&(SetTemperatureFlag==1))	
				{
					SegJumpShowFlag=0;
					if(CurrentTemperature[BufPosition]==' ')
					{
						CurrentTemperature[BufPosition]=CurrentTemperature[8];
						CurrentTemperature[8]=' ';
					}					//adjust
					
					if(BufPosition==2)
					{
						if(CurrentTemperature[2]==29)
						{
							CurrentTemperature[2]=20;
						}
						else
						{
							CurrentTemperature[2]++;
						}		
					}
					else
					{
						if((CurrentTemperature[BufPosition]==9))
						{
							CurrentTemperature[BufPosition]=0;	
						}
						else
						{
							CurrentTemperature[BufPosition]++;
						}	
					}
					SegJumpShowFlag=1;
				}
				//-
				if((KeyValue==(KEY_UP|KEY_VALUE3))&&(SetTemperatureFlag==1))	
				{
					SegJumpShowFlag=0;
					if(CurrentTemperature[BufPosition]==' ')
					{
						CurrentTemperature[BufPosition]=CurrentTemperature[8];
						CurrentTemperature[8]=' ';
					}					//adjust
					
					if(BufPosition==2)
					{
						if(CurrentTemperature[2]==20)
						{
							CurrentTemperature[2]=29;
						}
						else
						{
							CurrentTemperature[2]--;
						}		
					}
					else
					{
						if((CurrentTemperature[BufPosition]==0))
						{
							CurrentTemperature[BufPosition]=9;	
						}
						else
						{
							CurrentTemperature[BufPosition]--;
						}	
					}
					SegJumpShowFlag=1;
				}
			}break;
			if((KeyValue==(KEY_UP|KEY_VALUE4))&&(SetTimeFlag==1))
			{
				SegJumpShowFlag=0;
				if(TimeBuf3[7]==' ')
				{
					TimeBuf3[7]=TimeBuf3[8];
					TimeBuf3[8]=' ';
				}

				//to do 
				//向ds18b20写入所设的高温
					
				SetTimeFlag=0;
				//SegJumpShowFlag=1;结束了，数码管不用再蹦
			}

/*			case 4:
			{
			
			} break;

			case 5:
			{
				
			}break;
			default:break;
		}
/*
		if(KeyValue==(KEY_UP|KEY_VALUE1)&&((IrKeyVal.SegPicture==0)||(IrKeyVal.SegPicture==1)||(IrKeyVal.SegPicture==3)))//设置	
		{
		
			SetTimeFlag=1;//Start Set Time Mode And Stop Read DS1302;
			SegJumpShowFlag=0;//停止闪烁，调整

			if(LedDisBuffer[BufPos]==' ')//' '=空格；
			{
				LedDisBuffer[BufPos]=LedDisBuffer[8];
				LedDisBuffer[8]=' ';
			}
			BufPos++;
			SegJumpShowFlag=1;// Seg start jumpshow

			if((BufPos==2)||(BufPos==5))
			{
				BufPos++;
			}//do not change '-'
			
			if(BufPos==8)
			{
				BufPos=0;
			}
		}
		
			
		if((KeyValue==(KEY_UP|KEY_VALUE2))&&(SetTimeFlag==1))	
		{
			SegJumpShowFlag=0;
			if(LedDisBuffer[BufPos]==' ')
			{
				LedDisBuffer[BufPos]=LedDisBuffer[8];
				LedDisBuffer[8]=' ';
			}					//adjust
			
			if(++LedDisBuffer[BufPos]==10)//LedDisBuffer[BufPos]++;then if()
			{
				LedDisBuffer[BufPos]=0;	
			}
			SegJumpShowFlag=1;
		}
		//显示下组画面
		

		if((KeyValue==(KEY_UP|KEY_VALUE4))&&(SetTimeFlag==1))
		{
			SegJumpShowFlag=0;
			if(LedDisBuffer[BufPos]==' ')
			{
				LedDisBuffer[BufPos]=LedDisBuffer[8];
				LedDisBuffer[8]=' ';
			}

			if((IrKeyVal.SegPicture==0)&&(LedDisBuffer[0]<2)&&(LedDisBuffer[3]<=6)&&(LedDisBuffer[6]<=6)||((LedDisBuffer[0]==2)&&(LedDisBuffer[1]<=3)))//犯错处理
			{
				DS1302_SetTime_HMS((LedDisBuffer[0]<<4)|LedDisBuffer[1],(LedDisBuffer[3]<<4)|LedDisBuffer[4],(LedDisBuffer[6]<<4)|LedDisBuffer[7]);//向ds1302写入所设的小时，分钟，秒
			}

			if((IrKeyVal.SegPicture==1)&&(LedDisBuffer[6]<=3)&&((LedDisBuffer[3]==0)||((LedDisBuffer[3]==1)&&(LedDisBuffer[4]<=2))))
			{
				DS1302_SetTime_YMD((LedDisBuffer[0]<<4)|LedDisBuffer[1],(LedDisBuffer[3]<<4)|LedDisBuffer[4],(LedDisBuffer[6]<<4)|LedDisBuffer[7]);//向ds1302写入所设的年，月，日	
			}

			if((IrKeyVal.SegPicture==3)&&(LedDisBuffer[0]<2)&&(LedDisBuffer[3]<=6)&&(LedDisBuffer[6]<=6)||((LedDisBuffer[0]==2)&&(LedDisBuffer[1]<=3)))//犯错处理
			{
				BuzzerTime[0]=LedDisBuffer[0];//闹钟响的小时
				BuzzerTime[1]=LedDisBuffer[1];

				BuzzerTime[2]=LedDisBuffer[3];//闹钟响的分钟

				BuzzerTime[3]=LedDisBuffer[4];
				
				BuzzerTime[5]=LedDisBuffer[6];//闹钟要响的时间
				BuzzerTime[6]=LedDisBuffer[7];//单位:分钟
			}

			//SegJumpShowFlag=1;
			SetTimeFlag=0; */
		}
			
	}	
}
