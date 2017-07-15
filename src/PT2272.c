#include<reg52.h>
#include"pt2272.h"
#include"buzzer.h"
#include"delay.h"


unsigned char Pt2272Scan()
{
	unsigned char abcd;
	
	P3=0x00;

	if(!PT2272_D3)
	{
		abcd=4;	
	}
	if(!PT2272_D2)
	{
		abcd=3;
	}
	if(!PT2272_D1)
	{
		abcd=2;	
	}
	if(!PT2272_D0)
	{
		abcd=1;
	}
	return abcd;	
}
