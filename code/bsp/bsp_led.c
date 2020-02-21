/**
 **************************************************************************************************
 * @file        bsp_led.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

#include "bsp_led.h"
#include "bsp_mkl16_conf.h"

/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    bsp_led_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_led_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_led_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_led_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_led_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_led_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_led_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_led_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_led_Functions 
 * @brief         
 * @{  
 */
void BSP_LED_Init(void)
{
	CLOCK_EnableClock(kCLOCK_PortE);
	
	
	PORT_SetPinMux(PORTE, 18,kPORT_MuxAsGpio);
	gpio_pin_config_t gpio_pin_config = { 0 };
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
	GPIO_PinInit(GPIOE, 18, &gpio_pin_config);
	
	GPIO_WritePinOutput(GPIOE, 18, 0);
}


void BSP_LED_Open(uint8_t BSP_LEDx)
{
	switch(BSP_LEDx)
	{
		case BSP_LED1 : GPIO_WritePinOutput(GPIOE, 18, 1);break;
		default:break;
	}
}

void BSP_LED_Close(uint8_t BSP_LEDx)
{
	switch(BSP_LEDx)
	{
		case BSP_LED1 : GPIO_WritePinOutput(GPIOE, 18, 0);break;
		default:break;
	}
}


void BSP_LED_Toggle(uint8_t BSP_LEDx)
{
	switch(BSP_LEDx)
	{
		case BSP_LED1 : GPIO_TogglePins(GPIOE, 18);break;
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

