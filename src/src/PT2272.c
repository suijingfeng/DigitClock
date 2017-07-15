#include<reg52.h>
#include"pt2272.h"
#include"buzzer.h"
#include"delay.h"
#include"seg.h"

unsigned char Pt2272Scan()
{
	unsigned char abcd;
	if(!PT2272_D3)
	{
		acbd=3;
		LedDisBuffer[0]=3;	
	}
	if(!PT2272_D2)
	{
		abcd=2;
		BuzzerOn();
		DelayMs(500);
		BuzzerOff();
	}
	if(!PT2272_D1)
	{
		abcd=1;
		BuzzerOff();
	}
	if(!PT2272_D0)
	{
		abcd=0;
		BuzzerOn();
	}
	return abcd;	
}
