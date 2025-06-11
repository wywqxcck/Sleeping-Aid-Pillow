#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"
#include "Delay.h"

#define u8 uint8_t
#define u32 uint32_t

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define OLED0561_ADD	0x78  // OLED I2C地址
#define COM				0x00  // OLED 
#define DAT 			0x40  // OLED 

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64


//-----------------OLED IIC GPIO进行模拟----------------
#define  OLED_SCLK_PORT_CLK           (RCC_APB2Periph_GPIOB)
#define  OLED_SCLK_PORT_APBxClkCmd    RCC_APB2PeriphClockCmd
#define  OLED_SDIN_PORT_CLK           (RCC_APB2Periph_GPIOB)
#define  OLED_SDIN_PORT_APBxClkCmd    RCC_APB2PeriphClockCmd
#define  OLED_SCLK_PORT               GPIOB
#define  OLED_SCLK_PIN                GPIO_Pin_8
#define  OLED_SDIN_PORT               GPIOB
#define  OLED_SDIN_PIN                GPIO_Pin_9

#define OLED_SCLK_Clr() GPIO_ResetBits(OLED_SCLK_PORT, OLED_SCLK_PIN)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(OLED_SCLK_PORT,OLED_SCLK_PIN)

#define OLED_SDIN_Clr() GPIO_ResetBits(OLED_SDIN_PORT,OLED_SDIN_PIN)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(OLED_SDIN_PORT,OLED_SDIN_PIN)



//I2C GPIO模拟
void IIC_Start(void);
void IIC_Stop(void);
void IIC_WaitAck(void);
void IIC_WriteByte(unsigned char IIC_Byte);
void IIC_WriteCommand(unsigned char IIC_Command);
void IIC_WriteData(unsigned char IIC_Data);
void OLED_WR_Byte(unsigned dat,unsigned cmd);


//功能函数
void OLED_Init(void);
void OLED_WR_Byte(unsigned dat,unsigned cmd);

void OLED_FillPicture(unsigned char fill_Data);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_DisplayOn(void);
void OLED_DisplayOff(void);
void OLED_Clear(void);
void OLED_On(void);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);

void OLED_DrawBMP2(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

#endif
