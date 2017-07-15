#ifndef __EXINTERRUPT_H_
#define __EXINTERRUPT_H_

extern bit SegShowDisableFlag;
extern bit SetModeFlag;

extern struct
{	
	unsigned char SegPicture;
	unsigned char Number;
	unsigned char IrCode[4];
}IrKeyVal;

extern void Ex0IptInit();
extern void IrKeyMainLoop();

#endif 