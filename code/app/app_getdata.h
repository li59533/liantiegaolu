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

typedef enum
{
	MA4_20_NORMAL = 0,
	
	MA4_20_OVER   = 1,
	MA4_20_LOST	  = 2,
	MA4_20_NODevice ,
}APP_Data_DeviceStatus_e;
 
typedef  struct
{
	uint16_t original_value;
	uint8_t device_status;
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
void APP_GetData_Init(void);		// 4-20mA Init
void APP_GetData_DeInit(void);		// low power can use this func
App_Data_t * APP_GetData_Get(void);	// get 4-20mA Value Ptr
void APP_GetData_Calc(void);		// loop call this func <must>
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
