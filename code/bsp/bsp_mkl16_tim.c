/**
 **************************************************************************************************
 * @file        bsp_mkl16_tim.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "bsp_mkl16_conf.h"
#include "bsp_mkl16_tim.h"

#include "bsp_mkl16_clock.h"

#include "clog.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_led.h"
/**
 * @addtogroup    bsp_mkl16_tim_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_mkl16_tim_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_mkl16_tim_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_tim_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_mkl16_tim_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_tim_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_tim_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_tim_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static void bsp_mkl16_lptmr_init(void);

static void bsp_mkl16_tmp0_init(void);
/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_tim_Functions 
 * @brief         
 * @{  
 */

 
void BSP_MKL16_Clock_Init(uint8_t BSP_CLOCKx)
{
	switch (BSP_CLOCKx)
	{
		case BSP_CLOCK0 :bsp_mkl16_lptmr_init();break;
		case BSP_CLOCK1 : bsp_mkl16_tmp0_init();break;
		default :break;
	}
}

static void bsp_mkl16_tmp0_init(void)
{
	tpm_config_t config = { 0 };
	
	CLOCK_EnableClock(kCLOCK_Tpm0);
	CLOCK_SetTpmClock(1); //SIM->SOPT2   
	
	

	
	TPM_GetDefaultConfig(&config);
	
	config.enableDebugMode = false;
	config.enableDoze = false;
	config.enableReloadOnTrigger = false;
	config.enableStartOnTrigger = false ;
	config.enableStopOnOverflow = false;
	config.prescale = kTPM_Prescale_Divide_1;
	//config.triggerSelect = kTPM_Trigger_Select_8;
	config.useGlobalTimeBase = false;
	
	TPM_Init(TPM0, &config);
	
	TPM_SetTimerPeriod(TPM0, 48000);
	
	TPM_EnableInterrupts(TPM0, kTPM_TimeOverflowInterruptEnable);
	
	EnableIRQ(TPM0_IRQn);
	
	
	
	
	TPM_StartTimer(TPM0, kTPM_SystemClock);
}


static void bsp_mkl16_lptmr_init(void)
{
	lptmr_config_t lptmrConfig;

	CLOCK_EnableClock(kCLOCK_Lptmr0);


	LPTMR_GetDefaultConfig(&lptmrConfig);

	lptmrConfig.bypassPrescaler = false;
	lptmrConfig.enableFreeRunning = false;
	lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
	lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_0 ;   // 32.768kHz
	lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
	lptmrConfig.value = kLPTMR_Prescale_Glitch_1 ;
	LPTMR_Init(LPTMR0, &lptmrConfig);
	/* Sets the timer period */
	LPTMR_SetTimerPeriod(LPTMR0, 1);
	/* Enables a timer interrupt */
	LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
	/* Enables the NVIC */
	EnableIRQ(LPTMR0_IRQn);
	/* Starts counting */
	LPTMR_StartTimer(LPTMR0);	
}



uint32_t BSP_MKL16_GetTimrCurCount(uint8_t BSP_CLOCKx)
{
	uint32_t count = 0;
	switch(BSP_CLOCKx)
	{
		case BSP_CLOCK0 : count = LPTMR_GetCurrentTimerCount(LPTMR0);break;
		case BSP_CLOCK1 : count = TPM_GetCurrentTimerCount(TPM0);break;
		default :break;
	}
	return count ;
}

 // ----------IRQHandler ------------------
void LPTMR0_IRQHandler(void)
{
	DEBUG("LPTMR0_IRQHandler\r\n");
	DEBUG("Time Count : %d\r\n" , BSP_MKL16_GetTimrCurCount(BSP_CLOCK0));
	
	LPTMR_ClearStatusFlags(LPTMR0,LPTMR_CSR_TCF_MASK);

	//LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
	//LPTMR_SetTimerPeriod(LPTMR0, MSEC_TO_COUNT(10000U, BOARD_BOOTCLOCKRUN_CORE_CLOCK));
}



void TPM0_IRQHandler(void)
{
	//DEBUG("TPM0_IRQHandler\r\n");
	//DEBUG("Time Count : %d\r\n" , BSP_MKL16_GetTimrCurCount(BSP_CLOCK1));
	
	TPM_ClearStatusFlags( TPM0 ,kTPM_TimeOverflowFlag);
	BSP_LED_Toggle(BSP_LED1);
}

 // ---------------------------------------

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

