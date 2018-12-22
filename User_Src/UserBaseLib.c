#include "UserBaseLib.h"

u8 RestoreFlag(void)
{
	u8 RestFlag = 0;
	
	//看门狗复位标志
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		RestFlag = 1;
	}					
	//软件复位标志
	else if(RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
	{
		RestFlag =  2;  							 
	}
	//上电复位标志
	else if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		RestFlag = 3;  
	}
	RCC_ClearFlag();//清楚复位状态标志位
	return RestFlag;
}
