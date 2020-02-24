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
	}
	
	
	
	
	


}

static void bsp_mkl16_lptmr_init(void)
{
	lptmr_config_t lptmrConfig;

	CLOCK_EnableClock(kCLOCK_Lptmr0);


	LPTMR_GetDefaultConfig(&lptmrConfig);

	lptmrConfig.bypassPrescaler = 0;
	lptmrConfig.enableFreeRunning = 0;
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



uint32_t BSP_MKL16_GetCurCount(void)
{
	return LPTMR_GetCurrentTimerCount(LPTMR0);
}


void LPTMR0_IRQHandler(void)
{
	DEBUG("LPTMR0_IRQHandler\r\n");
	DEBUG("Time Count : %d\r\n",BSP_MKL16_GetCurCount());
	
	LPTMR_ClearStatusFlags(LPTMR0,LPTMR_CSR_TCF_MASK);
	
	BSP_LED_Toggle(BSP_LED1);
	
	//LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
	//LPTMR_SetTimerPeriod(LPTMR0, MSEC_TO_COUNT(10000U, BOARD_BOOTCLOCKRUN_CORE_CLOCK));
}


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

