#ifndef _APP_LIB_H
#define _APP_LIB_H

#include "UserBaseLib.h"
//#include "dwDriver.h"
#include "usart.h"
#include "delay.h"
#include "adc.h"
#include "stmflash.h"
#include "dwDriveD2.h"
#include "pad_com.h"

//Temperature table
#define TEMP_40 1418
#define TEMP_38 1503
#define TEMP_36 1582
#define TEMP_0 3132
#define TEMP_N5 3307
#define TEMP_N10 3464

//Hardware define
//NEW
#define SOUND_PIN PAout(11)
#define ULT_PIN PAout(12)
#define POWER_ON_PIN PBout(5)
#define COLD_HOT_PIN PBout(6)
#define PUMP220_PIN PDout(2)
#define PUMP24_PIN PCout(9)
#define O2_VALVE1_PIN PAout(5)
#define O2_VALVE2_PIN PAout(4)
#define GUN_CHANNEL_PIN PBout(4)
#define PEN_CHANNEL_PIN PBout(3)
#define BOTTLE1_PIN PCout(5)
#define BOTTLE2_PIN PCout(4)
#define BOTTLE3_PIN PAout(7)
#define BOTTLE4_PIN PAout(6)
#define LED_BLUE_PIN PBout(10)
#define LED_GREEN_PIN PBout(0)
#define LED_RED_PIN PCout(3)
#define COLD_CON_PIN PCout(10)
#define RF_CON_PIN PBout(11)
#define ULT_CON_PIN PBout(13)
#define GUN_CON_PIN PBout(15)
#define CAM_CON_PIN PBout(1)
#define PEN_CON_PIN PCout(7)
#define COLD_BUTTON_PIN PCin(11)
#define RF_BUTTON_PIN PBin(12)
#define ULT_BUTTON_PIN PBin(14)
#define GUN_BUTTON_PIN PCin(6)
#define PEN_BUTTON_PIN PCin(8)
#define WATER_LEVEL_PIN PAin(0)
#define TOUCH_BUTTON_PIN PAin(1)

//Picture Code
#define PIC_START_ANIMATION 1
#define PIC_MAIN 11
#define PIC_SCRUBBER 17
#define PIC_SCRUBBER_ADJ 19
#define PIC_CAMERA 7
#define PIC_PRINT 61
#define PIC_RF 16
#define PIC_ULTRASOUND 15
#define PIC_ICE 18
#define PIC_CLEAN 14
#define PIC_O2 12
#define PIC_SPRAYER 13
#define PIC_BIO1 16
#define PIC_EFFECT 21
#define PIC_EFFECT2 22
#define PIC_EFFECT3 23
#define PIC_NUMBER 7
#define PIC_ORIG 20
#define PIC_LANGUAGE 4

//music code
#define	MSC_BUTTON  0
#define	MSC_START   3
#define	MSC_STOP    2
#define MSC_POWERUP	4
#define MSC_STANDBY	5
#define MSC_WARN 		6
#define MSC_READY 	7

extern u8 nextPage;
#define FUNCTION_BIO1 1
#define FUNCTION_ULTRASOUND 2
#define FUNCTION_SCRUBBER 4
#define FUNCTION_ICE 10
#define FUNCTION_CLEAN 8
#define FUNCTION_SPRAYER 6
#define FUNCTION_O2 9

#define FUNCTION_RESTART 0
#define FUNCTION_RF 3
#define FUNCTION_SCRUBBER_ADJ 0x0d
#define FUNCTION_MAIN 0x0e
#define FUNCTION_LANGUAGE 0x0f
#define FUNCTION_CAM 7

//Flash function
#define FLASH_LANG_ADDR	 0X0800F7FC //language save
#define FLASH_FREQ_ADDR  0X0800F800 //频率保存,设置FLASH 保存地址(必须为偶数)
extern u16 ScrubberFrequency;

extern u8 TouchKey;
extern u8 buff2[5];
extern u8 rx2Flag;
void commRx2Handler(u8 byte);
extern u8 buff1[5];
extern u8 rx1Flag;
void commRx1Handler(u8 byte);
extern u8 Flag1ms;

void ScrubberPWM(u8 i, u8 Work);
void UltrasoundPWM(u8 i, u8 Work);
void BIO1PWM(u8 i, u8 Work);
void BIO2PWM(u8 i, u8 Work);
void RFPWM(u8 i, u8 Work);
void O2PWM(u8 i, u8 Work);
void RGB_Decode(const u16 *i);

extern u8 Flag1s;
extern u8 BIO1Mode, ScrubberMode, UltrasoundMode, RFMode, ColdMode, O2Mode, CleanMode;
extern u8 ScrubberWorkFlag, O2WorkFlag, RFWorkFlag, BIO1WorkFlag, CleanWorkFlag,
					UltrasoundWorkFlag, ColdWorkFlag, SprayerWorkFlag;
extern u8 menuExit;
void IO_Init(void);
void PageRestartAnimation(void);
void PageMain(void);
void PageScrubber(void);
void PageBIO1(void);
void PageUltrasound(void);
void PageIce(void);
void PageBIO2(void);
void PageO2(void);
void PageClean(void);
void PageRF(void);
void PageScrubberAdj(void);
void PageLanguage(void);
void PageSprayer(void);
void PageCAM(void);

extern u8 BeeMod, BeeTime;
void BeeFunction(void);
//#define INLINE_MUSIC_BUTTON() {BeeMod = 0; BeeTime = 1;}
#define INLINE_MUSIC_BUTTON()	{BeeMod = 0; BeeTime = 1;}
#define INLINE_MUSIC_START() {BeeMod = 1; BeeTime = 1;}
#define INLINE_MUSIC_STOP() {BeeMod = 2; BeeTime = 3;}
//#define INLINE_MUSIC_START() {BeeMod = 1; BeeTime = 1;}
//#define INLINE_MUSIC_STOP() {BeeMod = 2; BeeTime = 3;}
#define INLINE_MUSIC_REDAY() {BeeMod = 3; BeeTime = 1;}
#define INLINE_MUSIC_POWERUP() //dwPlayMusic(MSC_POWERUP, 1);
#define INLINE_MUSIC_STANDBY() {BeeMod = 3; BeeTime = 1;}
#define INLINE_MUSIC_ERROR() {BeeMod = 2; BeeTime = 1;}
#define INLINE_MUSIC_WATER_WARN() {BeeMod = 1; BeeTime = 4;}
#define INLINE_MUSIC_SCR_ADJ() {BeeMod = 1; BeeTime = 2;}

//key scan
extern u8 KeyDelay;
u8 KeyScan(void);

#endif
