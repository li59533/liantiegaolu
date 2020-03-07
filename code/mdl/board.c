/**
 **************************************************************************************************
 * @file        board.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

#include "board.h"
#include "bsp_clock.h"

#include "bsp_led.h"
#include "bsp_tim.h"
#include "bsp_rtc.h"
#include "bsp_adc.h"
#include "bsp_uart.h"
#include "bsp_flash.h"
/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    board_Modules 
 * @{  
 */

/**
 * @defgroup      board_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       board_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      board_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       board_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      board_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      board_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      board_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      board_Functions 
 * @brief         
 * @{  
 */
void Board_Init_ClockRUN(void)
{
	BSP_SystemClockInit();
	BSP_ShowClock();
}

void Board_Init_BSP(void)
{
	BSP_RTC_Init();   //RTC Init
	BSP_Flash_Init(); // Flash Init
	BSP_UART_Init( BSP_UART2 );
	
// --------Test func-------------	
//	BSP_ADC_Init();
//	BSP_UART_Init( BSP_UART0 );
//	BSP_UART_Init( BSP_UART2 );
// ------------------------------
	
}



// -----------Test code --------------
void BSP_Test_Delay(uint32_t count)
{
	uint32_t i = 1000;
	while(count -- )
	{
		while(i -- );
		i = 1000;
	}
}
// ------------------------------------

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

