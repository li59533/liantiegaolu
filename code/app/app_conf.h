/**
 **************************************************************************************************
 * @file        app_conf.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _APP_CONF_H_
#define _APP_CONF_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    app_conf_Modules 
 * @{  
 */

/**
 * @defgroup      app_conf_Exported_Macros 
 * @{  
 */


/**
 * @}
 */

/**
 * @defgroup      app_conf_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_conf_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_conf_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_conf_Exported_Functions 
 * @{  
 */
void APP_Conf_RevByteOneByte(uint8_t value);
void APP_Conf_SendData( uint8_t *buf , uint16_t len);
void APP_Conf_ID(uint8_t * payload , uint16_t len);
void APP_Conf_ReplyConf(uint8_t * payload , uint16_t len);
void APP_Conf_Stop(uint8_t * full_message , uint16_t full_len);
void APP_Conf_Start(uint8_t * full_message , uint16_t full_len) ;
void APP_Conf_SetConf(uint8_t * full_message , uint16_t full_len) ;
void APP_Conf_GotoLowPower(uint8_t * full_message , uint16_t full_len); 
void APP_Conf_Reply_Sample_Rate(uint8_t * payload , uint16_t len);
void APP_Conf_Set_Sample_Rate(uint8_t * payload , uint16_t len);
void APP_Conf_Set_Sample_Rate(uint8_t * full_message , uint16_t full_len );
void APP_Conf_Reply_SelfStart(uint8_t * payload , uint16_t len);
void APP_Conf_Set_SelfStart(uint8_t * full_message , uint16_t full_len);
void APP_Conf_Set_EQ(uint8_t * full_message , uint16_t full_len);
void APP_Conf_Set_AlarmTime(uint8_t * payload , uint16_t len);
void APP_Conf_Reply_Alarm(uint8_t * payload , uint16_t len);
void APP_Conf_Set_SNcode(uint8_t * full_message , uint16_t full_len);


// -------- Test Func ------------
void APP_Conf_TestFunc(void);
// -------------------------------


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
