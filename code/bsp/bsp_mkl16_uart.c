/**
 **************************************************************************************************
 * @file        bsp_mkl16_uart.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "bsp_mkl16_conf.h"
#include "bsp_mkl16_uart.h"
#include "clog.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_led.h"
/**
 * @addtogroup    bsp_mkl16_uart_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_mkl16_uart_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_mkl16_uart_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_uart_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_mkl16_uart_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_uart_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_uart_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_uart_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static void bsp_mkl16_uart0_init(void);
static void bsp_mkl16_uart1_init(void);
static void bsp_mkl16_uart2_init(void);
/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_uart_Functions 
 * @brief         
 * @{  
 */
/*
PE16 -> TX2  ALT3
PE17 -> RX2  ALT3


PA2 -> TX0	ALT2
PA1 -> RX0	ALT2
*/


void BSP_MKL16_UART_Init(uint8_t BSP_UARTX)
{
	switch(BSP_UARTX)
	{
		case BSP_UART0: bsp_mkl16_uart0_init();break;
		case BSP_UART1: bsp_mkl16_uart1_init();break;
		case BSP_UART2: bsp_mkl16_uart2_init();break;
		default:break;
	}
}


static void bsp_mkl16_uart0_init(void)
{
	// -------gpio init ------
	CLOCK_EnableClock(kCLOCK_PortA);
	PORT_SetPinMux(PORTA, 2,kPORT_MuxAlt2);
	gpio_pin_config_t gpio_pin_config;
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
	GPIO_PinInit(GPIOA, 2, &gpio_pin_config);
	
	PORT_SetPinMux(PORTA, 1,kPORT_MuxAlt2);
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalInput;
	GPIO_PinInit(GPIOA, 1, &gpio_pin_config);
	// -----------------------		
	
	// -------uart 0 init ----
	CLOCK_EnableClock(kCLOCK_Uart0);
	CLOCK_SetLpsci0Clock(SIM_SOPT2_UART0SRC(1) );
	
	lpsci_config_t config;
	LPSCI_GetDefaultConfig( &config );
/*
 *   lpsciConfig->baudRate_Bps = 115200U;
 *   lpsciConfig->parityMode = kLPSCI_ParityDisabled;
 *   lpsciConfig->stopBitCount = kLPSCI_OneStopBit;
 *   lpsciConfig->enableTx = false;
 *   lpsciConfig->enableRx = false;
*/	
	LPSCI_Init( UART0, &config, CLOCK_GetCoreSysClkFreq());
	LPSCI_EnableTx(UART0, true);
	LPSCI_EnableRx(UART0, true);
	
	// -----------------------
	
	// --------open irq-------
	LPSCI_EnableInterrupts( UART0, kLPSCI_RxDataRegFullInterruptEnable);
	
	LPSCI_EnableInterrupts( UART0, kLPSCI_TransmissionCompleteInterruptEnable);
	
	EnableIRQ(UART0_IRQn);
	
	// -----------------------
	
}

static void bsp_mkl16_uart1_init(void)
{
	
}

static void bsp_mkl16_uart2_init(void)
{
	// -------gpio init ------
	CLOCK_EnableClock(kCLOCK_PortE);
	PORT_SetPinMux(PORTE, 16,kPORT_MuxAlt3);
	gpio_pin_config_t gpio_pin_config;
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
	GPIO_PinInit(GPIOE, 16, &gpio_pin_config);
	
	PORT_SetPinMux(PORTE, 17,kPORT_MuxAlt3);
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalInput;
	GPIO_PinInit(GPIOE, 17, &gpio_pin_config);
	// -----------------------	
}


// -----------IRQ--------------
UART0_IRQn
// ----------------------------
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

