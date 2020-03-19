/**
 **************************************************************************************************
 * @file        bsp_power.c
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
#include "bsp_power.h"
#include "clog.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_e32.h"
#include "bsp_clock.h"
#include "bsp_uart.h"
#include "bsp_systick.h"
#include "bsp_adc.h"
#include "app_getdata.h"
#include "bsp_led.h"
/**
 * @addtogroup    bsp_power_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_power_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_power_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_power_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Functions 
 * @brief         
 * @{  
 */

typedef enum
{
	Power_Mode_Normal = 0,
	Power_Mode_STOP	  ,
}BSP_Power_Mode_e;


void BSP_Power_SetMode(BSP_Power_Mode_e mode)
{
	DEBUG("Current Mode:%X\r\n" , SMC_GetPowerModeState( SMC ));
	
	//SMC_SetPowerModeProtection( SMC , kSMC_AllowPowerModeVlls | kSMC_AllowPowerModeLls); 
}


void BSP_Power_Init(void)
{
 //------GPIO --------
	gpio_pin_config_t gpio_pin_config ;
	CLOCK_EnableClock(kCLOCK_PortD);
	
	PORT_SetPinMux(PORTD, 5,kPORT_MuxAsGpio);
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
	GPIO_PinInit(GPIOD, 5, &gpio_pin_config);	
	
	GPIO_WritePinOutput(GPIOD, 5, 0);
}

void BSP_Power_V30_ON(void)
{
	GPIO_WritePinOutput(GPIOD, 5, 1);
}

void BSP_Power_V30_OFF(void)
{
	GPIO_WritePinOutput(GPIOD, 5, 0);
}


void BSP_Power_EnterVLPS(void)
{
	DEBUG("ENTER VLPS\r\n");
	
	BSP_LED_Close(BSP_LED1);
	
	APP_GetData_DeInit();
	BSP_E32_Close();
	BSP_SysTick_DisableIRQ();
	
	BSP_Power_V30_OFF();
	
	BOARD_RUNClockToVLPS();
	
	BSP_Power_V30_ON();
	BSP_SysTick_Init();
	BSP_E32_Open();
	APP_GetData_Init();
}





// -------Test Func--------
void BSP_Power_ModeTest(void)
{

	BOARD_RUNClockToVLPS();
}

// ------------------------

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

