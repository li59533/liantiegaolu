/**
 **************************************************************************************************
 * @file        bsp_uart.c
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
#include "bsp_uart.h"
#include "clog.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_led.h"
/**
 * @addtogroup    bsp_uart_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_uart_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_uart_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_uart_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_uart_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_uart_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_uart_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_uart_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static void bsp_uart0_init(void);
static void bsp_uart1_init(void);
static void bsp_uart2_init(void);

static void bsp_uart2_dma_init(void);
/**
 * @}
 */

/**
 * @defgroup      bsp_uart_Functions 
 * @brief         
 * @{  
 */
/*
PE16 -> TX2  ALT3
PE17 -> RX2  ALT3


PA2 -> TX0	ALT2
PA1 -> RX0	ALT2
*/


void BSP_UART_Init(uint8_t BSP_UARTX)
{
	switch(BSP_UARTX)
	{
		case BSP_UART0: bsp_uart0_init();break;
		case BSP_UART1: bsp_uart1_init();break;
		case BSP_UART2: bsp_uart2_init();break;
		default:break;
	}
}


static void bsp_uart0_init(void)
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

static void bsp_uart1_init(void)
{
	
}

static void bsp_uart2_init(void)
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
	
	// -------uart 2 init ----
	CLOCK_EnableClock(kCLOCK_Uart2);
	
	uart_config_t config ;
	UART_GetDefaultConfig(&config);

/*
 *   uartConfig->baudRate_Bps = 115200U;
 *   uartConfig->bitCountPerChar = kUART_8BitsPerChar;
 *   uartConfig->parityMode = kUART_ParityDisabled;
 *   uartConfig->stopBitCount = kUART_OneStopBit;
 *   uartConfig->txFifoWatermark = 0;
 *   uartConfig->rxFifoWatermark = 1;
 *   uartConfig->enableTx = false;
 *   uartConfig->enableRx = false;	
*/	

	UART_Init(UART2, &config,  CLOCK_GetFreq(kCLOCK_BusClk));
	
	UART_EnableTx(UART2, true);
	UART_EnableRx(UART2, true);
	



// --------------------------------------

	// -----------------------
	
	// --------open irq-------
	//UART_EnableInterrupts(UART2, kUART_TransmissionCompleteInterruptEnable);
	UART_EnableInterrupts(UART2, kUART_RxDataRegFullInterruptEnable);
	
	NVIC_EnableIRQ(UART2_IRQn);
	
	// -----------------------	
	
	
// -----------DMA ------------------------

	//bsp_uart2_dma_init();
	

// ---------------------------------------
}

static void bsp_uart2_dma_init(void)
{
//	UART_EnableTxDMA(UART2, true);
//	/* Configure DMAMUX */
//    DMAMUX_Init(DMAMUX0);
//	
//    DMAMUX_SetSource(DMAMUX0, 0, kDmaRequestMux0ADC0); /* Map ADC source to channel 0 */
//    DMAMUX_EnableChannel(DMAMUX0, 0);

//    DMA_Init(DMA0);
//	
//	
//    DMA_CreateHandle(&dma_handle, DMA0, 0);
//	
//	
//	
//    /* Setup transfer */
//	
//    DMA_PrepareTransfer(&transferConfig, (void *)ADC16_RESULT_REG_ADDR, sizeof(uint32_t),
//                        (void *)bsp_adc_DataArray, sizeof(uint32_t), sizeof(bsp_adc_DataArray),
//                        kDMA_PeripheralToMemory);
//						
//    DMA_SetTransferConfig(DMA0, 0, &transferConfig);
//    /* Enable interrupt when transfer is done. */
//    DMA_EnableInterrupts(DMA0, 0);
//    /* Enable async DMA request. */
//    DMA_EnableAsyncRequest(DMA0, 0, true);
//    /* Forces a single read/write transfer per request. */
//    DMA_EnableCycleSteal(DMA0, 0, true);
//    /* Enable transfer. */
//    DMA_StartTransfer(&dma_handle);
//    /* Enable IRQ. */
//    NVIC_EnableIRQ(DMA0_IRQn);	
}




// --------Function -------------

		// ----Send ----

void BSP_UART_WriteBytes_Blocking(uint8_t BSP_UARTX , uint8_t *buf, uint16_t len)
{
	switch(BSP_UARTX)
	{
		case BSP_UART0 :break;
		case BSP_UART1 :UART_WriteBlocking(UART1, buf, len);break;
		case BSP_UART2 :UART_WriteBlocking(UART2, buf, len);break;
		default:break;
	}
}



		// -------------
		// ---Rev-------

		// -------------


// ------------------------------

// -----------IRQ--------------

void UART2_IRQHandler(void)
{
	DEBUG("UART2_IRQHandler\r\n");
	
	if((UART_GetStatusFlags(UART2) & kUART_RxDataRegFullFlag )== kUART_RxDataRegFullFlag)
	{
		UART_ClearStatusFlags(UART2, kUART_RxDataRegFullFlag);
		
		uint8_t c = 0;
		c = UART_ReadByte(UART2);
		DEBUG("Uart R:%X\r\n" , c);
	}
	
}

// ----------------------------


// ------------Test-------------
void BSP_Uart_Test_Send(void)
{
	DEBUG("BSP_Uart_Test_Send\r\n");
	uint32_t flag = 0;
	flag = UART_GetStatusFlags(UART2);
	
	//while((flag & kUART_TxDataRegEmptyFlag ) == 0);
	//UART_WriteByte(UART2, 0xf7);
	
	uint8_t test_bud[] = {0x00,0x12,0x32,0xff,0x00,0x11,0xcd,0x45};
	BSP_UART_WriteBytes_Blocking(BSP_UART2 , test_bud, sizeof(test_bud));	
}


// -----------------------------

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

