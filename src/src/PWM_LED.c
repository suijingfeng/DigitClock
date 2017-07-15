#include<reg52.h>
#include"timer.h" 


#define PWM_TIME_MAX 99
#define PWM_TIME_MIN 0

sbit LED=P1^4;

unsigned char LED_TimeCounter=0;
unsigned char PWM_TimeCounter=0;
unsigned char TimeCounter=0;

bit direction=0;

void LEDON()
{
	LED=0;
}

void LEDOFF()
{
	LED=1;
}

void LEDisplay()
{
	if(++TimeCounter>=100)
	{
		TimeCounter=0;
		
		if(direction)
		{
			PWM_TimeCounter++;
			if(PWM_TimeCounter>=PWM_TIME_MAX)
			{
				PWM_TimeCounter=PWM_TIME_MAX;
				direction=0;	
			} 	
		}

		else
		{
			PWM_TimeCounter--;
			if(PWM_TimeCounter<=0)
			{
				PWM_TimeCounter=0;
				direction=1;
			}
		}
		LED_TimeCounter=PWM_TimeCounter;
	}

	if(LED_TimeCounter>0)
	{	
		LEDON();
		LED_TimeCounter--;
	}
	else
	{
		LEDOFF();
	}			
}			


