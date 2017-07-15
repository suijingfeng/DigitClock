#ifndef __SEG_H_
#define __SEG_H_

#define DataPort P0
sbit LedBitSel=P2^6;
sbit LedSegSel=P2^7;

extern void LedDisplay(unsigned char* pBuffer);
extern void SetClock(unsigned char hour,unsigned char minute,unsigned char second);
extern void RunClock(void);

extern unsigned char LedDisBuffer[9];
extern unsigned char BufPos;

extern bit SetTimeFlag;
extern bit SegJumpShowFlag;
		
#endif