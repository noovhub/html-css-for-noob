/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"


#define my_usart_LIB_en
#include "my_gpio_lib_v3_2.h"
#include "my_uart_lib_v3_2.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t rx1_buf[10],go_buf, back_buf, move_buf, ad_buf[2],tx_buf[4];
uint8_t  old_buf_1 = 0,rx_irq_cnt = 0,distance1, distance2,ad_end_flag = 0, ad_cnt = 0,start_buf,ready_buf,program_buf=0;
uint32_t x_buf, shock=0;
uint16_t t_loop1=0,t_loop2=0,t_loop3=0,t_loop4=0;

char cnt=0;
//char tx_buf[4];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void my_time()
{
	t_loop1++;
	t_loop2++;
	t_loop3++;
	t_loop4++;
//----------------crush sensor
  if(t_loop1 > 5)
  {
	  shock=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
	  if(shock==1){
		  start_buf=0;
	  }
	  t_loop1=0;
  }
//----------------get uart1 transfer to uart2
  if(t_loop2 > 100)
  	  {
	  if(rx_end_flag_1==1){
	  my_printf("front ==");
	  tx_send(Rx_data_1[1],2);
	  my_printf("   back ==");
	  tx_send(Rx_data_1[2],2);
	  my_printf("   move ==");
	  tx_send(Rx_data_1[3],2);
	  tx_send('\n', 2);
	  tx_send('\r', 2);
	  rx_end_flag_1=0;
	  }
	  t_loop2=0;
  	  }

  if(t_loop3 > 20)
  {
	  if(ad_buf[0]>3000){
		  move_buf=2;
	  }
	  else if (ad_buf[0]<1000){
		  move_buf=1;
	  }
	  else if(ad_buf[1]<1000){
		  move_buf=6;
	  }
	  else if(ad_buf[1]>3000){
	  		  move_buf=4;
	  	  }
	  else{
	  		  move_buf=0;
	  	  }
	  tx_send(program_buf,1);//시동값 전달
	  tx_send(move_buf,1);//방향값 전달
	  t_loop3 =0;
  }
  if(t_loop4 > 15)
    {
	  if(Rx_data_1[3]=='1' && Rx_data_1[1]=='0'){
		  cw();
//		  my_printf("go");
	  }
	  else if(Rx_data_1[3]=='2' && Rx_data_1[2]=='0'){
		  ccw();
//		  my_printf("back");
	  }
	  else if(Rx_data_1[3]=='4'){
		  left();
//		  my_printf("left");
	  	  }
	  else if(Rx_data_1[3]=='6'){
		  right();
//		  my_printf("right");
	  	  }
	  else{
		  m_stop();
//		  my_printf("stop");
		  }
	  t_loop4=0;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  rx_irq_cnt++;
  if(rx_irq_cnt != 2)
   {
	if(huart -> Instance == USART1){
		if(Rx_data_1[0] == '\n')
		{
		rx_cnt_1 = 0;
		rx_end_flag_1=1;
		}
		else
		{
		rx_cnt_1++;
		Rx_data_1[rx_cnt_1] = Rx_data_1[0];
		}
		//__HAL_UART_CLEAR_PEFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1, (uint8_t *)Rx_data_1, 1);
		}
	else if(huart -> Instance == USART2){

//		}
		__HAL_UART_CLEAR_PEFLAG(&huart2);
		HAL_UART_Receive_IT(&huart2, (uint8_t *)Rx_data_2, 1);
		}
	else if(huart -> Instance == USART3){
		if(Rx_data_3[0] == '\n')
		{
		rx_cnt_1 = 0;
		rx_end_flag_1=1;
		}
		else
		{
		rx_cnt_1++;
		Rx_data_1[rx_cnt_1] = Rx_data_1[0];
		}
//		HAL_UART_Receive_IT(&huart3, (uint8_t *)Rx_data_3, 1);
		my_printf(Rx_data_3[0]);
	}
   }
  else {
	  rx_irq_cnt = 0;
  }
}
//**---------adc irq-------------------------------------------------------
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
 {
//if (hadc->Instance == ADC1) //ioc setting ??��?��?��??��?��?�� rank?���???????????????????? 1??��?��?��??��?��?�� 洹몃��??��?��?�� 二쇱��泥�?���????????????????
//    {
	ad_end_flag = 1;
//	ad_buf[0] = HAL_ADC_GetValue(hadc);

//   ad_cnt++;
//   if(ad_cnt ==2) ad_cnt=0;
//    }
 }
void cw()
{//go
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);

}
void ccw()
{//back
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);

}
void m_stop()
{
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);

}
void right(){

   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
}
void left(){
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */




#ifdef my_usart_LIB_en
  // uart rx int = ?��?�� ?��?��?��?�� ?��?��그�?? ?��?��?���??????????????????????????????? ?��주기 ?��?��?�� ?���???????????????????????????????
  memset(Rx_data_1, 0, 20 * sizeof(uint8_t));
  HAL_UART_Receive_IT(&huart1, (uint8_t *)Rx_data_1, 1);
  memset(Rx_data_2, 0, 20 * sizeof(uint8_t));
  HAL_UART_Receive_IT(&huart2, (uint8_t *)Rx_data_2, 1);
  memset(Rx_data_3, 0, 20 * sizeof(uint8_t));
    HAL_UART_Receive_IT(&huart2, (uint8_t *)Rx_data_3, 1);
//  HAL_UART_Transmit(&huart1, (uint8_t *)tx_data, 1, 10);


//  HAL_UART_Receive_IT(&huart3, (uint8_t *)Rx_data_3, 1);
//  HAL_UART_Receive_IT(&huart3, &rx_buf_3, 1);
#endif
	  HAL_ADC_Start_DMA(&hadc, (uint32_t)ad_buf, 2);
	  HAL_ADC_Start_IT(&hadc);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(program_buf==0){
		  program_buf=1;
		  while(program_buf==1){
	  	  ready_buf=HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	 	  if(ready_buf==0){
	 	  start_buf=1;
	 	  program_buf=0;
		  }
	 	  }
	 	 while(start_buf==1){
//	 		ready_buf=HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
//	 		if(ready_buf==1){
//	 			start_buf=1;

	     for(int k = 0; k<2; k++)
	     {
	       // ADC Start
	      HAL_ADC_Start(&hadc);
	       HAL_ADC_PollForConversion(&hadc, 50);
	       ad_buf[k] = HAL_ADC_GetValue(&hadc);

	       if(k == 0)
	       {// x
	    	   if(ad_buf[k] >= 1500 && ad_buf[k] <= 3500) {m_stop();}
	    	   else if(ad_buf[k] <= 100 && Rx_data_1[1]=='0') {cw();}// && Rx_data_1[1]=0
	    	   else if(ad_buf[k] >= 3800 && Rx_data_1[2]=='0') {ccw();}// && Rx_data_1[2]=0
	       }
	       else if(k == 1)
	       {// y
	          if(ad_buf[k] >= 1500 && ad_buf[k] <= 3500) {m_stop();}
	          else if(ad_buf[k] >= 3800) {// && Rx_data_1[1]==0
	        		  right();
	        	  }
	          else if(ad_buf[k] <= 100) {// && Rx_data_1[2]==0
	        	  left();
	          }
	       }

	  	    my_printf("adc = %d , %d \r\n",ad_buf[0], ad_buf[1]);

	     }



	  //----------------------movement
//	  if(Rx_data_1[3]==1){
//		  go_buf=1;
//		  back_buf=0;
//	  	  }
//	  else if(Rx_data_1[3]==2){
//		  go_buf=0;
//		  back_buf=1;
//	  	  }
//	  else{
//		  go_buf=0;
//		  back_buf=0;
//	  }
      /*--------------------*/
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
	 	 }
	  }
  }
}
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
