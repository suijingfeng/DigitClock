#include"MacroAndConst.h"

/*
void DelayMs(unsigned int ms)
{
	unsigned char i;
	while(ms--)
	for(i=122;i>0;i--)
	;
}
*/
void DelayUs(unsigned char us)
{
	while(us--);
}
