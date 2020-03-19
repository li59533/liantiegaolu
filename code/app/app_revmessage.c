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
#include "bsp_rtc.h"
#include "osal.h"
#include "app_conf.h"
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
typedef struct
{
	uint8_t ACK_status;
	uint32_t time_wait;
	uint32_t time_cur;
}app_ACKFlag_t;




/**
 * @}
 */

/**
 * @defgroup      app_revmessage_Private_Variables 
 * @brief         
 * @{  
 */
static app_ACKFlag_t app_ACKFlag;
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
static void app_revACK(uint8_t * payload , uint16_t len);
static void app_revsetAckFlag(void);
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

//rev_data_t break_buf = { 0 };
static rev_data_t temp_buf = { 0 };
static rev_data_t full_buf = { 0 };

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
						APP_RevMessage_Process(&temp_buf.buf[0] , temp_buf.len);   // deal with the cmd
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
							APP_RevMessage_Process(&full_buf.buf[0] , full_buf.len);   // deal with the cmd
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

void APP_RevMessage_Process(uint8_t * buf , uint16_t len)
{
	ln_protocolintance_t * ln_protocolintance = 0;
	
	ln_protocolintance = (ln_protocolintance_t *) buf;
	
	switch(ln_protocolintance->cmd)
	{
		case CMD_ReportDataACK:
		{
			app_revACK( (uint8_t *)&ln_protocolintance->payload, ln_protocolintance->len);
		}
		break;
		case CMD_Conf_ID:
		{
			APP_Conf_SetConfStatus();
			APP_Conf_ID( (uint8_t *)&ln_protocolintance->payload, ln_protocolintance->len);
		}
		break;
		case CMD_Conf_Reply_Conf:
		{
			APP_Conf_SetConfStatus();
			APP_Conf_ReplyConf( (uint8_t *)&ln_protocolintance->payload, ln_protocolintance->len);
		}
		break;
		case CMD_Conf_Set_Conf:
		{
			APP_Conf_SetConfStatus();
			APP_Conf_SetConf(buf , len);
		}
		break;
		case CMD_Conf_Start:
		{
			APP_Conf_ClearConfStatus();
			APP_Conf_Start(buf , len);
		}
		break;
		case CMD_Conf_Stop:
		{
			APP_Conf_Stop(buf , len);
		}
		break;
		case CMD_Conf_GotoLowpower:
		{
			APP_Conf_GotoLowPower(buf , len);
		}
		break;
		case CMD_Conf_Reply_Sample_Rate:
		{
			APP_Conf_SetConfStatus();
			APP_Conf_Reply_Sample_Rate((uint8_t *)&ln_protocolintance->payload, ln_protocolintance->len);			
		}
		break;
		case CMD_Conf_Set_Sample_Rate:
		{
			APP_Conf_SetConfStatus();
			APP_Conf_Set_Sample_Rate(buf , len );
		}
		break;
		case CMD_Conf_Reply_SelfStart:
		{
			APP_Conf_Reply_SelfStart((uint8_t *)&ln_protocolintance->payload, ln_protocolintance->len);
		}
		break;
		case CMD_Conf_Set_SelfStart:
		{
			APP_Conf_Set_SelfStart(buf , len);
		}
		break;
		case CMD_Conf_Set_EQ:
		{
			APP_Conf_Set_EQ(buf , len);
		}
		break;
		case CMD_Conf_Set_AlarmTime:
		{	
			APP_Conf_Set_AlarmTime((uint8_t *)&ln_protocolintance->payload, ln_protocolintance->len);
		}
		break;
		case CMD_Conf_Reply_AlarmTime:
		{
			APP_Conf_Reply_Alarm((uint8_t *)&ln_protocolintance->payload, ln_protocolintance->len);
		}
		break;		
		case CMD_Conf_Set_SNcode:
		{
			APP_Conf_Set_SNcode(buf , len);
		}
		break;
		case CMD_Conf_Calibration:
		{
			APP_Conf_SetConfStatus();
			APP_Conf_Set_ADCCalibration((uint8_t *)&ln_protocolintance->payload, ln_protocolintance->len);
		}
		break;
		case CMD_Conf_ADCToRealValue:
		{
			APP_Conf_SetConfStatus();
			APP_Conf_SetADCToRealValue((uint8_t *)&ln_protocolintance->payload, ln_protocolintance->len);
		}
		break;
		default:
		{
			DEBUG("lnProtoCMD:%X\r\n" , ln_protocolintance->cmd);
			
		}
		break;
	}
}




void APP_RevClearAckFlag(void)
{
	app_ACKFlag.ACK_status = APP_RevACK_Wait;
	app_ACKFlag.time_cur = 0;
	app_ACKFlag.time_wait = OS_Clock_GetSeconds();
}

static void app_revsetAckFlag(void)
{
	app_ACKFlag.ACK_status = APP_RevACK_Get;

}

uint8_t APP_RevGetAckFlag(void)
{
	app_ACKFlag.time_cur = OS_Clock_GetSeconds();	
	
	if((app_ACKFlag.time_cur - app_ACKFlag.time_wait) >= 1 && app_ACKFlag.ACK_status != APP_RevACK_Get) // timeout  : 1S
	{
		app_ACKFlag.ACK_status = APP_RevACK_Timeout;
	}
	return app_ACKFlag.ACK_status;
}

static void app_revACK(uint8_t * payload , uint16_t len)
{
	app_revsetAckFlag();
	
	rtc_datetime_t datetime;
	
	datetime.year = *(uint16_t *)&payload[0];
	datetime.month = payload[2];
	datetime.day = payload[3];
	datetime.hour = payload[4];
	datetime.minute = payload[5];
	datetime.second = payload[6];
	
	BSP_RTC_SetDate(&datetime);

	if(1 )
	{
		
	}
	
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

