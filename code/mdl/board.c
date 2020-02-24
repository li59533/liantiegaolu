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
#include "bsp_mkl16_clock.h"

#include "bsp_led.h"
#include "bsp_mkl16_tim.h"
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
	BSP_MKL16_SystemClockInit();
	BSP_MKL16_ShowClock();
}

void Board_Init_BSP(void)
{
	BSP_LED_Init();
	
	//BSP_MKL16_Clock_Init(BSP_CLOCK0);
	BSP_MKL16_Clock_Init(BSP_CLOCK1);
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

