#include "includes.h"

/*
** 中断组参数配置
*/
void NVIC_Config(void){
	NVIC_InitTypeDef NVIC_InitStructure;

	#if Android_HWhioce == Android_HWhioce_232
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	#endif

	#if Android_HWhioce == Android_HWhioce_485
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	#endif

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*
	** 定时器中断
	*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =  0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*
	**CAN中断初始化
	*/
	CAN1_NVIC_Init();

	/*
	** CAN中断初始化
	*/
	CAN2_NVIC_Init();	
}

/*
** 增加容错处理
*/
void HardFault_Handler(void){
	NVIC_SystemReset();
}

/*
** Eeprom初始化
*/
void Eeprom_Init(void){
	FM24_Init();
}

int8 eeprom_write(int32 addr, uint8 *buf, int32 len){
	return FMWrite(buf, addr, len);
}

int8 eeprom_read(int32 addr, uint8 *buf, int32 len){
	return FMRead(buf, addr, len);
}

uint32 BspCPUClkFreq(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	return ((uint32)rcc_clocks.HCLK_Frequency);
}

void BspInit(void){
	/*内部FLASH上锁*/
	FLASH_Lock();
	/*
	** 固件引脚配置
	*/
	GPIO_Init_Config();
	/*
	** RTC时钟初始化
	*/	
	rtc_Init();
	/*
	** 中断组参数配置
	*/
	NVIC_Config();
	/*
	** 使能备份区时钟
	*/
	PWR_BackupInit();
	/*
	** 看门狗初始化
	*/
	watchdogInit();
	/*
	** USART通讯初始化
	*/
	Com_Init();
	/*
	** 系统之滤波设计初始化参数
	*/
	SysFilterDesign_ParamInit();
	/*
	**CAN初始化
	*/
	CAN_Cfg_Init();
	/*
	** Eeprom初始化
	*/
	Eeprom_Init();
	/*
	** 初始化FLASH,提供外部文件调用
	*/
	spi_flash_init();
	/*
	** ADC 初始化
	*/
	AD_UseInit();
	/*
	** 通讯板升级参数初始化
	*/
	ComBoardUpgrPara_Init();
	/*-----------------------变量参数初始化-------------------------------------*/
	/*
	** 升级失败原因参数初始化--电池失败原因存在3(FIFOQueue_DepthLimit)级缓存
	*/
	init_UpgrFailPara();
	/*-----------------------------------------------------------------------*/
	/*
	** init Phase VC Debug Mode
	*/
	init_Phase_VC_Debug_Mode();
	/*
	** 系统参数初始化
	*/
	init_SysPara();	
}

