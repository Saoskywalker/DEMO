#include "AppLib.h"

//#define DEBUG_WDG
#define SCR_T_ON 0.2	//Scrubber PWM(Timer1) compare value
#define FRAME_HEAD 0xAA
#define FRAME_END 0x55

//const u16 CoolIntensity[] = {0,1700,1800,1900,2000,2175,2275,2375,2475,2550,2625};	//2.2 2.4 2.6 2.8 3.0 3.2 3.4 3.6 3.8 4.0 
u8 Flag1ms = 0;
u8 TouchKey = 0;
u8 nextPage = 0;
u8 menuExit = 0;
u16 WorkTime = 1800;
u8 Flag1s = 0;
u16 ScrubberFrequency = 250;
u8 ScrubberMode = 0, ScrubberIntensity = 0;
u8 O2Mode = 0;
u8 RFMode = 0, RFIntensity = 0;
u8 ColdMode = 0, ColdIntensity = 0;
u8 BIO1Mode = 0, BIO1Intensity = 0;
u8 CleanMode = 0;
u8 UltrasoundMode = 0, UltrasoundIntensity = 0;
u8 ScrubberWorkFlag = 0, O2WorkFlag = 0, RFWorkFlag = 0, BIO1WorkFlag = 0,	\
	 CleanWorkFlag = 0, UltrasoundWorkFlag = 0, ColdWorkFlag = 0, SprayerWorkFlag = 0;
const float ScrubberPWMTable[] = {0, 0.1, 0.13, 0.17, 0.2};
u16 ScrubberPWMIntensity = 0;
const u16 BIO1IntensityTable[] = {36000, 30000, 29000, 28000, 27000, 26000};
//{36000, 20000, 10000, 5000, 0, 0};
//{36000, 20000, 15000, 10000, 5000, 0};
u16 RGBValue = 0;

//Button press effect
static const Button btnEnter  = {PIC_START_ANIMATION, 555, 192, 674, 310};
static const Button btnClean = {PIC_CLEAN, 466,104,591,150};
static const Button btnSprayer = {PIC_SPRAYER, 316, 104, 440, 150};
static const Button btnO2 = {PIC_O2, 57,100,262,168};
static const Button btnScrubber = {PIC_SCRUBBER, 57,338,262,406};
static const Button btnUltrasound = {PIC_ULTRASOUND, 57,181,262,248};
static const Button btnRF = {PIC_RF, 57,257,262,327};
static const Button btnIce = {PIC_ICE, 57,419,262,486};
static const Button btnBIO1 = {PIC_RF, 57,257,262,327};
static const Button btnTimeUp = {PIC_EFFECT2, 679,223,719,265};
static const Button btnTimeDown = {PIC_EFFECT2, 537,223,577,265};
static const Button btnIntensityUp = {PIC_EFFECT2, 679,313,719,354};
static const Button btnIntensityDown = {PIC_EFFECT2, 537,313,577,354};
static const Button btnModeUp = {PIC_EFFECT2, 679,405,719,445};
static const Button btnModeDown = {PIC_EFFECT2, 537,405,577,445};
static const Button btnStart = {PIC_EFFECT, 686,524,744,581};
static const Button btnScrubberAdj = {PIC_EFFECT,770,0,799,30};
static const Button btnScrubberAdjUp = {PIC_EFFECT,172,244,256,325};
static const Button btnScrubberAdjDown = {PIC_EFFECT,539,243,623,330};
static const Button btnMin = {PIC_EFFECT,338,367,455,483};
static const Button btnSet = {PIC_EFFECT, 55, 6, 103, 55};
static const Button btnChinse = {PIC_LANGUAGE, 193, 228, 319, 343};
static const Button btnEnglish = {PIC_LANGUAGE, 482, 228, 608, 343};
static const Button btnTimeUp_Clean = {PIC_EFFECT, 679,310,719,350};
static const Button btnTimeDown_Clean = {PIC_EFFECT, 537,310,577,350};
static const Button btnTimeUp_O2 = {PIC_EFFECT3, 679,270,719,309};
static const Button btnTimeDown_O2 = {PIC_EFFECT3, 537,270,577,309};
static const Button btnModeUp_O2 = {PIC_EFFECT3, 679,360,719,397};
static const Button btnModeDown_O2 = {PIC_EFFECT3, 537,360,577,397};
static const Button btnBottleA = {PIC_EFFECT, 56, 517, 124, 584};
static const Button btnBottleB = {PIC_EFFECT, 180, 517, 246, 584};
static const Button btnBottleC = {PIC_EFFECT, 304, 517, 368, 584};
static const Button btnBottleD = {PIC_EFFECT, 426, 517, 490, 584};
static const Button btnStandby = {PIC_EFFECT, 129, 7, 172, 52};

//Botton free effect
static const Button btnTimeUp_O = {PIC_SCRUBBER, 679,223,719,265};
static const Button btnTimeDown_O = {PIC_SCRUBBER, 537,223,577,265};
static const Button btnTimeUp_Clean_O = {PIC_CLEAN, 679,310,719,350};
static const Button btnTimeDown_Clean_O = {PIC_CLEAN, 537,310,577,350};
static const Button btnTimeUp_O2_O = {PIC_O2, 679,270,719,309};
static const Button btnTimeDown_O2_O = {PIC_O2, 537,270,577,309};
static const Button btnIntensityUp_O = {PIC_SCRUBBER, 679,313,719,354};
static const Button btnIntensityDown_O = {PIC_SCRUBBER, 537,313,577,354};
static const Button btnModeUp_O = {PIC_SCRUBBER, 679,405,719,445};
static const Button btnModeDown_O = {PIC_SCRUBBER, 537,405,577,445};
static const Button btnModeUp_O2_O = {PIC_O2, 679,360,719,397};
static const Button btnModeDown_O2_O = {PIC_O2, 537,360,577,397};

/*language select*/
static void menuChinsePres(void)
{
	u16 i = 0;
	
	INLINE_MUSIC_BUTTON();
	dwSetLanguage(LANGUAGE_CHINESE);
	
	nextPage = FUNCTION_MAIN;
	STMFLASH_Write(FLASH_LANG_ADDR, &i, 1);		//Write language config
}

static void menuEnglishPres(void)
{
	u16 i = 123;
	
	INLINE_MUSIC_BUTTON();
	dwSetLanguage(LANGUAGE_ENGLISH);
	
	nextPage = FUNCTION_MAIN;
	STMFLASH_Write(FLASH_LANG_ADDR, &i, 1);		//Write language config
}

static void StandbyPres(void)
{
	dwDisButton(&btnStandby, 1, btnStandby.xs, btnStandby.ys);	
}

static void StandbyFree(void)
{
	INLINE_MUSIC_STANDBY();
	nextPage = FUNCTION_RESTART;
	dwSetBL(0x01);	//change display light
	PUMP24_PIN = 0;	//Close front LED
}

static void ScrubberPres(void)
{
	INLINE_MUSIC_BUTTON();
	dwDisButton(&btnScrubber, 1, btnScrubber.xs, btnScrubber.ys);
	
	nextPage = FUNCTION_SCRUBBER;
}

static void O2Pres(void)
{
	INLINE_MUSIC_BUTTON();
	dwDisButton(&btnO2, 1, btnO2.xs, btnO2.ys);
	
	nextPage = FUNCTION_O2;
}

static void RFPres(void)
{
	INLINE_MUSIC_BUTTON();
	dwDisButton(&btnRF, 1, btnRF.xs, btnRF.ys);
	
//	nextPage = FUNCTION_RF;
	nextPage = FUNCTION_BIO1;
}

static void SetPres(void)
{
	INLINE_MUSIC_BUTTON();
	dwDisButton(&btnSet, 1, btnSet.xs, btnSet.ys);
	
	nextPage = FUNCTION_LANGUAGE;
}

static void ScrubberAdjPres(void)
{
	INLINE_MUSIC_SCR_ADJ();
	nextPage = FUNCTION_SCRUBBER_ADJ;
}

static void CleanPres(void)
{
	INLINE_MUSIC_BUTTON();
	dwDisButton(&btnClean, 1, btnClean.xs, btnClean.ys);
	
	nextPage = FUNCTION_CLEAN;
}

static void UltrasoundPres(void)
{
	INLINE_MUSIC_BUTTON();
	dwDisButton(&btnUltrasound, 1, btnUltrasound.xs, btnUltrasound.ys);
	
	nextPage = FUNCTION_ULTRASOUND;
}

static void IcePres(void)
{
	INLINE_MUSIC_BUTTON();
	dwDisButton(&btnIce, 1, btnIce.xs, btnIce.ys);
	
	nextPage = FUNCTION_ICE;
}

static void BIO1Pres(void)
{
	INLINE_MUSIC_BUTTON();
	dwDisButton(&btnBIO1, 1, btnBIO1.xs, btnBIO1.ys);
	
	nextPage = FUNCTION_BIO1;	
}

static void SprayerPres(void)
{
	INLINE_MUSIC_BUTTON();
	dwDisButton(&btnSprayer, 1, btnSprayer.xs, btnSprayer.ys);
	
	nextPage = FUNCTION_SPRAYER;
}

static void IntensityUpPres(void)
{
	INLINE_MUSIC_BUTTON();
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberIntensity<4)
				ScrubberIntensity++;
			dwDisNum(DW_SIZE_24, 622, 321, ScrubberIntensity);
			dwDisButton(&btnIntensityUp, 1, btnIntensityUp.xs, btnIntensityUp.ys);
			ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
			TIM_SetCompare1(TIM1,ScrubberPWMIntensity);
			break;
		}
		case FUNCTION_SCRUBBER_ADJ:
		{
			if(ScrubberFrequency<300)
			{
				ScrubberFrequency++;
				dwDisNum(DW_SIZE_48, 319,    260,ScrubberFrequency/100);
				dwDisNum(DW_SIZE_48, 319+24, 260,ScrubberFrequency/10%10);
				dwDisChar(DW_SIZE_48,319+48, 260,'.');
				dwDisNum(DW_SIZE_48, 319+72, 260,ScrubberFrequency%10);				
				TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
				ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
				TIM_SetCompare1(TIM1, ScrubberPWMIntensity);
			}
			break;
		}
		case FUNCTION_BIO1:
		{
			if(BIO1Intensity<4)
				BIO1Intensity++;
			dwDisNum(DW_SIZE_24, 622, 321, BIO1Intensity);
			dwDisButton(&btnIntensityUp, 1, btnIntensityUp.xs, btnIntensityUp.ys);
			TIM_SetCompare4(TIM4, BIO1IntensityTable[BIO1Intensity]);
			break;
		}
		case FUNCTION_ULTRASOUND:
		{
			if(UltrasoundIntensity<4)
				UltrasoundIntensity++;
			dwDisNum(DW_SIZE_24, 622, 321, UltrasoundIntensity);
			dwDisButton(&btnIntensityUp, 1, btnIntensityUp.xs, btnIntensityUp.ys);
			break;
		}
		case FUNCTION_RF:
		{
			if(RFIntensity<4)
				RFIntensity++;
			dwDisNum(DW_SIZE_24, 622, 321, RFIntensity);
			dwDisButton(&btnIntensityUp, 1, btnIntensityUp.xs, btnIntensityUp.ys);
			break;
		}
		default: {break;}
	}	
}

static void IntensityUpFree(void)
{	
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: {dwDisButton(&btnIntensityUp_O, 1, btnIntensityUp_O.xs, btnIntensityUp_O.ys); break;}
		case FUNCTION_ULTRASOUND: {dwDisButton(&btnIntensityUp_O, 1, btnIntensityUp_O.xs, btnIntensityUp_O.ys); break;}
		case FUNCTION_RF: {dwDisButton(&btnIntensityUp_O, 1, btnIntensityUp_O.xs, btnIntensityUp_O.ys); break;}
		case FUNCTION_BIO1: {dwDisButton(&btnIntensityUp_O, 1, btnIntensityUp_O.xs, btnIntensityUp_O.ys); break;}
		default: {break;}
	}
}

static void IntensityDownPres(void)
{
	INLINE_MUSIC_BUTTON();	
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberIntensity>0)
				ScrubberIntensity--;
			dwDisNum(DW_SIZE_24, 622, 321, ScrubberIntensity);
			dwDisButton(&btnIntensityDown, 1, btnIntensityDown.xs, btnIntensityDown.ys);
			ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
			TIM_SetCompare1(TIM1,ScrubberPWMIntensity);
			break;
		}
		case FUNCTION_SCRUBBER_ADJ:
		{
			if(ScrubberFrequency>201)
			{
				ScrubberFrequency--;
				dwDisNum(DW_SIZE_48, 319,    260,ScrubberFrequency/100);
				dwDisNum(DW_SIZE_48, 319+24, 260,ScrubberFrequency/10%10);
				dwDisChar(DW_SIZE_48,319+48, 260,'.');
				dwDisNum(DW_SIZE_48, 319+72, 260,ScrubberFrequency%10);
				TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
				ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
				TIM_SetCompare1(TIM1, ScrubberPWMIntensity);
			}
			break;
		}
		case FUNCTION_BIO1:
		{
			if(BIO1Intensity>0)
				BIO1Intensity--;
			dwDisNum(DW_SIZE_24, 622, 321, BIO1Intensity);
			dwDisButton(&btnIntensityDown, 1, btnIntensityDown.xs, btnIntensityDown.ys);
			TIM_SetCompare4(TIM4, BIO1IntensityTable[BIO1Intensity]);
			break;
		}
		case FUNCTION_ULTRASOUND:
		{
			if(UltrasoundIntensity>0)
				UltrasoundIntensity--;
			dwDisNum(DW_SIZE_24, 622, 321, UltrasoundIntensity);
			dwDisButton(&btnIntensityDown, 1, btnIntensityDown.xs, btnIntensityDown.ys);
			break;
		}
		case FUNCTION_RF:
		{
			if(RFIntensity>0)
				RFIntensity--;
			dwDisNum(DW_SIZE_24, 622, 321, RFIntensity);
			dwDisButton(&btnIntensityDown, 1, btnIntensityDown.xs, btnIntensityDown.ys);
			break;
		}
		default: {break;}
	}	
}

static void IntensityDownFree(void)
{
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: {dwDisButton(&btnIntensityDown_O, 1, btnIntensityDown_O.xs, btnIntensityDown_O.ys); break;}
		case FUNCTION_ULTRASOUND: {dwDisButton(&btnIntensityDown_O, 1, btnIntensityDown_O.xs, btnIntensityDown_O.ys); break;}
		case FUNCTION_RF: {dwDisButton(&btnIntensityDown_O, 1, btnIntensityDown_O.xs, btnIntensityDown_O.ys); break;}
		case FUNCTION_BIO1: {dwDisButton(&btnIntensityDown_O, 1, btnIntensityDown_O.xs, btnIntensityDown_O.ys); break;}
		default: {break;}
	}
}

static void TimeUpPres(void)
{
	INLINE_MUSIC_BUTTON();

	if(WorkTime<3600)
	{
		WorkTime += 60;
	}	
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: {dwDisButton(&btnTimeUp, 1, btnTimeUp.xs, btnTimeUp.ys); DisSetAxis(600, 231); break;}
		case FUNCTION_ULTRASOUND: {dwDisButton(&btnTimeUp, 1, btnTimeUp.xs, btnTimeUp.ys); DisSetAxis(600, 231); break;}
		case FUNCTION_RF: {dwDisButton(&btnTimeUp, 1, btnTimeUp.xs, btnTimeUp.ys); DisSetAxis(600, 231); break;}
		case FUNCTION_BIO1: {dwDisButton(&btnTimeUp, 1, btnTimeUp.xs, btnTimeUp.ys); DisSetAxis(600, 231); break;}
		case FUNCTION_O2: {dwDisButton(&btnTimeUp_O2, 1, btnTimeUp_O2.xs, btnTimeUp_O2.ys); DisSetAxis(600, 278); break;}
		default: {dwDisButton(&btnTimeUp_Clean, 1, btnTimeUp_Clean.xs, btnTimeUp_Clean.ys); DisSetAxis(600, 318); break;}
	}
	funDisTime(WorkTime);
}

static void TimeUpFree(void)
{
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: {dwDisButton(&btnTimeUp_O, 1, btnTimeUp_O.xs, btnTimeUp_O.ys); break;}
		case FUNCTION_ULTRASOUND: {dwDisButton(&btnTimeUp_O, 1, btnTimeUp_O.xs, btnTimeUp_O.ys); break;}
		case FUNCTION_RF: {dwDisButton(&btnTimeUp_O, 1, btnTimeUp_O.xs, btnTimeUp_O.ys); break;}
		case FUNCTION_BIO1: {dwDisButton(&btnTimeUp_O, 1, btnTimeUp_O.xs, btnTimeUp_O.ys); break;}
		case FUNCTION_O2: {dwDisButton(&btnTimeUp_O2_O, 1, btnTimeUp_O2_O.xs, btnTimeUp_O2_O.ys); break;}
		default: {dwDisButton(&btnTimeUp_Clean_O, 1, btnTimeUp_Clean_O.xs, btnTimeUp_Clean_O.ys); break;}
	}
}

static void TimeDownPres(void)
{
	INLINE_MUSIC_BUTTON();

	if(WorkTime>60)
	{
		WorkTime -= 60;
	}
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: {dwDisButton(&btnTimeDown, 1, btnTimeDown.xs, btnTimeDown.ys); DisSetAxis(600, 231); break;}
		case FUNCTION_ULTRASOUND: {dwDisButton(&btnTimeDown, 1, btnTimeDown.xs, btnTimeDown.ys); DisSetAxis(600, 231); break;}
		case FUNCTION_RF: {dwDisButton(&btnTimeDown, 1, btnTimeDown.xs, btnTimeDown.ys); DisSetAxis(600, 231); break;}
		case FUNCTION_BIO1: {dwDisButton(&btnTimeDown, 1, btnTimeDown.xs, btnTimeDown.ys); DisSetAxis(600, 231); break;}
		case FUNCTION_O2: {dwDisButton(&btnTimeDown_O2, 1, btnTimeDown_O2.xs, btnTimeDown_O2.ys); DisSetAxis(600, 278); break;}
		default: {dwDisButton(&btnTimeDown_Clean, 1, btnTimeDown_Clean.xs, btnTimeDown_Clean.ys); DisSetAxis(600, 318); break;}
	}
	funDisTime(WorkTime);
}

static void TimeDownFree(void)
{
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: {dwDisButton(&btnTimeDown_O, 1, btnTimeDown_O.xs, btnTimeDown_O.ys); break;}
		case FUNCTION_ULTRASOUND: {dwDisButton(&btnTimeDown_O, 1, btnTimeDown_O.xs, btnTimeDown_O.ys); break;}
		case FUNCTION_RF: {dwDisButton(&btnTimeDown_O, 1, btnTimeDown_O.xs, btnTimeDown_O.ys); break;}
		case FUNCTION_BIO1: {dwDisButton(&btnTimeDown_O, 1, btnTimeDown_O.xs, btnTimeDown_O.ys); break;}
		case FUNCTION_O2: {dwDisButton(&btnTimeDown_O2_O, 1, btnTimeDown_O2_O.xs, btnTimeDown_O2_O.ys); break;}
		default: {dwDisButton(&btnTimeDown_Clean_O, 1, btnTimeDown_Clean_O.xs, btnTimeDown_Clean_O.ys); break;}
	}
}

static void ModeUpPres(void)
{
	INLINE_MUSIC_BUTTON();
	
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberMode<2)
				ScrubberMode++;
			dwDisChar(DW_SIZE_24, 622, 411, ScrubberMode+'A');
			dwDisButton(&btnModeUp, 1, btnModeUp.xs, btnModeUp.ys);
			break;
		}
		case FUNCTION_BIO1: 
		{
			if(BIO1Mode<2)
				BIO1Mode++;
			dwDisChar(DW_SIZE_24, 622, 411, BIO1Mode+'A');
			dwDisButton(&btnModeUp, 1, btnModeUp.xs, btnModeUp.ys);
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			if(UltrasoundMode<2)
				UltrasoundMode++;
			dwDisChar(DW_SIZE_24, 622, 411, UltrasoundMode+'A');
			dwDisButton(&btnModeUp, 1, btnModeUp.xs, btnModeUp.ys);
			break;
		}
		case FUNCTION_RF: 
		{
			if(RFMode<2)
				RFMode++;
			dwDisChar(DW_SIZE_24, 622, 411, RFMode+'A');
			dwDisButton(&btnModeUp, 1, btnModeUp.xs, btnModeUp.ys);
			break;
		}
		case FUNCTION_ICE: 
		{
			if(ColdMode<1)
				ColdMode++;
			dwDisChar(DW_SIZE_24, 550, 253, ColdMode+'A');
			break;
		}
		case FUNCTION_O2: 
		{
			if(O2Mode<3)
				O2Mode++;
			dwDisChar(DW_SIZE_24, 622, 364, O2Mode+'A');
			dwDisButton(&btnModeUp_O2, 1, btnModeUp_O2.xs, btnModeUp_O2.ys);
			break;
		}
		default: {break;}
	}
}

static void ModeUpFree(void)
{
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			dwDisButton(&btnModeUp_O, 1, btnModeUp_O.xs, btnModeUp_O.ys);
			break;
		}
		case FUNCTION_BIO1: 
		{
			dwDisButton(&btnModeUp_O, 1, btnModeUp_O.xs, btnModeUp_O.ys);
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			dwDisButton(&btnModeUp_O, 1, btnModeUp_O.xs, btnModeUp_O.ys);
			break;
		}
		case FUNCTION_RF: 
		{
			dwDisButton(&btnModeUp_O, 1, btnModeUp_O.xs, btnModeUp_O.ys);
			break;
		}
		case FUNCTION_ICE: 
		{
			dwDisButton(&btnModeUp_O2_O, 1, btnModeUp_O2_O.xs, btnModeUp_O2_O.ys);
			break;
		}
		case FUNCTION_O2: 
		{
			dwDisButton(&btnModeUp_O2_O, 1, btnModeUp_O2_O.xs, btnModeUp_O2_O.ys);
			break;
		}
		default: {break;}
	}
}

static void ModeDownPres(void)
{
	INLINE_MUSIC_BUTTON();
	
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberMode>0)
				ScrubberMode--;
			dwDisChar(DW_SIZE_24, 622, 411, ScrubberMode+'A');
			dwDisButton(&btnModeDown, 1, btnModeDown.xs, btnModeDown.ys);
			break;
		}
		case FUNCTION_BIO1: 
		{
			if(BIO1Mode>0)
				BIO1Mode--;
			dwDisChar(DW_SIZE_24, 622, 411, BIO1Mode+'A');
			dwDisButton(&btnModeDown, 1, btnModeDown.xs, btnModeDown.ys);
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			if(UltrasoundMode>0)
				UltrasoundMode--;
			dwDisChar(DW_SIZE_24, 622, 411, UltrasoundMode+'A');
			dwDisButton(&btnModeDown, 1, btnModeDown.xs, btnModeDown.ys);
			break;
		}
		case FUNCTION_RF: 
		{
			if(RFMode>0)
				RFMode--;
			dwDisChar(DW_SIZE_24, 622, 411, RFMode+'A');
			dwDisButton(&btnModeDown, 1, btnModeDown.xs, btnModeDown.ys);
			break;
		}
		case FUNCTION_ICE: 
		{
			if(ColdMode>0)
				ColdMode--;
			dwDisChar(DW_SIZE_24, 550, 253, ColdMode+'A');
			break;
		}
		case FUNCTION_O2: 
		{
			if(O2Mode>0)
				O2Mode--;
			dwDisChar(DW_SIZE_24, 622, 364, O2Mode+'A');
			dwDisButton(&btnModeDown_O2, 1, btnModeDown_O2.xs, btnModeDown_O2.ys);
			break;
		}
		default: {break;}
	}
}

static void ModeDownFree(void)
{
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			dwDisButton(&btnModeDown_O, 1, btnModeDown_O.xs, btnModeDown_O.ys);
			break;
		}
		case FUNCTION_BIO1: 
		{
			dwDisButton(&btnModeDown_O, 1, btnModeDown_O.xs, btnModeDown_O.ys);
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			dwDisButton(&btnModeDown_O, 1, btnModeDown_O.xs, btnModeDown_O.ys);
			break;
		}
		case FUNCTION_RF: 
		{
			dwDisButton(&btnModeDown_O, 1, btnModeDown_O.xs, btnModeDown_O.ys);
			break;
		}
		case FUNCTION_ICE: 
		{
			dwDisButton(&btnModeDown_O2_O, 1, btnModeDown_O2_O.xs, btnModeDown_O2_O.ys);
			break;
		}
		case FUNCTION_O2: 
		{
			dwDisButton(&btnModeDown_O2_O, 1, btnModeDown_O2_O.xs, btnModeDown_O2_O.ys);
			break;
		}
		default: {break;}
	}
}

static void StartPres(void)
{
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberWorkFlag)
			{				
				ScrubberWorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				ScrubberWorkFlag = 1;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_BIO1: 
		{
			if(BIO1WorkFlag)
			{						
				BIO1WorkFlag = 0;
				RF_CON_PIN = 1;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				BIO1WorkFlag = 1;
				RF_CON_PIN = 0;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			if(UltrasoundWorkFlag)
			{				
				UltrasoundWorkFlag = 0;
				ULT_CON_PIN = 1;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				UltrasoundWorkFlag = 1;
				ULT_CON_PIN = 0;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_RF: 
		{
			if(RFWorkFlag)
			{				
				RFWorkFlag = 0;
				RF_CON_PIN = 1;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				RFWorkFlag = 1;
				RF_CON_PIN = 0;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_ICE: 
		{
			if(ColdWorkFlag)
			{				
				ColdWorkFlag = 0;
				COLD_CON_PIN = 1;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				ColdWorkFlag = 1;
				COLD_CON_PIN = 0;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_O2: 
		{
			if(O2WorkFlag)
			{				
				O2WorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				O2WorkFlag = 1;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_CLEAN: 
		{
			if(CleanWorkFlag)
			{				
				CleanWorkFlag = 0;
				PEN_CON_PIN = 1;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				CleanWorkFlag = 1;
				PEN_CON_PIN = 0;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_SPRAYER: 
		{
			if(SprayerWorkFlag)
			{				
				SprayerWorkFlag = 0;
				GUN_CON_PIN = 1;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				SprayerWorkFlag = 1;
				GUN_CON_PIN = 0;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		default: 
		{
			ScrubberWorkFlag = 0;
			O2WorkFlag = 0;
			RFWorkFlag = 0;
			BIO1WorkFlag = 0;
			CleanWorkFlag = 0;
			UltrasoundWorkFlag = 0; 
			ColdWorkFlag = 0;
			SprayerWorkFlag = 0;
			dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);	//start button dis
			break;
		}
	}
	if(ScrubberWorkFlag||O2WorkFlag||RFWorkFlag||BIO1WorkFlag||CleanWorkFlag||
	 UltrasoundWorkFlag||ColdWorkFlag||SprayerWorkFlag)
		{INLINE_MUSIC_START();}
	else
		{INLINE_MUSIC_STOP();}
}

static void EnterPres(void)
{
	menuExit = 1;
	nextPage = FUNCTION_MAIN;
	INLINE_MUSIC_REDAY();
	dwDisButton(&btnEnter, 0, btnEnter.xs, btnEnter.ys);
	dwSetBL(0x40);	//change display light
	PUMP24_PIN = 1;
}

static void MinPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_MAIN;
}

static void BottleASelect(void)
{
	INLINE_MUSIC_BUTTON();
	dwCutPic(PIC_CLEAN, 56, 517, 490, 584, 56, 517);
	dwDisButton(&btnBottleA, 1, btnBottleA.xs, btnBottleA.ys);	
	
	CleanMode = 1;
}

static void BottleBSelect(void)
{
	INLINE_MUSIC_BUTTON();
	dwCutPic(PIC_CLEAN, 56, 517, 490, 584, 56, 517);
	dwDisButton(&btnBottleB, 1, btnBottleB.xs, btnBottleB.ys);
	
	CleanMode = 2;	
}

static void BottleCSelect(void)
{
	INLINE_MUSIC_BUTTON();
	dwCutPic(PIC_CLEAN, 56, 517, 490, 584, 56, 517);
	dwDisButton(&btnBottleC, 1, btnBottleC.xs, btnBottleC.ys);	
	
	CleanMode = 3;
}

static void BottleDSelect(void)
{
	INLINE_MUSIC_BUTTON();
	dwCutPic(PIC_CLEAN, 56, 517, 490, 584, 56, 517);
	dwDisButton(&btnBottleD, 1, btnBottleD.xs, btnBottleD.ys);
	
	CleanMode = 4;	
}

void WorkTimeDeal(void)
{
	if(ScrubberWorkFlag||O2WorkFlag||RFWorkFlag||BIO1WorkFlag||CleanWorkFlag||
		 UltrasoundWorkFlag||ColdWorkFlag||SprayerWorkFlag)
	{
		if(Flag1s)
		{
			Flag1s = 0;
			if(WorkTime>0)
			{
				WorkTime--;
			}
			else
			{	
				WorkTime = 1800;
				StartPres();
//				INLINE_MUSIC_STOP();
//				ScrubberWorkFlag = 0;
//				O2WorkFlag = 0;
//				RFWorkFlag = 0;
//				BIO1WorkFlag = 0;
//				CleanWorkFlag = 0;
//				UltrasoundWorkFlag = 0; 
//				ColdWorkFlag = 0;
//				SprayerWorkFlag = 0;
//				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);	//start button dis
			}
			switch(nextPage)
			{
				case FUNCTION_SCRUBBER: {DisSetAxis(600, 231); break;}
				case FUNCTION_ULTRASOUND: {DisSetAxis(600, 231); break;}
				case FUNCTION_RF: {DisSetAxis(600, 231); break;}
				case FUNCTION_BIO1: {DisSetAxis(600, 231); break;}
				case FUNCTION_O2: {DisSetAxis(600, 278); break;}
				default: {DisSetAxis(600, 318); break;}
			}
			funDisTime(WorkTime);
		}
	}
}

//Init not peripheral IO
void IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|
												RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	 
	
	//OUTPUT
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //Disable JTAG, release the pin	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_10|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_10|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_9);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_4|GPIO_Pin_5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_7|GPIO_Pin_10);
	
	//INPUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_15;	//SOUND		 
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	 
//	GPIO_ResetBits(GPIOB, GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_15); 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_15;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_15);

//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);		
//	//GPIO_ResetBits(GPIOC, GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
//	PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
//	RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后， PC14 PC15 才可以当普通IO用。
//	PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
//	BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
//	BKP_ITConfig(DISABLE);
}

//Power up 
void PageRestartAnimation(void)
{	
	if(RestoreFlag()==1)	//Check restore reason
	{
		INLINE_MUSIC_ERROR()
	}
	
	dwDisPicNoL(0);
	menuExit = 0;
	dwCancelKey();
	dwListenKey(EnterPres, 0, &btnEnter);

	while(!menuExit)
	{
		dwHandler();
	}
	
	dwWaitRelease();
	dwHandler();
	delay_ms(300);
}
////////////////////////////////

//select language
void PageLanguage(void)
{
	menuExit = 0;
	
	dwDisPicNoL(PIC_LANGUAGE);
	dwCancelKey();
	dwListenKey(menuChinsePres,  0, &btnChinse);
	dwListenKey(menuEnglishPres, 0, &btnEnglish);

	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_LANGUAGE)
		{
			menuExit = 1;
		}
	}
	dwWaitRelease();
	dwHandler();	
}
//////////////////////////////

//Main page
void PageMain(void)
{
	menuExit = 0;
	dwDisPicWithL(PIC_MAIN);
	
	dwCancelKey();
//	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(SprayerPres, 0, &btnSprayer);
	dwListenKey(SetPres, 0, &btnSet);
	dwListenKey(StandbyPres, StandbyFree, &btnStandby);	//Standby
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_MAIN)
		{
			menuExit = 1;
		}
	}
	
	dwWaitRelease();
	dwHandler();
}
///////////////////////////////

//Scrubber function
void PageScrubber(void)
{	
	dwDisPicWithL(PIC_SCRUBBER);
	DisTextSize(DW_SIZE_24);
	DisSetAxis(600, 231);
	funDisTime(WorkTime);
	DisSetAxis(622, 321);
	DisNumber(ScrubberIntensity, 1, 0, Dis_Dec);
	dwDisChar(DW_SIZE_24, 622, 411, ScrubberMode+'A');	
	
	TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
	ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
	TIM_SetCompare1(TIM1,ScrubberPWMIntensity);
	
	menuExit = 0;
	WorkTime = 1800;
	
	dwCancelKey();
//	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(SprayerPres, 0, &btnSprayer);
	dwListenKey(SetPres, 0, &btnSet);
	dwListenKey(TimeUpPres, TimeUpFree, &btnTimeUp);
	dwListenKey(TimeDownPres, TimeDownFree, &btnTimeDown);
	dwListenKey(IntensityUpPres, IntensityUpFree, &btnIntensityUp);
	dwListenKey(IntensityDownPres, IntensityDownFree, &btnIntensityDown);
	dwListenKey(ModeUpPres, ModeUpFree, &btnModeUp);
	dwListenKey(ModeDownPres, ModeDownFree, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
//	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(ScrubberAdjPres, 0, &btnScrubberAdj);
	dwListenKey(StandbyPres, StandbyFree, &btnStandby);	//Standby
	
	while(!menuExit)
	{
		dwHandler();
		
		if(BIT_GET(KeyScan(), 5))
		{
			TouchKey = 1;
		}
		else
		{
			TouchKey = 0;
		}
		
		if(nextPage!=FUNCTION_SCRUBBER)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				ScrubberIntensity = 0;
				ScrubberMode = 0;
				ScrubberWorkFlag = 0;
			}
		}
		WorkTimeDeal();		
	}
	
	ULT_CON_PIN = 1;
	dwWaitRelease();
	dwHandler();
}

//ScrubberPWM
const u16 ScrubberModPeriod[] = {20, 300, 500};	//{100, 200, 500, 3500};	//{12, 200, 7000, 3500}
const u16 ScrubberModCompare[] = {10, 150, 250};	//{100, 100, 250, 1200};	//{3, 40, 6000, 1200} 
void ScrubberPWM(u8 i, u8 Work)
{
	static u16 ScrubberTimeCnt = 0; //ScrubberTimeCnt2 = 0;
	static u8 ScrubberModRenew = 0;
	
	if(Work)
	{
		if(++ScrubberTimeCnt>=ScrubberModPeriod[ScrubberModRenew])
		{
			ScrubberTimeCnt = 0;
			ScrubberModRenew = i;
		}
		if(ScrubberTimeCnt<ScrubberModCompare[ScrubberModRenew])
		{
			TIM_CtrlPWMOutputs(TIM1,ENABLE);
//			if(++ScrubberTimeCnt2>=12)
//			{
//				ScrubberTimeCnt2 = 0;
////			POWER_ON_PIN = 1;
//			}
//			if(ScrubberTimeCnt2<3)
//			{
//				TIM_CtrlPWMOutputs(TIM1,ENABLE);
////			POWER_ON_PIN = 1;
//			}
//			else
//			{
//				TIM_CtrlPWMOutputs(TIM1,DISABLE);
//			}
		}
		else
		{
			TIM_CtrlPWMOutputs(TIM1,DISABLE);	
//			POWER_ON_PIN = 0;		
//			ScrubberTimeCnt2 = 0;
		}			
	}
	else
	{
//		ScrubberTimeCnt2 = 0;
		ScrubberTimeCnt = 0;
//		POWER_ON_PIN = 0;	
		TIM_CtrlPWMOutputs(TIM1,DISABLE);	
	}
}

//ScrubberAdj
void PageScrubberAdj(void)
{
	ScrubberWorkFlag = 1;
	ScrubberMode = 0;
	ScrubberIntensity = 4;
	TouchKey = 1;
	menuExit = 0;
	
	STMFLASH_Read(FLASH_FREQ_ADDR, &ScrubberFrequency, 1);	
	if(ScrubberFrequency<201||ScrubberFrequency>300)
	{
		ScrubberFrequency = 250;
		STMFLASH_Write(FLASH_FREQ_ADDR, &ScrubberFrequency, 1);
	}	
	
	dwDisPicWithL(PIC_SCRUBBER_ADJ);	
	dwDisNum(DW_SIZE_48, 319,    260,ScrubberFrequency/100);
	dwDisNum(DW_SIZE_48, 319+24, 260,ScrubberFrequency/10%10);
	dwDisChar(DW_SIZE_48,319+48, 260,'.');
	dwDisNum(DW_SIZE_48, 319+72, 260,ScrubberFrequency%10);
	
	TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
	ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
	TIM_SetCompare1(TIM1,ScrubberPWMIntensity);	

	dwCancelKey();
	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(IntensityUpPres, 0, &btnScrubberAdjUp);
	dwListenKey(IntensityDownPres, 0, &btnScrubberAdjDown);	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_SCRUBBER_ADJ)
		{
			menuExit = 1;
			ScrubberWorkFlag = 0;
			ScrubberMode = 0;
			ScrubberIntensity = 0;
			STMFLASH_Write(FLASH_FREQ_ADDR, &ScrubberFrequency, 1);
			TouchKey = 0;
		}
	}
	
	dwWaitRelease();
	dwHandler();
}
/////////////////////////////////

//Ultrasound function
void PageUltrasound(void)
{
	u8 keyTemp = 0;
	
	menuExit = 0;
	WorkTime = 1800;
	dwDisPicWithL(PIC_ULTRASOUND);
	DisTextSize(DW_SIZE_24);
	DisSetAxis(600, 231);
	funDisTime(WorkTime);
	DisSetAxis(622, 321);
	DisNumber(UltrasoundIntensity, 1, 0, Dis_Dec);
	dwDisChar(DW_SIZE_24, 622, 411, UltrasoundMode+'A');	
	
	dwCancelKey();
//	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(SprayerPres, 0, &btnSprayer);
	dwListenKey(SetPres, 0, &btnSet);
	dwListenKey(TimeUpPres, TimeUpFree, &btnTimeUp);
	dwListenKey(TimeDownPres, TimeDownFree, &btnTimeDown);
	dwListenKey(IntensityUpPres, IntensityUpFree, &btnIntensityUp);
	dwListenKey(IntensityDownPres, IntensityDownFree, &btnIntensityDown);
	dwListenKey(ModeUpPres, ModeUpFree, &btnModeUp);
	dwListenKey(ModeDownPres, ModeDownFree, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
//	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(StandbyPres, StandbyFree, &btnStandby);	//Standby
	
	while(!menuExit)
	{
		dwHandler();
		if(BIT_GET(KeyScan(), 2))
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				StartPres();
			}
		}
		else
		{
			keyTemp = 0;
		}
		if(nextPage!=FUNCTION_ULTRASOUND)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				UltrasoundIntensity = 0;
				UltrasoundMode = 0;
				UltrasoundWorkFlag = 0;
			}
		}
		WorkTimeDeal();		
	}
	
	dwWaitRelease();
	dwHandler();
}

//UltrasoundPWM
const u8 UltrasoundIntensityTable[] = {0, 1, 2, 3, 4, 6};
const u16 UltrasoundModPeriod[] = {1000, 510, 1020};
const u16 UltrasoundModCompare[] = {1000, 252, 504};
void UltrasoundPWM(u8 i, u8 Work)
{
	static u16 UltrasoundTimeCnt = 0; 
	static u16 UltrasoundTimeCnt2 = 0;
	static u8 UltrasoundModRenew = 0, UltrasoundModRenew2 = 0;
				
	if(Work)
	{
		//PWM1
		if(++UltrasoundTimeCnt>=UltrasoundModPeriod[UltrasoundModRenew])
		{
			UltrasoundTimeCnt = 0;
			UltrasoundModRenew = i;
		}
		if(UltrasoundTimeCnt<UltrasoundModCompare[UltrasoundModRenew])				
		{
			//PWM2
			if(++UltrasoundTimeCnt2>=6)
			{
				UltrasoundTimeCnt2 = 0;
				UltrasoundModRenew2 = UltrasoundIntensity;
			}
			if(UltrasoundTimeCnt2<UltrasoundIntensityTable[UltrasoundModRenew2])
				ULT_PIN = 1;	//Ultrasound open
			else
				ULT_PIN = 0;	//Ultrasound close
		}
		else
		{
			ULT_PIN = 0;	//Ultrasound close
			UltrasoundTimeCnt2 = 0;
		}	
	}
	else
	{
		UltrasoundTimeCnt = 0;
		UltrasoundTimeCnt2 = 0;
		ULT_PIN = 0;	//Ultrasound close
	}
}
//////////////////////////////////////////////////

//RF function
u8 rfKeyTemp = 0;
void PageRF(void)
{
	u8 keyTemp = 0;
//	u8 tt = 0;
	
	menuExit = 0;
	WorkTime = 1800;
	dwDisPicWithL(PIC_RF);
	DisTextSize(DW_SIZE_24);
	DisSetAxis(600, 231);
	funDisTime(WorkTime);
	DisSetAxis(622, 321);
	DisNumber(RFIntensity, 1, 0, Dis_Dec);
	dwDisChar(DW_SIZE_24, 622, 411, RFMode+'A');	
	
	dwCancelKey();
//	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(SprayerPres, 0, &btnSprayer);
	dwListenKey(SetPres, 0, &btnSet);
	dwListenKey(TimeUpPres, TimeUpFree, &btnTimeUp);
	dwListenKey(TimeDownPres, TimeDownFree, &btnTimeDown);
	dwListenKey(IntensityUpPres, IntensityUpFree, &btnIntensityUp);
	dwListenKey(IntensityDownPres, IntensityDownFree, &btnIntensityDown);
	dwListenKey(ModeUpPres, ModeUpFree, &btnModeUp);
	dwListenKey(ModeDownPres, ModeDownFree, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
//	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(StandbyPres, StandbyFree, &btnStandby);	//Standby
	
	while(!menuExit)
	{
		dwHandler();
//		if(RFWorkFlag!=1)
		{
			if(BIT_GET(KeyScan(), 1))
			{
				if(keyTemp==0)
				{
					keyTemp = 1;
					StartPres();
				}
			}
			else
			{
				keyTemp = 0;
			}
		}
//		else
//		{
//			if(rfKeyTemp)
//			{
//				if(tt==0)
//				{
//					tt = 1;
//					StartPres();
//				}
//			}
//			else
//			{
//				tt = 0;
//			}
//		}
		if(nextPage!=FUNCTION_RF)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				RFIntensity = 0;
				RFMode = 0;
				RFWorkFlag = 0;
			}
		}
		WorkTimeDeal();		
	}
	
	RF_CON_PIN = 1;
	dwWaitRelease();
	dwHandler();
}

//RF PWM contral
const u16 RFIntensityTable[] = {0, 12, 23, 28, 35, 43};
const u16 RFModPeriod[] = {300, 510, 1020};
const u16 RFModCompare[] = {300, 252, 504};
void RFPWM(u8 i, u8 Work)
{
	static u16 RFTimeCnt = 0; 
	static u8 RFModRenew = 0;
	static u8 RFCnt = 0;
				
	if(Work)
	{	
		//PWM1
		if(++RFTimeCnt>=RFModPeriod[RFModRenew])
		{
			RFTimeCnt = 0;
			RFModRenew = i;
		}
		if(RFTimeCnt<RFModCompare[RFModRenew])				
		{
			RFCnt = 0;
			TIM_SetCompare4(TIM3, RFIntensityTable[RFIntensity]);
		}
		else
		{
			TIM_SetCompare4(TIM3, 0);
//			if(RF_BUTTON_PIN==0)
//			{
//				if(++RFCnt>=20)
//				{
//					RFCnt = 20;
//					rfKeyTemp = 1;
//				}
//			}
//			else
//			{
//				if(--RFCnt<=0)
//				{
//					RFCnt = 1;
//					rfKeyTemp = 0;
//				}
//			}
		}	
	}
	else
	{
		rfKeyTemp = 0;
		RFCnt = 0;
		RFTimeCnt = 0;
		TIM_SetCompare4(TIM3, 0);
	}
}
/////////////////////////////////

//Ice function
void PageIce(void)
{
  u8 keyTemp = 0;
	
	menuExit = 0;
	WorkTime = 1800;
	dwDisPicWithL(PIC_ICE);
	DisTextSize(DW_SIZE_24);
	DisSetAxis(600, 318);
	funDisTime(WorkTime);	
	
	dwCancelKey();
//	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(SprayerPres, 0, &btnSprayer);
	dwListenKey(SetPres, 0, &btnSet);
	dwListenKey(TimeUpPres, TimeUpFree, &btnTimeUp_Clean);
	dwListenKey(TimeDownPres, TimeDownFree, &btnTimeDown_Clean);
	dwListenKey(StartPres, 0, &btnStart);
//	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(StandbyPres, StandbyFree, &btnStandby);	//Standby
	
	while(!menuExit)
	{
		dwHandler();
		if(BIT_GET(KeyScan(), 0))
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				StartPres();
			}
		}
		else
		{
			keyTemp = 0;
		}
		if(nextPage!=FUNCTION_ICE)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				ColdMode = 0;
				ColdWorkFlag = 0;
			}
		}
		WorkTimeDeal();	
		
		if(ColdWorkFlag)
		{
			POWER_ON_PIN = 1;
			if(ColdMode)
				COLD_HOT_PIN = 1;
			else
				COLD_HOT_PIN = 0;
		}
		else		
		{
			COLD_CON_PIN = 1;
			COLD_HOT_PIN = 0;
			POWER_ON_PIN = 0;
		}
	} 
	
	dwWaitRelease();
	dwHandler();
}
///////////////////////////////////

//O2 function
static const u8 RColor[] = {15, 7, 0, 0, 0, 11};
static const u8 GColor[] = {0, 10, 0, 9, 15, 0};
static const u8 BColor[] = {0, 0, 15, 2, 0, 4};
void PageO2(void)
{	
	menuExit = 0;
	WorkTime = 1800;
	dwDisPicWithL(PIC_O2);
	DisTextSize(DW_SIZE_24);
	DisSetAxis(600, 278);
	funDisTime(WorkTime);
	dwDisChar(DW_SIZE_24, 622, 364, O2Mode+'A');	
	
	dwCancelKey();
//	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(SprayerPres, 0, &btnSprayer);
	dwListenKey(SetPres, 0, &btnSet);
	dwListenKey(TimeUpPres, TimeUpFree, &btnTimeUp_O2);
	dwListenKey(TimeDownPres, TimeDownFree, &btnTimeDown_O2);
	dwListenKey(ModeUpPres, ModeUpFree, &btnModeUp_O2);
	dwListenKey(ModeDownPres, ModeDownFree, &btnModeDown_O2);
	dwListenKey(StartPres, 0, &btnStart);
//	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(StandbyPres, StandbyFree, &btnStandby);	//Standby
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_O2)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				O2Mode = 0;
				O2WorkFlag = 0;
			}
		}
		WorkTimeDeal();
	}
	
	dwWaitRelease();
	dwHandler();
}

//O2 PWM
void O2PWM(u8 i, u8 Work)
{
	static u16 O2Cnt10s = 0; 
	static u8 temp = 0, AutoColor = 0;
	static u16 O2Delay = 0;
	
	if(Work)
	{
		PUMP220_PIN = 1;
		switch(i)
		{
			case 0: {RGBValue = 0xF800; break;}	//RED
			case 1: {RGBValue = 0x07E0; break;}	//GREEN
			case 2: {RGBValue = 0x001F; break;}	//BLUE
			case 3:	//gradually change
			{
				if(++O2Delay>=3000)
				{
					O2Delay = 0;
					RGBValue = ((u16)RColor[AutoColor]<<11)|((u16)GColor[AutoColor]<<5)|(u16)BColor[AutoColor];
					if(++AutoColor>5)
						AutoColor = 0;
//					if(RGBValue>0x07E0)
//					{
//						if(RGBValue>=0xF800)
//							RGBValue = 1;
//						else
//							RGBValue = ((RGBValue>>11)+1)<<11;
//					}
//					else if(RGBValue>0x001F)
//						RGBValue = ((RGBValue>>5)+1)<<5;
//					else 
//						RGBValue++;
				}
				break;
			}
			default: {break;}
		}
		RGB_Decode(&RGBValue);	
		if(++O2Cnt10s>=10000)
		{
			O2Cnt10s = 0;
			temp = !temp;
		}
		if(temp)
		{	
			O2_VALVE1_PIN = 0;	//off
			O2_VALVE2_PIN = 1;	//on
		}
		else
		{
			O2_VALVE1_PIN = 1;	//on
			O2_VALVE2_PIN = 0;	//off
		}
	}
	else
	{
		O2_VALVE1_PIN = 0;	//off
		O2_VALVE2_PIN = 0;	//off
		PUMP220_PIN = 0;
		LED_RED_PIN = 0;
		LED_GREEN_PIN = 0;
		LED_BLUE_PIN = 0;
	}
}
////////////////////////////////////////////////////

//Clean function
void PageClean(void)
{
	u8 keyTemp = 0, keyTemp2 = 0;
	menuExit = 0;
	WorkTime = 1800;
	dwDisPicWithL(PIC_CLEAN);	
	DisTextSize(DW_SIZE_24);
	DisSetAxis(600, 318);
	funDisTime(WorkTime);
	BottleASelect();
	
	dwCancelKey();
//	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(SprayerPres, 0, &btnSprayer);
	dwListenKey(SetPres, 0, &btnSet);
	dwListenKey(TimeUpPres, TimeUpFree, &btnTimeUp_Clean);
	dwListenKey(TimeDownPres, TimeDownFree, &btnTimeDown_Clean);
	dwListenKey(StartPres, 0, &btnStart);
//	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(BottleASelect, 0, &btnBottleA);
	dwListenKey(BottleBSelect, 0, &btnBottleB);
	dwListenKey(BottleCSelect, 0, &btnBottleC);
	dwListenKey(BottleDSelect, 0, &btnBottleD);
	dwListenKey(StandbyPres, StandbyFree, &btnStandby);	//Standby
	
	while(!menuExit)
	{
		dwHandler();
		if(BIT_GET(KeyScan(), 4))	//handle start key
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				StartPres();
			}
		}
		else
		{
			keyTemp = 0;
		}
		if(BIT_GET(KeyScan(), 6))	//water full
		{
			if((keyTemp2==0)||CleanWorkFlag)
			{
				keyTemp2 = 1;
				dwCutPic(PIC_EFFECT, 595, 0, 799, 62, 595, 0);
				CleanWorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
				INLINE_MUSIC_WATER_WARN();
			}
		}
		else
		{
			if(keyTemp2)
			{
				keyTemp2 = 0;
				dwCutPic(PIC_CLEAN, 595, 0, 799, 62, 595, 0);
			}
		}
		if(nextPage!=FUNCTION_CLEAN)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				CleanMode = 0;
				CleanWorkFlag = 0;
			}
		}
		WorkTimeDeal();
		if(CleanWorkFlag)
		{
			PEN_CHANNEL_PIN = 1;
			PEN_CON_PIN = 0;
			switch(CleanMode)
			{
				case 1: {BOTTLE1_PIN = 1; BOTTLE2_PIN = 0; BOTTLE3_PIN = 0; BOTTLE4_PIN = 0; break;}
				case 2: {BOTTLE1_PIN = 0; BOTTLE2_PIN = 1; BOTTLE3_PIN = 0; BOTTLE4_PIN = 0; break;}
				case 3: {BOTTLE1_PIN = 0; BOTTLE2_PIN = 0; BOTTLE3_PIN = 1; BOTTLE4_PIN = 0; break;}
				case 4: {BOTTLE1_PIN = 0; BOTTLE2_PIN = 0; BOTTLE3_PIN = 0; BOTTLE4_PIN = 1; break;}
				default: {break;}
			}
		}
		else
		{
			PEN_CHANNEL_PIN = 0;
			PEN_CON_PIN = 1;
			BOTTLE1_PIN = 0;
			BOTTLE2_PIN = 0;
			BOTTLE3_PIN = 0;
			BOTTLE4_PIN = 0;
		}
	}
	
	dwWaitRelease();
	dwHandler();
}
//////////////////////////////////////////////

//BIO1 function
void PageBIO1(void)
{
	u8 keyTemp = 0;
	
	menuExit = 0;
	WorkTime = 1800;
	dwDisPicWithL(PIC_BIO1);
	DisTextSize(DW_SIZE_24);
	DisSetAxis(600, 231);
	funDisTime(WorkTime);
	DisSetAxis(622, 321);
	DisNumber(BIO1Intensity, 1, 0, Dis_Dec);
	dwDisChar(DW_SIZE_24, 622, 411, BIO1Mode+'A');	
	
	dwCancelKey();
//	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(SprayerPres, 0, &btnSprayer);
	dwListenKey(SetPres, 0, &btnSet);
	dwListenKey(TimeUpPres, TimeUpFree, &btnTimeUp);
	dwListenKey(TimeDownPres, TimeDownFree, &btnTimeDown);
	dwListenKey(IntensityUpPres, IntensityUpFree, &btnIntensityUp);
	dwListenKey(IntensityDownPres, IntensityDownFree, &btnIntensityDown);
	dwListenKey(ModeUpPres, ModeUpFree, &btnModeUp);
	dwListenKey(ModeDownPres, ModeDownFree, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
//	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(StandbyPres, StandbyFree, &btnStandby);	//Standby
	
	while(!menuExit)
	{
		dwHandler();
		if(BIT_GET(KeyScan(), 1))
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				StartPres();
			}
		}
		else
		{
			keyTemp = 0;
		}
		
		if(nextPage!=FUNCTION_BIO1)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				BIO1Intensity = 0;
				BIO1Mode = 0;
				BIO1WorkFlag = 0;
			}
		}
		
		WorkTimeDeal();		
	}
	TIM_SetCompare4(TIM4, BIO1IntensityTable[0]);
	RF_CON_PIN = 1;
	dwWaitRelease();
	dwHandler();
}

//BIO1 PWM
const u16 BIO1ModPeriod[] = {167, 250, 400};
const u16 BIO1ModCompare[] = {84, 140, 240};	
void BIO1PWM(u8 i, u8 Work)
{
	static u16 BIO1TimeCnt = 0;
	static u8 BIO1ModRenew = 0;
	
	if(Work)
	{
		if(++BIO1TimeCnt>=BIO1ModPeriod[BIO1ModRenew])
		{
			BIO1TimeCnt = 0;
			BIO1ModRenew = i;
		}
		if((BIO1TimeCnt<BIO1ModCompare[BIO1ModRenew]))
			TIM_SetCompare3(TIM4, 18000);	//PWM_BIO
		else
			TIM_SetCompare3(TIM4, 0);	//PWM_BIO
	}
	else
	{
		BIO1TimeCnt = 0;
		TIM_SetCompare3(TIM4, 0);	//PWM_BIO
	}
}
/////////////////////////////////////////////////////////

//Sprayer
void PageSprayer(void)
{
	u8 keyTemp = 0;
	menuExit = 0;
	WorkTime = 1800;
	dwDisPicWithL(PIC_SPRAYER);
	DisTextSize(DW_SIZE_24);
	DisSetAxis(600, 318);
	funDisTime(WorkTime);	
	
	dwCancelKey();
//	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(SprayerPres, 0, &btnSprayer);
	dwListenKey(SetPres, 0, &btnSet);
	dwListenKey(TimeUpPres, TimeUpFree, &btnTimeUp_Clean);
	dwListenKey(TimeDownPres, TimeDownFree, &btnTimeDown_Clean);
	dwListenKey(StartPres, 0, &btnStart);
//	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(StandbyPres, StandbyFree, &btnStandby);	//Standby
		
	while(!menuExit)
	{
		dwHandler();
		if(BIT_GET(KeyScan(), 3))
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				StartPres();
			}
		}
		else
		{
			keyTemp = 0;
		}
		if(nextPage!=FUNCTION_SPRAYER)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				SprayerWorkFlag = 0;
			}
		}
		WorkTimeDeal();	
		
		if(SprayerWorkFlag)
		{
			GUN_CHANNEL_PIN = 1;
		}
		else
		{
			GUN_CON_PIN = 1;
			GUN_CHANNEL_PIN = 0;
		}
	} 
	
	dwWaitRelease();
	dwHandler();
}

//Handle Key scan
u8 KeyDelay = 0;
u8 KeyScan(void)
{
	static u8 ColdCnt = 0, RFCnt = 0, UltCnt = 0, GunCnt = 0, PenCnt = 0, TPCnt = 0, WaterCnt = 0;
	static u8 ReValue = 0;
	
	if(KeyDelay)
	{
		KeyDelay = 0;
		if(COLD_BUTTON_PIN==0)
		{
			if(++ColdCnt>=20)
			{
				ColdCnt = 20;
				BIT_SET(ReValue, 0);
			}
		}
		else
		{
			if(--ColdCnt<=0)
			{
				ColdCnt = 1;
				BIT_CLEAR(ReValue, 0);
			}
		}
		
		if(RF_BUTTON_PIN==0)
		{
			if(++RFCnt>=20)
			{
				RFCnt = 20;
				BIT_SET(ReValue, 1);
			}
		}
		else
		{
			if(--RFCnt<=0)
			{
				RFCnt = 1;
				BIT_CLEAR(ReValue, 1);
			}
		}

		if(ULT_BUTTON_PIN==0)
		{
			if(++UltCnt>=20)
			{
				UltCnt = 20;
				BIT_SET(ReValue, 2);
			}
		}
		else
		{
			if(--UltCnt<=0)
			{
				UltCnt = 1;
				BIT_CLEAR(ReValue, 2);
			}
		}
		
		if(GUN_BUTTON_PIN==0)
		{
			if(++GunCnt>=20)
			{
				GunCnt = 20;
				BIT_SET(ReValue, 3);
			}
		}
		else
		{
			if(--GunCnt<=0)
			{
				GunCnt = 1;
				BIT_CLEAR(ReValue, 3);
			}
		}
		
		if(PEN_BUTTON_PIN==0)
		{
			if(++PenCnt>=20)
			{
				PenCnt = 20;
				BIT_SET(ReValue, 4);
			}
		}
		else
		{
			if(--PenCnt<=0)
			{
				PenCnt = 1;
				BIT_CLEAR(ReValue, 4);
			}
		}
		
		if(TOUCH_BUTTON_PIN)
		{
			if(++TPCnt>=20)
			{
				TPCnt = 50;
				BIT_SET(ReValue, 5);
			}
		}
		else
		{
			if(--TPCnt<=0)
			{
				TPCnt = 1;
				BIT_CLEAR(ReValue, 5);
			}
		}
		
		if(WATER_LEVEL_PIN)
		{
			if(++WaterCnt>=100)
			{
				WaterCnt = 100;
				BIT_SET(ReValue, 6);
			}
		}
		else
		{
			if(--WaterCnt<=0)
			{
				WaterCnt = 1;
				BIT_CLEAR(ReValue, 6);
			}
		}
	}
	return ReValue;
}

///////////////////////////////////////////////////////
//Bee function
const u16 BeeModPeriod[] = {110, 410, 1000}; 
const u16 BeeModCompare[] = {100, 400, 500};
u8 BeeMod = 0, BeeTime = 0;
void BeeFunction(void)
{	
	static u16 BeeTimeCnt = 0;

	if(BeeTime>0)
	{
		if(++BeeTimeCnt>=BeeModPeriod[BeeMod])
		{
			BeeTimeCnt = 0;
			BeeTime--;
		}
		else
		{
			if(BeeTimeCnt>=BeeModCompare[BeeMod])
				SOUND_PIN = 0;
			else
				SOUND_PIN = 1;		
		}	
	}
	else
	{
		SOUND_PIN = 0;
	}
}

//Rec and process frame
u8 buff2[5];
u8 rx2Flag = 0;
void commRx2Handler(u8 byte)
{
  static u8 count = 0;
  
  if(count == 0){
    if(byte == FRAME_HEAD){
      count++;
    }
  }else if(count < 4){
    buff2[count-1] = byte;
    count++;
  }else{
    count = 0;
    if(byte == FRAME_END){
			rx2Flag = 1;
    }
  }
}

u8 buff1[5];
u8 rx1Flag = 0;
void commRx1Handler(u8 byte)
{
  static u8 count = 0;
  
  if(count == 0){
    if(byte == FRAME_HEAD){
      count++;
    }
  }else if(count < 4){
    buff1[count-1] = byte;
    count++;
  }else{
    count = 0;
    if(byte == FRAME_END){
			rx1Flag = 1;
    }
  }
}

//RGB565 decode
static void RGB_PWM(u8 *i, u8 *p, u8 *g)
{
	static u8 _TimeCnt = 0;
	static u8 _RModRenew = 0, _GModRenew = 0, _BModRenew = 0;
	
	if(++_TimeCnt>=16)
	{
		_TimeCnt = 0;
		_RModRenew = *i;
		_GModRenew = *p;
		_BModRenew = *g;
	}
	
	//red
	if(_TimeCnt<_RModRenew)
	{	//OPEN
		LED_RED_PIN = 1;	
	}
	else	//COLSE
	{
		LED_RED_PIN = 0;
	}
	
	//green
	if(_TimeCnt<_GModRenew)
	{	//OPEN
		LED_GREEN_PIN = 1;	
	}
	else	//COLSE
	{
		LED_GREEN_PIN = 0;
	}
	
	//blue
	if(_TimeCnt<_BModRenew)
	{	//OPEN
		LED_BLUE_PIN = 1;	
	}
	else	//COLSE
	{
		LED_BLUE_PIN = 0;
	}
}

void RGB_Decode(const u16 *i)
{
	u8 R, G, B;
	
	R = *i>>11;
	G = (*i&0x07E0)>>5;
	B = *i&0x001F;
	
	RGB_PWM(&R, &G, &B);
}

