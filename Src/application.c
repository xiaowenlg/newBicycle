/*!
 *
 * TFT����74HC595����
 * date ���� 2019
 */
#include "application.h"
#include "system.h"
#include "usart.h"
#include "dwt_stm32_delay.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "BspConfig.h"
//#include "cmsis_os.h"
//************************************
// ����:    HC595_Init
// ����ȫ��:  HC595_Init
// ��������:    public 
// ����ֵ:   void
// Qualifier:595��ʼ��
//************************************
void HC595_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(HC595_PORT, HC595_PIN, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = HC595_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HC595_PORT, &GPIO_InitStruct);
}
//************************************
// ����:    SendData_595
// ����ȫ��:  SendData_595
// ��������:    public 
// ����ֵ:   void
// Qualifier:��������
// ����: uint16_t outdate
//************************************
void SendData_595(uint16_t outdate)
{
	uint8_t i = 0;
	for (i = 0; i < 16; i++)
	{
		
		if ((outdate & 0x8000) != 0)
			DS = 1;
		else
			DS = 0;
		SCK = 0;
		__NOP();
		__NOP();
		__NOP();
		SCK = 1;
		__NOP();
		__NOP();
		__NOP();
		outdate <<= 1;

	}
	RCK = 0;
	osDelay(1);
	RCK = 1;

}
/*TFT����*/
//************************************
// ����:    write_register_80_1byte
// ����ȫ��:  write_register_80_1byte
// ��������:    public 
// ����ֵ:   void
// Qualifier:д�Ĵ���1�ֽ�
// ����: uint8_t address
// ����: uint8_t data
//************************************
void write_register_80_1byte(uint8_t address, uint8_t data) 
{
	uint8_t i;
	uint8_t array[array_length];

	array[0] = USER_R3;
	array[1] = USER_RA;
	array[2] = 0x03;               //ָ���
	array[3] = 0x80;
	array[4] = address;
	array[5] = data;
	MY_USART_SendData(UART_TFT, array, 6);
}

void write_multiple_register_80(uint8_t address, uint8_t data_length, uint8_t *data)
{
	uint8_t i, nDataLen;
	uint8_t array[array_length];

	array[0] = USER_R3;
	array[1] = USER_RA;
	array[2] = 1 + 1 + data_length;   //����+��ַ+���ݳ���
	array[3] = 0x80;
	array[4] = address;

	for (i = 0; i < data_length; i++)
	{
		array[5 + i] = data[i];
	}

	nDataLen = array[2] + 3;        //��Ч�����

	MY_USART_SendData(UART_TFT, array, nDataLen);
}
//************************************
// ����:    write_variable_store_82_1word
// ����ȫ��:  write_variable_store_82_1word
// ��������:    public 
// ����ֵ:   void
// Qualifier:дһ������ֵ
// ����: uint16_t address
// ����: uint16_t data
//************************************
void write_variable_store_82_1word(uint16_t address, uint16_t data)
{
	uint8_t i;
	uint8_t array[array_length];

	array[0] = USER_R3;
	array[1] = USER_RA;
	array[2] = 1 + 2 + 2;                  
	array[3] = 0x82;
	array[4] = (address & 0xFF00) >> 8;    
	array[5] = address & 0x00FF;         
	array[6] = (data & 0xFF00) >> 8;      
	array[7] = data & 0x00FF;            

	//MY_USART_SendData(UART_TFT, array, 8);
	HAL_UART_Transmit(UART_TFT, array, 8, 0xffff);
}

//************************************
// ����:    write_multiple_variable_store_82
// ����ȫ��:  write_multiple_variable_store_82
// ��������:    public 
// ����ֵ:   void
// Qualifier:д�������ֵ
// ����: uint16_t address
// ����: uint8_t data_length
// ����: uint16_t * data
//************************************
void write_multiple_variable_store_82(uint16_t address, uint8_t data_length, uint16_t *data)
{
	uint8_t i, nDataLen;
	uint8_t array[array_length];

	array[0] = USER_R3;
	array[1] = USER_RA;
	array[2] = 1 + 2 + data_length * 2;  
	array[3] = 0x82;
	array[4] = (address & 0xFF00) >> 8;        
	array[5] = address & 0x00FF;             

	for (i = 0; i < data_length; i++)
	{
		array[6 + 2 * i] = (data[i] & 0xFF00) >> 8; 
		array[7 + 2 * i] = data[i] & 0x00FF;      
	}

	nDataLen = array[2] + 3;                 

	//MY_USART_SendData(UART_TFT, array, nDataLen);
	HAL_UART_Transmit(UART_TFT, array, nDataLen, 0xffff);
}
uint8_t  Line_2A_WTN6(uint8_t SB_DATA)
{
	uint8_t S_DATA, B_DATA, j;
	CLK_2A = 1; //ʱ����
	P_DATA_2A = 1; //������
	S_DATA = SB_DATA;
	CLK_2A = 0;
	osDelay(5);
	B_DATA = S_DATA & 0X01;
	//PBout(13) = 1;
	for (j = 0; j < 8; j++)
	{

		CLK_2A = 0; // ����
		P_DATA_2A = B_DATA; //��������һλ
		//SendByte2(B_DATA);
		
		DWT_Delay_us(300); //��ʱ 300us 
		CLK_2A = 1; //����
		DWT_Delay_us(300); //��ʱ 300us 

		S_DATA = S_DATA >> 1;
		B_DATA = S_DATA & 0X01;
	}
	P_DATA_2A = 1;
	CLK_2A = 1;


}
#ifdef TFTPLAY
void TFT_playsound(uint8_t data_length, uint8_t *data, UART_HandleTypeDef *huart)//TFT��������
{
	uint8_t i, nDataLen;
	uint8_t array[array_length];

	array[0] = USER_R3;
	array[1] = USER_RA;
	array[2] = 1 + 2 + data_length * 2;
	array[3] = 0x85;
	array[4] = 0x03;
	array[5] = 0x01;

	for (i = 0; i < data_length; i++)
	{
		//array[6 + 2 * i] = (data[i] & 0xFF00) >> 8;
		array[6 + 2 * i] = 0;
		array[7 + 2 * i] = data[i] & 0x00FF;
	}
	
	nDataLen = array[2] + 3;
	
	MY_USART_SendData(huart, array, nDataLen);
}
#else
void WTN6040_PlayOneByte(uint8_t val)
{
	while (HAL_GPIO_ReadPin(WTN6040_BUSY_GPIO_Port, WTN6040_BUSY_Pin) != 1);
	Line_2A_WTN6(val);
	
	
}
uint8_t WTN6040_PlayArray(uint8_t arrlen, uint8_t *soundarr)
{
	uint8_t res = 0;
	uint8_t i = 0;
	for (i = 0; i < arrlen; i++)
	{
		WTN6040_PlayOneByte(soundarr[i]);
		//osDelay(10);
		//Uart_printf(&DEBUG_UART.huart, "Data=========%d:%x\r\n", i, soundarr[i]);//���Բ�������
	}
	while (HAL_GPIO_ReadPin(WTN6040_BUSY_GPIO_Port, WTN6040_BUSY_Pin) != 1);
	if (i == arrlen)
		res = 1;
	else
		res = 1;//�������
	//Line_2A_WTN6(9);
	return res;
}
#endif
//************************************
// ����:    playmusic
// ����ȫ��:  playmusic
// ��������:    public 
// ����ֵ:   void
// Qualifier:���ֲ���
// ����: uint16_t num
// ����: uint8_t val
//************************************
void playmusic(uint16_t num, uint8_t val)
{
	uint8_t senddat[5] = {0};
	senddat[0] = 0x5b;
	senddat[1] = num >> 8;
	senddat[2] = num & 0x00ff;
	senddat[3] = 0x5A;
	if (val > 64)
		val = 64;
	senddat[4] = val;
	write_multiple_register_80(0x50, 5, senddat);
}
//************************************
// ����:    stopmusic
// ����ȫ��:  stopmusic
// ��������:    public 
// ����ֵ:   void
// Qualifier: //ֹͣ��������
// ����: uint16_t num    Ҫֹͣ�����ֵ�ַ��
//************************************
void stopmusic(uint16_t num)
{
	uint8_t senddat[3] = { 0 };
	senddat[0] = 0x5c;
	senddat[1] = num >> 8;
	senddat[2] = num & 0x00ff;
	write_multiple_register_80(0x50, 3, senddat);
}
//************************************
// ����:    Turen_Pic
// ����ȫ��:  Turen_Pic
// ��������:    public 
// ����ֵ:   void
// Qualifier:�л�ҳ��
// ����: uint16_t num
//************************************
void Turen_Pic(uint16_t num)
{
	uint8_t senddat[2] = { 0 };
	
	senddat[0] = num >> 8;
	senddat[1] = num & 0x00ff;
	write_multiple_register_80(0x03, 2, senddat);
	//pageID = num;
}
void TFT_Beep(uint8_t n)//TFT��������
{
	write_register_80_1byte(0x02, n);
}
void TFT_Readbytes(uint8_t adress,uint8_t readlen,UART_HandleTypeDef *huart)//���Ĵ���
{
	uint8_t senddat[3] = { 0 };
	senddat[0] = USER_R3;
	senddat[1] = USER_RA;
	senddat[2] = 3;
    senddat[3] = 0x81;
	senddat[4] = adress;
	senddat[5] = readlen;
	MY_USART_SendData(huart, senddat, 6);//���Ͷ�����

	
}
//���ͽ��շ�
void HMI_SendEnd(UART_HandleTypeDef *huart)
{
	MY_USART_SendByte(huart, 0xff);
	MY_USART_SendByte(huart, 0xff);
	MY_USART_SendByte(huart, 0xff);
}
//************************************
// ����:    HMI_SetVal
// ����ȫ��:  HMI_SetVal
// ��������:    public 
// ����ֵ:   void
// Qualifier: //�������ݵ�HMI��
// ����: UART_HandleTypeDef * huart
// ����: const char * com  �����ַ��� ��"n0.val="
// ����: uint32_t val    ����ֵ
//************************************
void HMI_SetVal(UART_HandleTypeDef *huart, const char *com,uint32_t val )//�������ݵ�HMI��
{
	Uart_printf(huart, "%s%d", com, val);
	HMI_SendEnd(huart);

}
void HMI_SetVal_t(UART_HandleTypeDef *huart, const char *com, uint32_t val)
{
	printf("%s%d", com, val); fflush(stdout);
	HMI_SendEnd(huart);

}

void HMI_SetTxt(UART_HandleTypeDef *huart, const char *com, char *txt)//�������ݵ�HMI��
{
	//char DArray[HMIARRAYLENGTH] = {0};
	//sprintf(DArray,"n0.val=%d", val);
	//sprintf(DArray, "%s%d", com, val);
	//MY_USART_chars(huart, "x");
	//sprintf(DArray, "%s\"%s\"",com, txt);
	//MY_USART_chars(huart, DArray);
	Uart_printf(huart, "%s\"%s\"", com, txt);
	HMI_SendEnd(huart);

}
