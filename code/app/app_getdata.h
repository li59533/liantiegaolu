/**
 **************************************************************************************************
 * @file        app_getdata.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _APP_GETDATA_H_
#define _APP_GETDATA_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    app_getdata_Modules 
 * @{  
 */

/**
 * @defgroup      app_getdata_Exported_Macros 
 * @{  
 */


/**
 * @}
 */

/**
 * @defgroup      app_getdata_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_getdata_Exported_Types 
 * @{  
 */
typedef  struct
{
	uint16_t original_value;
	float real_mV;
	float real_mA;
	float need_value;
}App_Data_t;
/**
 * @}
 */

/**
 * @defgroup      app_getdata_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_getdata_Exported_Functions 
 * @{  
 */
void APP_GetData_Init(void);

void APP_GetData_DeInit(void);
App_Data_t * APP_GetData_Get(void);
void APP_GetData_Calc(void);
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
