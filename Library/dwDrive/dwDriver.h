#ifndef __DWDRIVER_H
#define __DWDRIVER_H

#include "myDefine.h"

#define LANGUAGE_CHINESE     0
#define LANGUAGE_ENGLISH     20
#define LANGUAGE_ESPANOL     70

#define DW_COL_RED		0xF800
#define DW_COL_GREEN	0x07E0
#define DW_COL_BLUE 	0x001F
#define DW_COL_WHITE	0xFFFF
#define DW_COL_BLACK	0x0000

#define DW_CMD_PRESSED	0x73   // 按下
#define DW_CMD_RELEASE	0x72   // 松手
#define DW_CMD_DONE     0xE4   // 校正完成

#define DW_KEY_NONE     0xFF   // 无按键

extern u8 TP_Delay;
typedef enum TextSize{
	DW_SIZE_8 = 8,
	DW_SIZE_12 = 12,
	DW_SIZE_16 = 16,
	DW_SIZE_24 = 24,
	DW_SIZE_32 = 32,
	DW_SIZE_48 = 48,
}TextSize;

typedef struct Button{
	u8 picNum;
	u16  xs;
	u16  ys;
	u16  xe;
	u16  ye;
}Button;

typedef struct Image{
	u8 picNum;
	u16  xs;
	u16  ys;
	u16  xe;
	u16  ye;
}Image;

typedef struct{
	void(*pressHandle)(void);
	void(*freeHandle)(void);
	Button* button;
}DwKeyListen;

extern u16  locaX, locaY;
extern u8   dataBuff[10];
extern u8 language; 
extern DwKeyListen dwKeyListen[30];

void dwListenKey(void(*press)(void), void(*free)(void), const Button* btn);
u8  dwGetKey(void);
void dwHandler(void);

// param lang: LANGUAGE_CHINESE or LANGUAGE_ENGLISH
void dwSetLanguage(u8 lang);

/***向屏幕发送一个字节***/
void dwSendByte(u8 byte);

/*******帧结束符*********/
void dwSendOver(void);

// 设置调色板
void dwSetColor(u16 f,u16 b);


/***取指定位置作为背景色***/
void dwSetBackColor(u16 x,u16 y);

/***取指定位置作为前景色***/
void dwSetForeColor(u16 x,u16 y);

/*调亮度*/
void dwSetBL(u8 i);

// L = 1区分语言，l= 0 不区分语言
void dwDisButton(const Button* button,u8 l,u16 x,u16 y);

// L = 1区分语言，l= 0 不区分语言
void dwDisImage(const Image* image,u8 l,u16 x,u16 y);


/*****全屏显示指定位置图片不区分语言*****/
//para: Num 为位置索引号
void dwDisPicNoL(u8 picNum);

/*****全屏显示指定位置图片区分语言*****/
//para: Num 为位置索引号
void dwDisPicWithL(u8 picNum);

/**** 屏幕自带蜂鸣器响一声 *******/
void dwSound(u8 duration);  // duration*10ms


/****触屏校准***/
void dwCalibration(void);


/******配置屏幕工作模式******/
void dwSetMode(void);

/****剪切图片显示到指定位置区分语言****/
void dwCutPic(u8 picNum,u16 xs,u16 ys,u16 xe,u16 ye,u16 x,u16 y);

 /****剪切图片显示到指定位置不区分语言****/
void dwCutPicL(u8 picNum,u16 xs,u16 ys,u16 xe,u16 ye,u16 x,u16 y);


 /*****指示大小指定位置显示一个字符*****/
 //prama: size=8 8*8; 16 16*16; 24 24*24; 32 32*32
 void dwDisChar(TextSize size, u16 x, u16 y, u8 c);


 /*****指示大小指定位置显示一个数字*****/
 //prama: size=8 8*8; 16 16*16; 24 24*24; 32 32*32
 void dwDisNum(TextSize size, u16 x, u16 y, u8 c);


 /*****指示大小指定位置显示文字*****/
 //prama: size=8 8*8; 16 16*16; 24 24*24; 32 32*32
 void dwDisString(TextSize size, u16 x, u16 y, u8 string[]);


 /*****等待松手*****/
void dwWaitRelease(void);

 /*****等待按下*****/
void dwWaitPress(void);

 /***查询按键状态****/
 // 0x73  按下
 // 0x72  松手
 // 0xE4  触控校正完成
 u8 dwQueryCmd(void);

 /**将坐标数据转换成int 型**/
void dwConvertLocation(void);

void dwCancelKey(void);

void dwPlayMusic(u8 id, u8 num);

// 停止音乐
void dwStopMusic(void);

//需被放入串口接收中断函数
/*******收到屏幕的数据外理********/
void dwReceiveByte(u8 byte);

void screenCheck(void);

void funDisTime_Cool(u16 timing);
void IntensityDisplay(s8 i);

/**************************
extend function
**************************/

typedef struct{
	TextSize Size;
	u16 CursorX;	
	u16 CursorY;
	u16 BackColor;
	u16 FrontColor;
}Type_DisCharSet;

extern Type_DisCharSet DisCharSet;
static __inline void DisSetAxis(u16 x, u16 y)	//set dis char x, y axis
{
	DisCharSet.CursorX = x;
	DisCharSet.CursorY = y;
}

static __inline void DisSetColor(u16 _frontColor, u16 _backColor)	//set dis color
{
	DisCharSet.FrontColor = _frontColor;
	DisCharSet.BackColor = _backColor;
	dwSetColor(DisCharSet.FrontColor, DisCharSet.BackColor);
}

static __inline void DisTextSize(TextSize i)
{
	DisCharSet.Size = i;
}

/*******************
display number 
value:dis value
CharNum:dis char in total(max 10 for DEC, 8 for hex)
sign:dis sign or not
i:dis hex way
return:process result
******************/

typedef enum _Hex{
	Dis_Hex,
	Dis_Dec,
}_Hex;
 
u8 DisNumber(long int Value, u8 CharNum, u8 Sign, _Hex i);

u8 DisText(const char *str);	//dis str
void funDisTime(u16 timing);	//dis time

#endif
