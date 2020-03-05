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
	.SNcode ={ 0x12 , 0x34 ,0x56 , 0x78 , 0x90, 0xA0, 0xAB ,0x4F} ,
	.E32_conf.module_source_addr = 0x000A , 
	.E32_conf.module_destination_addr = 0x000B , 
	.E32_conf.module_datacheck = 0 ,//0:8N1; 1:8o1;  2:8E1;  3:8N1;
	.E32_conf.module_baudrate = 7 , //0:1200; 1:2400;  2:4800;  3:9600; 4:19200;  5:38400;  6:57600; 7:115200;
	.E32_conf.module_airspeed = 4 ,//0:0.3K 1:1.2K 2:2.4k 3:4.8K 4:9.6K 5:19.2K
	.E32_conf.module_channel = 0x0F , //00H-1FH,频率 410~441MHz ；410MHz+CHAN * 1MHz
	.E32_conf.module_transmission_mode = 1 ,//0:透明传输 1：定点传输
	.E32_conf.module_IO_workstyle = 1 , //0:推挽输出 1：开路输出
	.E32_conf.module_wakeup_time = 0,//0:250ms 1;500ms 2：750ms 3?1000  4：1250   5：1500 56:1750  72000
	.E32_conf.module_FEC = 1,   //0:关闭 1：开启
	.E32_conf.module_power = 0, //0:20dB 1:17dB 2:14dB 3:10dB

	.firsttimestamp = 0x5E0D5E85,
	.send_invteral = 10,
	.workmode = LowPower_WorkingMode,
	.battery = 100,
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
		DEBUG("SYS Param Read OK\r\n");
	}
	else
	{
		SystemParam_Reset();
	}

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
	DEBUG("Sys save\r\n");
}

void SystemParam_Reset(void)
{
	g_SystemParam_Config = SystemParam_Config_Default;
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

