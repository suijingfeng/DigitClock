#ifndef __BUZZER_H__
#define __BUZZER_H__

sbit buzzer=P1^4;

extern void BuzzerOn();
extern void BuzzerOff();

#endif
