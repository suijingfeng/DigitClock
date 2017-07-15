#ifndef __PT2272_H_
#define __PT2272_H_

sbit PT2272_VT=P3^3;
sbit PT2272_D3=P3^4;
sbit PT2272_D2=P3^5;
sbit PT2272_D1=P3^6;
sbit PT2272_D0=P3^7;

extern unsigned char Pt2272Scan();
 
#endif