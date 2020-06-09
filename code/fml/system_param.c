/**
 **************************************************************************************************
 * @file        system_param.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *              
 **************************************************************************************************
 */
#include "self_def.h"
#include "system_param.h"
#include "bsp_flash.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "clog.h"
#include "crc.h"

#include "bsp_conf.h"
/**
 * @addtogroup    system_param_Modules 
 * @{  
 */

/**
 * @defgroup      system_param_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Private_Variables 
 * @brief         
 * @{  
 */
const SystemParam_Config_t SystemParam_Config_Default = 
{
	.E32_conf.module_source_addr = 0x0002 , 
	.E32_conf.module_destination_addr = 4000 , 
	.E32_conf.module_datacheck = 0 ,//0:8N1; 1:8o1;  2:8E1;  3:8N1;
	.E32_conf.module_baudrate = 7 , //0:1200; 1:2400;  2:4800;  3:9600; 4:19200;  5:38400;  6:57600; 7:115200;
	.E32_conf.module_airspeed = 2 ,//0:0.3K 1:1.2K 2:2.4k 3:4.8K 4:9.6K 5:19.2K
	.E32_conf.module_channel = 0x0F , //00H-1FH,频率 410~441MHz ；410MHz+CHAN * 1MHz
	.E32_conf.module_transmission_mode = 1 ,//0:透明传输 1：定点传输
	.E32_conf.module_IO_workstyle = 0 , //0:推挽输出 1：开路输出
	.E32_conf.module_wakeup_time = 0,//0:250ms 1;500ms 2：750ms 3?1000  4：1250   5：1500 56:1750  72000
	.E32_conf.module_FEC = 1,   //0:关闭 1：开启
	.E32_conf.module_power = 0, //0:20dB 1:17dB 2:14dB 3:10dB
	
	.Analog_conf.adc_k = 1.0,
	.Analog_conf.adc_b = 0.0,
	.Analog_conf.real_k = 1.0,
	.Analog_conf.real_b = 0.0,
	
	.firsttimestamp = 0x386D4380,
	.send_invteral = 10,
	.workmode = LowPower_WorkingMode,
	.battery = 100,
	.waitstable = 2000,
	.total_sendcount = 90000,
};
/**
 * @}
 */

/**
 * @defgroup      system_param_Public_Variables 
 * @brief         
 * @{  
 */
SystemParam_Config_t g_SystemParam_Config;
/**
 * @}
 */

/**
 * @defgroup      system_param_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
// 12B -> 8B
static void sys_makeUIDtoSNcode(void);
/**
 * @}
 */

/**
 * @defgroup      system_param_Functions 
 * @brief         
 * @{  
 */
 
 
 
void SystemParam_Init(void)
{		
	BSP_Flash_ReadBytes(SYS_PARAM_SAVE_FLASH_FIRSTHEAD, (uint8_t *)&g_SystemParam_Config , sizeof(g_SystemParam_Config) );
	if(CRC16_Modbus((uint8_t *)&g_SystemParam_Config,sizeof(g_SystemParam_Config)) == 0) // Same Save
	{
		DEBUG("SYS First Read OK\r\n");
	}
	else
	{
		BSP_Flash_ReadBytes(SYS_PARAM_SAVE_FLASH_SECONDHEAD, (uint8_t *)&g_SystemParam_Config , sizeof(g_SystemParam_Config) );
		if(CRC16_Modbus((uint8_t *)&g_SystemParam_Config,sizeof(g_SystemParam_Config)) == 0)
		{
			
			DEBUG("SYS Second Read OK\r\n");
		}
		else
		{
			SystemParam_Reset();
		}
	}

}
// 12B -> 8B
static void sys_makeUIDtoSNcode(void)
{
	sim_uid_t sim_uid ; 
	uint8_t uid[12] = { 0 };
	uint8_t crc_temp[2] = { 0 };
	uint8_t sn_temp[8] = { 0 }; 
	uint16_t temp = 0;
	SIM_GetUniqueId(&sim_uid);
	memcpy(uid,(uint8_t *)&sim_uid , 12);
	
	crc_temp[0] = uid[0];
	crc_temp[1] = uid[11];
	temp = CRC16_Modbus(crc_temp,2);
	memcpy(&sn_temp[0] , (uint8_t *)&temp , 2);
	
	crc_temp[0] = uid[1];
	crc_temp[1] = uid[10];
	temp = CRC16_Modbus(crc_temp,2);
	memcpy(&sn_temp[2] , (uint8_t *)&temp , 2);	
	
	crc_temp[0] = uid[2];
	crc_temp[1] = uid[9];
	temp = CRC16_Modbus(crc_temp,2);
	memcpy(&sn_temp[4] , (uint8_t *)&temp , 2);	
		
	temp = CRC16_Modbus(&uid[3],6);
	memcpy(&sn_temp[6] , (uint8_t *)&temp , 2);	
			
	memcpy(g_SystemParam_Config.SNcode ,sn_temp , 8);
	
}


int16_t SystemParam_Read(uint8_t handle)
{
    switch (handle)
    {
        case SYSTEMPARAM_CONFIG:
        {
            if (1)
            {
                
            }
            break;
        }
    }
    return 0;
}

void SystemParam_Save(void)
{
	g_SystemParam_Config.crc = CRC16_Modbus((uint8_t*)&g_SystemParam_Config, sizeof(g_SystemParam_Config) - sizeof(g_SystemParam_Config.crc));
	BSP_Flash_WriteBytes(SYS_PARAM_SAVE_FLASH_FIRSTHEAD,(uint8_t *)&g_SystemParam_Config,sizeof(g_SystemParam_Config));
	BSP_Flash_WriteBytes(SYS_PARAM_SAVE_FLASH_SECONDHEAD,(uint8_t *)&g_SystemParam_Config,sizeof(g_SystemParam_Config));
	DEBUG("Sys save\r\n");
}

void SystemParam_Reset(void)
{
	g_SystemParam_Config = SystemParam_Config_Default;
	sys_makeUIDtoSNcode();
	
	g_SystemParam_Config.E32_conf.module_source_addr =(uint16_t)( g_SystemParam_Config.SNcode[6] * 256 + g_SystemParam_Config.SNcode[7]);
	
	SystemParam_Save();
	DEBUG("SystemParam_Reset Load DefaultConf\r\n");

}

void SystemParam_Apply(uint8_t handle)
{
    switch (handle)
    {
        case SYSTEMPARAM_CONFIG:
        {
 
            break;
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

