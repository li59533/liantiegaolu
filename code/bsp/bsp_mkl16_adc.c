/**
 **************************************************************************************************
 * @file        bsp_mkl16_adc.c
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

#include "clog.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_led.h"
/**
 * @addtogroup    bsp_mkl16_adc_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_mkl16_adc_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_mkl16_adc_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_adc_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_mkl16_adc_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_adc_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_adc_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_adc_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_adc_Functions 
 * @brief         
 * @{  
 */
void BSP_MKL16_ADC_Init(void)
{
	adc16_config_t config = { 0 };
	gpio_pin_config_t gpio_pin_config = { 0 };
	
	// ---------GPIO Init ----------------------
	CLOCK_EnableClock(kCLOCK_PortC);
	PORT_SetPinMux(PORTC, 2, kPORT_PinDisabledOrAnalog);
	GPIO_PinInit(GPIOC, 2, &gpio_pin_config);
	
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
	//ADC16_SetChannelMuxMode(ADC0, kADC16_ChannelMuxA);
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
	
}

uint32_t BSP_MKL16_ADC_GetValue(uint8_t channel)
{
	return ADC16_GetChannelConversionValue( ADC0 , 0);
}





// --------------Test -----
void BSP_MKL16_ADC_ShowValue(void)
{
	DEBUG("ADC Value:%d\r\n" ,BSP_MKL16_ADC_GetValue(0) );
}

// ------------------------


// -------IRQ ----------------
void ADC0_IRQHandler(void)
{
	DEBUG("ADC0_IRQHandler%d\r\n" );
	BSP_MKL16_ADC_ShowValue();
	BSP_LED_Toggle(BSP_LED_TEST);
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

