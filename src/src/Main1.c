#include<reg52.h>
#include<intrins.h>
#include"MacroAndConst.h"
#include"Timer.h"
#include"Timer2.h"
#include"Seg.h"

sbit LED2=P1^2;
sbit LED3=P1^3;


void main()
{	

	Timer0Init();
	Timer2Init();
	EA=1;
	P1=0xff;
	while(1)
	{
		LedDisplay(LedDisBuffer);
	}
}

/*
		T2_SystemTime1s++;
		
	
if(T2_SystemTime1s>=1000)
		{
			T2_SystemTime1s=0;
			Second++;
			
			if(Second>=60)
			{
				Second=0;
			}
		 	}
*/	
