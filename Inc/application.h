#ifndef __application_H
#define __application_H
#include "main.h"


/*
WTN6040����оƬ
*/
#define  CLK_2A                 PBout(13) //ʱ�������λ��������
#define  P_DATA_2A               PBout(12) //���������λ��������
#define  CLK_2A_PIN             GPIO_PIN_13//ʱ������
#define  P_DATA_2A_PIN          GPIO_PIN_12//��������
#define  SYSCLOSE_PIN		GPIO_PIN_8 //�ر�ϵͳ���ţ��ߵ�ƽ�ر�
#define  SYSCLOSE_PORT		GPIOB
#define  SYSIO				PBout(8)   //λ������
/************************************************************************/
/* 74HC595                                                              */
/************************************************************************/
#define  DS    PAout(5)
#define  RCK   PAout(6)
#define  SCK   PAout(7)

#define  HC595_PORT		GPIOA
#define  HC595_PIN      GPIO_PIN_5 | GPIO_PIN_5 | GPIO_PIN_7
void HC595_Init();
void SendData_595(uint16_t outdate);

/*TFT��������*/
#define array_length          100  //�������鳤��
#define USER_R3               0xA5  //֡ͷ���ֽ�
#define USER_RA               0x5A  //֡ͷ���ֽ�

void write_register_80_1byte(uint8_t address, uint8_t data);
void write_multiple_register_80(uint8_t address, uint8_t data_length, uint8_t *data);
void write_variable_store_82_1word(uint16_t address, uint16_t data);
void write_multiple_variable_store_82(uint16_t address, uint8_t data_length, uint16_t *data);
uint8_t Line_2A_WTN6(uint8_t SB_DATA);
void TFT_playsound(uint8_t data_length, uint8_t *data, UART_HandleTypeDef *huart);
void playmusic(uint16_t num, uint8_t val);
void stopmusic(uint16_t num);
void Turen_Pic(uint16_t num);
void TFT_Beep(uint8_t n);//����������
void TFT_Readbytes(uint8_t adress, uint8_t readlen, UART_HandleTypeDef *huart);

void HMI_SetVal(UART_HandleTypeDef *huart, const char *com, uint32_t val);//�������ݵ�HMI��;
void HMI_SetVal_t();//�������ݵ�HMI��;
void HMI_SetVal_t(UART_HandleTypeDef *huart, const char *com, uint32_t val);
void HMI_SetTxt(UART_HandleTypeDef *huart, const char *com, char *txt);//���������ַ���
uint8_t WTN6040_PlayArray(uint8_t arrlen, uint8_t *soundarr);
void WTN6040_PlayOneByte(uint8_t val);//���ŵ���
#endif
