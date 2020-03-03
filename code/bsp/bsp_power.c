/**
 **************************************************************************************************
 * @file        bsp_power.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "bsp_conf.h"
#include "bsp_power.h"
#include "clog.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_e32.h"
#include "bsp_clock.h"
/**
 * @addtogroup    bsp_power_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_power_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_power_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_power_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Functions 
 * @brief         
 * @{  
 */

typedef enum
{
	Power_Mode_Normal = 0,
	Power_Mode_STOP	  ,
}BSP_Power_Mode_e;



/*
void SMC_PreEnterStopModes(void);

void SMC_PostExitStopModes(void);

status_t SMC_SetPowerModeRun(SMC_Type *base);

SMC_SetPowerModeStop(SMC_Type *base, smc_partial_stop_option_t option);
SMC_SetPowerModeVlps(SMC_Type *base);
*/

void BSP_Power_SetMode(BSP_Power_Mode_e mode)
{
	DEBUG("Current Mode:%X\r\n" , SMC_GetPowerModeState( SMC ));
	
	//SMC_SetPowerModeProtection( SMC , kSMC_AllowPowerModeVlls | kSMC_AllowPowerModeLls); 

}


#include "bsp_uart.h"
// -------Test Func--------
void BSP_Power_ModeTest(void)
{
	BSP_E32_Power_OFF();
	BSP_Uart0_Close();
//	DEBUG("Current Mode:%X\r\n" , SMC_GetPowerModeState( SMC ));
//	SMC_SetPowerModeProtection(SMC , kSMC_AllowPowerModeAll);
//	SMC_SetPowerModeVlps(SMC);
//	
//	DEBUG("Current Mode:%X\r\n" , SMC_GetPowerModeState( SMC ));
//    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateVlpr)
//    {
//    }
	
	BOARD_BootClockVLPS();
	//BOARD_BootClockVLPR();
	
	
}

// ------------------------

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

