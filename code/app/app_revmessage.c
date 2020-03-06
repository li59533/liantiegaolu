/**
 **************************************************************************************************
 * @file        app_revmessage.c
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
#include "app_revmessage.h"
#include "lnprotocol.h"

/**
 * @addtogroup    app_revmessage_Modules 
 * @{  
 */

/**
 * @defgroup      app_revmessage_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_revmessage_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_revmessage_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_revmessage_Private_Types
 * @brief         
 * @{  
 */



/**
 * @}
 */

/**
 * @defgroup      app_revmessage_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_revmessage_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_revmessage_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static int8_t bsp_e32_rev(uint8_t * buf , uint16_t len);
/**
 * @}
 */

/**
 * @defgroup      app_revmessage_Functions 
 * @brief         
 * @{  
 */

typedef struct
{
	uint8_t buf[300];
	uint8_t len;
}rev_data_t;

rev_data_t break_buf = { 0 };
rev_data_t temp_buf = { 0 };
rev_data_t full_buf = { 0 };

void APP_RevMess_Analysis(uint8_t *buf , uint16_t len)
{
	bsp_e32_rev( buf ,  len);
}
	

static int8_t bsp_e32_rev(uint8_t * buf , uint16_t len)
{
	uint16_t i = 0; 
	
	memcpy( &temp_buf.buf[temp_buf.len] , buf, len );
	temp_buf.len += len;
	while(1)
	{
		if(temp_buf.buf[i] == LNPROTOCOL_HEAD)   // check head 
		{
			if(temp_buf.buf[i + 1] == LNPROTOCOL_HEAD)
			{
				i++;
				temp_buf.len--;
			}
			memcpy( &temp_buf.buf[0] , &temp_buf.buf[i], temp_buf.len); // offset
			i = 0;
			
//			if(temp_buf.len < 4)
//			{
//				break;
//			}
//			else
			{
				if((temp_buf.buf[ 2] + temp_buf.buf[3] * 256) == (temp_buf.len - 6) ) // check inf len 
				{
					if(LNprotocol_Checksum( &temp_buf.buf[0] , temp_buf.len) == 1) // check sum
					{
						//APP_ConfNet_Process(&temp_buf.buf[0] , temp_buf.len);   // deal with the cmd
					}					
					temp_buf.len = 0;
				}
				else
				{
					if((temp_buf.buf[ 2] + temp_buf.buf[3] * 256) > (temp_buf.len - 6) && (temp_buf.buf[ 2] + temp_buf.buf[3] * 256) < 300)
					{
						break;
					}
					else if((temp_buf.buf[ 2] + temp_buf.buf[3] * 256) < (temp_buf.len - 6))
					{
						memcpy( &full_buf.buf[0] , &temp_buf.buf[0] , (temp_buf.buf[ 2] + temp_buf.buf[3] * 256) + 6 );
						full_buf.len = (temp_buf.buf[ 2] + temp_buf.buf[3] * 256) + 6;
						
						temp_buf.len = temp_buf.len - (temp_buf.buf[ 2] + temp_buf.buf[3] * 256) - 6;
						memcpy( &temp_buf.buf[0] , &temp_buf.buf[(temp_buf.buf[ 2] + temp_buf.buf[3] * 256) + 6], temp_buf.len); // offset
						
						if(LNprotocol_Checksum( &full_buf.buf[0] , full_buf.len) == 1) // check sum
						{
							//APP_ConfNet_Process(&full_buf.buf[0] , full_buf.len);   // deal with the cmd
						}
					}
					else
					{
						temp_buf.len = 0;
					}
				}
			}
			
		}
		else
		{
			i ++;
			temp_buf.len --;

		}
		
		if(temp_buf.len <= 0)
		{
			break;
		}		
		
	}

	return 0;
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

