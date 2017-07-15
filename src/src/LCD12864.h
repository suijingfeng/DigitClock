#ifndef __LCD12864_H__
#define __LCD12864_H__

extern void LCD12864_WriteCommand(unsigned char content);
extern void LCD12864_WriteData(unsigned char content);
extern void LCD12864_Initial();
extern void LCD12864_Show();
extern unsigned char PowerDt[6];

#endif
