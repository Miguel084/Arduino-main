/* �÷���
   LCD_init();
   LCD_write_string(��,��,"�ַ���");
   LCD_write_char(��,��,'�ַ�'); 
 ---------------------------------------------------------------
������AVR��LCD������Ϣ
  PC6 ->RS
  PC7 ->EN
  ��  ->RW
  PA4 ->D4
  PA5 ->D5
  PA6 ->D6
  PA7 ->D7
ʹ�ö˿ڣ�1602:PC6,PC7,PA4~PA7 	
Ҫʹ�ñ��������ı�����������Ϣ����
-----------------------------------------------------------------*/
#define LCD_EN_PORT    PORTC   //����2��Ҫ��Ϊͬһ����
#define LCD_EN_DDR     DDRC
#define LCD_RS_PORT    PORTC   //����2��Ҫ��Ϊͬһ����
#define LCD_RS_DDR     DDRC
#define LCD_DATA_PORT  PORTA   //����3��Ҫ��Ϊͬһ����
#define LCD_DATA_DDR   DDRA    //Ĭ����������߱���ʹ�ø���λ�˿�,���������ע���޸�
#define LCD_DATA_PIN   PINA
#define LCD_RS         (1<<PC6) //0x20   portC6       out
#define LCD_EN         (1<<PC7) //0x40   portC7       out
#define LCD_DATA       ((1<<PA4)|(1<<PA5)|(1<<PA6)|(1<<PA7)) //0xf0   portA 4/5/6/7 out
/*--------------------------------------------------------------------------------------------------
����˵��
--------------------------------------------------------------------------------------------------*/
void LCD_init(void);
void LCD_en_write(void);
void LCD_write_command(unsigned  char command) ;
void LCD_write_data(unsigned char data);
void LCD_set_xy (unsigned char x, unsigned char y);
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s);
void LCD_write_char(unsigned char X,unsigned char Y,unsigned char data);

//-----------------------------------------------------------------------------------------

#include <macros.h>
#include "delay.h"

void LCD_init(void)         //Һ����ʼ��
{
  LCD_DATA_DDR|=LCD_DATA;   //���ݿڷ���Ϊ���
  LCD_EN_DDR|=LCD_EN;       //����EN����Ϊ���
  LCD_RS_DDR|=LCD_RS;       //����RS����Ϊ���
  LCD_write_command(0x28); 
  LCD_en_write();
  delay_nus(100);
  LCD_write_command(0x28);  //4λ��ʾ
  LCD_write_command(0x0c);  //��ʾ��
  LCD_write_command(0x01);  //����
  delay_nms(10);
  LCD_write_string(0,0,"Light:       "); 
  delay_nms(10);
}

void LCD_en_write(void)  //Һ��ʹ��
{
  LCD_EN_PORT|=LCD_EN;
  delay_nus(10);
  LCD_EN_PORT&=~LCD_EN;
}

void LCD_write_command(unsigned char command) //дָ��
{
  //����Ϊ��4λ��д��
  delay_nus(16);
  LCD_RS_PORT&=~LCD_RS;        //RS=0
  LCD_DATA_PORT&=0X0f;         //�����λ
  LCD_DATA_PORT|=command&0xf0; //д����λ
  LCD_en_write();
  command=command<<4;          //����λ�Ƶ�����λ
  LCD_DATA_PORT&=0x0f;         //�����λ
  LCD_DATA_PORT|=command&0xf0; //д����λ
  LCD_en_write();
 
/*
  //����Ϊ����λ��д��
  delay_nus(16);
  LCD_RS_PORT&=~LCD_RS;        //RS=0
  LCD_DATA_PORT&=0xf0;         //�����λ
  LCD_DATA_PORT|=(command>>4)&0x0f; //д����λ
  LCD_en_write();
  LCD_DATA_PORT&=0xf0;         //�����λ
  LCD_DATA_PORT|=command&0x0f; //д����λ
  LCD_en_write(); 
*/
  
}

void LCD_write_data(unsigned char data) //д����
{
  //����Ϊ��4λ��д��
  delay_nus(16);
  LCD_RS_PORT|=LCD_RS;       //RS=1
  LCD_DATA_PORT&=0X0f;       //�����λ
  LCD_DATA_PORT|=data&0xf0;  //д����λ
  LCD_en_write();
  data=data<<4;               //����λ�Ƶ�����λ
  LCD_DATA_PORT&=0X0f;        //�����λ
  LCD_DATA_PORT|=data&0xf0;   //д����λ
  LCD_en_write();
  
/*
  //����Ϊ����λ��д�� 
  delay_nus(16);
  LCD_RS_PORT|=LCD_RS;       //RS=1
  LCD_DATA_PORT&=0Xf0;       //�����λ
  LCD_DATA_PORT|=(data>>4)&0x0f;  //д����λ
  LCD_en_write();
 
  LCD_DATA_PORT&=0Xf0;        //�����λ
  LCD_DATA_PORT|=data&0x0f;   //д����λ
  LCD_en_write();
*/
  
}


void LCD_set_xy( unsigned char x, unsigned char y )  //д��ַ����
{
    unsigned char address;
    if (y == 0) address = 0x80 + x;
    else   address = 0xc0 + x;
    LCD_write_command( address);
}
  
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s) //��x=0~15,��y=0,1
{
    LCD_set_xy( X, Y ); //д��ַ    
    while (*s)  // д��ʾ�ַ�
    {
      LCD_write_data( *s );
      s ++;
    }
      
}

void LCD_write_char(unsigned char X,unsigned char Y,unsigned char data) //��x=0~15,��y=0,1
{
  LCD_set_xy( X, Y ); //д��ַ
  LCD_write_data( data);
  
}
