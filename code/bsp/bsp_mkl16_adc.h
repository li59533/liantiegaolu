/**
 **************************************************************************************************
 * @file        bsp_mkl16_adc.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _BSP_MKL16_ADC_H_
#define _BSP_MKL16_ADC_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    bsp_mkl16_adc_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_mkl16_adc_Exported_Macros 
 * @{  
 */


/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_adc_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_adc_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_adc_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_adc_Exported_Functions 
 * @{  
 */
void BSP_MKL16_ADC_Init(void);
uint32_t BSP_MKL16_ADC_GetValue(uint8_t channel);

void BSP_MKL16_ADC_ShowValue(void);

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
