/**
 **************************************************************************************************
 * @file        bsp_adc.c
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

#include "clog.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_led.h"
/**
 * @addtogroup    bsp_adc_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_adc_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_adc_Macros_Defines 
 * @brief         
 * @{  
 */
#define ADC16_RESULT_REG_ADDR (uint32_t)(&ADC0->R[0]) /* Get ADC16 result register address */
/**
 * @}
 */

/**
 * @defgroup      bsp_adc_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_adc_Private_Types
 * @brief         
 * @{  
 */
typedef struct
{
	uint16_t adcbuf[8];
	uint8_t in ;
	uint16_t average;
}bsp_adc_data_t;


/**
 * @}
 */

/**
 * @defgroup      bsp_adc_Private_Variables 
 * @brief         
 * @{  
 */
bsp_adc_data_t bsp_adc_data = 
{
	.in = 0 ,
	.average = 0,
};


/**
 * @}
 */

/**
 * @defgroup      bsp_adc_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_adc_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static void bsp_adc_addvalue(uint16_t value); 
 
//static void bsp_adcDMA_init(void);
/**
 * @}
 */

/**
 * @defgroup      bsp_adc_Functions 
 * @brief         
 * @{  
 */
void BSP_ADC_Init(void)
{
	adc16_config_t config;
	//gpio_pin_config_t gpio_pin_config;
	
	// ---------GPIO Init ----------------------
	CLOCK_EnableClock(kCLOCK_PortC);
	PORT_SetPinMux(PORTC, 2, kPORT_PinDisabledOrAnalog);
	//GPIO_PinInit(GPIOC, 2, &gpio_pin_config);
	
	// -----------------------------------------
	
	CLOCK_EnableClock(kCLOCK_Adc0);
	ADC16_GetDefaultConfig(&config);
/*
 *   config->referenceVoltageSource     = kADC16_ReferenceVoltageSourceVref;
 *   config->clockSource                = kADC16_ClockSourceAsynchronousClock;
 *   config->enableAsynchronousClock    = true;
 *   config->clockDivider               = kADC16_ClockDivider8;
 *   config->resolution                 = kADC16_ResolutionSE12Bit;
 *   config->longSampleMode             = kADC16_LongSampleDisabled;
 *   config->enableHighSpeed            = false;
 *   config->enableLowPower             = false;
 *   config->enableContinuousConversion = false;
*/

//	config.clockDivider = ;
//	config.clockSource = ;
//	config.enableAsynchronousClock = ;
//	config.enableContinuousConversion = ;
//	config.enableHighSpeed = ;
//	config.enableLowPower = ;
	config.longSampleMode = kADC16_LongSampleCycle24;
//	config.referenceVoltageSource = ;
	config.resolution = kADC16_Resolution16Bit;
	ADC16_Init( ADC0, &config);
	
	ADC16_DoAutoCalibration( ADC0 ); 
	
	// -----channel -------------------
	adc16_channel_config_t channel_config = { 0 };
	channel_config.channelNumber = 11;
	channel_config.enableDifferentialConversion = false;
	channel_config.enableInterruptOnConversionCompleted = true;
	ADC16_SetChannelConfig(ADC0, 0, &channel_config);
	
	
	EnableIRQ(ADC0_IRQn);
	
	
	// --------------------------------	
	// ---------select trigger --------
	ADC16_EnableHardwareTrigger( ADC0 , true );
	SIM->SOPT7 = SIM_SOPT7_ADC0TRGSEL(8)| SIM_SOPT7_ADC0ALTTRGEN(1);    // need to look reference maunl 
	// --------------------------------
	
	ADC16_SetOffsetValue(ADC0, 0) ;
	
	ADC16_SetHardwareAverage(ADC0, kADC16_HardwareAverageCount4); // Hardware average
	
	
	// ---------Open DMA -------
	//bsp_adcDMA_init();
	// -------------------------
}

void BSP_ADC_DeInit(void)
{
	adc16_config_t config;
	gpio_pin_config_t gpio_pin_config;
	
	// ---------GPIO Init ----------------------
	
	PORT_SetPinMux(PORTC, 2, kPORT_MuxAsGpio);
	gpio_pin_config.outputLogic = 0;
	gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
	GPIO_PinInit(GPIOC, 2, &gpio_pin_config);

	// -----------------------------------------	
	
	
/*
 *   config->referenceVoltageSource     = kADC16_ReferenceVoltageSourceVref;
 *   config->clockSource                = kADC16_ClockSourceAsynchronousClock;
 *   config->enableAsynchronousClock    = true;
 *   config->clockDivider               = kADC16_ClockDivider8;
 *   config->resolution                 = kADC16_ResolutionSE12Bit;
 *   config->longSampleMode             = kADC16_LongSampleDisabled;
 *   config->enableHighSpeed            = false;
 *   config->enableLowPower             = false;
 *   config->enableContinuousConversion = false;
*/

//	config.clockDivider = ;
//	config.clockSource = ;
	config.enableAsynchronousClock = false;
//	config.enableContinuousConversion = ;
//	config.enableHighSpeed = ;
//	config.enableLowPower = ;
//	config.longSampleMode = kADC16_LongSampleCycle24;
//	config.referenceVoltageSource = ;
//	config.resolution = kADC16_Resolution16Bit;
	ADC16_Init( ADC0, &config);	
	
	ADC16_Deinit(ADC0);
	CLOCK_DisableClock(kCLOCK_Adc0);
	DisableIRQ(ADC0_IRQn);
	
}


void BSP_ADC_DisableIRQ(void)
{
	DisableIRQ(ADC0_IRQn);
}

void BSP_ADC_EnableIRQ(void)
{
	EnableIRQ(ADC0_IRQn);
}

uint32_t BSP_ADC_GetValue(uint8_t channel)
{
	return ADC16_GetChannelConversionValue( ADC0 , 0);
}


static void bsp_adc_addvalue(uint16_t value)
{
	uint32_t sum  = 0 ;
	bsp_adc_data.adcbuf[bsp_adc_data.in ++] = value;
	bsp_adc_data.in %= 8;
	for(uint8_t i = 0 ; i < 8 ; i ++)
	{
		sum += bsp_adc_data.adcbuf[i];
	}
	bsp_adc_data.average  = sum >> 3;
}

uint16_t BSP_ADC_GetAverageValue(uint8_t channel)
{
	return bsp_adc_data.average;
}


// --------------Test -----
void BSP_ADC_ShowValue(void)
{
	DEBUG("ADC Value:%d\r\n" ,BSP_ADC_GetValue(0) );
}

// ------------------------


// -------IRQ ----------------
void ADC0_IRQHandler(void)
{
	//DEBUG("ADC0_IRQHandler\r\n" );
	bsp_adc_addvalue((uint16_t)ADC16_GetChannelConversionValue( ADC0 , 0));
}
// ---------------------------
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

