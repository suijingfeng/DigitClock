#ifndef __DS1302_H_
#define __DS1302_H_

extern unsigned char TimeBuf1[9];
extern unsigned char TimeBuf2[9];
extern unsigned char TimeBuf3[9];

extern void DS1302_ClockInit();
extern void DS1302_ClockUpdata_SMH(unsigned char* pLedCode);
extern void DS1302_ClockUpdata_DMY(unsigned char* pLedCode);
extern void DS1302_ClockUpdata_WEEK(unsigned char* pBuffer);

//extern void DS1302_ClockBurstRead();

extern void DS1302_SetTime_HMS(unsigned char,unsigned char,unsigned char);
extern void DS1302_SetTime_YMD(unsigned char,unsigned char,unsigned char);
extern void DS1302_SetTime_WEEK(unsigned char);
/*
extern struct
{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Date;
	unsigned char Month;
	unsigned char Week;
	unsigned char Year; 
}CurrentTime;
*/


#endif
