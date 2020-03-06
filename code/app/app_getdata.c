/**
 **************************************************************************************************
 * @file        app_getdata.c
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

#include "clog.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "app_getdata.h"
#include "bsp_adc.h"
#include "bsp_tim.h"
#include "clog.h"
/**
 * @addtogroup    app_getdata_Modules 
 * @{  
 */

/**
 * @defgroup      app_getdata_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_getdata_Macros_Defines 
 * @brief         
 * @{  
 */
#define APP_GETDATA_SAMPLE_R		100 
/**
 * @}
 */

/**
 * @defgroup      app_getdata_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_getdata_Private_Types
 * @brief         
 * @{  
 */



/**
 * @}
 */

/**
 * @defgroup      app_getdata_Private_Variables 
 * @brief         
 * @{  
 */
App_Data_t App_Data = 
{
	.original_value = 0,
	.real_mV = 0,
	.real_mA = 0,
	.need_value = 0,
};
/**
 * @}
 */

/**
 * @defgroup      app_getdata_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_getdata_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_getdata_Functions 
 * @brief         
 * @{  
 */
void APP_GetData_Init(void)
{
	BSP_ADC_Init();
	BSP_Clock_Init(BSP_CLOCK1);
}
	
void APP_GetData_DeInit(void)
{
	
}

App_Data_t * APP_GetData_Get(void)
{	
	return &App_Data;
}

// ref 3V so 3000/65536 =  0.045776f
void APP_GetData_Calc(void)
{
	App_Data.original_value = BSP_ADC_GetAverageValue( 0 );
	App_Data.real_mV = 0.045776f * App_Data.original_value;
	App_Data.real_mA = 0.00045776f * App_Data.original_value;
	App_Data.need_value = App_Data.real_mA;

//	char cbuf[30] = {0};
//	sprintf(cbuf,"APP_GetData_Calc:%f mA" , App_Data.real_mA);
//	DEBUG("%s\r\n",cbuf);
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

