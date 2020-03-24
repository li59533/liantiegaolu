/**
 **************************************************************************************************
 * @file        app_conf.c
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
#include "app_conf.h"
#include "lnprotocol.h"
#include "bsp_rtc.h"
#include "bsp_uart.h"
#include "app_revmessage.h"
#include "osal.h"
#include "system_param.h"
#include "bsp_e32.h"
#include "app_task.h"

#include "app_getdata.h"

#include "app_battery.h"
#include "version.h"
#include "bsp_led.h"
/**
 * @addtogroup    app_conf_Modules 
 * @{  
 */

/**
 * @defgroup      app_conf_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_conf_Macros_Defines 
 * @brief         
 * @{  
 */
#define APP_CONFREV_SPACE		100
/**
 * @}
 */

/**
 * @defgroup      app_conf_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_conf_Private_Types
 * @brief         
 * @{  
 */

typedef struct
{
	uint8_t buf[300];
	uint8_t len;
}rev_data_t;


/**
 * @}
 */

/**
 * @defgroup      app_conf_Private_Variables 
 * @brief         
 * @{  
 */
static rev_data_t temp_buf = { 0 };
static rev_data_t full_buf = { 0 };
static uint8_t app_conf_confstatus = 0;
static uint8_t app_confrev_space[APP_CONFREV_SPACE] = { 0 };
/**
 * @}
 */

/**
 * @defgroup      app_conf_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_conf_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

static int8_t bsp_conf_rev(uint8_t * buf , uint16_t len);
/**
 * @}
 */

/**
 * @defgroup      app_conf_Functions 
 * @brief         
 * @{  
 */


void APP_Conf_GetVersion(uint8_t * payload , uint16_t len)
{
	ln_protocolintance_t * ln_protocolintance = 0;
	uint8_t sendbuf[100] = { 0 };

	uint16_t send_len = 0; 

	ln_protocolintance = (ln_protocolintance_t *) sendbuf;
	ln_protocolintance->head = LNPROTOCOL_HEAD;
	ln_protocolintance->cmd = CMD_Conf_GetVersion;
	
	uint8_t * buf_ptr = (uint8_t *)&ln_protocolintance->payload;

	// --------Version ---

	uint32_t version = 0;
	version = Version_Get_Bin();
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&version, 4);
	send_len += 4;

	// ---------------------------------------
	ln_protocolintance->len = send_len;
	*buf_ptr = LNprotocol_GetChecksum(&ln_protocolintance->head , send_len + 6);
	buf_ptr ++;
	*(buf_ptr ) = LNPROTOCOL_FOOT;
	buf_ptr ++;


	// --------Send---------
	
	APP_Conf_SendData( &ln_protocolintance->head , buf_ptr - &ln_protocolintance->head);
	// ----------------------
	
	
	
}

void APP_Conf_SetADCToRealValue(uint8_t * payload , uint16_t len)
{
	float Y1 = 0.0f;
	float Y2 = 0.0f;
	if(len == 8)
	{
		Y1 = *(float *)&payload[0];
		Y2 = *(float *)&payload[4];		
		
		g_SystemParam_Config.Analog_conf.real_k = (Y2 - Y1) / 16;
		g_SystemParam_Config.Analog_conf.real_b = Y1 - (Y2 - Y1)/4;
		SystemParam_Save();
		
		ln_protocolintance_t * ln_protocolintance = 0;
		uint8_t sendbuf[100] = { 0 };
		uint8_t buf_temp[10] = { 0 };
		uint16_t send_len = 0; 

		ln_protocolintance = (ln_protocolintance_t *) sendbuf;
		ln_protocolintance->head = LNPROTOCOL_HEAD;
		ln_protocolintance->cmd = CMD_Conf_ADCToRealValue;
		
		uint8_t * buf_ptr = (uint8_t *)&ln_protocolintance->payload;

		// --------defaultvalue ---
		buf_temp[0] = 0x01;
		buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp, 1);
		send_len += 1;

		// ---------------------------------------
		ln_protocolintance->len = send_len;
		*buf_ptr = LNprotocol_GetChecksum(&ln_protocolintance->head , send_len + 6);
		buf_ptr ++;
		*(buf_ptr ) = LNPROTOCOL_FOOT;
		buf_ptr ++;

		// --------Send---------
		APP_Conf_SendData( &ln_protocolintance->head , buf_ptr - &ln_protocolintance->head);
		// ----------------------			
		
		
		
	}

}


void APP_Conf_Set_ADCCalibration(uint8_t * payload , uint16_t len)
{
	static uint8_t flag_4 = 0;
	static uint8_t flag_20 = 0;
	static float X1 = 0.0f;
	static float X2 = 0.0f;
	
	App_Data_t * App_Data;
	
	App_Data = APP_GetData_Get();	
	
	if(payload[0] == 0x01) 
	{
		flag_4 = 1;
		X1 = App_Data->original_value * 0.00045776f;
	}
	else if(payload[1] == 0x01)
	{
		flag_20 = 1;
		X2 = App_Data->original_value * 0.00045776f;		
	}
	
	if(flag_4 == 1 && flag_20 == 1)
	{
		flag_4 = 0;
		flag_20 = 0;
		if(X1 != X2)
		{
			g_SystemParam_Config.Analog_conf.adc_k = (-16) / (X1 - X2);
			g_SystemParam_Config.Analog_conf.adc_b = 4 + 16 * X1 / (X1 - X2);
			SystemParam_Save();	
			
			
			ln_protocolintance_t * ln_protocolintance = 0;
			uint8_t sendbuf[100] = { 0 };
			uint8_t buf_temp[10] = { 0 };
			uint16_t send_len = 0; 

			ln_protocolintance = (ln_protocolintance_t *) sendbuf;
			ln_protocolintance->head = LNPROTOCOL_HEAD;
			ln_protocolintance->cmd = CMD_Conf_Calibration;
			
			uint8_t * buf_ptr = (uint8_t *)&ln_protocolintance->payload;

			// --------defaultvalue ---
			buf_temp[0] = 0x01;
			buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp, 1);
			send_len += 1;

			// ---------------------------------------
			ln_protocolintance->len = send_len;
			*buf_ptr = LNprotocol_GetChecksum(&ln_protocolintance->head , send_len + 6);
			buf_ptr ++;
			*(buf_ptr ) = LNPROTOCOL_FOOT;
			buf_ptr ++;

			// --------Send---------
			APP_Conf_SendData( &ln_protocolintance->head , buf_ptr - &ln_protocolintance->head);
			// ----------------------	
		
		}

	}
}


void APP_Conf_Set_SNcode(uint8_t * full_message , uint16_t full_len)
{
	// --------Send---------
	APP_Conf_SendData( full_message ,full_len);
	// ----------------------
	memcpy(g_SystemParam_Config.SNcode , &full_message[4] , 8);
	
	// ------Save -------
	SystemParam_Save();
	// ------------------	
	
}

void APP_Conf_Reply_Alarm(uint8_t * payload , uint16_t len)
{
	ln_protocolintance_t * ln_protocolintance = 0;
	uint8_t sendbuf[100] = { 0 };
	uint16_t send_len = 0; 
	rtc_datetime_t  datetime ;
	
	ln_protocolintance = (ln_protocolintance_t *) sendbuf;
	ln_protocolintance->head = LNPROTOCOL_HEAD;
	ln_protocolintance->cmd = CMD_Conf_Reply_AlarmTime;
	
	uint8_t * buf_ptr = (uint8_t *)&ln_protocolintance->payload;

	BSP_RTC_ConvertSecondsToDatetime(g_SystemParam_Config.firsttimestamp , &datetime);
	
	
	// --------year ---
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&datetime.year, 2);
	send_len += 2;
	// --------mon -----
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&datetime.month, 1);
	send_len += 1;	
	// --------day -----
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&datetime.day, 1);
	send_len += 1;		
	// --------hour -----
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&datetime.hour, 1);
	send_len += 1;		
	// --------min -----
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&datetime.minute, 1);
	send_len += 1;
	// --------sec -----
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&datetime.second, 1);
	send_len += 1;	
	// ---------------------------------------
	ln_protocolintance->len = send_len;
	*buf_ptr = LNprotocol_GetChecksum(&ln_protocolintance->head , send_len + 6);
	buf_ptr ++;
	*(buf_ptr ) = LNPROTOCOL_FOOT;
	buf_ptr ++;

	// --------Send---------
	APP_Conf_SendData( &ln_protocolintance->head , buf_ptr - &ln_protocolintance->head);
	// ---------------------	

}


void APP_Conf_Set_AlarmTime(uint8_t * payload , uint16_t len)
{
	rtc_datetime_t  datetime ;
	
	datetime.year = *(uint16_t *)&payload[0];
	datetime.month = payload[2];
	datetime.day = payload[3];
	datetime.hour = payload[4];
	datetime.minute = payload[5];
	datetime.second = payload[6];
	
	g_SystemParam_Config.firsttimestamp = BSP_RTC_ConvertDatetimeToSeconds(&datetime);
	
	// ------Save -------
	SystemParam_Save();
	// ------------------		
	
}


void APP_Conf_Set_EQ(uint8_t * full_message , uint16_t full_len)
{
	// --------Send---------
	APP_Conf_SendData( full_message ,full_len);
	// ----------------------
	g_SystemParam_Config.current_boardtime = full_message[7];
	g_SystemParam_Config.battery = 100;
	APP_Battery_Reduce();		
}

void APP_Conf_Set_SelfStart(uint8_t * full_message , uint16_t full_len)
{
	// --------Send---------
	APP_Conf_SendData( full_message ,full_len);
	// ----------------------			
}

void APP_Conf_Reply_SelfStart(uint8_t * payload , uint16_t len)
{

	ln_protocolintance_t * ln_protocolintance = 0;
	uint8_t sendbuf[100] = { 0 };
	uint8_t buf_temp[10] = { 0 };
	uint16_t send_len = 0; 

	ln_protocolintance = (ln_protocolintance_t *) sendbuf;
	ln_protocolintance->head = LNPROTOCOL_HEAD;
	ln_protocolintance->cmd = CMD_Conf_Reply_SelfStart;
	
	uint8_t * buf_ptr = (uint8_t *)&ln_protocolintance->payload;

	// --------defaultvalue ---

	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp, 3);
	send_len += 3;
	
	// --------selfstart flag -----
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&buf_temp, 1);
	send_len += 1;	

	// ---------------------------------------
	ln_protocolintance->len = send_len;
	*buf_ptr = LNprotocol_GetChecksum(&ln_protocolintance->head , send_len + 6);
	buf_ptr ++;
	*(buf_ptr ) = LNPROTOCOL_FOOT;
	buf_ptr ++;

	// --------Send---------
	APP_Conf_SendData( &ln_protocolintance->head , buf_ptr - &ln_protocolintance->head);
	// ----------------------	

}

void APP_Conf_Set_Sample_Rate(uint8_t * full_message , uint16_t full_len )
{
	// --------Send---------
	APP_Conf_SendData( full_message , full_len);
	// ---------------------
	
	g_SystemParam_Config.send_invteral = (uint16_t )(full_message[7] | full_message[8] << 8);
	// ------Save -------
	SystemParam_Save();
	// ------------------	
	
}


void APP_Conf_Reply_Sample_Rate(uint8_t * payload , uint16_t len)
{
	ln_protocolintance_t * ln_protocolintance = 0;
	uint8_t sendbuf[100] = { 0 };
	uint8_t buf_temp[10] = { 0 };
	uint16_t send_len = 0; 

	ln_protocolintance = (ln_protocolintance_t *) sendbuf;
	ln_protocolintance->head = LNPROTOCOL_HEAD;
	ln_protocolintance->cmd = CMD_Conf_Reply_Sample_Rate;
	
	uint8_t * buf_ptr = (uint8_t *)&ln_protocolintance->payload;

	// --------budong shi shenme?? -----
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp, 3);
	send_len += 3;		
	
	// --------send_invteral in sec -----
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&g_SystemParam_Config.send_invteral, 2);
	send_len += 2;	

	// --------budong shi shenme?? -----
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp, 4);
	send_len += 4;		
	
	// ---------------------------------------
	ln_protocolintance->len = send_len;
	*buf_ptr = LNprotocol_GetChecksum(&ln_protocolintance->head , send_len + 6);
	buf_ptr ++;
	*(buf_ptr ) = LNPROTOCOL_FOOT;
	buf_ptr ++;


	// --------Send---------
	APP_Conf_SendData( &ln_protocolintance->head , buf_ptr - &ln_protocolintance->head);
	// ----------------------	
}


void APP_Conf_GotoLowPower(uint8_t * full_message , uint16_t full_len) 
{
	APP_Conf_SendData( full_message , full_len);	
}
 
void APP_Conf_SetConf(uint8_t * full_message , uint16_t full_len) 
{

	full_message[9]=full_message[9]&0x3F;//限制数据方式为8N1;
	
	// --------Send---------
	APP_Conf_SendData( full_message , full_len);
	// ----------------------	
	
	g_SystemParam_Config.E32_conf.module_destination_addr = full_message[5]+((uint16_t)full_message[4]<<8);
	g_SystemParam_Config.E32_conf.module_source_addr = full_message[8]+((uint16_t)full_message[7]<<8);
	
	g_SystemParam_Config.E32_conf.module_airspeed = full_message[9]&0x7;
	g_SystemParam_Config.E32_conf.module_baudrate = (full_message[9]>>3)&0x7;
	g_SystemParam_Config.E32_conf.module_datacheck = (full_message[9]>>6)&0x3;
	
	g_SystemParam_Config.E32_conf.module_channel = full_message[10];
	
	g_SystemParam_Config.E32_conf.module_power = full_message[11]&3;
	g_SystemParam_Config.E32_conf.module_FEC = (full_message[11]>>2)&1;
	g_SystemParam_Config.E32_conf.module_wakeup_time = (full_message[11]>>3)&7;
	g_SystemParam_Config.E32_conf.module_IO_workstyle = (full_message[11]>>6)&1;
	g_SystemParam_Config.E32_conf.module_transmission_mode = (full_message[11]>>7)&1;
	
	
	// ------Save -------
	SystemParam_Save();
	// ------------------
	AppTask_Timer_Stop_Event(APP_TASK_TRANSFER_CORELOOP_EVENT);
	AppTask_Clear_Event(APP_TASK_TRANSFER_CORELOOP_EVENT);
	
	BSP_E32_AddCmd(E32_CMD_GETCONF_Req , 0);
}
 
void APP_Conf_Stop(uint8_t * full_message , uint16_t full_len) 
{
	APP_Conf_SendData( full_message , full_len);
}
void APP_Conf_Start(uint8_t * full_message , uint16_t full_len) 
{
	APP_Conf_SendData( full_message , full_len);
}
 
void APP_Conf_ID(uint8_t * payload , uint16_t len)
{

	ln_protocolintance_t * ln_protocolintance = 0;
	uint8_t sendbuf[100] = { 0 };

	uint16_t send_len = 0; 

	ln_protocolintance = (ln_protocolintance_t *) sendbuf;
	ln_protocolintance->head = LNPROTOCOL_HEAD;
	ln_protocolintance->cmd = CMD_Conf_ID;
	
	uint8_t * buf_ptr = (uint8_t *)&ln_protocolintance->payload;

	// --------SNcode ---

	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)g_SystemParam_Config.SNcode , 8);
	send_len += 8;

	// ---------------------------------------
	ln_protocolintance->len = send_len;
	*buf_ptr = LNprotocol_GetChecksum(&ln_protocolintance->head , send_len + 6);
	buf_ptr ++;
	*(buf_ptr ) = LNPROTOCOL_FOOT;
	buf_ptr ++;


	// --------Send---------
	
	APP_Conf_SendData( &ln_protocolintance->head , buf_ptr - &ln_protocolintance->head);
	// ----------------------
}

void APP_Conf_ReplyConf(uint8_t * payload , uint16_t len)
{

	ln_protocolintance_t * ln_protocolintance = 0;
	uint8_t sendbuf[100] = { 0 };
	uint8_t buf_temp[10] = { 0 };
	uint16_t send_len = 0; 

	ln_protocolintance = (ln_protocolintance_t *) sendbuf;
	ln_protocolintance->head = LNPROTOCOL_HEAD;
	ln_protocolintance->cmd = CMD_Conf_Reply_Conf;
	
	uint8_t * buf_ptr = (uint8_t *)&ln_protocolintance->payload;

	// -------- DestAddr ---
	buf_temp[0] = g_SystemParam_Config.E32_conf.module_destination_addr >> 8;
	buf_temp[1] = g_SystemParam_Config.E32_conf.module_destination_addr;
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp , 2);
	send_len += 2;
	// ---------kanbudong---
	buf_temp[0] = 0xc0;
	buf_temp[1] = g_SystemParam_Config.E32_conf.module_source_addr >> 8;
	buf_temp[2] = g_SystemParam_Config.E32_conf.module_source_addr ;
	buf_temp[3] = g_SystemParam_Config.E32_conf.module_airspeed|\
				(g_SystemParam_Config.E32_conf.module_baudrate<<3)|\
				(g_SystemParam_Config.E32_conf.module_datacheck<<6);
	buf_temp[4] = g_SystemParam_Config.E32_conf.module_channel ;
	buf_temp[5] = g_SystemParam_Config.E32_conf.module_power|\
				(g_SystemParam_Config.E32_conf.module_FEC<<2)|\
				(g_SystemParam_Config.E32_conf.module_wakeup_time<<3)|\
				(g_SystemParam_Config.E32_conf.module_IO_workstyle<<6)|\
				(g_SystemParam_Config.E32_conf.module_transmission_mode<<7);
	
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp , 6);
	send_len += 6;	
	
	// ---------------------
	// ---------------------------------------
	ln_protocolintance->len = send_len;
	*buf_ptr = LNprotocol_GetChecksum(&ln_protocolintance->head , send_len + 6);
	buf_ptr ++;
	*(buf_ptr ) = LNPROTOCOL_FOOT;
	buf_ptr ++;


	// --------Send---------
	
	APP_Conf_SendData( &ln_protocolintance->head , buf_ptr - &ln_protocolintance->head);
	// ----------------------
}





void APP_Conf_RevByteOneByte(uint8_t value)
{
	bsp_conf_rev( &value , 1);	
}

static int8_t bsp_conf_rev(uint8_t * buf , uint16_t len)
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

void APP_Conf_SendData( uint8_t *buf , uint16_t len)
{
	memcpy(app_confrev_space , buf , len);
	BSP_UART_WriteBytes_DMA( BSP_UART2 , app_confrev_space,  len);
}


void APP_Conf_SetConfStatus(void)
{
	BSP_LED_Blink( BSP_LED_TEST , 0 , 50, 1000);
	app_conf_confstatus = 1;
}

void APP_Conf_ClearConfStatus(void)
{
	BSP_LED_Blink( BSP_LED_TEST , 3 , 10, 150);
	app_conf_confstatus = 0;
}

uint8_t APP_Conf_GetConfStatus(void)
{
	return app_conf_confstatus;
}


// -------- Test Func ------------

uint8_t temp[10] = {0x12,0xff,0x43,0xfe,0x88};
void APP_Conf_TestFunc(void)
{
	
	APP_Conf_SendData( temp , 10);
}


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

