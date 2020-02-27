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

/**
 * @}
 */

/**
 * @defgroup      bsp_e32_Private_Variables 
 * @brief         
 * @{  
 */

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
	gpio_pin_config_t gpio_pin_config = { 0 };
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
	
	GPIO_WritePinOutput(GPIOD, 7, 0); // M0
	GPIO_WritePinOutput(GPIOD, 6, 0); // M1
	GPIO_WritePinOutput(GPIOA, 4, 0); // AUX
	GPIO_WritePinOutput(GPIOE, 19, 0); // E32_Power
	
	// -------Uart ----------
	BSP_UART_Init(BSP_UART0);
	// ----------------------
}





uint8_t BSP_E32_GetAUX_Status(void)
{
	return E32_AUX_STATUS;
}

void BSP_E32_SetMode(E32_Mode_e mode)  // after change mode ,the module need 1ms delay . sug: check AUX high 2ms to start change mode
{
	switch((uint8_t )mode)
	{
		case E32_MODE_NORMAL : 
		{
			E32_M0_DOWN;
			E32_M1_DOWN;
		}
		break;
		case E32_MODE_WAKEUP:
		{
			E32_M0_UP;
			E32_M1_DOWN;			
		}
		break;
		case E32_MODE_LOWPOWER:
		{
			E32_M0_DOWN;
			E32_M1_UP;			
		}
		break;
		case E32_MODE_SLEEP:
		{
			E32_M0_UP;
			E32_M1_UP;			
		}
		break;
		default:
			break;
	}
}

static uint8_t BSP_E32_SetConfCMD_Save[6]	 = {0xC0 ,} ;
static uint8_t BSP_E32_GetConfCMD[3]		 = {0xC1 ,0xC1 , 0xC1} ;
static uint8_t BSP_E32_SetConfCMD_NoSave[6]	 = {0xC2 ,};
static uint8_t BSP_E32_GetVersionCMD[3]		 = {0xC3 , 0xC3 , 0xC3};
static uint8_t BSP_E32_RestCMD[3]			 = {0xC4 , 0xC4 ,0xC4};

void BSP_E32_WriteBytes(uint8_t *buf , uint16_t len)
{
	
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

