/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "usart.h"
#include "APPTooL.h"
#include "tim.h"
#include "BspConfig.h"
#include "application.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
QueueHandle_t xQueuel_sportmes;//��Ϣ����  ����tim.h������
uint8_t pstate = 0;
uint8_t iscolsesystem = 0;//
//�����˶���Ϣ����
uint16_t frend_data[3] = { 0 };


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId DataDetectionHandle;
osThreadId Uart_TFTHandle;
osThreadId DataInteractionHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void PlayCallback(uint8_t val);//�������Żص�����
Customerinfo SportInfo_Get = { 0 };   //��ȡ�����˶���Ϣ
void SendMessageToTFT(uint16_t address);

void SendDataToFrend(); //�������ݵ������
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void DataDetection_CallBack(void const * argument);
void Uart_TFT_CallBack(void const * argument);
void DataInteraction_CallBack(void const * argument);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */


/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
 
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);


}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
	/* definition and creation of DataDetection */
	taskENTER_CRITICAL();//�����ٽ���
	xQueuel_sportmes = xQueueCreate(5, sizeof(struct SportInfo*));//������Ϣ����
	if (xQueuel_sportmes == 0)
	{
		Error_Handler_t(ERROR_XQUEUE_CREAT);
	}
	osThreadDef(DataDetection, DataDetection_CallBack, 5, 0, 128);
	DataDetectionHandle = osThreadCreate(osThread(DataDetection), NULL);

	/* definition and creation of Uart_TFT */
	osThreadDef(Uart_TFT, Uart_TFT_CallBack, 1, 0, 128);
	Uart_TFTHandle = osThreadCreate(osThread(Uart_TFT), NULL);

	/* definition and creation of DataInteraction */
	osThreadDef(DataInteraction, DataInteraction_CallBack,3, 0, 128);
	DataInteractionHandle = osThreadCreate(osThread(DataInteraction), NULL);
	
  /* Infinite loop */
	Uart_printf(&huart2, "Start sub stask\r\n");
	vTaskDelete(defaultTaskHandle); //ɾ������
	taskEXIT_CRITICAL();//�Ƴ��ٽ���
	
    osDelay(100);
  
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_DataDetection_CallBack */
/**
* @brief Function implementing the DataDetection thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DataDetection_CallBack */
void DataDetection_CallBack(void const * argument)
{
  /* USER CODE BEGIN DataDetection_CallBack */
  /* Infinite loop */
	Customerinfo *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(1000); /* �������ȴ�ʱ��Ϊ200ms */
	
  for(;;)
  {
	 
	  xResult = xQueueReceive(
		  xQueuel_sportmes,                   /* ��Ϣ���о�� */
		  (void *)&ptMsg,             /* �����ȡ���ǽṹ��ĵ�ַ */
		  (TickType_t)xMaxBlockTime);/* ��������ʱ�� */

	  if (xResult == pdPASS)
	  {
		 // Uart_printf(&huart2, "xQueueData:Freq=%d, Tim=%d ,Sportcount=%d, Cal=%d  playstate=%d\r\n", ptMsg->freq, ptMsg->tim, ptMsg->count, ptMsg->hot, ptMsg->playstate);
		  SportInfo_Get.count = ptMsg->count;
		  SportInfo_Get.freq = ptMsg->freq;
		  SportInfo_Get.hot = ptMsg->hot;
		  SportInfo_Get.tim = ptMsg->tim;
		  SportInfo_Get.playstate = ptMsg->playstate;
		 SendMessageToTFT(TFT_VARIABLE_START);
		 //SendDataToFrend();//��������

		  SingleTrig(PlayCallback, SportInfo_Get.playstate, 0, 0, 1);
	  }
	 // Uartx_printf(&huart1, "thread2\r\n");
	
    osDelay(200);
  }
  /* USER CODE END DataDetection_CallBack */
}

/* USER CODE BEGIN Header_Uart_TFT_CallBack */
/**
* @brief Function implementing the Uart_TFT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Uart_TFT_CallBack */
void DataInteraction_CallBack(void const * argument)//
{
  /* USER CODE BEGIN Uart_TFT_CallBack */
  /* Infinite loop */
	static uint16_t tick = 0,loop = 0;
  for(;;)
  {
	  //Uart_printf(UART_CONNECTION, "System closed!\r\n");
	  SingleTrig(SensorCallBack, HAL_GPIO_ReadPin(Sensor_Mag_GPIO_Port, Sensor_Mag_Pin), 0, 1, 0);
	  if (iscolsesystem == 1)
	  {
		  if (tick++ > TIMER_CLOSESYSTEM / 50)    //�������5s��ر�ϵͳ
		  {
			  tick = 0;
			  SYSIO = 1;
			 // Uart_printf(UART_CONNECTION, "System closed!\r\n");
		  }
		  
	  }
	  else
		  tick = 0;
	  if (SportInfo_Get.count == 0)     //�ر�ϵͳ:��������û�˶�10s�ر�ϵͳ
	  {
		  if (loop++ > 2 * TIMER_CLOSESYSTEM / 50)
		  {
			  loop = 0;
			  SYSIO = 1;
			  //Uart_printf(UART_CONNECTION, "System closed---loop!\r\n");
		  }
	  }
	  else
		  loop = 0;
	  //Uart_printf(&huart1, "Uart2\r\n");
    osDelay(50);
  }
  /* USER CODE END Uart_TFT_CallBack */
}

/* USER CODE BEGIN Header_DataInteraction_CallBack */
/**
* @brief Function implementing the DataInteraction thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DataInteraction_CallBack */
void Uart_TFT_CallBack(void const * argument)
{
  /* USER CODE BEGIN DataInteraction_CallBack */
  /* Infinite loop */
	uint32_t tick = 0;
  for(;;)
  {
	  //Uartx_printf(&huart1, "thread4\r\n");
	  if (uart1_rec.reover == 1)
	  {
		  uart1_rec.reover = 0;
		  Uart_printf(&huart1, uart1_rec.redata); //�ȴ�������Ϣ

	  }
	  if (uart2_rec.reover == 1)
	  {
		  uart2_rec.reover = 0;
		  if (uart2_rec.redata[0]==RES_AA&uart2_rec.redata[1]==RES_55)
		  {
			  switch (uart2_rec.redata[2])
			  {
			  case RESPONSE_DATA:
				 // Uart_printf(&huart2, "receive RESPONSE_DATA\r\n"); //���յ�������
				  for (int i = 0; i < uart2_rec.redata[3] / 2; i++)
				  {
					  uint16_t temp = 0;
					  temp = uart2_rec.redata[2 * i + 4];
					  temp << 8;
					  temp = temp + uart2_rec.redata[2 * i + 5];
					  frend_data[i] = temp;
					  //HAL_UART_Transmit(&huart2, temp, 3, 0xffff);
					  // Uart_printf(&huart2, &temp); //�ȴ�������Ϣ
				  }
				  //SendDataToFrend();//��������
				  break;
			  case REQUEST_DATA:
				  //SendDataToFrend();//��������

			  default:
				  break;
			  }
			 
			  //Uart_printf(&huart2, frend_data); //�ȴ�������Ϣ
			 // HAL_UART_Transmit(&huart2, frend_data, 3, 0xfff);
		  }
		 
		 // Uart_printf(&huart2, "Uart2 is responsed\r\n"); //�ȴ�������Ϣ

	  }
	  if (tick%2==0)   //200ms����һ������
	  {
		 //SendCom(REQUEST_DATA);
		  //Uart_printf(&huart2, "xiaowenlg=%d\r\n",tick);
		  SendDataToFrend();//��������
	  }
	  tick++;
    osDelay(100);
  }
  /* USER CODE END DataInteraction_CallBack */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void vApplicationIdleHook(void)
{
	static uint16_t x = 0;
}
void PlayCallback(uint8_t val)//�������Żص�����
{
	uint8_t playarray[PLAYARRAYLENGTH] = { 0 };//������������
	uint8_t playdatalen = 0;
	TFT_Beep(2);// bi-bi ����
	//Uart_printf(&huart1, "playstate=%d   Sound is beginning...!\r\n",SportInfo_Get.playstate);
	playdatalen = GetPlayData(SportInfo_Get.count, SportInfo_Get.tim, (double)SportInfo_Get.hot, playarray);
	//���Դ�ӡ����
	for (int i = 0; i < playdatalen; i++)
	{
	Uart_printf(&huart2, "Data%d:%x\r\n", i, playarray[i]);
	}
	pstate = WTN6040_PlayArray(playdatalen, playarray);
	iscolsesystem = pstate;
	//Uart_printf(&huart2, "pstate is %d\r\n",pstate);
}

//��TFT����������
void SendMessageToTFT(uint16_t address)
{
	uint16_t TFT_SendArray[15] = { 0 };
	TFT_SendArray[0] = (uint16_t)(SportInfo_Get.freq*(PERIMETER))*60; //�ٶ�=�����ܳ�*ת/��
	TFT_SendArray[1] = 0;	//����-----------����
	TFT_SendArray[2] = SportInfo_Get.tim / 3600;			//ʱ
	TFT_SendArray[3] = SportInfo_Get.tim % 3600 / 60;		//��
	TFT_SendArray[4] = SportInfo_Get.tim % 60;				//��
	TFT_SendArray[5] = SportInfo_Get.count * PERIMETER;		//·��=�ܳ�*Ȧ��
	TFT_SendArray[6] = SportInfo_Get.hot;					//����
	//
	TFT_SendArray[8] = SportInfo_Get.freq;
	TFT_SendArray[9] = ADC_GetValue(&hadc1, 10);			//��ص���
	//������Ϣ
	TFT_SendArray[10] = frend_data[0]/360;
	TFT_SendArray[11] = frend_data[0]%360/60;
	TFT_SendArray[12] = frend_data[0]%60;
	TFT_SendArray[13] = frend_data[1];
	TFT_SendArray[14] = frend_data[2];
	write_multiple_variable_store_82(address, 15, TFT_SendArray);
	write_register_80_1byte(TFT_BUTTON, 1);
}

void SendDataToFrend() //�������ݵ������
{
	//Uart_printf(UART_CONNECTION, "SendMeData is beginning\r\n");
	uint8_t sendarray[10] = { 0 };
	//֡ͷ
	sendarray[0] = RES_AA;
	sendarray[1] = RES_55;
	//����
	sendarray[2] = RESPONSE_DATA;
	//���ݳ���
	sendarray[3] = 6; 
	//ʱ��
	sendarray[4] = SportInfo_Get.tim>>8;			//ʱ���8λ;
	sendarray[5] = SportInfo_Get.tim & 0x00ff;     //ʱ���8λ
	//�ٶ�
	sendarray[6] = ((uint16_t)(SportInfo_Get.freq*(PERIMETER)) * 60) >> 8;
	sendarray[7] = ((uint16_t)(SportInfo_Get.freq*(PERIMETER)) * 60) & 0x00ff;
	//·��
	sendarray[8] = ((uint16_t)(SportInfo_Get.count * PERIMETER)) >> 8;
	sendarray[9] = ((uint16_t)(SportInfo_Get.count * PERIMETER)) & 0x00ff;
	
	HAL_UART_Transmit(UART_CONNECTION, sendarray, 10, 0xffff);


}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
