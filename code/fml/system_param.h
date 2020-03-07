/**
 **************************************************************************************************
 * @file        system_param.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _SYSTEM_PARAM_H_
#define _SYSTEM_PARAM_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    system_param_Modules 
 * @{  
 */

/**
 * @defgroup      system_param_Exported_Macros 
 * @{  
 */
												
#define SYS_PARAM_SAVE_FLASH_FIRSTHEAD   		0x0000FC00
#define SYS_PARAM_SAVE_FLASH_SECONDHEAD			0x0000F800

#define SYSTEMPARAM_CONFIG     0



/**
 * @}
 */

/**
 * @defgroup      system_param_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Exported_Types 
 * @{  
 */
 
typedef enum
{
	Normal_WorkingMode = 0,
	LowPower_WorkingMode ,
}WorkMode_e;	
 
 
 
#pragma pack(1)

typedef struct
{
	uint16_t  module_source_addr;
	uint16_t  module_destination_addr;
	uint8_t   module_datacheck;//0:8N1; 1:8o1;  2:8E1;  3:8N1;
	uint8_t   module_baudrate;//0:1200; 1:2400;  2:4800;  3:9600; 4:19200;  5:38400;  6:57600; 7:115200;
	uint8_t   module_airspeed;//0:0.3K 1:1.2K 2:2.4k 3:4.8K 4:9.6K 5:19.2K
	uint8_t   module_channel; //00H-1FH,Ƶ�� 410~441MHz ��410MHz+CHAN * 1MHz
	uint8_t   module_transmission_mode;//0:���㴫�� 1��͸������
	uint8_t   module_IO_workstyle; //0:������� 1����·���
	uint8_t   module_wakeup_time;//0:250ms 1;500ms 2��750ms 3?1000  4��1250   5��1500 56:1750  72000
	uint8_t   module_FEC;   //0:�ر� 1������
	uint8_t   module_power; //0:20dB 1:17dB 2:14dB 3:10dB
}E32_conf_t;



typedef struct
{
	uint8_t SNcode[8];
	uint8_t workmode ;
	uint8_t battery ;
	uint32_t  firsttimestamp ;
	uint16_t send_invteral ;
	E32_conf_t E32_conf;
    uint16_t crc;           /*!< ���еĲ����ṹ����밴�ֽڶ��䡣�����һ�������
                                 ��һ��uint16_t���͵����ݣ������������ڲ��洢ʱ��
                                 crcУ��ʹ洢���û��ڱ������ʱ���ù��ĸ�ֵ��ֵ��*/
}SystemParam_Config_t;
#pragma pack()
/**
 * @}
 */

/**
 * @defgroup      system_param_Exported_Variables 
 * @{  
 */
extern SystemParam_Config_t g_SystemParam_Config;
/**
 * @}
 */

/**
 * @defgroup      system_param_Exported_Functions 
 * @{  
 */
void SystemParam_Init(void);

int16_t SystemParam_Read(uint8_t handle);

void SystemParam_Save(void);

void SystemParam_Reset(void);

void SystemParam_Apply(uint8_t handle);
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
