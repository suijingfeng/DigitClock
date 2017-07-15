#ifndef __AT24C02_H__
#define __AT24C02_H__

extern void At24c02ByteWrite(unsigned char address,unsigned char content);
extern void At24c02PageWrite(unsigned char address,unsigned char *pData);

extern unsigned char At24c02CurrentRead(void);
extern unsigned char At24c02RandomRead(unsigned char address);
void At24c02SequentialRead(unsigned char *pD);
/////////////
extern void write_word(unsigned char address,unsigned char dat);
extern unsigned char read_word(unsigned char address);
///////////

sbit AT24C02_SDA=P2^0;//AT24C02_Serial Data IO
sbit AT24C02_SCL=P2^1;//AT24C02_Serial Clock
#endif
