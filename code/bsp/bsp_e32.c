/**
 **************************************************************************************************
 * @file        bsp_e32.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

#include "bsp_e32.h"
#include "bsp_conf.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_uart.h"
#include "clog.h"
#include "net_task.h"
#include "bsp_systick.h"
#include "system_param.h"
#include "bsp_led.h"
#include "app_revmessage.h"
#include "app_task.h"

/**
 * @addtogroup    bsp_e32_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_e32_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_e32_Macros_Defines 
 * @brief         
 * @{  
 */
#define E32_M0_UP  		GPIO_WritePinOutput(GPIOD, 7, 1)
#define E32_M0_DOWN  	GPIO_WritePinOutput(GPIOD, 7, 0)
#define E32_M1_UP  		GPIO_WritePinOutput(GPIOD, 6, 1)
#define E32_M1_DOWN  	GPIO_WritePinOutput(GPIOD, 6, 0)
#define E32_AUX_STATUS	GPIO_ReadPinInput(GPIOA, 4)

#define E32_POWER_ON	GPIO_WritePinOutput(GPIOE, 19, 0)
#define E32_POWER_OFF	GPIO_WritePinOutput(GPIOE, 19, 1)


#define BSP_E32_SENDBUF_LEN 100 
/**
 * @}
 */

/**
 * @defgroup      bsp_e32_Constants_Defines 
 * @brief         
 * @{  
 */



/**
 * @}
 */

/**
 * @defgroup       bsp_e32_Private_Types
 * @brief         
 * @{  
 */
#pragma pack(1)
typedef struct
{
	uint16_t destaddr;
	uint8_t channel;
	uint8_t * payload;
}bsp_e32_senddata_t ;
#pragma pack()

typedef struct 
{
	uint8_t buf[BSP_E32_SENDBUF_LEN] ;
	uint8_t len ;
}bsp_e32_databuf_t;	
 
typedef struct
{
	uint8_t in ;
	uint8_t out ; 
	uint8_t count ;
	uint8_t size ;
	bsp_e32_databuf_t queue[3];
}BSP_E32_DataQueue_t;

BSP_E32_DataQueue_t BSP_E32_SendQueue =
{
	.in = 0,
	.out = 0,
	.count = 0,
	.size = sizeof(BSP_E32_SendQueue.queue)/sizeof(BSP_E32_SendQueue.queue[0]),
};

/**
 * @}
 */

/**
 * @defgroup      bsp_e32_Private_Variables 
 * @brief         
 * @{  
 */

static uint8_t BSP_E32_SetConfCMD_Save[6]	 = {0xC0 ,} ;
static uint8_t BSP_E32_GetConfCMD[3]		 = {0xC1 ,0xC1 , 0xC1} ;
//static uint8_t BSP_E32_SetConfCMD_NoSave[6]	 = {0xC2 ,};
//static uint8_t BSP_E32_GetVersionCMD[3]		 = {0xC3 , 0xC3 , 0xC3};
//static uint8_t BSP_E32_RestCMD[3]			 = {0xC4 , 0xC4 ,0xC4};

static uint8_t rev_flag = 0;
static uint8_t BSP_E32_CurrentStatus = 0;



static uint8_t bsp_e32_senddatabuf[BSP_E32_SENDBUF_LEN] = { 0 }; 

static BSP_E32_CmdQueue_t BSP_E32_CmdQueue = 
{
	.in = 0,
	.out = 0,
	.count = 0,
	.size = sizeof(BSP_E32_CmdQueue.buf) / sizeof(BSP_E32_CmdQueue.buf[0]),
};
/**
 * @}
 */

/**
 * @defgroup      bsp_e32_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_e32_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static void bsp_e32_halinit(void); 
static void bsp_e32_getconf(void);
static void bsp_e32_setconf(void);
static void bsp_e32_ClearCmd(void);
static void BSP_E32_ModlueRevAnalysis(uint8_t * buf, uint8_t len );
static void bsp_e32_haldeinit(void);
/**
 * @}
 */

/**
 * @defgroup      bsp_e32_Functions 
 * @brief         
 * @{  
 */
 

 
void BSP_E32_Init(void)
{
	bsp_e32_halinit();
	// -------Uart ----------
	BSP_UART_Init(BSP_UART0);
	// ----------------------	
	BSP_E32_Power_OFF();
	BSP_Systick_Delayms(5);
	BSP_E32_Power_ON();
	BSP_Systick_Delayms(1);
	DEBUG("BSP_E32_Init\r\n");
	BSP_E32_AddCmd(E32_CMD_GETCONF_Req , 0);
	
}
static void bsp_e32_haldeinit(void)
{


	gpio_pin_config_t gpio_pin_config ;
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
	GPIO_PinInit(GPIOD, 7, &gpio_pin_config);
	

	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
	GPIO_PinInit(GPIOD, 6, &gpio_pin_config);


	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;  //AUX is input pin
	GPIO_PinInit(GPIOA, 4, &gpio_pin_config);


	
	//GPIO_WritePinOutput(GPIOD, 7, 0); // M0
	//GPIO_WritePinOutput(GPIOD, 6, 0); // M1
	GPIO_WritePinOutput(GPIOA, 4, 0); // AUX


	
}
/*
M0 -> PD7
M1 -> PD6
AUX -> PA4
E32_Power -> PE19

Uart0
*/
static void bsp_e32_halinit(void)
{
	// ------GPIO --------
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortE);
	
	PORT_SetPinMux(PORTD, 7,kPORT_MuxAsGpio);
	gpio_pin_config_t gpio_pin_config ;
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
	GPIO_PinInit(GPIOD, 7, &gpio_pin_config);
	
	PORT_SetPinMux(PORTD, 6,kPORT_MuxAsGpio);
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
	GPIO_PinInit(GPIOD, 6, &gpio_pin_config);

	PORT_SetPinMux(PORTA, 4,kPORT_MuxAsGpio);
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalInput;  //AUX is input pin
	GPIO_PinInit(GPIOA, 4, &gpio_pin_config);

	PORT_SetPinMux(PORTE, 19,kPORT_MuxAsGpio);
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
	GPIO_PinInit(GPIOE, 19, &gpio_pin_config);	
	
	BSP_E32_SetMode(E32_MODE_SLEEP);
	GPIO_WritePinOutput(GPIOA, 4, 0); // AUX	
}





uint8_t BSP_E32_GetAUX_Status(void)
{
	return E32_AUX_STATUS;
}

void BSP_E32_Power_ON(void)
{
	E32_POWER_ON;
}

void BSP_E32_Power_OFF(void)
{
	E32_POWER_OFF;
}

void BSP_E32_SetMode(E32_Mode_e mode)  // after change mode ,the module need 1ms delay . sug: check AUX high 2ms to start change mode
{
	switch((uint8_t )mode)
	{
		case E32_MODE_NORMAL : 
		{
			DEBUG("E32 Enter Normal Mode\r\n");
			E32_M0_DOWN;
			E32_M1_DOWN;
			BSP_E32_CurrentStatus = E32_MODE_NORMAL;
		}
		break;
		case E32_MODE_WAKEUP:
		{
			DEBUG("E32 Enter Wakeup Mode\r\n");
			E32_M0_UP;
			E32_M1_DOWN;		
			BSP_E32_CurrentStatus = E32_MODE_WAKEUP;
		}
		break;
		case E32_MODE_LOWPOWER:
		{
			DEBUG("E32 Enter Lowpower Mode\r\n");
			E32_M0_DOWN;
			E32_M1_UP;	
			BSP_E32_CurrentStatus = E32_MODE_LOWPOWER;
		}
		break;
		case E32_MODE_SLEEP:
		{
			DEBUG("E32 Enter Sleep Mode\r\n");
			E32_M0_UP;
			E32_M1_UP;		
			BSP_E32_CurrentStatus = E32_MODE_SLEEP;			
		}
		break;
		default:
			break;
	}
}


uint8_t BSP_E32_GetMode(void)
{
	return BSP_E32_CurrentStatus;
}


void BSP_E32_WriteBytes(uint8_t *buf , uint16_t len)
{
	BSP_UART_WriteBytes_DMA( BSP_UART0 , buf,  len);
}



void BSP_E32_SendData(uint16_t destaddr , uint8_t channel, uint8_t *buf , uint16_t len)
{	
	bsp_e32_senddata_t * sendbuf = (bsp_e32_senddata_t *) &bsp_e32_senddatabuf;
	sendbuf->destaddr = (*(uint8_t *)&destaddr << 8)| (*((uint8_t *)&destaddr + 1) );
	sendbuf->channel = channel;
	memcpy( &sendbuf->payload , buf , len);
	BSP_E32_AddSendBuf( (uint8_t *)sendbuf , len  + 3);
}

void BSP_E32_AddSendBuf(uint8_t *buf , uint16_t len)
{
	memcpy(BSP_E32_SendQueue.queue[BSP_E32_SendQueue.in].buf , buf , len);
	BSP_E32_SendQueue.queue[BSP_E32_SendQueue.in].len = len ;
	BSP_E32_SendQueue.in ++ ;
	BSP_E32_SendQueue.count ++;
	BSP_E32_SendQueue.in %= BSP_E32_SendQueue.size;
	
	NetTask_Send_Event(NET_TASK_SEND_EVENT);
}

static void bsp_e32_ClearCmd(void)
{
	BSP_E32_CmdQueue.count = 0;
	BSP_E32_CmdQueue.in = 0;
	BSP_E32_CmdQueue.out = 0;
}

void BSP_E32_AddCmd(BSP_E32_CMD_e cmd , uint32_t delay_ms)
{
	BSP_E32_CmdQueue.buf[BSP_E32_CmdQueue.in] = cmd;
	BSP_E32_CmdQueue.in ++;
	BSP_E32_CmdQueue.count ++;
	BSP_E32_CmdQueue.in %= BSP_E32_CmdQueue.size;
	
	if(delay_ms > 0)
	{
		NetTask_Timer_Start_Event(NET_TASK_CORE_LOOP_EVENT,10);
	}
	else
	{
		NetTask_Send_Event(NET_TASK_CORE_LOOP_EVENT);
	}
	
	
	
}
uint8_t BSP_E32_LoadCmd(void)
{
	uint8_t cmd = E32_CMD_IDLE;
	if(BSP_E32_CmdQueue.count > 0)
	{
		cmd = BSP_E32_CmdQueue.buf[BSP_E32_CmdQueue.out];
		BSP_E32_CmdQueue.out ++;
		BSP_E32_CmdQueue.count --;
		BSP_E32_CmdQueue.out %= BSP_E32_CmdQueue.size;		
	}
	return cmd ; 
}

uint8_t BSP_E32_GetCmdCount(void)
{
	return BSP_E32_CmdQueue.count;
}


static uint8_t e32_cmd_getconf_flag = 0 ; 

void BSP_E32_CoreLoop(void)
{
	uint8_t currnt_cmd = 0;
	uint32_t time_cout = 0;
	static uint8_t AUX_timeout  = 0;
	
	if(E32_AUX_STATUS == 1)
	{
		AUX_timeout = 0;
	}
	else
	{
		if(AUX_timeout > 250)  // rest module
		{
			AUX_timeout = 0;
			DEBUG("AUX_timeout\r\n");
			
			NetTask_Clear_Event(NET_TASK_CORE_LOOP_EVENT);
			NetTask_Timer_Stop_Event(NET_TASK_CORE_LOOP_EVENT);
			
			bsp_e32_ClearCmd();
			NetTask_Send_Event(NET_TASK_MODULE_INIT_EVENT);
		}
		AUX_timeout ++;
		return;
	}
	
	currnt_cmd = BSP_E32_LoadCmd();

	switch(currnt_cmd)
	{
		case E32_CMD_IDLE : 
			{
				//DEBUG("E32_CMD_IDLE\r\n"); 
			}break;
		case E32_CMD_REST : DEBUG("E32_CMD_REST\r\n"); break;
		case E32_CMD_GETCONF_Req : 
			{
				DEBUG("E32_CMD_GETCONF_Req\r\n");
				bsp_e32_getconf();
				BSP_E32_AddCmd( E32_CMD_GETCONF_Resp , 20);
				time_cout = OS_Clock_GetSystemClock();
			}; 
			break;
		case E32_CMD_GETCONF_Resp:
			{
				if(e32_cmd_getconf_flag == 0)
				{
					if( (OS_Clock_GetSystemClock() - time_cout) > 100)
					{
						BSP_E32_AddCmd( E32_CMD_GETCONF_Req , 0);
					}
					else
					{
						BSP_E32_AddCmd( E32_CMD_GETCONF_Resp , 20);
					}
				}
				else
				{
					e32_cmd_getconf_flag = 0;
					DEBUG("E32_CMD_GETCONF_Resp\r\n");
				}	
			}
			break;
		case E32_CMD_SETCONF_Req:
			{
				DEBUG("E32_CMD_SETCONF_Req\r\n");
				bsp_e32_setconf();
				BSP_E32_AddCmd( E32_CMD_GETCONF_Resp , 20);
			}
			break;	
		case E32_CMD_CONF_OK:
			{
				DEBUG("E32_CMD_CONF_OK\r\n");
				BSP_UART_SetBaudRate(BSP_UART0 , 115200);
				AppTask_Send_Event(APP_TASK_TRANSFER_CORELOOP_EVENT);
				BSP_LED_Blink( BSP_LED_TEST , 3 , 50, 1000); // 
			}
			break;
		default:break;
	}
	
}

static void bsp_e32_getconf(void)
{
	
	BSP_UART_SetBaudRate(BSP_UART0 , 9600);
	BSP_E32_SetMode(E32_MODE_SLEEP);
	BSP_E32_Power_ON();	
	BSP_E32_AddSendBuf(BSP_E32_GetConfCMD , sizeof(BSP_E32_GetConfCMD));
}


static void bsp_e32_setconf(void)
{
	BSP_E32_SetConfCMD_Save[0]=0xC0;  //µôµç±£´æ
	BSP_E32_SetConfCMD_Save[1]=g_SystemParam_Config.E32_conf.module_source_addr>>8;
	BSP_E32_SetConfCMD_Save[2]=g_SystemParam_Config.E32_conf.module_source_addr;
	BSP_E32_SetConfCMD_Save[3]=g_SystemParam_Config.E32_conf.module_airspeed|\
								(g_SystemParam_Config.E32_conf.module_baudrate<<3)|\
								(g_SystemParam_Config.E32_conf.module_datacheck<<6);
	BSP_E32_SetConfCMD_Save[4]=g_SystemParam_Config.E32_conf.module_channel;
	BSP_E32_SetConfCMD_Save[5]=g_SystemParam_Config.E32_conf.module_power|\
								(g_SystemParam_Config.E32_conf.module_FEC<<2)|\
								(g_SystemParam_Config.E32_conf.module_wakeup_time<<3)|\
								(g_SystemParam_Config.E32_conf.module_IO_workstyle<<6)|\
								(g_SystemParam_Config.E32_conf.module_transmission_mode<<7);
	BSP_E32_AddSendBuf(BSP_E32_SetConfCMD_Save , sizeof(BSP_E32_SetConfCMD_Save));
}

typedef struct
{
	uint8_t buf[100];
	uint8_t len ; 
}bsp_e32_revbuf_t;

static bsp_e32_revbuf_t bsp_e32_revbuf = 
{
	.len = 0
};

void BSP_E32_Rev(void)
{
	rev_flag = 0;
	DEBUG("BSP_E32_Rev len:%d\r\n" , bsp_e32_revbuf.len);
	DEBUG("Value:\r\n");
	for(uint8_t i = 0 ; i < bsp_e32_revbuf.len ; i ++)
	{
		DEBUG(" %X ",bsp_e32_revbuf.buf[i]);
	}
	DEBUG("\r\n");
	
	
	uint8_t e32_revbuf[100];
	uint8_t e32_revlen = 0;
	e32_revlen = bsp_e32_revbuf.len;
	memcpy(e32_revbuf , bsp_e32_revbuf.buf ,bsp_e32_revbuf.len );

	bsp_e32_revbuf.len = 0;
	
	//if(e32_revbuf[0] == 0xC0 && e32_revlen == 6)
		
	if(BSP_E32_GetMode() == E32_MODE_SLEEP)
	{
		e32_cmd_getconf_flag = 1;
		BSP_E32_ModlueRevAnalysis(e32_revbuf , e32_revlen );
	}
	else if(BSP_E32_GetMode() == E32_MODE_NORMAL)
	{
		APP_RevMess_Analysis(e32_revbuf , e32_revlen );
		BSP_LED_Blink( BSP_LED_TEST , 2 , 10, 80);
	}
}

static void BSP_E32_ModlueRevAnalysis(uint8_t * buf, uint8_t len )
{
	uint16_t module_localaddr = 0 ;
	uint8_t module_TTLcheck  = 0 ;
	uint8_t module_TTLbaudrate = 0;
	uint8_t module_Airbaudrate = 0;
	uint8_t module_chan = 0;
	uint8_t module_FEC = 0;
	uint8_t module_power = 0;
	uint8_t module_transmission_mode = 0;
	uint8_t module_IO_workstyle = 0;
	uint8_t module_wakeup_time = 0;
	
	module_localaddr = buf[1] * 256 + buf[2];
	module_TTLcheck = (buf[3] & 0xC0) >> 6;
	module_TTLbaudrate = (buf[3] & 0x38) >> 3;
	module_Airbaudrate= (buf[3] & 0x07);
	module_chan = buf[4];
	module_transmission_mode = (buf[5] & 0x80) >> 7;
	module_IO_workstyle = (buf[5] & 0x40) >> 6;
	module_wakeup_time = (buf[5] & 0x38) >> 3;
	module_FEC = (buf[5] & 0x04) >> 2;
	module_power = (buf[5] & 0x03);
	
	if(module_localaddr != g_SystemParam_Config.E32_conf.module_source_addr || \
		module_TTLcheck != g_SystemParam_Config.E32_conf.module_datacheck || \
		module_TTLbaudrate != g_SystemParam_Config.E32_conf.module_baudrate || \
		module_Airbaudrate != g_SystemParam_Config.E32_conf.module_airspeed || \
		module_chan != g_SystemParam_Config.E32_conf.module_channel ||\
		module_transmission_mode != g_SystemParam_Config.E32_conf.module_transmission_mode || \
		module_IO_workstyle != g_SystemParam_Config.E32_conf.module_IO_workstyle || \
		module_wakeup_time != g_SystemParam_Config.E32_conf.module_wakeup_time || \
		module_FEC != g_SystemParam_Config.E32_conf.module_FEC || \
		module_power != g_SystemParam_Config.E32_conf.module_power
		)
	{
		DEBUG("E32 has Some different\r\n");
		
		BSP_E32_AddCmd( E32_CMD_SETCONF_Req , 0);
	}
	else
	{
		BSP_E32_AddCmd( E32_CMD_CONF_OK , 0);
	}
	
	DEBUG("LocalAddr:%04X\r\n" , module_localaddr);

	DEBUG("g_LocalAddr:%04X\r\n" , g_SystemParam_Config.E32_conf.module_source_addr);

		
}

void BSP_E32_ConfChange(void)
{
	BSP_E32_AddCmd( E32_CMD_SETCONF_Req , 0);
}


void BSP_E32_RevByteOneByte(uint8_t value)
{
	bsp_e32_revbuf.buf[bsp_e32_revbuf.len ++] = value;
	bsp_e32_revbuf.len %= sizeof(bsp_e32_revbuf.buf) /  sizeof(bsp_e32_revbuf.buf[0]);
	if(rev_flag == 0)
	{
		NetTask_Timer_Start_Event(NET_TASK_REV_EVENT,10);
	}
}


void BSP_E32_Close(void)
{
	BSP_E32_SetMode(E32_MODE_SLEEP);
	BSP_E32_Power_OFF();
	
	BSP_Uart0_Close();
	bsp_e32_haldeinit();
	
	BSP_E32_CmdQueue.in = 0;
	BSP_E32_CmdQueue.count = 0;
	BSP_E32_CmdQueue.out = 0;
	
	NetTask_Clear_Event(NET_TASK_MODULE_INIT_EVENT);
	NetTask_Clear_Event(NET_TASK_CORE_LOOP_EVENT);
	NetTask_Clear_Event(NET_TASK_REV_EVENT);
}

void BSP_E32_Open(void)
{
	
	bsp_e32_halinit();
	BSP_E32_SetMode(E32_MODE_NORMAL);
	
	BSP_E32_Power_ON();
	BSP_Uart0_Open();

	NetTask_Send_Event(NET_TASK_CORE_LOOP_EVENT);
}

void BSP_E32_SendLoop(void)
{

	if(E32_AUX_STATUS == 1)
	{

	}
	else
	{
		
		NetTask_Timer_Start_Event(NET_TASK_SEND_EVENT , 10);
		
//		if(AUX_timeout > 250)  // rest module
//		{
//			AUX_timeout = 0;
//			DEBUG("AUX_timeout\r\n");
//			
//			NetTask_Clear_Event(NET_TASK_CORE_LOOP_EVENT);
//			NetTask_Timer_Stop_Event(NET_TASK_CORE_LOOP_EVENT);
//			
//			bsp_e32_ClearCmd();
//			NetTask_Send_Event(NET_TASK_MODULE_INIT_EVENT);
//		}

		return;
	}	
	
	
	if(BSP_E32_SendQueue.count > 0)
	{
		DEBUG("BSP_E32_SendLen : %d\r\n" , BSP_E32_SendQueue.queue[BSP_E32_SendQueue.out].len);
		BSP_LED_Blink( BSP_LED_TEST , 2 , 10, 80);
		BSP_E32_WriteBytes(BSP_E32_SendQueue.queue[BSP_E32_SendQueue.out].buf , BSP_E32_SendQueue.queue[BSP_E32_SendQueue.out].len);
		BSP_E32_SendQueue.count -- ;
		BSP_E32_SendQueue.out ++;
		BSP_E32_SendQueue.out %= BSP_E32_SendQueue.size;

		if(BSP_E32_SendQueue.count > 0)
		{
			NetTask_Send_Event(NET_TASK_SEND_EVENT);
		}
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

