#ifndef __SEG_H_
#define __SEG_H_


extern void LedDisplay(unsigned char* pBuffer);
extern void SetClock(unsigned char hour,unsigned char minute,unsigned char second);
extern void RunClock(void);
extern void SegJumpShow();

//extern void SegFlash();

extern void SegJumpShow(unsigned char *pBuffer);

extern unsigned char LedDisBuffer[24];
extern unsigned char g_uBufPosition;//global

extern bit SetTimeFlag;
extern bit SegJumpShowFlag;		
#endif