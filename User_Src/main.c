/**********************************************************************
Name: Space
Version: v0.5 Beta
Editor: Aysi
Function: Scrubber, Ultrasound, Two BIO, Cold&Heat, use DW L Display, O2 Mask, Pen, clean pen
History:2018.1.13: Debug completed
2018.1.17: Repair some bug, and add the sound when scrubber adjust function works
2018.1.19: Cancel the sound of adjust working, change ScrubberPWM(), Scrubber PWM switch becomes TIM1OUTPUT
2018.3.9: add fan2, change scrubber frequency range to 20.1~30.0, Voltage Adj change, Scrubber bug repaired
flash way changed, intensity changed, screenCheck(); advance work
2018.11.1: Change STM32F103C8T6 to STM32F103R8T6(more pin), use for v2.0 broad
2018.11.13: Product Beta version completed
2018.11.15: add two O2 valve, use BIO pin, use display sound 
2019.2.19:  Change DWC2 to DWD2, Add camera, Sprayer change(without led and button, LED contral use for camera, 
			O2 add light off mode
**********************************************************************/
//#define DEBUG
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
//#include "dwDriver.h"
#include "AppLib.h"

int main()
{
	u16 i = 0;

	NVIC_Configuration(); //设置NVIC�?�?分组2:2位抢占优先级�?2位响应优先级
	delay_init();		  //Init Systick timer
	IO_Init();
	uart1_init(9600);					   //To PC
	uart2_init(115200);					   //To Dw display
  //	Adc_Init();
	TIM4_PWM_Init(36000, 0);			   //BIO 2Khz.
	TIM_SetCompare4(TIM4, 36000);		   //BIO voltage
	TIM_SetCompare3(TIM4, 0);			   //BIO output
	TIM3_PWM_Init(102, 0);				   //RF 700Khz.
	TIM1_PWM_Init((u16)(72000 / 26.3), 0); //Scrubber Init 26.3kHz
	if (dwD2Check())					   //DW power on check
	{
		while (1)
		{
			SOUND_PIN = ~SOUND_PIN;
			delay_ms(200);
		}
	}
	dwD2DisPicNoL(0);
	dwD2SetBL(0X64, 0X03, 0X02EE);	//change display light
	TIM2_Int_Init(999, 70); //1ms
	INLINE_MUSIC_POWERUP();
	// delay_ms(1000);
	// delay_ms(1000);
	//	delay_ms(1000);
	//	delay_ms(1000);

	/*scrubber frequency init*/
	STMFLASH_Read(FLASH_FREQ_ADDR, &ScrubberFrequency, 1);
	if (ScrubberFrequency < 201 || ScrubberFrequency > 300)
	{
		ScrubberFrequency = 250;
		STMFLASH_Write(FLASH_FREQ_ADDR, &ScrubberFrequency, 1);
		INLINE_MUSIC_ERROR();
	}

	STMFLASH_Read(FLASH_LANG_ADDR, &i, 1); //read language config ago
	if (i)
		dwD2SetLanguage(LANGUAGE_ENGLISH);
	else
		dwD2SetLanguage(LANGUAGE_CHINESE);

	uasrt1SendByte(0x12);
	nextPage = FUNCTION_RESTART;
	//	dwD2DisPicNoL(PIC_LANGUAGE);

#ifndef DEBUG
	IWDG_Init(IWDG_Prescaler_256, 625); //与分频数�?256,重载值为625,溢出时间�?4s
#endif

	while (1)
	{
		switch (nextPage)
		{
		case FUNCTION_RESTART:
		{
			PageRestartAnimation();
			break;
		}
		case FUNCTION_MAIN:
		{
			PageMain();
			break;
		}
		case FUNCTION_LANGUAGE:
		{
			PageLanguage();
			break;
		}
		case FUNCTION_SCRUBBER:
		{
			PageScrubber();
			break;
		}
		case FUNCTION_SCRUBBER_ADJ:
		{
			PageScrubberAdj();
			break;
		}
		case FUNCTION_ULTRASOUND:
		{
			PageUltrasound();
			break;
		}
		case FUNCTION_O2:
		{
			PageO2();
			break;
		}
		case FUNCTION_ICE:
		{
			PageIce();
			break;
		}
		case FUNCTION_BIO1:
		{
			PageBIO1();
			break;
		}
		case FUNCTION_CLEAN:
		{
			PageClean();
			break;
		}
		case FUNCTION_RF:
		{
			PageRF();
			break;
		}
		case FUNCTION_CAM:
		{
			PageCAM();
			break;
		}
		case FUNCTION_SPRAYER:
		{
			PageSprayer();
			break;
		}
		default:
		{
			break;
		}
		}
	}
}
