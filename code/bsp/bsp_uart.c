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
#include "bsp_e32.h"
#include "app_conf.h"
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

static void bsp_uart0_dma_init(void);
static void bsp_uart2_dma_init(void);

static void uart_dma_transfer_callback(UART_Type *base,
                                             uart_dma_handle_t *handle,
                                             status_t status,
                                             void *userData);

static void lpsci_dma_transfer_callback(UART0_Type *base,
                                              lpsci_dma_handle_t *handle,
                                              status_t status,
                                              void *userData);


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

void BSP_UART_SetBaudRate(uint8_t BSP_UARTX , uint32_t buadrate)
{
	switch(BSP_UARTX)
	{
		case BSP_UART0: 
		{
			LPSCI_SetBaudRate(UART0, buadrate, CLOCK_GetFreq(kCLOCK_CoreSysClk));
			LPSCI_EnableRx(UART0, true);
	
			// -----------------------

			// --------open irq-------
			LPSCI_EnableInterrupts( UART0, kLPSCI_RxDataRegFullInterruptEnable);

			//LPSCI_EnableInterrupts( UART0, kLPSCI_TransmissionCompleteInterruptEnable);

			EnableIRQ(UART0_IRQn);
		}
		break;
		case BSP_UART1: bsp_uart1_init();break;
		case BSP_UART2: bsp_uart2_init();break;
		default:break;
	}
}
	 

void BSP_Uart0_Close(void)
{
	LPSCI_Deinit(UART0);
	
	//CLOCK_EnableClock(kCLOCK_PortA);
	PORT_SetPinMux(PORTA, 2,kPORT_MuxAsGpio);
	gpio_pin_config_t gpio_pin_config;
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalInput;
	GPIO_PinInit(GPIOA, 2, &gpio_pin_config);
	
	PORT_SetPinMux(PORTA, 1,kPORT_MuxAsGpio);
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalInput;
	GPIO_PinInit(GPIOA, 1, &gpio_pin_config);
	// -----------------------		
}

void BSP_Uart0_Open(void)
{
	BSP_UART_Init( BSP_UART0 );
	BSP_UART_SetBaudRate(BSP_UART0 , 115200);
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
	CLOCK_SetLpsci0Clock(1);
	
	lpsci_config_t config;
	LPSCI_GetDefaultConfig( &config );
	config.baudRate_Bps = 9600U;
/*
 *   lpsciConfig->baudRate_Bps = 115200U;
 *   lpsciConfig->parityMode = kLPSCI_ParityDisabled;
 *   lpsciConfig->stopBitCount = kLPSCI_OneStopBit;
 *   lpsciConfig->enableTx = false;
 *   lpsciConfig->enableRx = false;
*/	
	LPSCI_Init( UART0, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));
	LPSCI_EnableTx(UART0, true);
	LPSCI_EnableRx(UART0, true);
	
	// -----------------------
	
	// --------open irq-------
	LPSCI_EnableInterrupts( UART0, kLPSCI_RxDataRegFullInterruptEnable);
	
	//LPSCI_EnableInterrupts( UART0, kLPSCI_TransmissionCompleteInterruptEnable);
	
	EnableIRQ(UART0_IRQn);
	
	// -----------------------
	
// -----------DMA ------------------------
	bsp_uart0_dma_init();
// ---------------------------------------	
	
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
	config.baudRate_Bps = 9600U;

/*
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
	UART_EnableInterrupts(UART2, kUART_RxActiveEdgeInterruptEnable);
	
	
	NVIC_EnableIRQ(UART2_IRQn);

	// -----------------------	
// -----------DMA ------------------------
	bsp_uart2_dma_init();
// ---------------------------------------
}

dma_handle_t uart0_dmahandle;
dma_transfer_config_t uart0_dmatran_config;

#define BSP_UART0_TX_BUFLEN   1
uint8_t bsp_uart0_Tx_buf[BSP_UART0_TX_BUFLEN] = { 0 }; 
lpsci_dma_handle_t  uart0_dma_handle;

static void bsp_uart0_dma_init(void)
{
	CLOCK_EnableClock(kCLOCK_Dmamux0);
	CLOCK_EnableClock(kCLOCK_Dma0);

	//LPSCI_EnableTxDMA(UART0 , true);
	/* Configure DMAMUX */
    DMAMUX_Init(DMAMUX0);
	
	DMAMUX_SetSource(DMAMUX0, 1, kDmaRequestMux0UART0Tx); /* Map Uart0 source to channel 0 */
    DMAMUX_EnableChannel(DMAMUX0, 1);
	DMA_Init(DMA0);
	
	DMA_CreateHandle(&uart0_dmahandle, DMA0, 1);
	
	LPSCI_TransferCreateHandleDMA(UART0,
                           &uart0_dma_handle,
                           lpsci_dma_transfer_callback,
                           0,
                           &uart0_dmahandle,
                           0);	
	
    /* Setup transfer */
	
    DMA_PrepareTransfer(&uart0_dmatran_config, (void *)bsp_uart0_Tx_buf, sizeof(uint8_t),
                        (void *)LPSCI_GetDataRegisterAddress(UART0), sizeof(uint8_t), sizeof(bsp_uart0_Tx_buf),
                        kDMA_MemoryToPeripheral);

    DMA_SetTransferConfig(DMA0, 1, &uart0_dmatran_config);
    /* Enable interrupt when transfer is done. */
    DMA_EnableInterrupts(DMA0, 1);
    /* Enable async DMA request. */
    DMA_EnableAsyncRequest(DMA0, 1, true);
    /* Forces a single read/write transfer per request. */
    DMA_EnableCycleSteal(DMA0, 1, true);
    /* Enable transfer. */
    //DMA_StartTransfer(&uart0_dmahandle);
    /* Enable IRQ. */
    NVIC_EnableIRQ(DMA0_IRQn);	
						
}


dma_handle_t uart2_dmahandle;
dma_transfer_config_t uart2_dmatran_config;

#define BSP_UART2_TX_BUFLEN   1
uint8_t bsp_uart2_Tx_buf[BSP_UART2_TX_BUFLEN] = { 0 }; 
uart_dma_handle_t  uart_dma_handle;

static void bsp_uart2_dma_init(void)
{
	CLOCK_EnableClock(kCLOCK_Dmamux0);
	CLOCK_EnableClock(kCLOCK_Dma0);
	
	//UART_EnableTxDMA(UART2, true);
	/* Configure DMAMUX */
    DMAMUX_Init(DMAMUX0);
	
    DMAMUX_SetSource(DMAMUX0, 0, kDmaRequestMux0UART2Tx); /* Map Uart2 source to channel 0 */
    DMAMUX_EnableChannel(DMAMUX0, 0);

    DMA_Init(DMA0);

    DMA_CreateHandle(&uart2_dmahandle, DMA0, 0);

	UART_TransferCreateHandleDMA(UART2,
								  &uart_dma_handle,
								  uart_dma_transfer_callback,
								  0,
								  &uart2_dmahandle,
								  0);
	
    /* Setup transfer */
	
    DMA_PrepareTransfer(&uart2_dmatran_config, (void *)bsp_uart2_Tx_buf, sizeof(uint8_t),
                        (void *)UART_GetDataRegisterAddress(UART2), sizeof(uint8_t), sizeof(bsp_uart2_Tx_buf),
                        kDMA_MemoryToPeripheral);
						
    DMA_SetTransferConfig(DMA0, 0, &uart2_dmatran_config);
    /* Enable interrupt when transfer is done. */
    DMA_EnableInterrupts(DMA0, 0);
    /* Enable async DMA request. */
    DMA_EnableAsyncRequest(DMA0, 0, true);
    /* Forces a single read/write transfer per request. */
    DMA_EnableCycleSteal(DMA0, 0, true);
    /* Enable transfer. */
    //DMA_StartTransfer(&uart2_dmahandle);
    /* Enable IRQ. */
    NVIC_EnableIRQ(DMA0_IRQn);	
						
}
static void uart_dma_transfer_callback(UART_Type *base,
                                             uart_dma_handle_t *handle,
                                             status_t status,
                                             void *userData)
{
	DEBUG("uart_dma_transfer_callback?\r\n");
}


static void lpsci_dma_transfer_callback(UART0_Type *base,
                                              lpsci_dma_handle_t *handle,
                                              status_t status,
                                              void *userData)
{
	DEBUG("lpsci_dma_transfer_callback?\r\n");
}


// --------Function -------------

		// ----Send ----
			/*
				Uart0 blocking mode has some bug...
			*/
void BSP_UART_WriteBytes_Blocking(uint8_t BSP_UARTX , uint8_t *buf, uint16_t len)
{
	switch(BSP_UARTX)
	{
		case BSP_UART0 :LPSCI_WriteBlocking(UART0, buf , len);break;
		case BSP_UART1 :UART_WriteBlocking(UART1, buf, len);break;
		case BSP_UART2 :UART_WriteBlocking(UART2, buf, len);break;
		default:break;
	}
}

void BSP_UART_WriteBytes_DMA(uint8_t BSP_UARTX , uint8_t *buf, uint16_t len)
{
	uart_transfer_t  xfer;
	xfer.data = buf;
	xfer.dataSize = len;	
	switch(BSP_UARTX)
	{
		case BSP_UART0:LPSCI_TransferSendDMA(UART0, &uart0_dma_handle, (lpsci_transfer_t *)&xfer); break;
		case BSP_UART1:break;
		case BSP_UART2:UART_TransferSendDMA(UART2, &uart_dma_handle, &xfer); break;
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
		
		APP_Conf_RevByteOneByte(c);
		
	}
	
	if((UART_GetStatusFlags(UART2) & kUART_RxActiveEdgeInterruptEnable )== kUART_RxActiveEdgeInterruptEnable)
	{
		UART2->S2 |= UART_S2_RXEDGIF(1);  
	}	
	
}

void UART0_IRQHandler(void)
{
	//DEBUG("UART0_IRQHandler\r\n");
	
	if((LPSCI_GetStatusFlags(UART0) & kLPSCI_RxDataRegFullFlag )== kLPSCI_RxDataRegFullFlag)
	{
		LPSCI_ClearStatusFlags(UART0, kLPSCI_RxDataRegFullFlag);
		
//		uint8_t c = 0;
//		c = LPSCI_ReadByte(UART0);

		BSP_E32_RevByteOneByte(LPSCI_ReadByte(UART0));

		//DEBUG("Uart R:%X\r\n" , c);
	}

	if((LPSCI_GetStatusFlags(UART0) & kLPSCI_TxDataRegEmptyFlag )== kLPSCI_TxDataRegEmptyFlag)
	{
		LPSCI_ClearStatusFlags(UART0, kLPSCI_TxDataRegEmptyFlag);
	}	

	if((LPSCI_GetStatusFlags(UART0) & kLPSCI_RxOverrunFlag )== kLPSCI_RxOverrunFlag)
	{
		LPSCI_ClearStatusFlags(UART0, kLPSCI_RxOverrunFlag);
	}	
	
	
	
}

void DMA0_IRQHandler(void)
{
	DEBUG("DMA0_IRQHandler\r\n");
	if((DMA_GetChannelStatusFlags(DMA0 , 0) & kDMA_TransactionsBCRFlag )== kDMA_TransactionsBCRFlag)
	{
		DMA_ClearChannelStatusFlags( DMA0 , 0 , kDMA_TransactionsBCRFlag);
	}
	if((DMA_GetChannelStatusFlags(DMA0 , 0) & kDMA_TransactionsDoneFlag )== kDMA_TransactionsDoneFlag)
	{
		UART_TransferAbortSendDMA(UART2, &uart_dma_handle);
		DMA_ClearChannelStatusFlags( DMA0 , 0 , kDMA_TransactionsDoneFlag);
	}
	if((DMA_GetChannelStatusFlags(DMA0 , 0) & kDMA_TransactionsBusyFlag )== kDMA_TransactionsBusyFlag)
	{
		DMA_ClearChannelStatusFlags( DMA0 , 0 , kDMA_TransactionsBusyFlag);
	}
	if((DMA_GetChannelStatusFlags(DMA0 , 0) & kDMA_TransactionsRequestFlag )== kDMA_TransactionsRequestFlag)
	{
		DMA_ClearChannelStatusFlags( DMA0 , 0 , kDMA_TransactionsRequestFlag);
	}
	if((DMA_GetChannelStatusFlags(DMA0 , 0) & kDMA_BusErrorOnDestinationFlag )== kDMA_BusErrorOnDestinationFlag)
	{
		DMA_ClearChannelStatusFlags( DMA0 , 0 , kDMA_BusErrorOnDestinationFlag);
	}
	
	
	if((DMA_GetChannelStatusFlags(DMA0 , 1) & kDMA_TransactionsBCRFlag )== kDMA_TransactionsBCRFlag)
	{
		DMA_ClearChannelStatusFlags( DMA0 , 1 , kDMA_TransactionsBCRFlag);
	}
	if((DMA_GetChannelStatusFlags(DMA0 , 1) & kDMA_TransactionsDoneFlag )== kDMA_TransactionsDoneFlag)
	{
		LPSCI_TransferAbortSendDMA(UART0 , &uart0_dma_handle);
		DMA_ClearChannelStatusFlags( DMA0 , 1 , kDMA_TransactionsDoneFlag);
	}
	if((DMA_GetChannelStatusFlags(DMA0 , 1) & kDMA_TransactionsBusyFlag )== kDMA_TransactionsBusyFlag)
	{
		DMA_ClearChannelStatusFlags( DMA0 , 1 , kDMA_TransactionsBusyFlag);
	}
	if((DMA_GetChannelStatusFlags(DMA0 , 1) & kDMA_TransactionsRequestFlag )== kDMA_TransactionsRequestFlag)
	{
		DMA_ClearChannelStatusFlags( DMA0 , 1 , kDMA_TransactionsRequestFlag);
	}
	if((DMA_GetChannelStatusFlags(DMA0 , 1) & kDMA_BusErrorOnDestinationFlag )== kDMA_BusErrorOnDestinationFlag)
	{
		DMA_ClearChannelStatusFlags( DMA0 , 1 , kDMA_BusErrorOnDestinationFlag);
	}	
	
}

// ----------------------------


// ------------Test-------------
void BSP_Uart_Test_Send(void)
{
	DEBUG("BSP_Uart_Test_Send\r\n");
//	uint32_t flag = 0;
//	flag = UART_GetStatusFlags(UART2);
	
	//while((flag & kUART_TxDataRegEmptyFlag ) == 0);
	//UART_WriteByte(UART2, 0xf7);
//	uint8_t test_bud[] = {0x00,0x12,0x32,0xff,0x00,0x11,0xcd,0x45};
//	BSP_UART_WriteBytes_Blocking(BSP_UART2 , test_bud, sizeof(test_bud));	
	
	uint8_t test_bud[] = {0x00,0x12,0x32,0xff,0x00,0x11,0xcd,0x45};
	//BSP_UART_WriteBytes_DMA(0,test_bud , sizeof(test_bud));
	
	BSP_UART_WriteBytes_DMA(BSP_UART0 , test_bud, sizeof(test_bud));
	
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

