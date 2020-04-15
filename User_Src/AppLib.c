#include "AppLib.h"

//#define DEBUG_WDG
#define SCR_T_ON 0.2	//Scrubber PWM(Timer1) compare value
#define FRAME_HEAD 0xAA
#define FRAME_END 0x55

#define TIME_ADDRESS 0X1B00
#define NUM_ADDRESS 0X1B08
#define MODE_ADDRESS 0X1B04
#define FRE_ADDRESS 0X1B0C
#define START_ADDRESS 0X1001
#define BOTTLE_ADDRESS 0X1006
#define LEVEL_ADDRESS 0X100E

//const u16 CoolIntensity[] = {0,1700,1800,1900,2000,2175,2275,2375,2475,2550,2625};	
								//2.2 2.4 2.6 2.8 3.0 3.2 3.4 3.6 3.8 4.0 
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
const float ScrubberPWMTable[] = {0, 0.1, 0.13, 0.16, 0.18, 0.2, 0.22, 0.24, 0.27, 0.3, 0.35};
u16 ScrubberPWMIntensity = 0;
//const u16 BIO1IntensityTable[] = {36000, 29000, 28000, 28500, 27000, 26500, 26000,
//									25000, 24500, 24000, 23500};
const u16 BIO1IntensityTable[] = {36000, 28000, 27000, 26500, 26000,
																	25000, 24500, 24000, 23500, 23000, 22500};
//{36000, 20000, 10000, 5000, 0, 0};
//{36000, 20000, 15000, 10000, 5000, 0};
u16 RGBValue = 0;
static u8 keyPrintBack = 0;

//Button
static const u16 btnEnter = 0X0031;
static const u16 btnClean = 0X000A;
static const u16 btnSprayer = 0X0009;
static const u16 btnO2 = 0X0004;
static const u16 btnScrubber = 0X0007;
static const u16 btnUltrasound = 0X0005;
static const u16 btnRF = 0X0006;
static const u16 btnIce = 0X0008;
static const u16 btnBIO1 = 0X0006;
static const u16 btnTimeUp = 0X000E;
static const u16 btnTimeDown = 0X000D;
static const u16 btnIntensityUp = 0X0012;
static const u16 btnIntensityDown = 0X0011;
static const u16 btnModeUp = 0X0010;
static const u16 btnModeDown = 0X000F;
static const u16 btnStart = 0X0016;
static const u16 btnScrubberAdj = 0X0021;
static const u16 btnScrubberAdjUp = 0X0013;
static const u16 btnScrubberAdjDown = 0X0014;
static const u16 btnSet = 0X000C;
static const u16 btnChinse = 0X0002;
static const u16 btnEnglish = 0X0003;
static const u16 btnMin = 0X0015;
static const u16 btnBottleA = 0X0017;
static const u16 btnBottleB = 0X0018;
static const u16 btnBottleC = 0X0019;
static const u16 btnBottleD = 0X001A;
static const u16 btnStandby = 0X000B;
static const u16 btnCamBack = 0X0055;
static const u16 btnCam = 0X0056;
static const u16 btnPrintBack = 0X0057;

/*language select*/
static void menuChinsePres(void)
{
	u16 i = 0;
	
	INLINE_MUSIC_BUTTON();
	dwD2SetLanguage(LANGUAGE_CHINESE);
	
	nextPage = FUNCTION_MAIN;
	STMFLASH_Write(FLASH_LANG_ADDR, &i, 1);		//Write language config
}

static void menuEnglishPres(void)
{
	u16 i = 123;
	
	INLINE_MUSIC_BUTTON();
	dwD2SetLanguage(LANGUAGE_ENGLISH);
	
	nextPage = FUNCTION_MAIN;
	STMFLASH_Write(FLASH_LANG_ADDR, &i, 1);		//Write language config
}

static void StandbyPres(void)
{
	INLINE_MUSIC_STANDBY();
	nextPage = FUNCTION_RESTART;
	dwD2SetBL(0X03, 0X03, 0XFFFF);	//change display light
	PUMP24_PIN = 0;	//Close front LED
}

static void CamPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_CAM;
	CAM_CON_PIN = 1;
}

static void CamBackPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_MAIN;
	CAM_CON_PIN = 0;
}

static void PrintBackPres(void)
{
	INLINE_MUSIC_BUTTON();
	keyPrintBack = 0;
	GUN_CON_PIN = 1;
	
	//Open Camera
	dwD2DisPicNoL(PIC_CAMERA);
	dwD2CameraOpen();
}

static void ScrubberPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_SCRUBBER;
}

static void O2Pres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_O2;
}

static void RFPres(void)
{
	INLINE_MUSIC_BUTTON();
//	nextPage = FUNCTION_RF;
	nextPage = FUNCTION_BIO1;
}

static void SetPres(void)
{
	INLINE_MUSIC_BUTTON();
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
	nextPage = FUNCTION_CLEAN;
}

static void UltrasoundPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_ULTRASOUND;
}

static void IcePres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_ICE;
}

static void BIO1Pres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_BIO1;	
}

static void SprayerPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_SPRAYER;
}

static void IntensityUpPres(void)
{
	INLINE_MUSIC_BUTTON();
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberIntensity<10)
				ScrubberIntensity++;
			
			dwD2DisNum(NUM_ADDRESS, ScrubberIntensity);
			ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
			TIM_SetCompare1(TIM1,ScrubberPWMIntensity);
			break;
		}
		case FUNCTION_SCRUBBER_ADJ:
		{
			if(ScrubberFrequency<300)
			{
				ScrubberFrequency++;

				dwD2DisFre(FRE_ADDRESS, ScrubberFrequency);		

				TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
				ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
				TIM_SetCompare1(TIM1, ScrubberPWMIntensity);
			}
			break;
		}
		case FUNCTION_BIO1:
		{
			if(BIO1Intensity<10)
				BIO1Intensity++;
			dwD2DisNum(NUM_ADDRESS, BIO1Intensity);
			TIM_SetCompare4(TIM4, BIO1IntensityTable[BIO1Intensity]);
			break;
		}
		case FUNCTION_ULTRASOUND:
		{
			if(UltrasoundIntensity<10)
				UltrasoundIntensity++;
			dwD2DisNum(NUM_ADDRESS, UltrasoundIntensity);
			break;
		}
		case FUNCTION_RF:
		{
			if(RFIntensity<4)
				RFIntensity++;
			dwD2DisNum(NUM_ADDRESS, RFIntensity);
			break;
		}
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
			
			dwD2DisNum(NUM_ADDRESS, ScrubberIntensity);
			ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
			TIM_SetCompare1(TIM1,ScrubberPWMIntensity);
			break;
		}
		case FUNCTION_SCRUBBER_ADJ:
		{
			if(ScrubberFrequency>201)
			{
				ScrubberFrequency--;
				dwD2DisFre(FRE_ADDRESS, ScrubberFrequency);	
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
			dwD2DisNum(NUM_ADDRESS, BIO1Intensity);
			TIM_SetCompare4(TIM4, BIO1IntensityTable[BIO1Intensity]);
			break;
		}
		case FUNCTION_ULTRASOUND:
		{
			if(UltrasoundIntensity>0)
				UltrasoundIntensity--;
			dwD2DisNum(NUM_ADDRESS, UltrasoundIntensity);
			break;
		}
		case FUNCTION_RF:
		{
			if(RFIntensity>0)
				RFIntensity--;
			dwD2DisNum(NUM_ADDRESS, RFIntensity);
			break;
		}
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

	dwD2DisTime(TIME_ADDRESS, WorkTime);
}

static void TimeDownPres(void)
{
	INLINE_MUSIC_BUTTON();

	if(WorkTime>60)
	{
		WorkTime -= 60;
	}

	dwD2DisTime(TIME_ADDRESS, WorkTime);
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
			
			dwD2DisChar(MODE_ADDRESS, ScrubberMode);
			break;
		}
		case FUNCTION_BIO1: 
		{
			if(BIO1Mode<2)
				BIO1Mode++;
			dwD2DisChar(MODE_ADDRESS, BIO1Mode);
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			if(UltrasoundMode<2)
				UltrasoundMode++;
			dwD2DisChar(MODE_ADDRESS, UltrasoundMode);
			break;
		}
		case FUNCTION_RF: 
		{
			if(RFMode<2)
				RFMode++;
			dwD2DisChar(MODE_ADDRESS, RFMode);
			break;
		}
		case FUNCTION_ICE: 
		{
			if(ColdMode<1)
				ColdMode++;
			dwD2DisChar(MODE_ADDRESS, ColdMode);
			break;
		}
		case FUNCTION_O2: 
		{
			if(O2Mode<4)
				O2Mode++;

			if (O2Mode == 0)
				dwD2DisChar(MODE_ADDRESS, 61);
			else
				dwD2DisChar(MODE_ADDRESS, O2Mode-1);
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
			dwD2DisChar(MODE_ADDRESS, ScrubberMode);
			break;
		}
		case FUNCTION_BIO1: 
		{
			if(BIO1Mode>0)
				BIO1Mode--;
			dwD2DisChar(MODE_ADDRESS, BIO1Mode);
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			if(UltrasoundMode>0)
				UltrasoundMode--;
			dwD2DisChar(MODE_ADDRESS, UltrasoundMode);
			break;
		}
		case FUNCTION_RF: 
		{
			if(RFMode>0)
				RFMode--;
			dwD2DisChar(MODE_ADDRESS, RFMode);
			break;
		}
		case FUNCTION_ICE: 
		{
			if(ColdMode>0)
				ColdMode--;
			dwD2DisChar(MODE_ADDRESS, ColdMode);
			break;
		}
		case FUNCTION_O2: 
		{
			if(O2Mode>0)
				O2Mode--;

			if (O2Mode == 0)
				dwD2DisChar(MODE_ADDRESS, 61);
			else
				dwD2DisChar(MODE_ADDRESS, O2Mode-1);
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
				dwD2DisICO(START_ADDRESS, ScrubberWorkFlag);
			}
			else
			{
				ScrubberWorkFlag = 1;
				dwD2DisICO(START_ADDRESS, ScrubberWorkFlag);
			}
			break;
		}
		case FUNCTION_BIO1: 
		{
			if(BIO1WorkFlag)
			{						
				BIO1WorkFlag = 0;
				RF_CON_PIN = 1;
				dwD2DisICO(START_ADDRESS, BIO1WorkFlag);
			}
			else
			{
				BIO1WorkFlag = 1;
				RF_CON_PIN = 0;
				dwD2DisICO(START_ADDRESS, BIO1WorkFlag);
			}
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			if(UltrasoundWorkFlag)
			{				
				UltrasoundWorkFlag = 0;
				ULT_CON_PIN = 1;
				dwD2DisICO(START_ADDRESS, UltrasoundWorkFlag);
			}
			else
			{
				UltrasoundWorkFlag = 1;
				ULT_CON_PIN = 0;
				dwD2DisICO(START_ADDRESS, UltrasoundWorkFlag);
			}
			break;
		}
		case FUNCTION_RF: 
		{
			if(RFWorkFlag)
			{				
				RFWorkFlag = 0;
				RF_CON_PIN = 1;
				dwD2DisICO(START_ADDRESS, RFWorkFlag);
			}
			else
			{
				RFWorkFlag = 1;
				RF_CON_PIN = 0;
				dwD2DisICO(START_ADDRESS, RFWorkFlag);
			}
			break;
		}
		case FUNCTION_ICE: 
		{
			if(ColdWorkFlag)
			{				
				ColdWorkFlag = 0;
				COLD_CON_PIN = 1;
				dwD2DisICO(START_ADDRESS, ColdWorkFlag);
			}
			else
			{
				ColdWorkFlag = 1;
				COLD_CON_PIN = 0;
				dwD2DisICO(START_ADDRESS, ColdWorkFlag);
			}
			break;
		}
		case FUNCTION_O2: 
		{
			if(O2WorkFlag)
			{				
				O2WorkFlag = 0;
				dwD2DisICO(START_ADDRESS, O2WorkFlag);
			}
			else
			{
				O2WorkFlag = 1;
				dwD2DisICO(START_ADDRESS, O2WorkFlag);
			}
			break;
		}
		case FUNCTION_CLEAN: 
		{
			if(CleanWorkFlag)
			{				
				CleanWorkFlag = 0;
				PEN_CON_PIN = 1;
				dwD2DisICO(START_ADDRESS, CleanWorkFlag);
			}
			else
			{
				CleanWorkFlag = 1;
				PEN_CON_PIN = 0;
				dwD2DisICO(START_ADDRESS, CleanWorkFlag);
			}
			break;
		}
		case FUNCTION_SPRAYER: 
		{
			if(SprayerWorkFlag)
			{				
				SprayerWorkFlag = 0;
				// GUN_CON_PIN = 1;
				dwD2DisICO(START_ADDRESS, SprayerWorkFlag);
			}
			else
			{
				SprayerWorkFlag = 1;
				// GUN_CON_PIN = 0;
				dwD2DisICO(START_ADDRESS, SprayerWorkFlag);
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
			dwD2DisICO(START_ADDRESS, 0);
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
	dwD2DisPicNoL(PIC_START_ANIMATION);
	dwD2SetBL(0X64, 0X64, 0XFFFF);	//change display light
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
	CleanMode = 0;
	dwD2DisICO(BOTTLE_ADDRESS, CleanMode);
}

static void BottleBSelect(void)
{
	INLINE_MUSIC_BUTTON();
	CleanMode = 1;
	dwD2DisICO(BOTTLE_ADDRESS, CleanMode);
}

static void BottleCSelect(void)
{
	INLINE_MUSIC_BUTTON();
	CleanMode = 2;
	dwD2DisICO(BOTTLE_ADDRESS, CleanMode);
}

static void BottleDSelect(void)
{
	INLINE_MUSIC_BUTTON();
	CleanMode = 3;
	dwD2DisICO(BOTTLE_ADDRESS, CleanMode);
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
				// WorkTime--;
			}
			else
			{	
				WorkTime = 1800;
				StartPres();
			}
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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_3|GPIO_Pin_4| \
									GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_3|
					GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//Power up 
void PageRestartAnimation(void)
{	
	if(RestoreFlag()==1)	//Check restore reason
	{
		INLINE_MUSIC_ERROR()
	}
	
	menuExit = 0;
	dwD2DisPicNoL(0);
	dwD2CancelKey();
	dwD2ListenKey(EnterPres, btnEnter);
	while(!menuExit)
	{
		dwD2Handler();
	}
	
	delay_ms(300);
}
////////////////////////////////

//select language
void PageLanguage(void)
{
	menuExit = 0;
	
	dwD2DisPicNoL(PIC_LANGUAGE);
	dwD2CancelKey();
	dwD2ListenKey(menuChinsePres, btnChinse);
	dwD2ListenKey(menuEnglishPres, btnEnglish);

	while(!menuExit)
	{
		dwD2Handler();
		if(nextPage!=FUNCTION_LANGUAGE)
		{
			menuExit = 1;
		}
	}
}
//////////////////////////////

//Main page
void PageMain(void)
{
	menuExit = 0;
	
	while(!menuExit)
	{
		if(nextPage!=FUNCTION_MAIN)
		{
			menuExit = 1;
		}
	}
}
///////////////////////////////

//Scrubber function
void PageScrubber(void)
{
	menuExit = 0;
	WorkTime = 1800;	
	
	TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
	ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
	TIM_SetCompare1(TIM1,ScrubberPWMIntensity);

	while(!menuExit)
	{
		if(pad_agreement())
		{
			if(pad_rec_data[0]!=ADJ_COMMAND)
			{
				nextPage = pad_rec_data[1];
				if(nextPage==FUNCTION_SCRUBBER)
				{
					if(pad_rec_data[3])
						ScrubberWorkFlag = 1;
					else
						ScrubberWorkFlag = 0;
					if(pad_rec_data[0]==PARAMETER_COMMAND)
					{
						if(pad_rec_data[4]==0)
						{
							ScrubberIntensity = pad_rec_data[5];
							if(ScrubberIntensity<10)
							{
								ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
								TIM_SetCompare1(TIM1,ScrubberPWMIntensity);
							}
						}
						else
						{
							ScrubberMode = pad_rec_data[4];
							if(ScrubberMode>2)
								ScrubberMode = 2;
						}
					}
					else
					{
						ScrubberMode = pad_rec_data[4];
						if(ScrubberMode>2)
							ScrubberMode = 2;
						ScrubberIntensity = pad_rec_data[5];
						if(ScrubberIntensity<10)
						{
							ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
							TIM_SetCompare1(TIM1,ScrubberPWMIntensity);
						}
					}
					pad_clear();
				}
			}
			else
			{
				nextPage = FUNCTION_SCRUBBER_ADJ;
			}
		}
		
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
		}
		else
		{
			TIM_CtrlPWMOutputs(TIM1,DISABLE);	
		}			
	}
	else
	{
		ScrubberTimeCnt = 0;
		TIM_CtrlPWMOutputs(TIM1,DISABLE);	
	}
}

//ScrubberAdj
void PageScrubberAdj(void)
{
	u16 tempADJ = 0;
	ScrubberWorkFlag = 1;
	ScrubberMode = 0;
	ScrubberIntensity = 10;
	TouchKey = 1;
	menuExit = 0;
	
	STMFLASH_Read(FLASH_FREQ_ADDR, &ScrubberFrequency, 1);	
	if(ScrubberFrequency<201||ScrubberFrequency>300)
	{
		ScrubberFrequency = 250;
		STMFLASH_Write(FLASH_FREQ_ADDR, &ScrubberFrequency, 1);
	}	
	
	TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
	ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
	TIM_SetCompare1(TIM1,ScrubberPWMIntensity);	
	
	while(!menuExit)
	{
		if(pad_agreement())
		{
			if(pad_rec_data[0]!=ADJ_COMMAND)
			{
				nextPage = pad_rec_data[1];
			}
			else
			{
				if(pad_rec_data[3])
				{
					ScrubberWorkFlag = 1;
					tempADJ = ((u16)pad_rec_data[4]<<8)+pad_rec_data[5];
					if(tempADJ<=300&&tempADJ>=100)
					{
						ScrubberFrequency = tempADJ;
						TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
						ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
						TIM_SetCompare1(TIM1, ScrubberPWMIntensity);
					}
				}
				else
				{
					ScrubberWorkFlag = 0;
				}
				pad_clear();
			}
		}

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
}
/////////////////////////////////

//Ultrasound function
void PageUltrasound(void)
{
	u8 keyTemp = 0;
	
	menuExit = 0;
	WorkTime = 1800;
	
	while(!menuExit)
	{
		if(pad_agreement())
		{
			if(pad_rec_data[0]!=ADJ_COMMAND)
			{
				nextPage = pad_rec_data[1];
				if(nextPage==FUNCTION_ULTRASOUND)
				{
					if(pad_rec_data[3])
					{
						UltrasoundWorkFlag = 1;
						ULT_CON_PIN = 0;
					}
					else
					{
						UltrasoundWorkFlag = 0;
						ULT_CON_PIN = 1;
					}
					if(pad_rec_data[0]==PARAMETER_COMMAND)
					{
						if(pad_rec_data[4]==0)
						{
							UltrasoundIntensity = pad_rec_data[5];
							if(UltrasoundIntensity>10)
								UltrasoundIntensity = 10;
						}
						else
						{
							UltrasoundMode = pad_rec_data[4];
							if(UltrasoundMode>2)
								UltrasoundMode = 2;
						}
					}
					else
					{
						UltrasoundMode = pad_rec_data[4];
						if(UltrasoundMode>2)
							UltrasoundMode = 2;
						UltrasoundIntensity = pad_rec_data[5];
						if(UltrasoundIntensity>10)
							UltrasoundIntensity = 10;
					}
					pad_clear();
				}
			}
			else
			{
				nextPage = FUNCTION_SCRUBBER_ADJ;
			}
		}

		if(BIT_GET(KeyScan(), 2))
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				pad_send(&pad_key[0], sizeof(pad_key));
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
	
	ULT_CON_PIN = 1;
}

//UltrasoundPWM
const u8 UltrasoundIntensityTable[] = {0, 1, 2, 3, 4, 5, 6, 6, 7, 7, 8, 12};
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
			if(++UltrasoundTimeCnt2>=12)
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

//Camera function
void PageCAM(void)
{	
	u8 keyTemp = 0;
	
	menuExit = 0;
	
	//Open Camera
	dwD2DisPicNoL(PIC_CAMERA);
	dwD2CameraOpen();
	
	dwD2CancelKey();
	dwD2ListenKey(CamBackPres, btnCamBack);
	dwD2ListenKey(PrintBackPres, btnPrintBack);

	while(!menuExit)
	{
		dwD2Handler();
		
		if(BIT_GET(KeyScan(), 3))	//handle print key
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				if(keyPrintBack)
				{
					INLINE_MUSIC_BUTTON();
					keyPrintBack = 0;
					GUN_CON_PIN = 1;
					
					//Open Camera
					dwD2DisPicNoL(PIC_CAMERA);
					dwD2CameraOpen();
				}
				else
				{
					INLINE_MUSIC_BUTTON();
					GUN_CON_PIN = 0;
					keyPrintBack = 1;
					dwD2Print();
					delay_ms(625); //display processing
					GUN_CON_PIN = 1;
					delay_ms(625); 
					GUN_CON_PIN = 0;
					delay_ms(625);
					GUN_CON_PIN = 1;
					delay_ms(625); 
					GUN_CON_PIN = 0;
					dwD2DisPicNoL(PIC_PRINT); //display picture
					delay_ms(500); //let display respond				
				}
			}
		}
		else
		{
			keyTemp = 0;
		}
		
		if(nextPage!=FUNCTION_CAM)
		{
			menuExit = 1;
		}
	}
}
/////////////////////////////////////////////////

//RF function
u8 rfKeyTemp = 0;
void PageRF(void)
{
	u8 keyTemp = 0;
//	u8 tt = 0;
	
	menuExit = 0;
	WorkTime = 1800;
	dwD2DisPicWithL(PIC_RF);
	dwD2DisTime(TIME_ADDRESS, WorkTime);
	dwD2DisNum(NUM_ADDRESS, RFIntensity);
	dwD2DisChar(MODE_ADDRESS, RFMode);	
	
	dwD2CancelKey();
//	dwD2ListenKey(BIO1Pres, btnBIO1);
	dwD2ListenKey(ScrubberPres, btnScrubber);
	dwD2ListenKey(UltrasoundPres, btnUltrasound);
	dwD2ListenKey(RFPres, btnRF);
	dwD2ListenKey(O2Pres, btnO2);
	dwD2ListenKey(CleanPres, btnClean);
	dwD2ListenKey(IcePres, btnIce);
	dwD2ListenKey(SprayerPres, btnSprayer);
	dwD2ListenKey(CamPres, btnCam);
	dwD2ListenKey(SetPres, btnSet);
	dwD2ListenKey(StandbyPres, btnStandby);	//Standby
	dwD2ListenKey(TimeUpPres, btnTimeUp);
	dwD2ListenKey(TimeDownPres, btnTimeDown);
	dwD2ListenKey(IntensityUpPres, btnIntensityUp);
	dwD2ListenKey(IntensityDownPres, btnIntensityDown);
	dwD2ListenKey(ModeUpPres, btnModeUp);
	dwD2ListenKey(ModeDownPres, btnModeDown);
	dwD2ListenKey(StartPres, btnStart);
	
	while(!menuExit)
	{
		dwD2Handler();
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
				dwD2DisICO(START_ADDRESS, 0);
			}
		}
		WorkTimeDeal();		
	}
	
	RF_CON_PIN = 1;
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
	
	while(!menuExit)
	{
		if(pad_agreement())
		{
			if(pad_rec_data[0]!=ADJ_COMMAND)
			{
				nextPage = pad_rec_data[1];
				if(nextPage==FUNCTION_ICE)
				{
					if(pad_rec_data[3])
					{
						ColdWorkFlag = 1;
						COLD_CON_PIN = 0;
					}
					else
					{
						ColdWorkFlag = 0;
						COLD_CON_PIN = 1;
					}
					pad_clear();
				}
			}
			else
			{
				nextPage = FUNCTION_SCRUBBER_ADJ;
			}
		}

		if(BIT_GET(KeyScan(), 0))
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				pad_send(&pad_key[0], sizeof(pad_key));
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
	
	while(!menuExit)
	{
		if(pad_agreement())
		{
			if(pad_rec_data[0]!=ADJ_COMMAND)
			{
				nextPage = pad_rec_data[1];
				if(nextPage==FUNCTION_O2)
				{
					if(pad_rec_data[3])
					{
						O2WorkFlag = 1;
					}
					else
					{
						O2WorkFlag = 0;
					}
					if(pad_rec_data[0]==PARAMETER_COMMAND)
					{
						if(pad_rec_data[4]==0)
						{
						}
						else
						{
							O2Mode = pad_rec_data[4];
							if(O2Mode>4)
								O2Mode = 4;
						}
					}
					else
					{
						O2Mode = pad_rec_data[4];
						if(O2Mode>4)
							O2Mode = 4;
					}
					pad_clear();
				}
			}
			else
			{
				nextPage = FUNCTION_SCRUBBER_ADJ;
			}
		}

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
			case 0: {RGBValue = 0x0000; break;}	//RED
			case 1: {RGBValue = 0xF800; break;}	//RED
			case 2: {RGBValue = 0x07E0; break;}	//GREEN
			case 3: {RGBValue = 0x001F; break;}	//BLUE
			case 4:	//gradually change
			{
				if(++O2Delay>=3000)
				{
					O2Delay = 0;
					RGBValue = ((u16)RColor[AutoColor]<<11)|((u16)GColor[AutoColor]<<5)|(u16)BColor[AutoColor];
					if(++AutoColor>5)
						AutoColor = 0;
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
	
	while(!menuExit)
	{
		if(pad_agreement())
		{
			if(pad_rec_data[0]!=ADJ_COMMAND)
			{
				nextPage = pad_rec_data[1];
				if(nextPage==FUNCTION_CLEAN)
				{
					if(pad_rec_data[3])
					{
						CleanWorkFlag = 1;
						PEN_CON_PIN = 0;
					}
					else
					{
						CleanWorkFlag = 0;
						PEN_CON_PIN = 1;
					}
					if(pad_rec_data[0]==PARAMETER_COMMAND)
					{
						if(pad_rec_data[4]==0)
						{
						}
						else
						{
							CleanMode = pad_rec_data[4];
							if(CleanMode>3)
								CleanMode = 3;
						}
					}
					else
					{
						CleanMode = pad_rec_data[4];
						if(CleanMode>3)
							CleanMode = 3;
					}
					pad_clear();
				}
			}
			else
			{
				nextPage = FUNCTION_SCRUBBER_ADJ;
			}
		}

		if(BIT_GET(KeyScan(), 4))	//handle start key
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				pad_send(&pad_key[0], sizeof(pad_key));
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
				CleanWorkFlag = 0;
				pad_send(&pad_water_full[0], sizeof(pad_water_full));
			}
		}
		else
		{
			if(keyTemp2)
			{
				keyTemp2 = 0;
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
				case 0: {BOTTLE1_PIN = 1; BOTTLE2_PIN = 0; BOTTLE3_PIN = 0; BOTTLE4_PIN = 0; break;}
				case 1: {BOTTLE1_PIN = 0; BOTTLE2_PIN = 1; BOTTLE3_PIN = 0; BOTTLE4_PIN = 0; break;}
				case 2: {BOTTLE1_PIN = 0; BOTTLE2_PIN = 0; BOTTLE3_PIN = 1; BOTTLE4_PIN = 0; break;}
				case 3: {BOTTLE1_PIN = 0; BOTTLE2_PIN = 0; BOTTLE3_PIN = 0; BOTTLE4_PIN = 1; break;}
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
}
//////////////////////////////////////////////

//BIO1 function
void PageBIO1(void)
{
	u8 keyTemp = 0;
	
	menuExit = 0;
	WorkTime = 1800;
	
	while(!menuExit)
	{
		if(pad_agreement())
		{
			if(pad_rec_data[0]!=ADJ_COMMAND)
			{
				nextPage = pad_rec_data[1];
				if(nextPage==FUNCTION_BIO1)
				{
					if(pad_rec_data[3])
					{
						BIO1WorkFlag = 1;
						RF_CON_PIN = 0;
					}
					else
					{
						BIO1WorkFlag = 0;
						RF_CON_PIN = 1;
					}
					if(pad_rec_data[0]==PARAMETER_COMMAND)
					{
						if(pad_rec_data[4]==0)
						{
							BIO1Intensity = pad_rec_data[5];
							if(BIO1Intensity<10)
							{
								TIM_SetCompare4(TIM4, BIO1IntensityTable[BIO1Intensity]);
							}
						}
						else
						{
							BIO1Mode = pad_rec_data[4];
							if(BIO1Mode>2)
								BIO1Mode = 2;
						}
					}
					else
					{
						BIO1Mode = pad_rec_data[4];
						if(BIO1Mode>2)
							BIO1Mode = 2;
						BIO1Intensity = pad_rec_data[5];
						if(BIO1Intensity<10)
						{
							TIM_SetCompare4(TIM4, BIO1IntensityTable[BIO1Intensity]);
						}
					}
					pad_clear();
				}
			}
			else
			{
				nextPage = FUNCTION_SCRUBBER_ADJ;
			}
		}

		if(BIT_GET(KeyScan(), 1))
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				pad_send(&pad_key[0], sizeof(pad_key));
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
		
	while(!menuExit)
	{
		if(pad_agreement())
		{
			if(pad_rec_data[0]!=ADJ_COMMAND)
			{
				nextPage = pad_rec_data[1];
				if(nextPage==FUNCTION_SPRAYER)
				{
					if(pad_rec_data[3])
					{
						SprayerWorkFlag = 1;
						GUN_CON_PIN = 0;
					}
					else
					{
						SprayerWorkFlag = 0;
						GUN_CON_PIN = 1;
					}
					pad_clear();
				}
			}
			else
			{
				nextPage = FUNCTION_SCRUBBER_ADJ;
			}
		}

		if(BIT_GET(KeyScan(), 3))
		{
			if(keyTemp==0)
			{
				keyTemp = 1;
				pad_send(&pad_key[0], sizeof(pad_key));
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
			// GUN_CON_PIN = 1;
			GUN_CHANNEL_PIN = 0;
		}
	} 
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
const u16 BeeModPeriod[] = {110, 410, 1000, 1000}; 
const u16 BeeModCompare[] = {100, 400, 500, 800};
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

