
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main core of DeepStomp program
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are  IN BETWEEN comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Author: HAMURO
  * COPYRIGHT(c) 2018 DEEPTRONIC.COM
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of DEEPTRONIC.COM nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "deepstomp.h"
#include "dstlib_leveldetector.h"
#include "dstlib_dcremover.h"

//ENUMERATED CONSTANTS
enum {dsdisplaytype_bar, dsdisplaytype_dot, dsdisplaytype_blinkdot};
enum {editexit_expired,editexit_click,editexit_longpress};

//ERROR BLINK CODES
#define ERR_TOO_MANY_CAL_TRIALS	2
#define ERR_TOO_LOW_RESISTOR	3
#define ERR_TOO_HIGH_RESISTOR	4
#define ERR_STORAGE_ALLOCATION	5
#define ERR_MODULE_SETUP		6

//PWM
#define LO_PWM_Pin GPIO_PIN_3
#define LO_PWM_GPIO_Port GPIOB
#define HI_PWM_Pin GPIO_PIN_6
#define HI_PWM_GPIO_Port GPIOB

//ROTARY
#define ROT_SW_Pin GPIO_PIN_1
#define ROT_SW_GPIO_Port GPIOB
#define ROT_PH_0_Pin GPIO_PIN_10
#define ROT_PH_0_GPIO_Port GPIOB
#define ROT_PH_1_Pin GPIO_PIN_11
#define ROT_PH_1_GPIO_Port GPIOB

//PARAMS LED
#define LED_INDI_0_Pin GPIO_PIN_7
#define LED_INDI_0_GPIO_Port GPIOB
#define LED_INDI_1_Pin GPIO_PIN_15
#define LED_INDI_1_GPIO_Port GPIOB
#define LED_INDI_2_Pin GPIO_PIN_8
#define LED_INDI_2_GPIO_Port GPIOA
#define LED_INDI_3_Pin GPIO_PIN_4
#define LED_INDI_3_GPIO_Port GPIOB
#define LED_INDI_4_Pin GPIO_PIN_12
#define LED_INDI_4_GPIO_Port GPIOA
#define LED_INDI_5_Pin GPIO_PIN_15
#define LED_INDI_5_GPIO_Port GPIOA

//LED BAR
#define LED_BAR_0_Pin GPIO_PIN_15
#define LED_BAR_0_GPIO_Port GPIOC
#define LED_BAR_1_Pin GPIO_PIN_14
#define LED_BAR_1_GPIO_Port GPIOC
#define LED_BAR_2_Pin GPIO_PIN_13
#define LED_BAR_2_GPIO_Port GPIOC
#define LED_BAR_3_Pin GPIO_PIN_9
#define LED_BAR_3_GPIO_Port GPIOB
#define LED_BAR_4_Pin GPIO_PIN_8
#define LED_BAR_4_GPIO_Port GPIOB
#define LED_BAR_5_Pin GPIO_PIN_3
#define LED_BAR_5_GPIO_Port GPIOB
#define LED_BAR_6_Pin GPIO_PIN_11
#define LED_BAR_6_GPIO_Port GPIOA
#define LED_BAR_7_Pin GPIO_PIN_14
#define LED_BAR_7_GPIO_Port GPIOB
#define LED_BAR_8_Pin GPIO_PIN_12
#define LED_BAR_8_GPIO_Port GPIOB
#define LED_BAR_9_Pin GPIO_PIN_13
#define LED_BAR_9_GPIO_Port GPIOB


typedef struct
{
	int32_t storagecounter;
	int8_t params[6];
	int8_t presetbuffer[800];	//4 params * 10 modules * 20 presets = 800
} storagestruct;

//preset data structure element
typedef struct
{
	int8_t control[4];
} controlarray;

//preset data structure element
typedef struct
{
	controlarray module[10];
} modulearray;

typedef struct
{
	modulearray preset[19];
} preset;

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//serial display tx/rx variables

#ifdef USEDEBUGMONITOR
	char txbuff[533];
	char rxbuff[40];
	int rxbuffindex = 0;
	int rxbuffsize = 40;
	unsigned char rxchar;
	char* display;
	int displaysize = 530;
	char linebuffer[51];
	volatile uint8_t serialtxcomplete = 1;
#ifdef USELEVELPROBE
	dst_leveldetector_handle levelprobe1;
	dst_leveldetector_handle levelprobe2;
	q15_t levelp1 = 0;
	q15_t levelp2 = 0;
#endif	//USELEVELPROBE
#endif	//USEDEBUGMONITOR

	//ADC calibration
	int CALIBRATIONMODE = 0;
	uint32_t allchannelsum = 0;
	uint32_t onechannelsum = 0;

	//double pwm calibration variables
	uint16_t histep=0;
	uint16_t lostep=0;
	int calibcount=0;

	//audio input-output variables
	q15_t INPUTLEVEL;	//the detected input level before any processing by effect application
	uint16_t adcbuffer[16];
	q15_t insample=0;
	q15_t outsample=0;
	int adccounter=0;
	uint16_t cpucount=0;
	q15_t paramlevel;
	dst_leveldetector_handle leveldetector;
	dst_dcremover_handle dcremover;
	uint32_t RUNTIME=0;

	//parameter edit control
	//handle of the edited module (PARAMS[0]-PARAMS[3] in multi-effect mode
	dsthandle SUBMODULE;
	dstparam* PARAMS[6];	//The attached parameters (to be rotated and edited)
	int8_t EDITEXITSTATUS;	//the status of PARAMS edit mode exit
	static int8_t paramindex;
	static int8_t presettostore;
	static paramchangecall ROTATE_CALLBACK = 0;
	static uint32_t ROTATEDMODULE = 0;
	dstparam paramdump;

	//parameter saving variables
	//assume a 64k flash memory, use the last four pages  for params and presets storage (page address 0x800f00 to 0x800fc00)
	uint32_t* storagepageaddress[] = {(uint32_t*)0x800f000,(uint32_t*)0x800f400,(uint32_t*)0x800f800,(uint32_t*)0x800fc00};
	storagestruct* storagebuffer;
	int activepageindex;
	int32_t storagecounter;
	int datasetlength;
	int datasetindex;
	int8_t requeststorageupdate=0;
	preset* PRESETBUFFER;	//preset buffer pointer

	//led indicator and led bar constants
const GPIO_TypeDef* ledbarport[] = {LED_BAR_0_GPIO_Port,LED_BAR_1_GPIO_Port,LED_BAR_2_GPIO_Port,
		LED_BAR_3_GPIO_Port,LED_BAR_4_GPIO_Port,LED_BAR_5_GPIO_Port,
		LED_BAR_6_GPIO_Port,LED_BAR_7_GPIO_Port,LED_BAR_8_GPIO_Port,
		LED_BAR_9_GPIO_Port};
const uint16_t ledbarpin[] = {LED_BAR_0_Pin,LED_BAR_1_Pin,LED_BAR_2_Pin,LED_BAR_3_Pin,
		LED_BAR_4_Pin,LED_BAR_5_Pin,LED_BAR_6_Pin,LED_BAR_7_Pin,
		LED_BAR_8_Pin,LED_BAR_9_Pin};
const GPIO_TypeDef* ledindiport[] = {LED_INDI_0_GPIO_Port,LED_INDI_1_GPIO_Port,
		LED_INDI_2_GPIO_Port,LED_INDI_3_GPIO_Port,LED_INDI_4_GPIO_Port,LED_INDI_5_GPIO_Port};
const uint16_t ledindipin[] = {LED_INDI_0_Pin,LED_INDI_1_Pin,LED_INDI_2_Pin,
		LED_INDI_3_Pin,LED_INDI_4_Pin,LED_INDI_5_Pin};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void errorhalt(int blinkcount);
static void errorblink(int blinkcount);
static uint16_t generatecalsignal();
static void runcalibrationdisplay();

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

static void runcalibrationdisplay()
{

}

void loadfromstorage()
{
	//read the flash memory
	uint32_t* bufferptr = (uint32_t*) storagebuffer;

	storagecounter = 0;
	int maxindex = 0;
	for(int i=0;i<4;i++)
	{
		int32_t* counterptr = (int32_t*)storagepageaddress[i];
		if(counterptr[0] > storagecounter)
		{
			storagecounter = counterptr[0];
			maxindex = i;
		}
	}
	activepageindex = maxindex;

	//load from maxindex page
	uint32_t* storageptr = storagepageaddress[maxindex];
	for(uint16_t i=0;i<256;i++)
	{
		bufferptr[i] = storageptr[i];
	}
}

void writetostorage()
{
	requeststorageupdate=1;
}

uint32_t storagewrite_currtick = 0;
uint32_t storagewrite_prevtick = 0;
int storagewriteindex;
enum {writestate_wait,writestate_selectpage,writestate_erase,writestate_program};
uint8_t writestate = writestate_wait;
static void runstoragewrite()
{
	storagewrite_currtick = HAL_GetTick();
	if(storagewrite_currtick-storagewrite_prevtick<10)
		return;
	storagewrite_prevtick = storagewrite_currtick;

	switch(writestate)
	{
		case writestate_wait:
		{
			if(requeststorageupdate)
			{
				requeststorageupdate=0;
				storagewriteindex = 0;
				writestate= writestate_selectpage;
			}
			break;
		}
		case writestate_selectpage:
		{
			//unlock the flash
			HAL_FLASH_Unlock();

			uint32_t* data = (uint32_t*) storagebuffer;
			storagecounter++;
			data[0] = storagecounter;

			//select the next page for the next storage to increase total storage durability
			activepageindex++;
			if(activepageindex>3)
				activepageindex = 0;
			writestate = writestate_erase;
			break;
		}
		case writestate_erase:
		{
			//erase the page
			FLASH_EraseInitTypeDef eraseinit;
			eraseinit.PageAddress = (uint32_t)storagepageaddress[activepageindex];
			eraseinit.Banks = FLASH_BANK_1;
			eraseinit.NbPages = 1;
			eraseinit.TypeErase = FLASH_TYPEERASE_PAGES;
			uint32_t pageerror;
			HAL_FLASHEx_Erase(&eraseinit,&pageerror);
			writestate = writestate_program;
			break;
		}
		case writestate_program:
		{
			//write the page
			uint32_t* data = (uint32_t*) storagebuffer;
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t) &(storagepageaddress[activepageindex][storagewriteindex]),(uint64_t)data[storagewriteindex]);

			//increment the index and lock the page if done
			storagewriteindex++;
			if(storagewriteindex==256)
			{
				HAL_FLASH_Lock();
				writestate = writestate_wait;
			}
			break;
		}
	}

}

static void errorhalt(int blinkcount)
{
	//turn off all LEDs
	for(int i=0;i<6;i++)
	{
		HAL_GPIO_WritePin((GPIO_TypeDef*)ledbarport[i],ledindipin[i],0);
	}
	for(int i=0; i<10; i++)
	{
		HAL_GPIO_WritePin((GPIO_TypeDef*)ledbarport[i],ledbarpin[i],0);
	}

	//long 2 seconds fast blinks first
	for(int i=0;i<20;i++)
	{
		HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[0],ledindipin[0],1);
		HAL_Delay(50);
		HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[0],ledindipin[0],0);
		HAL_Delay(50);
	}

	//period
	HAL_Delay(1000);

	//then blink forever
	while(1)
	{
		for(int i=0;i<blinkcount;i++)
		{
			HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[0],ledindipin[0],1);
			HAL_Delay(250);
			HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[0],ledindipin[0],0);
			HAL_Delay(250);
		}
		HAL_Delay(500);
	}
}

static void errorblink(int blinkcount)
{
	//turn off all LEDs
	for(int i=0;i<6;i++)
	{
		HAL_GPIO_WritePin((GPIO_TypeDef*)ledbarport[i],ledindipin[i],0);
	}
	for(int i=0; i<10; i++)
	{
		HAL_GPIO_WritePin((GPIO_TypeDef*)ledbarport[i],ledbarpin[i],0);
	}

	//long 2 seconds fast blinks first
	for(int i=0;i<20;i++)
	{
		HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[0],ledindipin[0],1);
		HAL_Delay(50);
		HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[0],ledindipin[0],0);
		HAL_Delay(50);
	}

	//period
	HAL_Delay(1000);

	//then blinks the error code
	for(int i=0;i<blinkcount;i++)
	{
		HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[0],ledindipin[0],1);
		HAL_Delay(500);
		HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[0],ledindipin[0],0);
		HAL_Delay(500);
	}
}

//digital anti aliasing filter at 20kHz cut-off frequency
#define antialiasingGAIN   1259
static q31_t xv[3], yv[3];
static void antialiasing(q15_t *input, q15_t* output)
{
	xv[0] = xv[1];
	xv[1] = xv[2];
	xv[2] = ((q31_t)(*input)<<10) / antialiasingGAIN;
	yv[0] = yv[1];
	yv[1] = yv[2];
	yv[2] =   (((xv[0] + xv[2]) + 2 * xv[1])<<10)
				 + ( -678 * yv[0]) + ( -1629 * yv[1]);
	yv[2] = yv[2] >> 10;
	*output = (q15_t) yv[2];
}

static void initantialiasing()
{
	for(int i=0;i<3;i++)
	{
		xv[i]=0;
		yv[i]=0;
	}
}

GPIO_PinState  readrotsw()
{
	return HAL_GPIO_ReadPin(ROT_SW_GPIO_Port,ROT_SW_Pin);
}

GPIO_PinState  readrotp0()
{
	return HAL_GPIO_ReadPin(ROT_PH_0_GPIO_Port,ROT_PH_0_Pin);
}

GPIO_PinState  readrotp1()
{
	return HAL_GPIO_ReadPin(ROT_PH_1_GPIO_Port,ROT_PH_1_Pin);
}

int8_t rotscancount = 0;
int8_t rotsw = 1;
int8_t rotp0 = 1;
int8_t rotp1 = 1;

int8_t rotsw_sum = 0;
int8_t rotp0_sum = 0;
int8_t rotp1_sum = 0;
uint32_t rotscan_currtick = 0;
uint32_t rotscan_prevtick = 0;
void rotscan()
{
	rotscan_currtick = HAL_GetTick();
	if(rotscan_currtick-rotscan_prevtick<1)
		return;
	rotscan_prevtick = rotscan_currtick;
	rotsw_sum += (int8_t)readrotsw();
	rotp0_sum += (int8_t)readrotp0();
	rotp1_sum += (int8_t)readrotp1();
	rotscancount++;
	if(rotscancount>9)
	{
		rotscancount = 0;
		if(rotsw_sum>5) rotsw = 1; else rotsw = 0;
		if(rotp0_sum>5) rotp0 = 1; else rotp0 = 0;
		if(rotp1_sum>5) rotp1 = 1; else rotp1 = 0;
		rotsw_sum = 0;
		rotp0_sum = 0;
		rotp1_sum = 0;
	}
}

static uint8_t prevrotp0,prevrotsw;
enum {rotencode_idle,rotencode_startwaittrigger,rotencode_waittrigger,rotencode_startrun,rotencode_run};
static uint8_t rotencode_state = rotencode_idle;
static uint32_t rotencode_currtick = 0;
static uint32_t rotencode_prevtick = 0;
uint8_t rotwaitexit = 0;
static int8_t* rotatedindex = 0;
static int8_t rotatedindexmax = 0;
static int8_t rotatetrigger = 0;
enum {rotateexit_notyet,rotateexit_click,rotateexit_expired,
	rotateexit_hold,rotateexit_longpress};
static int8_t rotateexit = rotateexit_notyet;
static int rotateidle;
static int rotateholdcounter;
static int8_t rotate_editparam = 0;

void rotencode()
{
	rotencode_currtick = HAL_GetTick();
	if(rotencode_currtick-rotencode_prevtick<10)
		return;
	rotencode_prevtick = rotencode_currtick;

	if(rotencode_state == rotencode_startwaittrigger)
	{
		rotatetrigger = 0;
		if(rotsw && rotp0)
			rotencode_state = rotencode_waittrigger;
	}
	else if(rotencode_state == rotencode_waittrigger)
	{
		if((rotp0 && (!prevrotp0))||(rotsw && (!prevrotsw)))
		{
			rotatetrigger = 1;
			rotencode_state = rotencode_idle;
		}
	}
	else if(rotencode_state == rotencode_startrun)
	{
		rotateexit = rotateexit_notyet;
		if(rotsw && rotp0)
		{
			rotencode_state = rotencode_run;
			rotateidle = 0;
		    rotateholdcounter = 0;
		}
	}
	else if(rotencode_state == rotencode_run)
	{
		if(rotateexit == rotateexit_notyet)
		{
			if(rotsw)
			{
				if((rotp0==0)&& prevrotp0)
				{
					int8_t newval = *rotatedindex;

					if(rotp1)
					{
						newval++;
						if(newval > rotatedindexmax)
							newval = rotatedindexmax;
					}
					else
					{
						newval --;
						if(newval < 0)
							newval = 0;
					}
					rotateidle = 0;

					if(rotate_editparam)	//editing param's value
					{
						if(ROTATE_CALLBACK)
						{
							ROTATE_CALLBACK(ROTATEDMODULE,newval);
						}
						else *rotatedindex = newval;
					}
					else // selecting params
					{
						// if PARAMS[newval] has stepcount > 1 then rotatedindex = newval
						// else then increment/decrement newval until PARAMS[newval] has stepcount > 1
						// or reaching the boundary (0-5)
						if(PARAMS[newval]->stepcount > 1)
						{
							*rotatedindex = newval;
						}
						else
						{
							int direction = 1;
							if(newval < *rotatedindex)
								direction = -1;
							while(1)
							{
								if((newval == 0)||(newval == 5))
									break;

								newval = newval + direction;
								if(PARAMS[newval]->stepcount > 1)
								{
									(*rotatedindex) = newval;
									break;
								}
							}
						}

						ROTATEDMODULE = MAINMODULE;
						if(((dstinterface*)MAINMODULE)->multimode)
							if((*rotatedindex) < 4) ROTATEDMODULE = SUBMODULE;
					}

				}
				else	//no rotation
				{
					rotateidle++;
					if(rotateidle > 400)
						rotateexit = rotateexit_expired;
				}
			}
			else //rotary switch is pressed
			{
				rotateexit = rotateexit_hold;
			}

		}
		else if(rotateexit == rotateexit_hold)
		{
			if(rotsw)
			{
				if(rotateholdcounter>100)	// 1 second
				{
					rotateexit = rotateexit_longpress;
				}
				else rotateexit = rotateexit_click;
			}
			else
			{
				rotateholdcounter++;
				if(rotateholdcounter>100)
					rotateexit = rotateexit_longpress;
			}
		}
	}
	prevrotp0 = rotp0;
	prevrotsw = rotsw;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	 TIM1->CNT = 0;
	 adccounter++;

	 if(CALIBRATIONMODE)	//adc calibration mode
	 {
		 int32_t temp=0;
		 for(int i=8; i<16;i++)
		 {
			 temp += adcbuffer[i];
		 }

		 //record the sum of both single and multi-channel
		 onechannelsum += (((uint32_t)adcbuffer[8]) << 3);
		 allchannelsum += temp;

		 //run calibration display
		 runcalibrationdisplay();

		 //write calibration signal to the output
		 temp = (int32_t) generatecalsignal();
		 TIM4->CCR1 = (0xff00 & temp)>>8;
		 TIM2->CCR2 = 0xff & temp;
		 cpucount = TIM1->CNT;
	 }
	 else	//normal mode
	 {
		 int32_t temp=0;
		 for(int i=8; i<16;i++)
		 {
			 temp += adcbuffer[i];
		 }
		 //convert dc to ac signal
		 insample = (temp<<1)-32767;

		 //preprocessing the audio sample
		 q15_t nodcsignal;
		 dst_dcremover_process(dcremover,&insample,&nodcsignal);
		 dst_leveldetector_process(leveldetector,&nodcsignal,&INPUTLEVEL);
	 #ifdef USELEVELPROBE
		 dst_leveldetector_process(levelprobe1,&LEVELPROBE1,&levelp1);
		 dst_leveldetector_process(levelprobe2,&LEVELPROBE2,&levelp2);
	 #endif
		 q15_t noaliasing;
		 antialiasing(&nodcsignal,&noaliasing);

		 //processing the audio sample
		 deepstomp_process(&noaliasing,&outsample);

		 //write the sample to the output
		 temp = (int32_t) outsample + 32767; //convert outsample to dc signal
		 TIM4->CCR1 = (0xff00 & temp)>>8;
		 TIM2->CCR2 = 0xff & temp;
		 cpucount = TIM1->CNT;
	 }

}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	 TIM1->CNT = 0;
	 adccounter++;
	 if(CALIBRATIONMODE)	//adc calibration mode
	 {
		 int32_t temp=0;
		 for(int i=0; i<8;i++)
		 {
			 temp += adcbuffer[i];
		 }

		 //record the sum of both single and multi-channel
		 onechannelsum += (((uint32_t)adcbuffer[0]) << 3);
		 allchannelsum += temp;

		 //run calibration display
		 runcalibrationdisplay();

		 //write calibration signal to the output
		 temp = (int32_t) generatecalsignal();
		 TIM4->CCR1 = (0xff00 & temp)>>8;
		 TIM2->CCR2 = 0xff & temp;
		 cpucount = TIM1->CNT;
	 }
	 else	//normal mode
	 {
		 int32_t temp=0;
		 for(int i=0; i<8;i++)
		 {
			 temp += adcbuffer[i];
		 }
		 //convert dc to ac signal
		 insample = (temp<<1)-32767;

		 //pre-processing the audio sample

		 q15_t nodcsignal;
		 dst_dcremover_process(dcremover,&insample,&nodcsignal);
		 dst_leveldetector_process(leveldetector,&nodcsignal,&INPUTLEVEL);
	#ifdef USELEVELPROBE
		 dst_leveldetector_process(levelprobe1,&LEVELPROBE1,&levelp1);
		 dst_leveldetector_process(levelprobe2,&LEVELPROBE2,&levelp2);
	#endif
		 q15_t noaliasing;
		 antialiasing(&nodcsignal,&noaliasing);

		 //processing the audio sample
		 deepstomp_process(&noaliasing,&outsample);

		 //write the sample to the output
		 temp = (int32_t) outsample + 32767;	//convert outsample to dc signal
		 TIM4->CCR1 = (0xff00 & temp)>>8;
		 TIM2->CCR2 = 0xff & temp;
		 cpucount = TIM1->CNT;
	 }
}

#ifdef USEDEBUGMONITOR

	void HAL_UART_TxCpltCallback (UART_HandleTypeDef * huart)
	{
		if(huart == &huart1)
		{
			serialtxcomplete = 1;
		}

	}

	void serialdisplay(char* string, int line)
	{
		if(line>9)
			return;
		char* dest = &display[53*line];
		strncpy(dest,string,50);
		int len = strlen(string);
		if(len>50)
			len=50;
		for(int i=len;i<50;i++)
			dest[i]=' ';	//overwrite the old data with space
	}

	void createleveprobe(char* buff,q15_t level,char* label)
	{
		strncpy(buff,label,2);
		buff[2]=':';
		uint8_t x = level/819;
		for(uint8_t i=3;i<43;i++)
		{
			if((i-3) > x)
				buff[i]= ' ';
			else buff[i]= '#';
		}
		snprintf(&buff[43],8," %5d",(int)level);
	}

	void serialdisplayinit()
	{
		  txbuff[0] = 27;
		  txbuff[1] ='[';
		  txbuff[2] = 'H';
		  display = &txbuff[3];
		  for(int i=50;i<530;i++)
		  {
			  if((i%53)==0)
			  {
				  display[i-2]='\n';
				  display[i-1]='\r';
			  }
		  }
		  serialdisplay("DEEPSTOMP PEDAL PROCESSOR DEBUG MONITOR",0);
	}

	uint32_t displaydebug_prevtick = 0;
	uint32_t displaydebug_currtick = 0;
	void displaydebuginfo()
	{
		displaydebug_currtick = HAL_GetTick();
		  if(displaydebug_currtick-displaydebug_prevtick<100)
			  return;

		  displaydebug_prevtick = displaydebug_currtick;
		  int sr = adccounter * 10;
		  adccounter = 0;
		  snprintf(linebuffer,51,"Run: %d seconds, sfreq: %d",(int)displaydebug_currtick/1000,sr);
		  serialdisplay(linebuffer,1);
		  snprintf(linebuffer,51,"CAL (Hi: %d, Low: %d, ARR2: %d, Trial: %d)",(int)histep,(int)lostep,(int)TIM2->ARR,(int)calibcount);
		  serialdisplay(linebuffer,2);
		  snprintf(linebuffer,51,"Module CPU: %d%%, Storage Cycle: %ld",100*cpucount/1632, storagecounter);
		  serialdisplay(linebuffer,3);
		#ifndef USELEVELPROBE
		  snprintf(linebuffer,51,"DTXT: %s",DEBUGTEXT);
		  serialdisplay(linebuffer,4);
		  snprintf(linebuffer,51,"DVAR: %d, %d, %d, %d, %d, %d",
				  DEBUGVARS[0],DEBUGVARS[1],DEBUGVARS[2],DEBUGVARS[3],DEBUGVARS[4],DEBUGVARS[5]);
		  serialdisplay(linebuffer,5);
		#endif	//not USELEVELPROBE
		#ifdef USELEVELPROBE
		  createleveprobe(linebuffer,INPUTLEVEL,"IN");
		  serialdisplay(linebuffer,4);
		  createleveprobe(linebuffer,levelp1,"P1");
		  serialdisplay(linebuffer,5);
		  createleveprobe(linebuffer,levelp2,"P2");
		  serialdisplay(linebuffer,6);
		  snprintf(linebuffer,51,"DTXT: %s",DEBUGTEXT);
		  serialdisplay(linebuffer,7);
		  snprintf(linebuffer,51,"DVAR: %d, %d, %d, %d, %d, %d",
				  DEBUGVARS[0],DEBUGVARS[1],DEBUGVARS[2],DEBUGVARS[3],DEBUGVARS[4],DEBUGVARS[5]);
		  serialdisplay(linebuffer,8);
		#endif	//USELEVELPROBE

		  //return if DMA transfer is still working
		  if(serialtxcomplete == 0)
			  return;
		  //reset the flag and start DMA transmit
		  serialtxcomplete = 0;
		  HAL_UART_Transmit_DMA(&huart1,(uint8_t*)txbuff,512);
	}
#endif

uint16_t readcalinput()
{
	uint32_t temp=0;
	for(int i=0; i<32; i++)
	{
		HAL_ADC_Start(&hadc2);
		if(HAL_ADC_PollForConversion(&hadc2,10)==HAL_OK)
			temp += HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
	}
	return temp>>5;
}

void calibratepwm()
{
		TIM4->ARR=256;
		TIM2->ARR=256;
		while(1)
		{
			  calibcount++;
			  if(calibcount>50)
				  break;
			  TIM4->CCR1 = 1;
			  TIM2->CCR2 = 0;
			  HAL_Delay(1);
			  uint16_t prevlevel = readcalinput();
			  histep=0;
			  lostep=0;
			  for(int a=1; a<14; a++)
			  {
				  TIM4->CCR1 = a+1;
				  TIM2->CCR2 = 0;
				  HAL_Delay(1);
				  uint16_t currentlevel = readcalinput();
				  histep += currentlevel-prevlevel;

				  TIM4->CCR1 = a;
				  TIM2->CCR2 = 0xff;
				  HAL_Delay(1);
				  lostep += readcalinput()-prevlevel;

				  prevlevel = currentlevel;
			  }

			  histep = histep/13;
			  lostep = lostep/13;

			  //predict lostep update if the difference is too large
			  if((lostep-histep)>10)
			  {
				  float newarr = (float)lostep/(float)histep * (float)TIM2->ARR;
				  TIM2->ARR = round(newarr);
			  }

			  if(lostep>=histep)
				  TIM2->ARR = TIM2->ARR+1;
			  else if(lostep < (histep-1))
				  TIM2->ARR = TIM2->ARR-1;
			  else break;
		}
}

uint8_t barstate[10];
enum {barchannel_input,barchannel_param,barchannel_presettostore,barchannel_button};
uint8_t bardisplaychannel = barchannel_input;

void displaybar(int8_t level, uint8_t levelcount, uint8_t type)
{
	if(levelcount<11)
	{
		if(type==dsdisplaytype_bar)
			for(int i=0;i<10;i++)
			{
				if(i>level)
					barstate[i]=0;
				else barstate[i]=2;
			}
		if(type==dsdisplaytype_blinkdot)
			for(int i=0;i<10;i++)
			{
				if(i==level)
					barstate[i]=1;
				else barstate[i]=0;
			}
		else	//dot type
			for(int i=0;i<10;i++)
			{
				if(i==level)
					barstate[i]=2;
				else barstate[i]=0;
			}
	}
	else if(levelcount<20)
	{
		uint8_t halflevel = level/2;
		for(int i=0;i<10;i++)
		{
			if(type==dsdisplaytype_bar)
			{
				for(int i=0;i<10;i++)
				{
					if(i <= halflevel)
						barstate[i]=2;
					else if((halflevel+(level%2))==i)
					{
						barstate[i]=1;
					}
					else barstate[i]=0;
				}
			}
			if(type==dsdisplaytype_blinkdot)	//blinkdot type
			{
				for(int i=0;i<10;i++)
				{
					if(i < halflevel)
						barstate[i]=0;
					else if(i == halflevel)
						barstate[i]=1;
					else if((halflevel+(level%2))==i)
					{
						barstate[i]=1;
					}
					else barstate[i]=0;
				}
			}
			else	//dot type
			{
				for(int i=0;i<10;i++)
				{
					if(i < halflevel)
						barstate[i]=0;
					else if(i == halflevel)
						barstate[i]=2;
					else if((halflevel+(level%2))==i)
					{
						barstate[i]=2;
					}
					else barstate[i]=0;
				}
			}
		}
	}
	else	//levelcount 20-32, any type will be displayed in dot mode
	{
		uint8_t quarterlevel = level/4;
		uint8_t remainder = level%4;
		for(int i=0;i<10;i++)
		{
			if(i==quarterlevel)
			{
				if((remainder==0||(remainder==1)||remainder==2))
					barstate[i]=2;
				else if(remainder==3)
					barstate[i]=1;
				else barstate[i]=0;
			}
			else if(i==(quarterlevel+1))
			{
				if((remainder==2)||(remainder==3))
					barstate[i]=2;
				else if(remainder==1)
					barstate[i]=1;
				else barstate[i]=0;
			}
			else barstate[i]=0;
		}
	}
}

uint8_t animatebar_state = 0;
uint32_t animatebar_prevtick = 0;
uint32_t animatebar_currtick = 0;
void animatebar()
{
	animatebar_currtick = HAL_GetTick();
	  if(animatebar_currtick-animatebar_prevtick<100)
		  return;
	  animatebar_prevtick = animatebar_currtick;
	  RUNTIME = animatebar_currtick/1000;

	  //select channel
	  if(bardisplaychannel == barchannel_input)
	  {
		  displaybar(INPUTLEVEL/3276,10,dsdisplaytype_bar);
	  }
	  else if(bardisplaychannel == barchannel_param)
	  {
		  int8_t templevel = PARAMS[paramindex]->value;
		  displaybar(templevel, PARAMS[paramindex]->stepcount,dsdisplaytype_dot);
	  }
	  else if(bardisplaychannel == barchannel_presettostore)
	  {
		  int8_t templevel = presettostore;
		  displaybar(templevel, 19 ,dsdisplaytype_blinkdot);
	  }
	  else if(bardisplaychannel == barchannel_button)
	  {
		  //?
	  }

	  for(int i=0;i<10;i++)
	  {
		  if(barstate[i]==0)
		  {
			  HAL_GPIO_WritePin((GPIO_TypeDef*)ledbarport[i],ledbarpin[i],0);
		  }
		  else if(barstate[i]==1)
		  {
			  if(animatebar_state)
				  HAL_GPIO_WritePin((GPIO_TypeDef*)ledbarport[i],ledbarpin[i],1);
			  else HAL_GPIO_WritePin((GPIO_TypeDef*)ledbarport[i],ledbarpin[i],0);
		  }
		  else HAL_GPIO_WritePin((GPIO_TypeDef*)ledbarport[i],ledbarpin[i],1);
	  }
	  if(animatebar_state)
		  animatebar_state = 0;
	  else animatebar_state = 1;
}

//led indicator states buffer
uint8_t ledindibuttonstate[] = {0,0,0,0,0,0};	//when activated by toggle or radio button

uint8_t animateindi_state = 0;
uint32_t animateindi_prevtick = 0;
uint32_t animateindi_currtick = 0;

enum {blinkselect,blinkedit,blinknone};
int8_t indiblinktype = blinkselect;
uint8_t indiblinkindex = 0;
int8_t indiblinkoncounter = 0;
int8_t blinkon = 1;
void indiblink()
{
	animateindi_currtick = HAL_GetTick();
	  if(animateindi_currtick-animateindi_prevtick<100)
		  return;
	  animateindi_prevtick = animateindi_currtick;

	  if(indiblinktype==blinknone)
	  {
		  indiblinkoncounter = 0;
		  for(int i = 0;i<6;i++)
		  {
			  HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[i],ledindipin[i],ledindibuttonstate[i]);
		  }
	  }
	  else  if(indiblinktype==blinkselect)
	  {
		  for(int i = 0;i<6;i++)
		  {
			  HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[i],ledindipin[i],0);
		  }
		  HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[indiblinkindex],ledindipin[indiblinkindex],1);
	  }
	  else	//blinkedit
	  {
		  for(int i = 0;i<6;i++)
		  {
			  HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[i],ledindipin[i],0);
		  }
		  if(blinkon)
		  {
			  HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[indiblinkindex],ledindipin[indiblinkindex],1);
			  blinkon = 0;
		  }
		  else
		  {
			  blinkon = 1;
		  }

	  }

}

void indishowselect(uint8_t index)
{
	indiblinktype = blinkselect;
	indiblinkindex = index;
}

void indishowedit(uint8_t index)
{
	indiblinktype = blinkedit;
	indiblinkindex = index;
}

void indishowbutton()
{
	indiblinktype = blinknone;
}

int8_t prevparamvalue;
void on_editenter()
{
	prevparamvalue = *rotatedindex;
}

void on_editexit()
{
	int dowrite  = 0;
	if(prevparamvalue != (*rotatedindex))
	{
		//copy all params values to parambuffer
		for(int i=0;i<((dstinterface*)MAINMODULE)->paramcount;i++ )
		{
			storagebuffer->params[i] = PARAMS[i]->value;
		}

		if(((dstinterface*)MAINMODULE)->multimode)
		{
			//copy all modules param value to the active setting (preset[19])
			dstmultimodule* hmm = (dstmultimodule*) MAINMODULE;
			preset* presetptr = (preset*) storagebuffer->presetbuffer;
			for(int i=0; i< hmm->module.stepcount; i++)
			{
				dstsinglemodule* hsm = (dstsinglemodule*) hmm->modulehandles[i];
				for(int p = 0; p <hsm->interface.paramcount; p++)
				{
					if(p>3)
						break;
					presetptr->preset[19].module[i].control[p] = hsm->params[p].value;
				}
			}

		}

		dowrite = 1;
	}
	//if its a long press on params controls or module selector (not preset)
	//then save all module setting to the current preset
	if(((dstinterface*)MAINMODULE)->multimode)
	{
		dstmultimodule* hmm = (dstmultimodule*) MAINMODULE;
		preset* presetptr = (preset*) storagebuffer->presetbuffer;

		if((EDITEXITSTATUS == editexit_longpress) && (paramindex < 4))
		{
			for(int i=0; i< hmm->module.stepcount; i++)
			{
				dstsinglemodule* hsm = (dstsinglemodule*) hmm->modulehandles[i];

				for(int p = 0; p <hsm->interface.paramcount; p++)
				{
					if(p>3)
						break;
					presetptr->preset[presettostore].module[i].control[p] =
							presetptr->preset[19].module[i].control[p] ;
				}
			}

			dowrite=1;
		}

		//if long press on preset selector then reset the selected preset to default
		//then reset all module's controls with the default
		if((EDITEXITSTATUS == editexit_longpress) && (paramindex == 5))
		{
			int8_t currentpreset = PARAMS[5]->value;
			for(int i=0; i< hmm->module.stepcount; i++)
			{
				dstsinglemodule* hsm = (dstsinglemodule*) hmm->modulehandles[i];

				for(int p = 0; p <hsm->interface.paramcount; p++)
				{
					if(p>3)
						break;
					presetptr->preset[currentpreset].module[i].control[p] =
							DEFAULTPRESET[currentpreset][i][p];
					hsm->params[p].onchange((dsthandle) hsm, DEFAULTPRESET[currentpreset][i][p]);
				}
			}
			dowrite=1;
		}
	}
	if(dowrite)
		writetostorage();
}

//parameters edit menu
enum {menustate_start,menustate_waittrigger, menustate_rotateparamselect,
		menustate_editparamvalue,menustate_selectpresettostore};
uint8_t menustate = menustate_start;
uint32_t menu_currtick = 0;
uint32_t menu_prevtick = 0;
void menu()
{
	menu_currtick = HAL_GetTick();
	if(menu_currtick-menu_prevtick<50)
		return;
	menu_prevtick = menu_currtick;

	if(menustate==menustate_start)
	{
		indishowbutton();
		bardisplaychannel = barchannel_input;
		rotatetrigger = 0;
		rotencode_state = rotencode_startwaittrigger;
		menustate=menustate_waittrigger;
		rotate_editparam = 0;
	}
	else if(menustate==menustate_waittrigger)
	{
		if(rotatetrigger)
		{
			bardisplaychannel = barchannel_param;
			rotatedindex = &paramindex;
			rotatedindexmax=5;
			rotencode_state = rotencode_startrun;
			rotateexit = rotateexit_notyet;
			menustate = menustate_rotateparamselect;
		}
	}
	else if(menustate==menustate_rotateparamselect)
	{
		indishowselect(paramindex);
		if((rotateexit == rotateexit_click)||(rotateexit == rotateexit_longpress))
		{
			rotatedindex = &(PARAMS[paramindex]->value);
			rotatedindexmax= PARAMS[paramindex]->stepcount-1;
			ROTATE_CALLBACK = PARAMS[paramindex]->onchange;
			dstmultimodule * mm = (dstmultimodule *) MAINMODULE;
			if(mm->interface.multimode)	//multi-effect mode
			{
				if(paramindex < 4)	//controls is selected
					ROTATEDMODULE = SUBMODULE;
				else	//preset or module selector is selected
					ROTATEDMODULE = MAINMODULE;
			}
			else 	//single-effect mode
			{
				ROTATEDMODULE = MAINMODULE;
			}
			rotencode_state = rotencode_startrun;
			rotateexit = rotateexit_notyet;
			menustate = menustate_editparamvalue;
			rotate_editparam = 1;
			indishowedit(paramindex);
			on_editenter();
		}
		else if(rotateexit == rotateexit_expired)
		{
			menustate=menustate_start;
		}
	}
	else if(menustate==menustate_editparamvalue)
	{
		if(rotateexit == rotateexit_expired)
		{
			EDITEXITSTATUS = editexit_expired;
			on_editexit();
			menustate=menustate_start;
		}
		else if(rotateexit == rotateexit_click)
		{
			EDITEXITSTATUS = editexit_click;
			on_editexit();
			menustate=menustate_start;
		}
		else if(rotateexit == rotateexit_longpress)
		{
			if(((dstinterface*)MAINMODULE)->multimode)
			{
				if(paramindex < 5)	//not the preset selector
				{
					menustate= menustate_selectpresettostore;
					presettostore = PARAMS[5]->value;
					rotatedindex = &presettostore;
					ROTATE_CALLBACK = NULL;
					rotatedindexmax = 18;
					rotateexit = rotateexit_notyet;
					rotencode_state = rotencode_startrun;
					bardisplaychannel = barchannel_presettostore;
					indishowbutton();
				}
				else
				{
					EDITEXITSTATUS = editexit_longpress;
					on_editexit();
					menustate=menustate_start;
				}
			}
			else	//single effect mode
			{
				EDITEXITSTATUS = editexit_longpress;
				on_editexit();
				menustate=menustate_start;
			}
		}
	}
	else if(menustate==menustate_selectpresettostore)
	{
		if(rotateexit == rotateexit_expired)
		{
			EDITEXITSTATUS = editexit_expired;
			on_editexit();
			menustate=menustate_start;
		}
		else if(rotateexit == rotateexit_click)
		{
			EDITEXITSTATUS = editexit_click;
			on_editexit();
			menustate=menustate_start;
		}
		else if(rotateexit == rotateexit_longpress)
		{
			EDITEXITSTATUS = editexit_longpress;
			on_editexit();
			menustate=menustate_start;
		}
	}
}

static int8_t multieffectpresetchange(dsthandle handle, int8_t newvalue)
{
	dstmultimodule* hmm = (dstmultimodule*) handle;

	for(int i=0; i< hmm->module.stepcount; i++)
	{
		dstsinglemodule* hsm = (dstsinglemodule*) hmm->modulehandles[i];
		//load all parameter from the presetbuffer
		for(int p = 0; p <hsm->interface.paramcount; p++)
		{
			if(p>3)
				break;
			int8_t data = PRESETBUFFER->preset[newvalue].module[i].control[p];
			hsm->params[p].onchange((dsthandle)hsm,data);
		}
	}
	hmm->preset.value = newvalue;
	return newvalue;
}

static int8_t multieffectmodulechange(uint32_t handle, int8_t newvalue)
{
	//find the number of parameter of the current module
	dstmultimodule* hmm = (dstmultimodule*) handle;
	hmm->module.value = newvalue;
	dstsinglemodule* hsm = hmm->modulehandles[newvalue];

	SUBMODULE = (dsthandle) hsm;
	//assign the module's parameter to the control (only the first 4 parameters)
	for(int i=0;i < 4 ;i++)
	{
		if(i < hsm->interface.paramcount)
			PARAMS[i] = &(hsm->params[i]);
		else PARAMS[i] = &paramdump;
	}
	return newvalue;
}

//32767 + 128
#define maxcalsignallevel 32895
//32767 - 128
#define mincalsignallevel 32639
#define calsignalstep 1

uint16_t calsignalcurrlevel = 32767;
int8_t calsignalstate = 0;

static uint16_t generatecalsignal()
{
	if(calsignalstate)
	{
		calsignalcurrlevel += calsignalstep;
		if(calsignalcurrlevel >= maxcalsignallevel)
		{
			calsignalstate = 0;
		}
	}
	else
	{
		calsignalcurrlevel -= calsignalstep;
		if(calsignalcurrlevel <= mincalsignallevel)
		{
			calsignalstate = 1;
		}
	}
	return calsignalcurrlevel;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

	#ifdef USEDEBUGMONITOR
		serialdisplayinit();
	#ifdef USELEVELPROBE
		levelprobe1 = dst_leveldetector_create(4410);
		levelprobe2 = dst_leveldetector_create(4410);
	#endif //USELEVELPROBE
	#endif	//USEDEBUGMONITOR

	//create and initialize signal preprocessors
	leveldetector = dst_leveldetector_create(4410);	//create level detector for 1/10 second sampling window
	dcremover = dst_dcremover_create();
	initantialiasing();

	//ADC calibration
	  HAL_ADCEx_Calibration_Start(&hadc1);
	  HAL_ADCEx_Calibration_Start(&hadc2);

	//start PWM
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);


	/*
	 * //uncomment to test all params LEDs and bar LEDs
	while(1)
	{
		for(int i=0;i<5;i++)
		{
			HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[0],ledindipin[0],1);
			HAL_Delay(100);
			HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[0],ledindipin[0],0);
			HAL_Delay(100);

		}

		for(int i=0;i<6;i++)
		{
			HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[i],ledindipin[i],1);
			HAL_Delay(500);
			HAL_GPIO_WritePin((GPIO_TypeDef*)ledindiport[i],ledindipin[i],0);
		}
		for(int i=0;i<10;i++)
		{
			HAL_GPIO_WritePin((GPIO_TypeDef*)ledbarport[i],ledbarpin[i],1);
			HAL_Delay(500);
			HAL_GPIO_WritePin((GPIO_TypeDef*)ledbarport[i],ledbarpin[i],0);
		}
	}

*/

	//Auto calibrate 16-bit PWM DAC
	calibratepwm();
	 //error blink: too long calibration trial
	  if(calibcount>100)
		  errorblink(ERR_TOO_MANY_CAL_TRIALS);
	//error blink: too low resistor value
	  if(TIM2->ARR < 257)
		  errorblink(ERR_TOO_LOW_RESISTOR);
	  //error blink: too high resistor value
	  if(TIM2->ARR > 350)
		  errorblink(ERR_TOO_HIGH_RESISTOR);

	//start timer 1: cpu cycle counter
	HAL_TIM_Base_Start(&htim1);

	//START AUDIO ADC
	TIM3->ARR = 1632;	//44.1kHz sampling rate
	HAL_TIM_Base_Start(&htim3);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adcbuffer,16);

	//init blank param
	paramdump.onchange=NULL;
	paramdump.stepcount=1;
	paramdump.value = 0;
	paramdump.displaytype = displaytype_bar10;

	//allocate storage buffer
	storagebuffer =(storagestruct*)malloc(1024);
	if(storagebuffer==NULL)
		errorhalt(ERR_STORAGE_ALLOCATION);

	// run module's setup
	if(deepstomp_modulesetup()!=setup_ok)
	{
		errorhalt(ERR_MODULE_SETUP);
	}

	//load params and preset data from flash memory storage
	loadfromstorage();

	//configure the core's user interface
	if(((dstinterface*)MAINMODULE)->multimode)	//if a multi-effect mode
	{
		dstmultimodule* h= (dstmultimodule*) MAINMODULE;

		//assign the PRESETBUFFER pointer with the allocated storage, and initialize to default if not set
		PRESETBUFFER = (preset*) storagebuffer->presetbuffer;
		for(int p=0; p<h->preset.stepcount;p++)
		{
			for(int m=0;m<h->module.stepcount;m++)
			{
				for(int par=0; par < h->modulehandles[m]->interface.paramcount;par++)
				{
					if(par>3)	//manage only the first 4 parameters
						break;
					if(PRESETBUFFER->preset[p].module[m].control[par] < 0)	//not set (after mass erase on firmware programming)
						PRESETBUFFER->preset[p].module[m].control[par] = DEFAULTPRESET[p][m][par];
				}
			}
		}

		//setup last active preset[19]
		for(int m=0;m<h->module.stepcount;m++)
		{
			for(int par=0; par < h->modulehandles[m]->interface.paramcount;par++)
			{
				if(par>3)	//manage only the first 4 parameters
					break;
				if(PRESETBUFFER->preset[19].module[m].control[par] < 0)	//not set (after mass erase on firmware programming)
					PRESETBUFFER->preset[19].module[m].control[par] = DEFAULTPRESET[19][m][par];
			}
		}


		//module and preset control setup
		h->module.onchange = multieffectmodulechange;
		h->preset.onchange = multieffectpresetchange;
		PARAMS[4] = &(h->module);
		PARAMS[5] = &(h->preset);
		//set the active module and preset from the last saved states
		int preset = storagebuffer->params[5];
		int module = storagebuffer->params[4];
		if(preset < 0) preset = 0;
		if(module < 0) module = 0;
		h->module.onchange((dsthandle)h,module);
		h->preset.value = preset;	//set the preset without actually setting up any modules

		//load the module with the last active setting (preset[19]) which could have
		//the same or different  values with the last selected preset
		for(int i=0; i< h->module.stepcount; i++)
		{
			dstsinglemodule* hsm = (dstsinglemodule*) h->modulehandles[i];
			//load all parameter from the presetbuffer
			for(int p = 0; p <hsm->interface.paramcount; p++)
			{
				if(p>3)
					break;
				int8_t data = PRESETBUFFER->preset[19].module[i].control[p];
				hsm->params[p].onchange((dsthandle)hsm,data);
			}
		}
	}
	else	//single effect mode
	{
		dstsinglemodule* h = (dstsinglemodule*) MAINMODULE;
		int8_t* paramptr = storagebuffer->params;
		for(int p=0; p<6 ; p++)
		{
			if(p < h->interface.paramcount)
			{
				PARAMS[p] = &(h->params[p]);
				//load the param with the last saved value from storage if set
				if(paramptr[p] < 0 )
					PARAMS[p]->onchange((dsthandle)h,paramptr[p]);
			}
			else PARAMS[p] = &paramdump;
		}
	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	if(CALIBRATIONMODE)
	{
		while(1)
		{
			//do nothing, all routine will handled by interrupt service routine
		}
	}
	else
	{
	  while (1)
	  {

		#ifdef USEDEBUGMONITOR
			  displaydebuginfo();
		#endif
			  menu();
			  animatebar();
			  rotscan();
			  rotencode();
			  indiblink();
			  runstoragewrite();

	  /* USER CODE END WHILE */

	  /* USER CODE BEGIN 3 */

	  }
	}
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 8;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* ADC2 init function */
static void MX_ADC2_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 256;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim2);

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 2048;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 256;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim4);

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB10 PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 
                           PB4 PB5 PB7 PB8 
                           PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA11 PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

	/*
  while(1)
  {
  }
  */

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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/