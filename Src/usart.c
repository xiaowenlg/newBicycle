/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include <stdlib.h>
#include <stdarg.h>
//#include "MainConfig.h"
/* USER CODE BEGIN 0 */
#define  DEBUG
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
ReceMessage uart1_rec={0};//���ձ���
ReceMessage uart2_rec={0};
ReceMessage uart3_rec={0};
/* �����ź������ */
SemaphoreHandle_t xSemaphore = NULL;
/* printf �ض������---------�ɹ�*/
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{
	int handle;
	/* Whatever you require here. If the only file you are using is */
	/* standard output using printf() for debugging, no file handling */
	/* is required. */
};
/* FILE is typedef�� d in stdio.h. */
FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x)
{
	x = x;
}
//�ض���fputc����
//printf�������ָ��fputc����fputc���������
//����ʹ�ô���1(USART1)���printf��Ϣ
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0);//�ȴ���һ�δ������ݷ������  
	USART1->DR = (u8)ch;       //дDR,����1����������
	return ch;
}
#endif 
//�ض����������
int _write(int fd, char *pBuffer, int size)
{
	HAL_UART_Transmit(&huart1, pBuffer, size, 0xff);
	fflush(stdout);//����ˢ�������
	return size;
	
}
/* printf �ض������---------�ɹ�*/
void MX_USART_UART_Init(UART_HandleTypeDef *huart, USART_TypeDef *uart, uint32_t baud)
{

	huart->Instance = uart;
	huart->Init.BaudRate = baud;
	huart->Init.WordLength = UART_WORDLENGTH_8B;
	huart->Init.StopBits = UART_STOPBITS_1;
	huart->Init.Parity = UART_PARITY_NONE;
	huart->Init.Mode = UART_MODE_TX_RX;
	huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(huart) != HAL_OK)
	{
		Error_Handler();
	}

}
/* USER CODE END 0 */
/* USART1 init function */


void MX_USART1_UART_Init(uint32_t baud)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = baud;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}
void MX_USART2_UART_Init(uint32_t baud)
{

	huart2.Instance = USART2;
	huart2.Init.BaudRate = baud;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}

}
void MX_USART3_UART_Init(uint32_t baud)
{

	huart3.Instance = USART3;
	huart3.Init.BaudRate = baud;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_Handler();
	}
}
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
	__HAL_UART_ENABLE_IT(uartHandle, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(uartHandle, UART_IT_IDLE);
  /* USER CODE END USART1_MspInit 1 */
  }
  else if (uartHandle->Instance == USART2)
  {
	  /* USER CODE BEGIN USART2_MspInit 0 */

	  /* USER CODE END USART2_MspInit 0 */
	  /* USART2 clock enable */
	  __HAL_RCC_USART2_CLK_ENABLE();

	  /**USART2 GPIO Configuration
	  PA2     ------> USART2_TX
	  PA3     ------> USART2_RX
	  */
	  GPIO_InitStruct.Pin = GPIO_PIN_2;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_3;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /* USART2 interrupt Init */
	  HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
	  HAL_NVIC_EnableIRQ(USART2_IRQn);
	  /* USER CODE BEGIN USART2_MspInit 1 */
	  __HAL_UART_ENABLE_IT(uartHandle, UART_IT_RXNE);
	  __HAL_UART_ENABLE_IT(uartHandle, UART_IT_IDLE);
	  /* USER CODE END USART2_MspInit 1 */
  }
  else if (uartHandle->Instance == USART3)
  {
	  /* USER CODE BEGIN USART3_MspInit 0 */

	  /* USER CODE END USART3_MspInit 0 */
	  /* USART3 clock enable */
	  __HAL_RCC_USART3_CLK_ENABLE();

	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  /**USART3 GPIO Configuration
	  PB10     ------> USART3_TX
	  PB11     ------> USART3_RX
	  */
	  GPIO_InitStruct.Pin = GPIO_PIN_10;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_11;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	  /* USART3 interrupt Init */
	  HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
	  HAL_NVIC_EnableIRQ(USART3_IRQn);
	  /* USER CODE BEGIN USART3_MspInit 1 */
	  __HAL_UART_ENABLE_IT(uartHandle, UART_IT_RXNE);
	  __HAL_UART_ENABLE_IT(uartHandle, UART_IT_IDLE);
	  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
 
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if (uartHandle->Instance == USART2)
  {
	  /* USER CODE BEGIN USART2_MspDeInit 0 */

	  /* USER CODE END USART2_MspDeInit 0 */
	  /* Peripheral clock disable */
	  __HAL_RCC_USART2_CLK_DISABLE();

	  /**USART2 GPIO Configuration
	  PA2     ------> USART2_TX
	  PA3     ------> USART2_RX
	  */
	  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);

	  /* USART2 interrupt Deinit */
	  HAL_NVIC_DisableIRQ(USART2_IRQn);
	  /* USER CODE BEGIN USART2_MspDeInit 1 */

	  /* USER CODE END USART2_MspDeInit 1 */
  }

  else if (uartHandle->Instance == USART3)
  {
	  /* USER CODE BEGIN USART2_MspDeInit 0 */

	  /* USER CODE END USART2_MspDeInit 0 */
	  /* Peripheral clock disable */
	  __HAL_RCC_USART3_CLK_DISABLE();

	  /**USART2 GPIO Configuration
	  PB10     ------> USART3_TX
	  PB11     ------> USART3_RX
	  */
	  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10 | GPIO_PIN_11);

	  /* USART2 interrupt Deinit */
	  HAL_NVIC_DisableIRQ(USART3_IRQn);
	  /* USER CODE BEGIN USART2_MspDeInit 1 */

	  /* USER CODE END USART2_MspDeInit 1 */
  }
} 
//************************************
// ��������:    MY_USART_SendData
// ����ȫ��:  MY_USART_SendData
// Ȩ��:    public 
// ����ֵ:   HAL_StatusTypeDef
// ����:	�Զ��崮�ڷ��ͺ���
// ����: UART_HandleTypeDef * huart
// ����: uint8_t * pData
// ����: uint16_t Size
//************************************
HAL_StatusTypeDef MY_USART_SendData(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	uint8_t i;
	uint16_t Data = 0;
	huart->Instance->SR;
	if ((pData == NULL) || (Size == 0U))
	{
		return  HAL_ERROR;
	}
	else
	{
		for (i = 0; i < Size; i++)
		{
			Data = *pData++;
			__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);                                                      //�����־λ
			huart->Instance->DR = (Data & (uint16_t)0x01FF);                                                 //д���ݵ����ڷ��ͼĴ���
			while (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);                                       //�ȴ��������

		}
		return HAL_OK;
	}
	return 0;
}
HAL_StatusTypeDef MY_USART_SendByte(UART_HandleTypeDef *huart, uint8_t byt)
{
	uint8_t i;
	huart->Instance->SR;
	__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);                                                      //�����־λ
	huart->Instance->DR = (byt);                                                 //д���ݵ����ڷ��ͼĴ���
	while (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);                                       //�ȴ��������

	return HAL_OK;
}
HAL_StatusTypeDef MY_USART_chars(UART_HandleTypeDef *huart, uint8_t *pData)
{
	uint8_t i;
	huart->Instance->SR;
	while (*pData)
	{
		__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);                                                      //�����־λ
		huart->Instance->DR = (*pData++);                                                 //д���ݵ����ڷ��ͼĴ���
		while (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);                                       //�ȴ��������
	}


	return HAL_OK;
}

/* USER CODE BEGIN 1 */

#ifdef  DEBUG
char number2char(int num)
{
	char ch;

	switch (num)
	{
	case 0: ch = '0'; break;
	case 1: ch = '1'; break;
	case 2: ch = '2'; break;
	case 3: ch = '3'; break;
	case 4: ch = '4'; break;
	case 5: ch = '5'; break;
	case 6: ch = '6'; break;
	case 7: ch = '7'; break;
	case 8: ch = '8'; break;
	case 9: ch = '9'; break;
	case 10: ch = 'A'; break;
	case 11: ch = 'B'; break;
	case 12: ch = 'C'; break;
	case 13: ch = 'D'; break;
	case 14: ch = 'E'; break;
	case 15: ch = 'F'; break;

	default: ch = '0'; break;
	}

	return ch;
}


/*
* ������ ch ����ʮ����/ʮ������ת�����ַ�
*/
char * convert_func(char *str, unsigned int ch, int num)
{
	char *pstr = str;
	int ww, qw, bw, sw, gw;
	int hw2, lw2, hw1, lw1;

	switch (num)
	{
	case 10:
		if (ww = ch / 10000)
			*pstr++ = number2char(ww);

		if (qw = (ch / 1000 % 10))
			*pstr++ = number2char(qw);
		else if (ww)
			*pstr++ = number2char(0);

		if (bw = (ch / 100) % 10)
			*pstr++ = number2char(bw);
		else if (qw)
			*pstr++ = number2char(0);

		if (sw = (ch / 10) % 10)
			*pstr++ = number2char(sw);
		else if (bw)
			*pstr++ = number2char(0);

		if (gw = ch % 10)
			*pstr++ = number2char(gw);
		else
			*pstr++ = number2char(0);


		break;
	case 16:
		*pstr++ = '0';
		*pstr++ = 'x';

		if (hw2 = ch / 0x1000)
			*pstr++ = number2char(hw2);

		if (hw2 = (ch / 0x100) % 0x10)
			*pstr++ = number2char(hw2);
		else if (hw2)
			*pstr++ = number2char(0);


		if (hw1 = (ch / 0x10) % 0x10)
			*pstr++ = number2char(hw1);
		else
			*pstr++ = number2char(0);

		if (lw1 = ch % 0x10)
			*pstr++ = number2char(lw1);
		else
			*pstr++ = number2char(0);

		break;
	default:
		break;
	}


	return pstr;
}


int my_vsprintf(char *buf, const char *fmt, va_list args)
{
	int i;
	char * str;
	char *s;
	unsigned char ch;


	for (str = buf; *fmt; ++fmt)
	{
		if (*fmt == '\n') // ����ǻ��з���ת���ɻس���+���з�
		{
			*str++ = '\r';
			*str++ = '\n';
			continue;
		}

		if (*fmt != '%') // �����'%',�����str��������ǣ�Խ��ȡ��һ���ַ�����switch (*fmt)
		{
			*str++ = *fmt;
			continue;
		}

		fmt++;


		switch (*fmt)  // ����ֻ�ܽ���%c, %s, %x, %X, %d. �� %02d ���ֿ����Լ���չ
		{
		case 'c':
			*str++ = (unsigned char)va_arg(args, int);  // ����args��ָ�������Ȼ��argsָ����һ����������va_start(ap,fmt); ��argsָ��ڶ���������Ȼ���������ȡ����ͬ
			break;


		case 's':
			s = va_arg(args, char *);
			while (*s != '\0')
				*str++ = *s++;

			break;


		case 'd':
			str = convert_func(str, va_arg(args, int), 10);
			break;

		case 'x':
			str = convert_func(str, va_arg(args, int), 16);
			break;
		case 'f':
			str = convert_func(str, va_arg(args, float), 10);
			break;
		default:
			break;
		}
	}

	*str = '\0';

	return str - buf;
}

void sendstring(unsigned char *string,UART_HandleTypeDef *huart)
{
	while (*string)
	{
#if UART_MUTEX
		xSemaphoreTake(xSemaphore, portMAX_DELAY);
		{
			HAL_UART_Transmit(huart, string, 1, 0xff);
			string++;
		}
		xSemaphoreGive(xSemaphore);
#elif UART_MUTEX==0
		HAL_UART_Transmit(huart, string, 1, 0xff);
		string++;
#else
		HAL_UART_Transmit(huart, string, 1, 0xff);
		string++;
#endif // MUTEX_ON

		
	}
}

void Uart_printf(UART_HandleTypeDef *huart, const char *fmt, ...)
{
	va_list ap;   //va_list ��һ���ַ�ָ�룬�������Ϊָ��ǰ������һ��ָ�룬ȡ�α���ͨ�����ָ�����
	unsigned char string[128];

	va_start(ap, fmt);//��apָ��ɱ����������ĵ�һ������
	my_vsprintf(string, fmt, ap);
	va_end(ap);
	sendstring(string,huart);
}
#endif //  DEBUG

void Uartx_printf(UART_HandleTypeDef * huart,  char * fmt, ...)
{
	unsigned char buffer[128];
	uint16_t i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 100, fmt, arg_ptr);
	while (i < 99 && buffer[i])
	{
		HAL_UART_Transmit(huart, (uint8_t *)&buffer[i], 1, 0xFFFF);
		i++;
	}
	va_end(arg_ptr);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
