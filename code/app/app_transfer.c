/**
 **************************************************************************************************
 * @file        app_transfer.c
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
#include "app_transfer.h"
#include "system_param.h"
#include "clog.h"
#include "bsp_rtc.h"
#include "app_getdata.h"
#include "bsp_e32.h"
#include "lnprotocol.h"
#include "app_task.h"
#include "bsp_power.h"
#include "app_revmessage.h"
#include "app_conf.h"
#include "app_battery.h"
#include "bsp_e32.h"
#include "bsp_led.h"	

/**
 * @addtogroup    app_transfer_Modules 
 * @{  
 */

/**
 * @defgroup      app_transfer_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_transfer_Macros_Defines 
 * @brief         
 * @{  
 */
#define APP_TRANSFER_RESEND_TIMES     2 // if no ack need report times
/**
 * @}
 */

/**
 * @defgroup      app_transfer_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_transfer_Private_Types
 * @brief         
 * @{  
 */

typedef enum
{
	AppTransfer_CheckTime = 1,
	AppTransfer_SendReq ,
	App_Transfer_ReSendReq,
	AppTransfer_SendResp ,
	AppTransfer_SerNextTime ,
	AppTransfer_LowPower,
	AppTransfer_NormalPower,
	AppTransfer_OpenNet,
	AppTransfer_OpenADC,
	
}app_transfer_cmd_e;


typedef struct
{
	uint8_t cmd[3];
	uint8_t in ; 
	uint8_t out ;
	uint8_t count;
	uint8_t size ;
}app_transfer_cmd_queue_t;

typedef struct
{
	uint8_t buf[100];
	uint8_t len;
}app_transfer_resend_t;

/**
 * @}
 */

/**
 * @defgroup      app_transfer_Private_Variables 
 * @brief         
 * @{  
 */

app_transfer_cmd_queue_t app_transfer_cmd_queue = 
{
	.in = 0,
	.out = 0,
	.count = 0,
	.size = sizeof(app_transfer_cmd_queue.cmd) / sizeof(app_transfer_cmd_queue.cmd[0]),
};


static app_transfer_resend_t app_transfer_resend;

/**
 * @}
 */

/**
 * @defgroup      app_transfer_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_transfer_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static void app_transfer_enqueue_cmd(uint8_t cmd);
static uint8_t app_transfer_dequeue_cmd(void);
static uint8_t app_transfer_getcmdcount(void);
static void app_transfer_settime(void);
static void app_transfer_senddata_req(void);
static int8_t app_transfer_checktime(void);
static void app_transfer_lowpower(void);
static void app_transfer_senddata_resp(void);
static void app_transfer_re_senddata_req(void);
/**
 * @}
 */

/**
 * @defgroup      app_transfer_Functions 
 * @brief         
 * @{  
 */

static void app_transfer_enqueue_cmd(uint8_t cmd)
{
	app_transfer_cmd_queue.cmd[app_transfer_cmd_queue.in] = cmd;
	app_transfer_cmd_queue.in ++;
	app_transfer_cmd_queue.count ++;
	app_transfer_cmd_queue.in %= app_transfer_cmd_queue.size;
}

static uint8_t app_transfer_dequeue_cmd(void)
{
	uint8_t cmd = 0;
	if(app_transfer_cmd_queue.count > 0)
	{
		cmd = app_transfer_cmd_queue.cmd[app_transfer_cmd_queue.out ];
		app_transfer_cmd_queue.count --;
		app_transfer_cmd_queue.out ++;
		app_transfer_cmd_queue.out %= app_transfer_cmd_queue.size;
	}
	return cmd;
}
static uint8_t app_transfer_getcmdcount(void)
{
	return app_transfer_cmd_queue.count;
}

void APP_Transfer_cmdClear(void)
{
	app_transfer_cmd_queue.count = 0;
	app_transfer_cmd_queue.in = 0;
	app_transfer_cmd_queue.out = 0;
}


void APP_Transfer_CoreLoop(void)
{
	uint8_t cmd = 0;
	
	if(app_transfer_getcmdcount() > 0)
	{
		cmd = app_transfer_dequeue_cmd();
	}
	else
	{
		cmd = AppTransfer_CheckTime;
	}
	
	switch(cmd)
	{
		case AppTransfer_CheckTime :
		{
			DEBUG("AppTransfer_CheckTime\r\n");
			
			if(APP_Conf_GetConfStatus() == 1)
			{
				AppTask_Timer_Start_Event(APP_TASK_TRANSFER_CORELOOP_EVENT , 1000);
			}
			else
			{
				if(app_transfer_checktime() == 1)
				{
					app_transfer_enqueue_cmd(AppTransfer_SendReq);
					AppTask_Timer_Start_Event(APP_TASK_TRANSFER_CORELOOP_EVENT , 500);
					//AppTask_Send_Event(APP_TASK_TRANSFER_CORELOOP_EVENT);
				}
				else
				{
					app_transfer_enqueue_cmd(AppTransfer_SerNextTime);
					AppTask_Timer_Start_Event(APP_TASK_TRANSFER_CORELOOP_EVENT , 1000);
				}
			}
		}
		break;
		case AppTransfer_SendReq:
		{
			DEBUG("AppTransfer_SendReq\r\n");
			
			BSP_E32_SetMode(E32_MODE_NORMAL);
			BSP_E32_Power_ON();
			app_transfer_senddata_req();
			app_transfer_enqueue_cmd(AppTransfer_SendResp);
			AppTask_Timer_Start_Event(APP_TASK_TRANSFER_CORELOOP_EVENT , 1000);
			APP_RevClearAckFlag();				


		} 
		break;
		case App_Transfer_ReSendReq:
		{
			DEBUG("App_Transfer_ReSendReq\r\n");
			BSP_E32_Power_ON();
			BSP_E32_SetMode(E32_MODE_NORMAL);
			app_transfer_re_senddata_req();
			app_transfer_enqueue_cmd(AppTransfer_SendResp);
			AppTask_Timer_Start_Event(APP_TASK_TRANSFER_CORELOOP_EVENT , 1000);
			APP_RevClearAckFlag();				
	
		}
		break;
		case AppTransfer_SendResp:
		{
			app_transfer_senddata_resp();
			DEBUG("APPTransfer_SendResp\r\n");
		}
		break;
		case AppTransfer_SerNextTime:
		{
			DEBUG("AppTransfer_SerNextTime\r\n");
			app_transfer_settime();
			app_transfer_enqueue_cmd(AppTransfer_LowPower);
			AppTask_Send_Event(APP_TASK_TRANSFER_CORELOOP_EVENT);
		}
		break;
		case AppTransfer_LowPower:
		{
			DEBUG("AppTransfer_LowPower\r\n");
			APP_Battery_Reduce();	
			app_transfer_lowpower();
			app_transfer_enqueue_cmd(AppTransfer_CheckTime);
			AppTask_Send_Event(APP_TASK_TRANSFER_CORELOOP_EVENT);
		}
		break;
		default:break;
	}
}


void APP_LowPower_BeforeFunc(void)
{
	BSP_LED_Close(BSP_LED1);
	BSP_E32_Close();
	APP_GetData_DeInit();
	BSP_Power_V30_OFF();


	
}
void APP_LowPower_AfterFunc(void)
{
	BSP_E32_Open();
	BSP_Power_V30_ON();
	APP_GetData_Init();	
}


static void app_transfer_lowpower(void)
{
	BSP_Power_EnterVLPS_WithCallFunc();
}


static int8_t app_transfer_checktime(void)
{
	uint32_t cur_time = 0 ;
	cur_time = BSP_RTC_GetCurTimeStamp();
		
	if(g_SystemParam_Config.workmode == Normal_WorkingMode || g_SystemParam_Config.send_invteral < 10)
	{
		if((cur_time - g_SystemParam_Config.firsttimestamp) % 10 <= 1)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if((cur_time - g_SystemParam_Config.firsttimestamp) % g_SystemParam_Config.send_invteral <=1 )
		{
			return 1;
		}
		else
		{
			
			return -1;
		}		
	}
}


static void app_transfer_settime(void)
{
	uint32_t cur_time = 0 ;
	uint32_t next_time = 0;
	cur_time = BSP_RTC_GetCurTimeStamp();
	
	if(g_SystemParam_Config.workmode == Normal_WorkingMode || g_SystemParam_Config.send_invteral < 10)
	{
		g_SystemParam_Config.send_invteral = 10;
	}
	
	if(g_SystemParam_Config.firsttimestamp != 0)
	{
		next_time = ((cur_time - g_SystemParam_Config.firsttimestamp) / g_SystemParam_Config.send_invteral + 1) * \
					g_SystemParam_Config.send_invteral	+ g_SystemParam_Config.firsttimestamp ;
	}
	else
	{
		next_time = cur_time + 10;
	}
	
	BSP_RTC_SetAlarm_InTimeStamp(next_time);
}


void APP_Transfer_SendDataSerial_Process(void)
{
	if(APP_Conf_GetConfStatus() == 1)
	{
		APP_Transfer_SendData_Serial();
		AppTask_Timer_Start_Event(APP_TASK_REPORTVALUE_INSERIAL_EVENT,10000);
	}
	else
	{
		
	}
}


void APP_Transfer_SendData_Serial(void)
{
	
	App_Data_t * App_Data;
	ln_protocolintance_t * ln_protocolintance = 0;
	uint8_t sendbuf[100] = { 0 };
	uint8_t buf_temp[10] = { 0 };
	uint16_t len = 0; 


	App_Data = APP_GetData_Get();


	ln_protocolintance = (ln_protocolintance_t *) sendbuf;
	ln_protocolintance->head = LNPROTOCOL_HEAD;
	ln_protocolintance->cmd = CMD_ReportData;
	
	uint8_t * buf_ptr = (uint8_t *)&ln_protocolintance->payload;
	// -------- add one flag ---
	buf_temp[0] = 0x01;
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp , 1);
	len += 1;
	// --------Value ---
	if(App_Data->device_status == MA4_20_OVER)
	{
		float errcode = 999.999f;
		buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&errcode , 4);
	}
	else if(App_Data->device_status == MA4_20_LOST)
	{
		float errcode = -999.999f;
		buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&errcode , 4);
	}
	else if(App_Data->device_status == MA4_20_NORMAL)
	{
		buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&App_Data->need_value , 4);
	}	

	len += 4;
	// --------Battery ---
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&g_SystemParam_Config.battery , 1);
	len += 1;
	// --------SNcode ---
	buf_temp[0] = g_SystemParam_Config.SNcode[6];
	buf_temp[1] = g_SystemParam_Config.SNcode[7]; 
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp , 2);
	len += 2;

	// ---------------------------------------
	ln_protocolintance->len = len ; //
	*buf_ptr = LNprotocol_GetChecksum(&ln_protocolintance->head , len + 6);

	buf_ptr += 1;
	*(buf_ptr ) = LNPROTOCOL_FOOT;
	buf_ptr ++;

	APP_Conf_SendData(&ln_protocolintance->head ,buf_ptr - &ln_protocolintance->head)	;	
		 
}


static void app_transfer_senddata_req(void)
{
	App_Data_t * App_Data;
	ln_protocolintance_t * ln_protocolintance = 0;
	uint16_t checksum = 0;
	uint8_t sendbuf[100] = { 0 };
	uint8_t buf_temp[10] = { 0 };
	uint16_t len = 0; 

	rtc_datetime_t datetime;
	App_Data = APP_GetData_Get();
	BSP_RTC_GetDate(&datetime);

	ln_protocolintance = (ln_protocolintance_t *) sendbuf;
	ln_protocolintance->head = LNPROTOCOL_HEAD;
	ln_protocolintance->cmd = CMD_ReportData;
	
	uint8_t * buf_ptr = (uint8_t *)&ln_protocolintance->payload;
	// -------- add one flag ---
	buf_temp[0] = 0x01;
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp , 1);
	len += 1;
	// --------Value ---
	if(App_Data->device_status == MA4_20_OVER)
	{
		float errcode = 999.999f;
		buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&errcode , 4);
	}
	else if(App_Data->device_status == MA4_20_LOST)
	{
		float errcode = -999.999f;
		buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&errcode , 4);
	}
	else if(App_Data->device_status == MA4_20_NORMAL)
	{
		buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&App_Data->need_value , 4);
	}	

	len += 4;
	// --------Battery ---
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)&g_SystemParam_Config.battery , 1);
	len += 1;
	// --------SNcode ---
					
	buf_temp[0] = g_SystemParam_Config.SNcode[6];
	buf_temp[1] = g_SystemParam_Config.SNcode[7] ;
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp , 2);
	len += 2;
	// --------module srcaddr ---
	buf_temp[0] = g_SystemParam_Config.E32_conf.module_source_addr >> 8;
	buf_temp[1] = (uint8_t )g_SystemParam_Config.E32_conf.module_source_addr ;
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp , 2);
	len += 2;	
	// --------year mon day hour min sec ---
	buf_temp[0] = datetime.year >> 8;
	buf_temp[1] = (uint8_t )datetime.year ;
	buf_temp[2] = datetime.month ;
	buf_temp[3] = datetime.day ;
	buf_temp[4] = datetime.hour ;
	buf_temp[5] = datetime.minute ;
	buf_temp[6] = datetime.second ;
	buf_ptr = LNprotocol_AddPayload(buf_ptr, (uint8_t *)buf_temp , 7);
	len += 7;	
	// ---------------------------------------
	ln_protocolintance->len = len + 1; // 奇怪的报文格式~~~
	//*buf_ptr = LNprotocol_GetChecksum(&ln_protocolintance->head , len + 6);
	checksum = LNprotocol_GetChecksum116bits(&ln_protocolintance->head , len + 6);
	
	buf_temp[0] = checksum >> 8;
	buf_temp[1] = (uint8_t )checksum ;	
	
	memcpy(buf_ptr , (uint8_t *)&buf_temp , 2);
	buf_ptr += 2;
	*(buf_ptr ) = LNPROTOCOL_FOOT;
	buf_ptr ++;

	BSP_E32_SendData(g_SystemParam_Config.E32_conf.module_destination_addr ,\
					g_SystemParam_Config.E32_conf.module_channel, \
					&ln_protocolintance->head ,\
					buf_ptr - &ln_protocolintance->head);
	app_transfer_resend.len = buf_ptr - &ln_protocolintance->head;		
	memcpy(app_transfer_resend.buf ,&ln_protocolintance->head, app_transfer_resend.len);
	
}


static void app_transfer_re_senddata_req(void)
{
	BSP_E32_SendData(g_SystemParam_Config.E32_conf.module_destination_addr ,\
					g_SystemParam_Config.E32_conf.module_channel, \
					app_transfer_resend.buf ,\
					app_transfer_resend.len);	
}

static void app_transfer_senddata_resp(void)
{
	static uint8_t timeout_count = 0;
	switch(APP_RevGetAckFlag())
	{
		case APP_RevACK_Wait : // wait
		{
			DEBUG("APP_RevACK_Wait\r\n");
			app_transfer_enqueue_cmd(AppTransfer_SendResp);
			AppTask_Timer_Start_Event(APP_TASK_TRANSFER_CORELOOP_EVENT , 1000);			
		}
		break;
		case APP_RevACK_Get: //get
		{
			timeout_count = 0;
			DEBUG("APP_RevACK_Get\r\n");
			app_transfer_enqueue_cmd(AppTransfer_SerNextTime);
			AppTask_Send_Event(APP_TASK_TRANSFER_CORELOOP_EVENT);
		}
		break;
		case APP_RevACK_Timeout: // timeout
		{
			timeout_count ++;
			DEBUG("APP_RevACK_Timeout :%d\r\n" , timeout_count);
			if(timeout_count <= APP_TRANSFER_RESEND_TIMES)
			{
				app_transfer_enqueue_cmd(App_Transfer_ReSendReq);
				AppTask_Send_Event(APP_TASK_TRANSFER_CORELOOP_EVENT);
			}
			else
			{
				timeout_count = 0 ;
				app_transfer_enqueue_cmd(AppTransfer_SerNextTime);
				AppTask_Send_Event(APP_TASK_TRANSFER_CORELOOP_EVENT);
			}
		}
		break;
		default:break;
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

