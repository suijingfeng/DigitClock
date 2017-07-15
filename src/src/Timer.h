#ifndef _TIMER_H_
#define _TIMER_H_

extern unsigned char T1_250us;

extern bit SegJump1HzFlag;
extern bit SegFlash1s;
extern bit Ds1302UpdateFlag;
extern bit Ds18b20UpdateFlag;
extern bit SegScanFlag;
extern bit KeyScanFlag;
//extern bit IrOverflowFlag;
extern bit IrScanFlag;

extern bit Flag_4KHz;

extern bit Led_PWM_Flag;

extern void Timer0Init();
extern void Timer1Init();
extern void Timer2Init();

#endif