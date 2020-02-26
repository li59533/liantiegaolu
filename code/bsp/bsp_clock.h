/**
 **************************************************************************************************
 * @file        bsp_clock.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _BSP_CLOCK_H_
#define _BSP_CLOCK_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "bsp_conf.h"
/**
 * @addtogroup    bsp_clock_Modules 
 * @{  
 */
/*

00> kCLOCK_CoreSysClk:48000000
00> kCLOCK_PlatClk:48000000
00> kCLOCK_BusClk:24000000
00> kCLOCK_FlashClk:24000000
00> kCLOCK_PllFllSelClk:48000000
00> kCLOCK_Er32kClk:1000
00> kCLOCK_McgFixedFreqClk:250000
00> kCLOCK_McgInternalRefClk:32768
00> kCLOCK_McgFllClk:0
00> kCLOCK_McgPll0Clk:96000000
00> kCLOCK_LpoClk:1000
00> kCLOCK_Osc0ErClk:8000000

*/
/**
 * @defgroup      bsp_clock_Exported_Macros 
 * @{  
 */
#define BOARD_XTAL0_CLK_HZ                          8000000U  /*!< Board xtal0 frequency in Hz */

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockRUN configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKRUN_CORE_CLOCK              48000000U  /*!< Core clock frequency: 48000000Hz */

#define BOARD_BOOTCLOCKVLPR_CORE_CLOCK              4000000U  /*!< Core clock frequency: 4000000Hz */
/**
 * @}
 */

/**
 * @defgroup      bsp_clock_Exported_Constants
 * @{  
 */
 
/*! @brief MCG set for BOARD_BootClockRUN configuration.
 */
extern const mcg_config_t mcgConfig_BOARD_BootClockRUN;
/*! @brief SIM module set for BOARD_BootClockRUN configuration.
 */
extern const sim_clock_config_t simConfig_BOARD_BootClockRUN;
/*! @brief OSC set for BOARD_BootClockRUN configuration.
 */
extern const osc_config_t oscConfig_BOARD_BootClockRUN;


/*! @brief MCG set for BOARD_BootClockVLPR configuration.
 */
extern const mcg_config_t mcgConfig_BOARD_BootClockVLPR;
/*! @brief SIM module set for BOARD_BootClockVLPR configuration.
 */
extern const sim_clock_config_t simConfig_BOARD_BootClockVLPR;
/*! @brief OSC set for BOARD_BootClockVLPR configuration.
 */
extern const osc_config_t oscConfig_BOARD_BootClockVLPR;
/**
 * @}
 */

/**
 * @defgroup      bsp_clock_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_clock_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_clock_Exported_Functions 
 * @{  
 */
void BSP_SystemClockInit(void);
void BSP_ShowClock(void);
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
#endif
